/*
 *
 * Copyright Niels Post 2019.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 *
*/

#ifndef IPASS_SPI_BLUE_PILL_HPP
#define IPASS_SPI_BLUE_PILL_HPP

#include <hwlib.hpp>
#include <spi/bus_base.hpp>

namespace spi {
    /**
     * \addtogroup spi_ex
     * @{
     */

    /**
     * \brief Hardware SPI implementation for the STM32 Bluepill
     *
     * Uses hardware SPI1, and DMA, for extra fast transfer.
     * Unfortunately this bus doesn't support other modes than the default one.
     * Uses the default SPI1 pins (A4-A7 (CSN,CLK,MISO,MOSI)
     */
    class bus_stm32f10xxx : public spi_base_bus {
        /// \brief 32 Free 0 bytes, for when only reading
        uint8_t data_out_empty[32] = {0};

    public:
        /**
         * \brief Create a Blue_pill spi bus
         *
         * Sets pins A4-A7 to their right mode for SPI transfer, prepares DMA1 channels 2 and 3 for SPI transfer.
         * @param mode SPI Mode to use, as stated before, changing this has no effect now
         */
        bus_stm32f10xxx(spi_mode mode);

    private:
        /**
         * \brief Write_Read implementation
         * @param n Amount of bytes to write
         * @param data_out Pointer to data to write
         * @param data_in  Pointer to memory location to read into
         *
         * This method waits until SPI and DMA both signal that they're done and then returns.
         */
        void write_read(size_t n, const uint8_t *data_out, uint8_t *data_in) override;

    protected:
        /**
         * \brief Pulls CSN low, ignores the set CSN pin
         * @param transaction The starting transaction
         */
        void onStart(spi_transaction &transaction) override;

        /**
         * \brief Pulls CSN high, ignores the set CSN pin
         * @param transaction The ending transaction
         */
        void onEnd(spi_transaction &transaction) override;

    };

    /**
     * @}
     */
}


#endif //IPASS_SPI_BLUE_PILL_HPP
