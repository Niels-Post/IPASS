/*
 *
 * Copyright Niels Post 2019.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 *
*/

#ifndef PROJECT_NRF24L01_HPP
#define PROJECT_NRF24L01_HPP

#include <hwlib.hpp>
#include <spi/bus_base.hpp>
#include <nrf24l01plus/definitions.hpp>
#include <nrf24l01plus/address.hpp>

namespace nrf24l01 {
    /**
     * \defgroup nrf24l01Plus NRF24L01Plus Library
     * \brief Library for the NRF24l01+ tranceiver
     *
     * This library should be usable for a "normal" nrf24l01 module, although some functions will not work.
     * Includes all possible commands, and registers for the module, as well as an abstract data type for an NRF address.
     */

    /**
     * \addtogroup nrf24l01Plus
     * @{
     */

    /**
     * Interface for the NRF24L01+ NRF tranceiver.
     */
    class nrf24l01plus {
    public:
        /// \brief SPI_BUS the NRF module is connected to
        spi::spi_base_bus &bus;
        /// \brief CSN pin for the NRF module
        hwlib::pin_out &csn;
        /// \brief CE pin for the NRF module
        hwlib::pin_out &ce;

        /**
         * \brief Get register size in bytes
         * @param address Register address to get size for
         * @return The size
         */
        uint8_t register_bytes(const uint8_t &address) {
            if (address == NRF_REGISTER::RX_ADDR_P0 || address == NRF_REGISTER::RX_ADDR_P1 ||
                address == NRF_REGISTER::TX_ADDR) {
                return 5;
            }
            return 1;
        }

        /// The mode this module is currently in
        uint8_t currentMode = 0;
        //! No mode, this usually indicates Power Off or Standby mode
        const uint8_t MODE_NONE = 0;
        //! Primary TX mode, ce is only asserted when transmitting payloads
        const uint8_t MODE_PTX = 1;
        //!Primary RX mode, ce is always asserted, except when writing registers. Note that while writing registers, the NRF24L01 cannot receive a payload
        const uint8_t MODE_PRX = 2;

        //! The status register's last known value
        uint8_t last_status;

        /**
         * Create NRF24L01Plus object
         * @param bus Spi_Bus to use for communication
         * @param csn Chip Select pin
         * @param ce Chip Enable Pin
         */
        nrf24l01plus(spi::spi_base_bus &bus, hwlib::pin_out &csn, hwlib::pin_out &ce) : bus(bus),
                                                                                        csn(csn),
                                                                                        ce(ce), last_status(0) {}

        /**
         * \brief Sends SPI command to NRF24L01Plus device
         *
         * device will send back status register, which is saved in last_status
         * Possible commands can be found in NRF_INSTRUCTION
         * \details
         * @param command_word Command to send
         * @param data_out Payload to send with the command, in single word commands (and by default), this can be a nullpointer
         * @param n Size of the payload sent with command
         * @param data_in Pointer to save the command response in, make sure there are at least n bytes available at this address\
         * @param lsbyte_first Should the data be read and written LSByte first (reversed)
         */
        void send_command(const uint8_t &command_word, const uint8_t *data_out = nullptr, const uint8_t &n = 0,
                          uint8_t *data_in = nullptr, bool lsbyte_first = false) {
            auto transaction = bus.transaction(csn);
            transaction.write_read(1, &command_word, &last_status);
            if (n == 0) {
                return;
            }
            if (lsbyte_first) {
                transaction.write_read_reverse(n, data_out, data_in);
            } else {
                transaction.write_read(n, data_out, data_in);
            }
        }

        /**
         * \brief Sends SPI command to NRF24L01Plus device
         * @tparam n Size of the command data (in  bytes)
         * @param command_word Command to send
         * @param data_out Data to write
         * @param data_in Empty array to read into
         * @param lsbyte_first Should the data be read and written LSByte first (reversed)
         */
        template<size_t n>
        void send_command(const uint8_t &command_word, const std::array<uint8_t, n> &data_out,
                          std::array<uint8_t, n> &data_in, bool lsbyte_first = false) {
            send_command(command_word, data_out.begin(), n, data_in.begin(), lsbyte_first);
        }


        /**
         * \brief Read memory register from NRF24L01
         *
         * Addresses can be found in NRF_REGISTER.
         * This method uses unsafe byte pointers, prefer to use nrf24l01plus::read_register(const uint8_t&, std::array<uint8_t, n>&, bool)
         * \details
         * @param address Address of the register to read
         * @param in Memory location to read register into
         * @param lsbyte_first Should the data be read LSByte first (reversed)
         */
        void read_register(const uint8_t &address, uint8_t *in = nullptr, bool lsbyte_first = false) {
            send_command(NRF_INSTRUCTION::R_REGISTER | address, nullptr, register_bytes(address), in, lsbyte_first);
        }

