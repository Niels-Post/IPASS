//
// Created by Niels on 6/27/2019.
//

#include "rotary_encoder.hpp"
#include "../../util/cout_debug.hpp"


#define dly        60                            //Delay size

volatile int val = 0;
domotica_value lastVal;
volatile bool debounce = false;

void __attribute__((interrupt("IRQ"))) TIM2_IRQHandler() {
    debounce=false;
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~(TIM_SR_UIF);
        // Toggle the LED output pin.
        debounce = false;
    }
//    LOG("TIMERT", "");
}

void  __attribute__((interrupt("IRQ"))) EXTI9_5_IRQHandler(void){
    uint32_t idr = GPIOA->IDR & ((0x1u << 9u) | (0x1u << 10u));
    if(!debounce) {
        if(idr == (3u << 9u)) {
            val++;
        } else if(idr == (1u << 9u)) {
            val--;
        }
    }
    debounce = true;

    EXTI->PR |= (1 << 9);
}


rotary_encoder::rotary_encoder(uint8_t id) : domotica_output_module(id), a(a),
                                             b(b) {
    auto p = hwlib::target::pin_in(hwlib::target::pins::a9);


    RCC->APB2ENR |=
            RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN |
            RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPDEN;

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

     TIM2->PSC = 65000;
    TIM2->ARR = 10;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1 |= TIM_CR1_CEN;




    GPIOA->CRL &= ~(0xF<<9);
    GPIOA->CRL &= ~(0xF<<10);

    GPIOA->CRL |= (0x3 << 9);
    GPIOA->CRL |= (0x3 << 10);

//    GPIOA->ODR |= (1 << 9) | (1 << 10);


    auto q = hwlib::target::pin_in(hwlib::target::pins::a10);
    p.read();
    q.read();

//    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
//
//    AFIO->EXTICR[3] &= ~(0xF << ((9 % 4) * 4));
//    AFIO->EXTICR[3] |=  (0x1 << ((9 % 4) * 4));


    AFIO->EXTICR[3] &= ~(AFIO_EXTICR3_EXTI9);
    AFIO->EXTICR[3] |= (AFIO_EXTICR3_EXTI9_PA);

    // Setup the button's EXTI line as an interrupt.
    EXTI->IMR  |=  (1 << 9);
// Disable the 'rising edge' trigger (button release).
    EXTI->FTSR &= ~(1 << 9);
// Enable the 'falling edge' trigger (button press).
    EXTI->RTSR |=  (1 << 9);

    NVIC_SetPriority(EXTI9_5_IRQn,0x03);
    NVIC_EnableIRQ(EXTI9_5_IRQn);

    NVIC_SetPriority(TIM2_IRQn,0x03);
    NVIC_EnableIRQ(TIM2_IRQn);
}

bool rotary_encoder::get_output(uint8_t data[4], bool force) {
    if(val != lastVal.numeric || force )
    {
        data[0] = lastVal.data[0];
        data[1] = lastVal.data[1];
        data[2] = lastVal.data[2];
        data[3] = lastVal.data[3];
        lastVal.numeric = val;
        return true;
    }
    return false;
//    bool aVal = a.read();
//    if (aVal != aLast) {
//        if (b.read() != aVal) {
//            currentValue.numeric++;
//        } else {
//            currentValue.numeric--;
//        }
//        data[0] = currentValue.data[0];
//        data[1] = currentValue.data[1];
//        data[2] = currentValue.data[2];
//        data[3] = currentValue.data[3];
//        aLast = aVal;
//        return true;
//    }
//    return false;
}
