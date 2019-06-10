
#ifndef PROJECT_RXPIPEALL_HPP
#define PROJECT_RXPIPEALL_HPP

#include "NRF24L01Plus.hpp"
#include "NRF24L01_Definitions.hpp"

namespace NRF24L01 {
    /**
     * Used to interact with all RX pipes in an NRF24L01Plus at the same time
     * All functions in this class will return the instance of the class, to allow for method chaining
     */
    class RXPipeAll {
    private:
        NRF24L01Plus &nrf;

        uint8_t fill(bool val);

    public:
        /**
         * Create RXPipeAll from NRF24L01Plus
         * @param nrf Instance of connection to an NRF24L01Plus chip
         */
        RXPipeAll(NRF24L01Plus &nrf);

        /**
         * Enable or disable  auto acknowledgement feature for all RX pipes in the NRF24.
         * \copydetails RXPipe::auto_acknowledgement
         * @param value true for enabled, false for disabled
         * @return This RXPipeAll instance, for method chaining
         */
        RXPipeAll &auto_acknowledgement(bool value);

        /**
         * Enable or disable all RX pipes in the NRF24.
         * \copydetails RXPipe::enabled
         * @param value true for enabled, false for disabled
         * @return This RXPipeAll instance, for method chaining
         */
        RXPipeAll &enabled(bool value);

        /**
         * Set Payload Width parameter for all RX pipes in the NRF24
         * \copydetails RXPipe::setPayloadWidth
         * @param width The address width in bytes
         * @return This RXPipeAll instance, for method chaining
         */
        RXPipeAll &setPayloadWidth(const uint8_t &width);

        /**
         * Enable or disable Dynamic Packet Length feature for all RX pipes in the NRF24
         * @param value true for enabled, false for disabled
         * @return This RXPipeAll instance, for method chaining
         */
        RXPipeAll &setDynamicPacketLength(bool value);
    };
}

#endif //PROJECT_RXPIPEALL_HPP
