
#include "rx_all_pipes.hpp"

namespace nrf24l01 {

    uint8_t rx_all_pipes::fill(bool val) {
        return uint8_t(val ? 0x3F : 0x00);
    }

    rx_all_pipes::rx_all_pipes(nrf24l01plus &nrf) : nrf(nrf) {}

    rx_all_pipes &rx_all_pipes::auto_acknowledgement(bool value) {
        nrf.write_register(NRF_REGISTER::EN_AA, fill(value));
        return *this;
    }

    rx_all_pipes &rx_all_pipes::enabled(bool value) {
        nrf.write_register(NRF_REGISTER::EN_RXADDR, fill(value));
        return *this;
    }

    rx_all_pipes &rx_all_pipes::setPayloadWidth(const uint8_t &width) {
        for (uint8_t i = 0; i < 6; i++) {
            nrf.write_register(NRF_REGISTER::RX_PW_P0 + i, width);
        }
        return *this;
    }

    rx_all_pipes &rx_all_pipes::setDynamicPacketLength(bool value) {
        nrf.write_register(NRF_REGISTER::DYNPD, fill(value));
        return *this;
    }

}