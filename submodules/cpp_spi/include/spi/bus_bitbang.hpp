/*
 *
 * Copyright Niels Post 2019.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 *
*/

#ifndef OOPC_OPDRACHTEN_SPI_BITBANG_HPP
#define OOPC_OPDRACHTEN_SPI_BITBANG_HPP

#include <spi/bus_base.hpp>

namespace spi {
    /**
     * \addtogroup spi_ex
     * @{
     */

    /**
     * \brief BitBanged SPI implementation
     *
     * Has full support for operating modes.
     */
    class bus_bitbang : public spi_base_bus {
    protected:
        /// \brief Clock pin
        hwlib::pin_direct_from_out_t sclk;
        /// \brief Master Out Slave In pin
        hwlib::pin_direct_from_out_t mosi;
        /// \brief Master In Slave Out pin
        hwlib::pin_direct_from_in_t miso;
    public:
        /**
         * \brief Create a bitbang-bus using pins and a spi mode
         * @param _sclk Clock Pin
         * @param _mosi Master out slave in pin
         * @param _miso Master In Slave Out pin
         * @param mode SPI_Mode to use
         */
        bus_bitbang(hwlib::pin_out &_sclk, hwlib::pin_out &_mosi, hwlib::pin_in &_miso,
                    const spi::spi_mode &mode);

    protected:
        /**
         * \brief Wait for half a clock period, to let the lines settle
         */
        void wait_half_period();

        /**
         * \brief Writes + Reads a single byte
         *
         * @param d Byte to write, the read byte is written into this aswell
         */
        void write_read_byte(uint8_t &d);

        /**
         * \brief Writes + reads multiple bytes
         * @param n Amount of bytes
         * @param data_out Pointer to data to write
         * @param data_in Pointer to memory location to read into
         */
        void write_read(size_t n, const uint8_t *data_out, uint8_t *data_in) override;

        /**
         * \brief Writes + read multiple bytes in reverse (LSByte first)
         * @param n Amount of bytes
         * @param data_out Pointer to data to write
         * @param data_in Pointer to memory location to read into
         */
        void write_read_reverse(size_t n, const uint8_t *data_out, uint8_t *data_in) override;

    };

    /**
     * @}
     */
}

#endif //OOPC_OPDRACHTEN_SPI_BITBANG_HPP