        /**
         * \brief Read memory register from NRF24L01
         *
         * @tparam n Size of register
         * @param address Memory address to read from
         * @param in Array to put register contents into
         * @param lsbyte_first Should the data be read LSByte first (reversed)
         */
        template<size_t n>
        void read_register(const uint8_t &address, std::array<uint8_t, n> &in, bool lsbyte_first = false) {
            read_register(address, in.begin(), lsbyte_first);
        }


        /**
         * \brief Write value into register
         *
         * Addresses can be found in NRF_REGISTER
         * This method uses unsafe byte pointers, prefer to use nrf24l01plus::write_register(const uint8_t&, const std::array<uint8_t, n>&, bool)
         * @param address Address of the register to write to
         * @param data Pointer to the data to write into the register
         * @param lsbyte_first Should the written data be written LSByte first (reversed)
         */
        void write_register(const uint8_t &address, const uint8_t *data, bool lsbyte_first = false) {
            send_command(NRF_INSTRUCTION::W_REGISTER | address, data, register_bytes(address), nullptr, lsbyte_first);
        }

        /**
         * \brief Write value into register
         *
         * @tparam n Size of the register data
         * @param address Address of the register to write to
         * @param out Array containing the data to write
         * @param lsbyte_first Should the written data be written LSByte first (reversed)
         */
        template<size_t n>
        void write_register(const uint8_t &address, const std::array<uint8_t, n> &out, bool lsbyte_first = false) {
            write_register(address, out.begin(), lsbyte_first);
        }


        /**
         * \brief Write value into register, without using arrays or pointers, easy to use for constants
         *
         * Addresses can be found in NRF_REGISTER
         * @param address Address of the register to write to
         * @param data Data to write into the register
         */
        void write_register(const uint8_t &address, uint8_t data) {
            write_register(address, &data);
        }

        /**
         * \brief No Operation, can be used to retrieve last_status
         */
        void no_operation() {
            send_command(NRF_INSTRUCTION::RF24_NOP);
        }

        /**
         * \brief Set retransmit options, set both values to 0 to disable auto retransmit
         *
         * @param retry_delay Time to wait before retransmitting (multiplied by 250μs
         * @param retry_count Amount of retries before giving up
         */
        void auto_retransmit(uint8_t retry_delay, uint8_t retry_count) {
            write_register(NRF_REGISTER::SETUP_RETR, retry_delay << 4 | retry_count);
        }

        /**
         * \brief Set RF channel to transmit/receive on
         *
         * @param channel  Channel offset from 2.4gHz (in mHz)
         */
        void channel(uint8_t channel) {
            write_register(NRF_REGISTER::RF_CH, channel & uint8_t(0xEF));
        }

        /**
         * \brief Set Address to transmit on
         *
         * @param address Pointer to location of address.
         */
        void tx_set_address(const address &address) {
            write_register(NRF_REGISTER::TX_ADDR, address.address_bytes, true);
        }

        /**
         * \brief Retrieve current TX address of this module
         *
         * @return The address
         */
        address tx_get_address() {
            uint8_t data[5];
            read_register(NRF_REGISTER::TX_ADDR, data);
            return {data[0], data[1], data[2], data[3], data[4]};
        }

        /**
         * \brief Get mode the module is in at the moment
         *
         * @return The current mode
         */
        uint8_t get_mode() {
            return currentMode;
        }

        /**
         * \brief Set operating mode:
         *
         * Operating modes:
         *  0: None
         *  1: Primary Transmit
         *  2: Primary Receive
         *  When the new mode is equal to the old mode, this method doesn't do anything.
         * @param newMode
         */
        void mode(uint8_t newMode) {
            if (currentMode == newMode) {
                return;
            }
            bool fromActiveState = currentMode == MODE_PRX || currentMode == MODE_PTX;

            currentMode = newMode;
            if (fromActiveState) {
                ce.write(false);
                ce.flush();
                hwlib::wait_us(130);
            }


            uint8_t lastConfig;
            switch (newMode) {
                case 1:
                    read_register(NRF_REGISTER::CONFIG, &lastConfig);
                    write_register(NRF_REGISTER::CONFIG, lastConfig & ~NRF_CONFIG::CONFIG_PRIM_RX);
                    break;

                case 2:
                    read_register(NRF_REGISTER::CONFIG, &lastConfig);
                    write_register(NRF_REGISTER::CONFIG, lastConfig | NRF_CONFIG::CONFIG_PRIM_RX);
                    ce.write(true);
                    ce.flush();
                    hwlib::wait_us(130);
                    break;

                case 0:
                default: {
                    break;
                }

            }


        }

