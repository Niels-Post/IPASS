//
// Created by Niels on 6/28/2019.
//

#include "spi_blue_pill.hpp"

void spi_blue_pill::write_read(size_t n, const uint8_t *data_out, uint8_t *data_in) {

    if (data_in != nullptr) {
        SPI1->DR;
        DMA1_Channel2->CMAR = (uint32_t) data_in;
        DMA1_Channel2->CNDTR = n;
        DMA1_Channel2->CCR |= 1u;
    }

    if (data_out != nullptr) {
        DMA1_Channel3->CMAR = (uint32_t) data_out;
    } else {
        DMA1_Channel3->CMAR = (uint32_t) data_out_empty;
    }


    DMA1_Channel3->CNDTR = n;
    DMA1_Channel3->CCR |= 1u;

    while ((DMA1->ISR & DMA_ISR_TCIF2) == 0) {}
    while ((SPI1->SR & SPI_SR_TXE) == 0) {}
    while ((SPI1->SR & SPI_SR_BSY) > 0) {}
    DMA1_Channel2->CCR &= ~1u;
    DMA1_Channel3->CCR &= ~1u;

//    else {
//        if(data_out == nullptr && data_in != nullptr) {
//            for(size_t i = 0 ; i < n; i++) {
//                data_in[i] = 0;
//            }
//            DMA1_Channel3->CMAR = (uint32_t) data_in;
//        }
//    }




//    uint8_t data_out_repl[n];
//    uint8_t data_in_repl[n];
//
//    if(data_in == nullptr) {
//        data_in = data_in_repl;
//    }
//
//    if(data_out == nullptr) {
//        data_out = data_out_repl;
//    }



//    if (data_in == nullptr) {
//
//        for (uint8_t i = 0; i < n; i++) {
//            SPI1->DR = *data_out++;
//            SPI1->DR;
//        }
//    } else if (data_out == nullptr) {
//        for (uint8_t i = 0; i < n; i++) {
//            SPI1->DR = 0;
//            *data_in++ = SPI1->DR;
//        }
//    } else {
//        for (uint8_t i = 0; i < n; i++) {
//            SPI1->DR = *data_out++;
//            *data_in++ = SPI1->DR;
//        }
//    }

}


spi_blue_pill::spi_blue_pill(const hwlib_ex::spi_mode mode) : spi_base_bus(mode) {

//    hwlib::target::pin_out(hwlib::target::pins::a3).write(false);

    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN |
                    RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN |
                    RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPDEN;

    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    GPIOA->CRL &= 0x0000FFFFu;
    // NSSPin
    GPIOA->CRL |= (0x3u << 16u);
    //Clock pin
    GPIOA->CRL |= (0xAu << 20u);
    //MISO pin
    GPIOA->CRL |= (0x8u << 24u);
    GPIOA->ODR |= 1 << 6;
    // MOSI pin
    GPIOA->CRL |= (0xAu << 28u);


    TIM1->CR1 |= TIM_CR1_CEN;
    TIM2->CR1 |= TIM_CR1_CEN;

    SPI1->I2SCFGR = 0;

    SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_SPE | SPI_CR1_BR_1 | SPI_CR1_SSM | SPI_CR1_SSI;
    SPI1->CR2 = SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN;


    //Configure DMA channel 1 rx
    DMA1_Channel2->CCR = 0x0080;
    DMA1_Channel2->CPAR = (uint32_t) &(SPI1->DR);

    //Configure DMA channel 2 tx
    DMA1_Channel3->CCR = 0x0090;
    DMA1_Channel3->CPAR = (uint32_t) &(SPI1->DR);
    SPI1->CR1 |= SPI_CR1_SPE;

//Enable

//    for (;;) {
//        SPI1->CR1 |= SPI_CR1_SPE;
//        SPI1->DR = 0x000A;
//        LOG("TEST", SPI1->DR);
//        for (uint8_t i = 0; i < 32; i++) {
//            LOG("Status", hwlib::bin << SPI1->SR);
//        }
//        hwlib::wait_ms(1);
//
//    }

}

void spi_blue_pill::onStart(hwlib_ex::spi_base_bus::spi_transaction &transaction) {
    GPIOA->BSRR |= 1u << 20u;
//    transaction.csn.write(false);
//    hwlib::target::pin_out
}

void spi_blue_pill::onEnd(hwlib_ex::spi_base_bus::spi_transaction &transaction) {
//    SPI1->CR1 &= ~SPI_CR1_SPE;
    GPIOA->BSRR |= 1u << 4u;
//    transaction.csn.write(true);
}


