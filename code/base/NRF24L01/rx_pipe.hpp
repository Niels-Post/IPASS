
#ifndef PROJECT_RXPIPE_HPP
#define PROJECT_RXPIPE_HPP

#include <hwlib.hpp>
#include "nrf24l01plus.hpp"

namespace nrf24l01 {
    /**
     * Interface for managing a single RX pipe on the NRF24L01
     * Can get and set all pipe specific settings, without interfering with other pipes
     * All functions in this class will return the instance of the class, to allow for method chaining
     */
    class rx_pipe {
        nrf24l01plus &nrf;
        uint8_t pipe_number;
    public:
        /**
         * Creates an RX pipe object
         * @param nrf Instance of NRF24L01 to create a pipe for
         * @param pipe_number Number of pipe (0-5)
         */
        rx_pipe(nrf24l01plus &nrf, uint8_t pipe_number) : nrf(nrf), pipe_number(pipe_number) {};

        /**
         * \brief Enable or disable auto acknowledgement for this pipe
         * \details Note: Enabling Auto Acknowledgement will force enable CRC checksums as well
         * To use this feature the transmitting chip needs to have this enabled as well
         * @param value True for enable, False for disable
         * @return This RXPipe instance, for method chaining
         */
        rx_pipe &auto_acknowledgement(bool value);

        /**
         * \brief Enable or disable the selected pipe
         * @param value True for enable, False for disable
         * @return This RXPipe instance, for method chaining
         * \details Note that to receive through an RX Pipe the Payload width for that pipe needs to be set (or Dynamic Payload Width enabled)
         */
        rx_pipe &enabled(bool value);

        /**
         * \brief Retrieves the full address of this pipe
         * @param out Pointer to where the address should be stored
         * @return This RXPipe instance, for method chaining
         * \details If pipe_number is between 2 and 5, the first 4 bytes of the address will be retrieved from pipe 1
         */
        nrf_address getAddress();

        /**
         * \brief Sets address of this pipe
         * @param address
         * @return This RXPipe instance, for method chaining
         * \details If pipe_number is 0 or 1, this will use a 5 byte address, else only 1 byte will be used
         */
        rx_pipe &setAddress(const nrf_address &address);

        /**
         * \brief Set the payload width for this RX Pipe
         * \details This value is ignored if Dynamic Payload Width is enabled
         * @param width Address width, in bytes
         * @return This RXPipe instance, for method chaining
         */
        rx_pipe &setPayloadWidth(const uint8_t &width);

        /**
         * Retrieves payload width for this RX pipe
         * @param out memory space to write payload width into
         * @return This RXPipe instance, for method chaining
         */
        rx_pipe &getPayloadWidth(uint8_t *out);

        /**
         * Enable Dynamic Packet Length feature for this pipe
         * @param value true for enabled, false for disabled
         * @return This RXPipe instance, for method chaining
         */
        rx_pipe &setDynamicPacketLength(bool value);
    };
}

#endif //PROJECT_RXPIPE_HPP