        /**
         * \brief Power the NRF24L01 chip up or down
         * @param value
         */
        void power(bool value) {
            uint8_t lastConfig;
            read_register(NRF_REGISTER::CONFIG, &lastConfig);
            if (value) {
                lastConfig |= NRF_CONFIG::CONFIG_PWR_UP;
            } else {
                lastConfig &= ~NRF_CONFIG::CONFIG_PWR_UP;
            }
            write_register(NRF_REGISTER::CONFIG, &lastConfig);
        }

        /**
         * \brief Get status of FIFO registers
         *
         * @return FIFO status byte
         */
        uint8_t fifo_status() {
            uint8_t status;
            read_register(NRF_REGISTER::FIFO_STATUS, &status);
            return status;
        }

//////////////////////////////////////////////////////////////////////////////  RX Payload Functions
        /**
         * \brief Read width of currently available RX payload
         *
         * @return The length in bytes
         */
        uint8_t rx_payload_width() {
            uint8_t pw;
            send_command(nrf24l01::NRF_INSTRUCTION::R_RX_PL_WID, nullptr, 1, &pw);
            return pw;
        }


        /**
         * \brief Reads the first available payload.
         *
         * If there is none, this will retrieve null bytes.
         * When using Dynamic Payload Length, read_rx_payload_width should be checked before reading the payload.
         * This method uses unsage byte pointers, prefer to use nrf24l01plus::rx_read_payload(std::array<uint8_t,n>&)
         * @param data Memory location to write data to
         * @param payload_width Width of the payload to read
         */
        void rx_read_payload(uint8_t *data, const uint8_t &payload_width = 32) {
            send_command(NRF_INSTRUCTION::R_RX_PAYLOAD, nullptr, payload_width, data);
        }

        /**
         * \brief Reads the first available payload.
         *
         * If there is none, this will retrieve null bytes.
         * @tparam n Size of the payload to read
         * @param data Array to read data into
         */
        template<size_t n>
        void rx_read_payload(std::array<uint8_t, n> &data) {
            rx_read_payload(data.begin(), n);
        }

        /**
         * \brief Clear RX FIFO register
         */
        void rx_flush() {
            send_command(NRF_INSTRUCTION::FLUSH_RX);
        }


//////////////////////////////////////////////////////////////////////////////  TX Payload Functions
        /**
         * \brief Transmit the first available TX Payload in TX FIFO register
         */
        void tx_send_payload() {
            ce.write(true);
            hwlib::wait_us(10);
            ce.write(false);
        }

        /**
         * \brief Write data into TX FIFO register
         *
         * This method used unsafe byte pointers, prefer to use nrf24l01plus::tx_write_payload(const std::array<uint8_t, n>&, bool)
         * For using NOACK, the feature needs to be enabled in the FEATURE register
         * @param data Memory location to write data from
         * @param size Size of the data to write
         * @param noack If True, the payload is written with NO_ACK enabled
         */
        void tx_write_payload(uint8_t *data, const uint8_t &size, bool noack = false) {
            if (noack) {
                send_command(NRF_INSTRUCTION::W_TX_PAYLOAD_NO_ACK, data, size, nullptr);
            } else {
                send_command(NRF_INSTRUCTION::W_TX_PAYLOAD, data, size, nullptr);
            }

            tx_send_payload();
        }

        /**
         * \brief Write data into TX FIFO register
         *
         * For using NOACK, the feature needs to be enabled in the FEATURE register
         * @tparam n Size of the data to write
         * @param out Array of data to write
         * @param noack If True, the payload is written with NO_ACK enabled
         */
        template<size_t n>
        void tx_write_payload(const std::array<uint8_t, n> &out, bool noack = false) {
            tx_write_payload(out.begin(), n, noack);
        }


        /**
         * \brief Clear TX FIFO register
         */
        void tx_flush() {
            send_command(NRF_INSTRUCTION::FLUSH_TX);
        }


        /**
         * \brief enable or disable auto acknowledgement for a given pipe
         *
         * @param pipe Number of the pipe to set
         * @param value True if AutoAck needs to be enabled
         */
        void rx_auto_acknowledgement(const uint8_t &pipe, const bool &value) {
            uint8_t full_register;
            read_register(NRF_REGISTER::EN_AA, &full_register);
            if (value) {
                full_register |= 1 << pipe;
            } else {
                full_register &= ~(1 << pipe);
            }
            write_register(NRF_REGISTER::EN_AA, &full_register);
        }

