
#ifndef PROJECT_NRF24L01_HPP
#define PROJECT_NRF24L01_HPP

#include "hwlib.hpp"
#include "../SPI/spi_bitbang.hpp"
#include  "NRF24L01_Definitions.hpp"

namespace nrf24l01 {
    /**
     * Interface for the NRF24L01+ NRF tranceiver.
     */
    class nrf24l01plus {
        hwlib_ex::spi_base_bus &bus;
        hwlib::pin_out &csn;
        hwlib::pin_out &ce;

        uint8_t register_bytes(const uint8_t &address);

        uint8_t currentMode = 0;
    public:
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
        nrf24l01plus(hwlib_ex::spi_base_bus &bus, hwlib::pin_out &csn, hwlib::pin_out &ce);

        /**
         * Sends SPI command to NRF24L01Plus device
         * device will send back status register, which is saved in last_status
         * @param command_word Command to send
         * @param data_out Payload to send with the command, in single word commands (and by default), this can be a nullpointer
         * @param n Size of the payload sent with command
         * @param data_in Pointer to save the command response in, make sure there are at least n bytes available at this address\
         * @param lsbyte_first Should the data be read and written LSByte first (reversed)
         */
        void send_command(const uint8_t &command_word, const uint8_t *data_out = nullptr, const uint8_t &n = 0,
                          uint8_t *data_in = nullptr, bool lsbyte_first = false);

        /**
         * Read memory register from NRF24L01
         * Addresses can be found in NRF_REGISTER
         * @param address Address of the register to read
         * @param out Memory location to read register into
         * @param lsbyte_first Should the data be read LSByte first (reversed)
         */
        void read_register(const uint8_t &address, uint8_t *out = nullptr, bool lsbyte_first = false);

        /**
         * Write value into register
         * Addresses can be found in NRF_REGISTER
         * @param address Address of the register to write to
         * @param data Pointer to the data to write into the register
         * @param lsbyte_first Should the written data be written LSByte first (reversed)
         */
        void write_register(const uint8_t &address, const uint8_t *data, bool lsbyte_first = false);

        /**
         * Write value into register
         * Addresses can be found in NRF_REGISTER
         * @param address Address of the register to write to
         * @param data Data to write into the register
         */
        void write_register(const uint8_t &address, uint8_t data);

        /**
         * Reads the latest available payload. If there is none, this will write null bytes
         * @param data Memory location to write data to
         */
        void read_rx_payload(uint8_t *data, const uint8_t payload_width);

        /**
         * Clear RX FIFO register
         */
        void flush_rx();

        /**
         * Write data into TX FIFO register
         * @param data Memory location to write data from
         * @param size Size of the data to write
         */
        void write_tx_payload(uint8_t *data, const uint8_t &size);

        /**
         * Transmit the first available TX Payload in TX FIFO register
         */
        void send_buffered_tx_payload();

        /**
         * Clear TX FIFO register
         */
        void flush_tx();

        /**
         * No Operation, can be used to retrieve last_status
         */
        void no_operation();

        /**
         * Set retransmit options, set both values to 0 to disable auto retransmit
         * @param retry_delay Time to wait before retransmitting (multiplied by 250μs
         * @param retry_count Amount of retries before giving up
         */
        void auto_retransmit(uint8_t retry_delay, uint8_t retry_count);

        /**
         * Set RF channel to transmit/receive on
         * @param channel  Channel offset from 2.4gHz (in mHz)
         */
        void set_channel(uint8_t channel);



        /**
         * Set Address to transmit on
         * @param address Pointer to location of address.
         */
        void set_tx_address(const uint8_t *address);

        /**
         * Power the NRF24L01 chip up or down
         * @param value
         */
        void power(bool value);

        /**
         * Set operating mode:
         * 0: None
         * 1: Primary Transmit
         * 2: Primary Receive
         * @param newMode
         */
        void set_mode(uint8_t newMode);

        /**
         * Get status of FIFO registers
         * @return FIFO status byte
         */
        uint8_t fifo_status();


        /**
         * Read width of currently available RX payload
         * @return The length in bytes
         */
        uint8_t read_rx_payload_width();


    };
}

#endif //PROJECT_NRF24L01_HPP
