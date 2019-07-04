/*
 *
 * Copyright Niels Post 2019.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 *
*/

#include <mesh_domotics/module/rotary_encoder.hpp>

volatile int val = 0;
volatile bool debounce = false;

void __attribute__((interrupt("IRQ"))) TIM2_IRQHandler(void) {
    debounce = false;
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~(TIM_SR_UIF);
        debounce = false;
    }
}

void __attribute__((interrupt("IRQ"))) EXTI9_5_IRQHandler(void) {
    uint32_t idr = GPIOA->IDR & ((0x1u << 9u) | (0x1u << 10u));
    if (!debounce) {
        if (idr == (3u << 9u)) {
            val++;
        } else if (idr == (1u << 9u)) {
            val--;
        }
    }
    debounce = true;

    EXTI->PR |= (1 << 9);
}


namespace mesh_domotics {
    namespace modules {

        rotary_encoder::rotary_encoder(uint8_t id) : input_module(id) {
            // Enable peripheral ports
            RCC->APB2ENR |=
                    RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN |
                    RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPDEN;

            // Use a General Purpose Timer for debouncing the rotary encoder input
            // Enable TIM2
            RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
            // Set prescaler for TIM2, (TIM2 should now tick a little less than once every ms, since we can't go any higher than that)
            TIM2->PSC = 65000;
            // Autoreload every 10 timer "ticks", (so a little less than every 10 ms)
            TIM2->ARR = 10;
            // Enable Timer Interrupt (this wil call TIM2_IRQHandler)
            TIM2->DIER |= TIM_DIER_UIE;
            // Enable Timer 2
            TIM2->CR1 |= TIM_CR1_CEN;

            // Clear mode for Pin 9 and 10
            GPIOA->CRH &= ~(0xF << 1);
            GPIOA->CRH &= ~(0xF << 2);

            // Set mode to input floating for pin 9 and 10
            GPIOA->CRH |= (0x8 << 1);
            GPIOA->CRH |= (0x8 << 2);


            // Clear EXTI interrupt line selection, then set it to port A
            AFIO->EXTICR[3] &= ~(AFIO_EXTICR3_EXTI9);
            AFIO->EXTICR[3] |= (AFIO_EXTICR3_EXTI9_PA);

            // Unmask interrupt 9
            EXTI->IMR |= (1 << 9);
            // Disable falling edge trigger
            EXTI->FTSR &= ~(1 << 9);
            // Enable rising edge trigger
            EXTI->RTSR |= (1 << 9);

            // Set priority and enable External Input interrupt
            NVIC_SetPriority(EXTI9_5_IRQn, 0x03);
            NVIC_EnableIRQ(EXTI9_5_IRQn);

            // Set priority for, an enable interrupt for Timer 2
            NVIC_SetPriority(TIM2_IRQn, 0x03);
            NVIC_EnableIRQ(TIM2_IRQn);
        }

        bool rotary_encoder::get_output(uint8_t data[4], bool force) {
            if (val != last_value.numeric || force) {
                data[0] = last_value.data[0];
                data[1] = last_value.data[1];
                data[2] = last_value.data[2];
                data[3] = last_value.data[3];
                last_value.numeric = val;
                return true;
            }
            return false;

        }

    }
}