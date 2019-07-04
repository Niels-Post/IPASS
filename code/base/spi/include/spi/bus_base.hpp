/*
 *
 * Copyright Niels Post 2019.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 *
*/

#ifndef OOPC_OPDRACHTEN_SPI_BASE_HPP
#define OOPC_OPDRACHTEN_SPI_BASE_HPP

#include <hwlib.hpp>

namespace spi {

    /**
     * \defgroup spi_ex SPI bus Extended Library
     * \brief Library for SPI implementations
     *
     * Includes support for reverse reading and writing (LSByte first), and setting SPI operating modes.
     */

    /**
     * \addtogroup spi_ex
     * @{
     */

    /**
     * \brief Struct for storing a SPI operating mode
     *
     * See https://en.wikipedia.org/wiki/Serial_Peripheral_Interface for more information
     */
    struct spi_mode {
        /// \brief Clock polarity
        bool clock_polarity = false;
        /// \brief Clock phase
        bool clock_phase = false;
        /// \brief Duration of half a cycle in nanoseconds
        uint32_t half_time_ns = 1000;

        /// \brief Empty constructor, assumes polarity 0, phase 0, and half_time 1000
        spi_mode();

        /// \brief Create a new spi_mode
        spi_mode(bool clockPolarity, bool clockPhase, uint32_t halfTimeNs);
    };

    /**
     * \brief abstract class for SPI implementations
     *
     * Supports byte-reversed transactions.
     * Automatically handles transaction start and end.
     * When overriding, the choice can be made to implement de reverse functions. When this is not done, they rely on the base read and write implementations, and use a buffer to reverse these.
     */
    class spi_base_bus {
    protected:
        /// \brief Mode of this SPI bus, implementations of SPI need to interpret this
        spi_mode mode;

        /**
         * \brief Write_read implementation.
         *
         * When this method is not overridden, it's based on the write_read_reverse implementation.
         * Make sure to implement at least ONE of these two
         * @param n Size of the data to write
         * @param data_out Memory pointer to the data to write
         * @param data_in Memory pointer to a location to read data into
         */
        virtual void write_read(size_t n, const uint8_t *data_out, uint8_t *data_in);

        /**
         * \brief Write_read_reverse implementation.
         *
         * Writes and reads in reverse byte order (LSByte first)
         * When this method is not overridden, it's based on the write_read implementation.
         * Make sure to implement at least ONE of these two
         * @param n Size of the data to write
         * @param data_out Memory pointer to the data to write (writing starts at data_out+n)
         * @param data_in Memory pointer to a location to read data into
         */
        virtual void write_read_reverse(size_t n, const uint8_t *data_out, uint8_t *data_in);

    public:
        /**
         * \brief Transaction handler for SPI
         *
         * This class does not need to be extended, it uses the write_read implementations from the bus!
         */
        class spi_transaction final {
        private:
            /// \brief The SPI bus for this transaction.
            spi_base_bus &bus;
        public:
            /// \brief Chip select pin for this transaction.
            hwlib::pin_out &csn;

            /**
             * \brief Create a transaction from a bus and CSN pin.
             *
             * Prefer to use bus.transaction(), since implementations can override this
             */
            explicit spi_transaction(spi_base_bus &bus, hwlib::pin_out &csn);

            /**
             * \brief Transaction destructor, used to call bus.onEnd().
             */
            ~spi_transaction();

            /**
             * \brief Write and read n bytes through the bus.
             *
             * @param n  Number of bytes to transfer
             * @param data_out Pointer to the data to write
             * @param data_in Pointer to the memory location to read into
             * @return This transaction, used for method chaining.
             */
            spi_transaction &write_read(size_t n, const uint8_t *data_out, uint8_t *data_in);

            /**
             * \brief Write and read n bytes through the bus LSByte first.
             *
             * \copydetails write_read(size_t,const uint8_t*,uint8_t*)
             */
            spi_transaction &write_read_reverse(size_t n, const uint8_t *data_out, uint8_t *data_in);

            /**
             * \brief Write n bytes through the bus.
             *
             * Any input is ignored
             * @param n  Number of bytes to transfer
             * @param data_out Pointer to the data to write
             * @return This transaction, for method chaining
             */
            spi_transaction &write(size_t n, const uint8_t *data_out);

            /**
             * \brief Write n bytes through the bus LSByte first.
             *
             * \copydetails write(size_t,const uint8_t*)
             */
            spi_transaction &write_reverse(size_t n, const uint8_t *data_out);

            /**
             * \brief Read n bytes through the bus.
             *
             * zero bytes are written as output
             * @param n  Number of bytes to transfer
             * @param data_in Pointer to the memory location to read into
             * @return This transaction, for method chaining
             */
            spi_transaction &read(size_t n, uint8_t *data_in);

            /**
             * \brief Read n bytes through the bus LSByte first.
             *
             * \copydetails read(size_t,uint8_t*)
             */
            spi_transaction &read_reverse(size_t n, uint8_t *data_in);

            /**
             * \brief Read a single byte from the bus.
             *
             * Note that the needed return value of this function prevents any further method chaining.
             * @param data_out Pointer to any optional data to write while reading
             */
            uint8_t read_byte(const uint8_t *data_out = nullptr);

            /**
             * \brief Write a single byte to the bus
             * @param byte Byte to write
             * @param data_in Optional pointer to memory location to read into while writing
             * @return This transaction, for method chaining
             */
            spi_transaction &write_byte(const uint8_t &byte, uint8_t *data_in = nullptr);
        };

    protected:
        /**
         * \brief Called when a transaction starts
         *
         * Implementations of spi_bus should use this to start the transaction when CSN output is not handled through pin_outs
         * @param transaction
         */
        virtual void onStart(spi_transaction &transaction);


        /**
         * \brief Called when a transaction ends
         *
         * Implementations of spi_bus should use this to start the transaction when CSN output is not handled through pin_outs
         * @param transaction
         */
        virtual void onEnd(spi_transaction &transaction);

    public:

        /**
         * \brief Start a transaction
         *
         * When CSN output is not handled through pin_outs, implementations can hide this method in favor of one with a default value set to hwlib::pin_out_dummy
         * @param csn Chip select pin
         * @return The transaction created
         */
        spi_transaction transaction(hwlib::pin_out &csn);

        /**
         * \brief Create a bus, using a spi mode
         * @param mode Mode to use
         */
        explicit spi_base_bus(spi_mode mode);
    };

    /**
     * @}
     */
}

#endif //OOPC_OPDRACHTEN_SPI_BASE_HPP