        /**
         * \brief Enable or disable auto acknowledgement for all pipes
         * @param value True of AutoAck needs to be enabled
         */
        void rx_auto_acknowledgement(const bool &value) {
            write_register(NRF_REGISTER::EN_AA, uint8_t(value ? 0x3F : 0x00));
        }

        /**
         * \brief Enable or disable  a given RX pipe
         *
         * @param pipe Number of the pipe to set
         * @param value True if the pipe needs to be enabled
         */
        void rx_enabled(const uint8_t &pipe, const bool &value) {
            uint8_t full_register;
            read_register(NRF_REGISTER::EN_RXADDR, &full_register);
            if (value) {
                full_register |= 1 << pipe;
            } else {
                full_register &= ~(1 << pipe);
            }
            write_register(NRF_REGISTER::EN_RXADDR, &full_register);
        }

        /**
         * \brief Enable or disable all rx pipes
         *
         * @param value True if the pipe needs to be enabled
         */
        void rx_enabled(const bool &value) {
            write_register(NRF_REGISTER::EN_RXADDR, uint8_t(value ? 0x3F : 0x00));
        }

        /**
         * \brief Retrieve the address for a given rx pipe
         *
         * Adds the first 4 bytes from pipe 1 for all pipe numbers between 2 and 5.
         * @param pipe Number of the pipe to get address for
         * @return The full pipe address
         */
        address rx_get_address(const uint8_t &pipe) {
            if (pipe > 1) {
                address base_address;
                uint8_t end;
                read_register(NRF_REGISTER::RX_ADDR_P1, base_address.address_bytes);
                read_register(NRF_REGISTER::RX_ADDR_P0 + pipe, &end);
                return {base_address, end};
            }

            uint8_t address[5] = {0};
            read_register(NRF_REGISTER::RX_ADDR_P0 + pipe, address, true);
            return {address};
        }

        /**
         * \brief Set the address for a given rx pipe
         *
         * Note that for pipes 2-5, the first 4 bytes of the address are ignore.
         * Set the address for pipe 1 aswell to change these.
         * @param pipe Pipe number
         * @param address New address for the pipe
         */
        void rx_set_address(const uint8_t &pipe, address &address) {
            if (pipe > 1) {
                write_register(NRF_REGISTER::RX_ADDR_P0 + pipe, address.address_bytes + 4, true);
            } else {
                write_register(NRF_REGISTER::RX_ADDR_P0 + pipe, address.address_bytes, true);
            }
        }

        /**
         * \brief Set the payload width for a given rx_pipe.
         *
         * Note that when dynamic payload length(DPL) is enabled, this doesn't matter.
         * @param pipe Pipe number
         * @param width new payload width for this pipe (can be in range 0-32)
         */
        void rx_set_payload_width(const uint8_t &pipe, const uint8_t &width) {
            write_register(NRF_REGISTER::RX_PW_P0 + pipe, width & uint8_t(0x3F));
        }

        /**
         * \brief Set the payload width for all rx_pipes
         *
         * @param width new payload width for all pipes (can be in range 0-32)
         */
        void rx_set_payload_width(const uint8_t &width) {
            for (uint8_t i = 0; i < 6; i++) {
                rx_set_payload_width(i, width);
            }
        }

        /**
         * \brief Retrieve payload width of a given rx_pipe
         *
         * Only Useful when DPL is disabled
         * @param pipe Pipe number
         * @return The payload width (0-32)
         */
        uint8_t rx_get_payload_width(const uint8_t &pipe) {
            uint8_t width;
            read_register(NRF_REGISTER::RX_PW_P0 + pipe, &width);
            return width;
        }

        /**
         * \brief Enable or disable Dynamic Payload Length for a given pipe
         *
         * @param pipe Pipe number
         * @param enabled True if DPL needs to be enabled
         */
        void rx_set_dynamic_payload_length(const uint8_t &pipe, const bool &enabled) {
            uint8_t previousVal;
            read_register(NRF_REGISTER::DYNPD, &previousVal);
            if (enabled) {
                previousVal |= 1u << pipe;
            } else {
                previousVal &= ~(1u << pipe);
            }
            write_register(NRF_REGISTER::DYNPD, &previousVal);
        }

        /**
         * \brief Enable or disable Dynamic Payload Length for all rx_pipes
         *
         * @param enabled True if DPL needs to be enabled
         */
        void rx_set_dynamic_payload_length(const bool &enabled) {
            write_register(NRF_REGISTER::DYNPD, uint8_t(enabled ? 0x3F : 0x00));
        }

    };

    /**
     * @}
     */
}

#endif //PROJECT_NRF24L01_HPP
