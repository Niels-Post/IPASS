
#include "RXPipe.hpp"
#include "NRF24L01_Definitions.hpp"

namespace NRF24L01 {
    RXPipe &RXPipe::auto_acknowledgement(bool value) {
        uint8_t full_register;
        nrf.read_register(NRF_REGISTER::EN_AA, &full_register);
        if (value) {
            full_register |= 1 << pipe_number;
        } else {
            full_register &= 0 << pipe_number;
        }
        nrf.write_register(NRF_REGISTER::EN_AA, &full_register);
        return *this;
    }

    RXPipe &RXPipe::enabled(bool value) {
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

    RXPipe &RXPipe::getAddress(uint8_t out[5]) {
        // Todo probably do this properly
        uint8_t address_length;
        nrf.read_register(NRF_REGISTER::SETUP_AW, &address_length);
        address_length += 2;
        auto *address = new uint8_t[address_length];
        nrf.read_register(pipe_number == 0 ? NRF_REGISTER::RX_ADDR_P0 : NRF_REGISTER::RX_ADDR_P1, address);
//        reverse_char_array(address, address_length);
        if (pipe_number > 1) {
            nrf.read_register(NRF_REGISTER::RX_ADDR_P0 + pipe_number, address + address_length - 1);
        }
        return *this;

    }

    RXPipe &RXPipe::setAddress(uint8_t *address) {
        nrf.write_register(NRF_REGISTER::RX_ADDR_P0 + pipe_number, address, true);
        return *this;
    }


    RXPipe &RXPipe::setPayloadWidth(const uint8_t &width) {
        nrf.write_register(NRF_REGISTER::RX_PW_P0 + pipe_number, width & uint8_t(0x0F));
        return *this;
    }

    RXPipe &RXPipe::getPayloadWidth(uint8_t *out) {
        nrf.read_register(NRF_REGISTER::RX_PW_P0 + pipe_number, out);
        return *this;
    }

    RXPipe &RXPipe::setDynamicPacketLength(bool value) {
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