/*
 *
 * Copyright Niels Post 2019.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 *
*/

#ifndef IPASS_SPI_TESTING_HPP
#define IPASS_SPI_TESTING_HPP

#include <spi/bus_base.hpp>

namespace spi {
/**
 * \addtogroup spi_ex
 * @{
 */


    /**
     * \brief SPI-bus meant for testing
     *
     * Buffers the written output.
     * Allows for preparing a read buffer to return to the class using this bus.
     */
    class bus_testing : public spi_base_bus {
    public:
        /// \brief Buffer for written messages
        std::array<uint8_t, 128> out_buffer = {0};
        /// \brief Buffer to read from
        std::array<uint8_t, 128> in_buffer = {0};
        /// \brief Current size of the write buffer
        size_t out_buffer_size = 0;
        /// \brief Current index to read data from
        size_t in_buffer_index = 0;
        /// \brief Current full size of the read buffer
        size_t in_buffer_size = 0;

        /**
         * \brief Create a testbus
         *
         * Mode doesn't matter here, since we're not actually using SPI anyway
         */
        bus_testing() : spi_base_bus(spi_mode(false, false, 1)) {}

    protected:
        /**
         * \brief Writes and reads from/to the buffers
         * @param n Amount of bytes to read/write
         * @param data_out Pointer to data to write into the out_buffer
         * @param data_in  Pointer to memory space to read the in_buffer into
         */
        void write_read(size_t n, const uint8_t *data_out, uint8_t *data_in) override {

            for (size_t i = 0; i < n; i++) {
                if (data_out != nullptr) {
                    out_buffer[out_buffer_size] = *data_out++;
                }
                out_buffer_size++;
                if (data_in != nullptr) {
                    *data_in++ = in_buffer[in_buffer_index];
                }
                in_buffer_index++;

            }
        }

    public:
        /**
         * \brief Append n items to the in_buffer
         * @tparam n Amount of bytes to add
         * @param to_add Array of bytes to add
         */
        template<size_t n>
        void append_in_buffer(const std::array<uint8_t, n> &to_add) {
            for (const uint8_t &val : to_add) {
                in_buffer[in_buffer_size++] = val;
            }
        }

        /**
         * \brief Match an array of bytes against either the in or out buffer
         * @tparam n Amount of items to match
         * @param match_array Array of bytes to match
         * @param is_out Should be true if the out buffer is to be matched
         * @return True if the array matched the marked buffer
         */
        template<size_t n>
        bool match(const std::array<uint8_t, n> &match_array, bool is_out) {
            std::array<uint8_t, 128> buffer = is_out ? out_buffer : in_buffer;
            for (size_t i = 0; i < n; i++) {
                if (match_array[i] != buffer[i]) {
                    return false;
                }
            }
            return true;
        }


        /**
         * \brief Clear both buffers
         *
         * Also resets the size and index indicators
         */
        void clear() {
            for (uint8_t &val : out_buffer) {
                val = 0;
            }
            for (uint8_t &val : in_buffer) {
                val = 0;
            }
            out_buffer_size = 0;
            in_buffer_index = 0;
            in_buffer_size = 0;
        }

    };

    /**
     * @}
     */

}

#endif //IPASS_SPI_TESTING_HPP
