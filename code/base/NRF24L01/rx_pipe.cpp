
#include "rx_pipe.hpp"
#include "NRF24L01_Definitions.hpp"

namespace nrf24l01 {
    rx_pipe &rx_pipe::auto_acknowledgement(bool value) {
        uint8_t full_register;
        nrf.read_register(NRF_REGISTER::EN_AA, &full_register);
        if (value) {
            full_register |= 1 << pipe_number;
        } else {
            full_register &= ~(1 << pipe_number);
        }
        nrf.write_register(NRF_REGISTER::EN_AA, &full_register);
        return *this;
    }

    rx_pipe &rx_pipe::enabled(bool value) {
        uint8_t full_register;
        nrf.read_register(NRF_REGISTER::EN_RXADDR, &full_register);
        if (value) {
            full_register |= 1 << pipe_number;
        } else {
            full_register &= ~(1 << pipe_number);
        }
        nrf.write_register(NRF_REGISTER::EN_RXADDR, &full_register);
        return *this;
    }

    nrf_address rx_pipe::getAddress() {
        // TODO Implement Address width

        if(pipe_number > 1) {
            nrf_address base_address;
            uint8_t end;
            nrf.read_register(NRF_REGISTER::RX_ADDR_P1, base_address.address_bytes);
            nrf.read_register(NRF_REGISTER::RX_ADDR_P0 + pipe_number, &end);
            return {base_address, end};
        }

        uint8_t address[5] = {0};
        nrf.read_register(NRF_REGISTER::RX_ADDR_P0 + pipe_number, address, true);
        return {address};
    }

    rx_pipe &rx_pipe::setAddress(const nrf_address &address) {
        nrf.write_register(NRF_REGISTER::RX_ADDR_P0 + pipe_number, address.address_bytes, true);
        return *this;
    }


    rx_pipe &rx_pipe::setPayloadWidth(const uint8_t &width) {
        nrf.write_register(NRF_REGISTER::RX_PW_P0 + pipe_number, width & uint8_t(0x0F));
        return *this;
    }

    rx_pipe &rx_pipe::getPayloadWidth(uint8_t *out) {
        nrf.read_register(NRF_REGISTER::RX_PW_P0 + pipe_number, out);
        return *this;
    }

    rx_pipe &rx_pipe::setDynamicPacketLength(bool value) {
        uint8_t previousVal;
        nrf.read_register(NRF_REGISTER::DYNPD, &previousVal);
        if (value) {
            previousVal |= 1 << pipe_number;
        } else {
            previousVal &= ~(1 << pipe_number);
        }
        nrf.write_register(NRF_REGISTER::DYNPD, &previousVal);
        return *this;
    }
}