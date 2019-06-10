
#include "RXPipeAll.hpp"

namespace NRF24L01 {

    uint8_t RXPipeAll::fill(bool val) {
        return uint8_t(val ? 0x3F : 0x00);
    }

    RXPipeAll::RXPipeAll(NRF24L01Plus &nrf) : nrf(nrf) {}

    RXPipeAll &RXPipeAll::auto_acknowledgement(bool value) {
        nrf.write_register(NRF_REGISTER::EN_AA, fill(value));
        return *this;
    }

    RXPipeAll &RXPipeAll::enabled(bool value) {
        nrf.write_register(NRF_REGISTER::EN_RXADDR, fill(value));
        return *this;
    }

    RXPipeAll &RXPipeAll::setPayloadWidth(const uint8_t &width) {
        for (uint8_t i = 0; i < 6; i++) {
            nrf.write_register(NRF_REGISTER::RX_PW_P0 + i, width);
        }
        return *this;
    }

    RXPipeAll &RXPipeAll::setDynamicPacketLength(bool value) {
        nrf.write_register(NRF_REGISTER::DYNPD, fill(value));
        return *this;
    }

}