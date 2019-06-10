
// ==========================================================================
//
// blink the LED on a blue-pill board
//
// (c) Wouter van Ooijen (wouter@voti.nl) 2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

#include "hwlib.hpp"

// Setup SPI1 pins A4/SS, A5/SCK, A6/MISO, A7/MOSI on port A
// Hardware slave selectis not used. That is indicated in the settings




int main() {
    auto uart_pin = hwlib::target::pin_out(hwlib::target::pins::b1);
    RCC_TypeDef *rcc = RCC;

    rcc->APB2ENR |= RCC_APB2ENR_SPI1EN;
    TIM1->CR1 |= TIM_CR1_CEN;
    TIM2->CR1 |= TIM_CR1_CEN;
    SPI1->CR1 |= SPI_CR1_MSTR | SPI_CR1_SPE | SPI_CR1_BR_1;
    SPI1->CR1 &= ~SPI_CR1_CRCEN;
    SPI1->CR2 = SPI_CR2_SSOE;
    SPI1->CR1 &= ~(SPI_CR1_DFF);

//Full duplex mode
    SPI1->CR1 &= ~(SPI_CR1_BIDIMODE);
    SPI1->CR1 &= ~(SPI_CR1_RXONLY);

//MSB first
    SPI1->CR1 &= ~(SPI_CR1_LSBFIRST);

//Master mode
    SPI1->CR1 |= SPI_CR1_MSTR;

//Software NSS mode off
    SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;

//Enable
    SPI1->CR1 |= SPI_CR1_SPE;


    for (;;) {
        SPI1->DR = 'H';
        hwlib::wait_ms(1000);
        uint32_t st = SPI1->SR;
        for (uint8_t i = 0; i < 32; i++) {
            hwlib::uart_putc_bit_banged_pin(((st & (1 << 31)) > 0) ? '1' : '0', uart_pin);
            hwlib::wait_ms(50);
            st <<= 1;
        }
        hwlib::uart_putc_bit_banged_pin('\r', uart_pin);
        hwlib::wait_ms(1);
        hwlib::uart_putc_bit_banged_pin('\n', uart_pin);

    }

}

//void t() {
//    auto led = hwlib::target::pin_in_out(hwlib::target::pins::);
//    hwlib::blink(led, 500);
//
//
////    char t[10] = "hallotest";
//    while (true) {
//        for (int i = 0; i < 10; i++) {
//            int number = HWLIB_BAUDRATE;
//            while (number > 0) {
//                int digit = number % 10;
//                number /= 10;
//                //print digit
////                hwlib::uart_putc_bit_banged_pin(digit | 48, p);
//                hwlib::wait_ms(50);
//            }
//            hwlib::uart_putc_bit_banged_pin('\n', p);
//            hwlib::wait_ms(50);
//        }
//        hwlib::wait_ms(500);
//    }
//
//}