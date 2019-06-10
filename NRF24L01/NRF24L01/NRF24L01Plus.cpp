#include <hwlib.hpp>
#include "NRF24L01Plus.hpp"

namespace NRF24L01 {

    NRF24L01Plus::NRF24L01Plus(hwlib_ex::spi_base_bus &bus, hwlib::pin_out &csn, hwlib::pin_out &ce) : bus(bus),
                                                                                               csn(csn),
                                                                                               ce(ce), last_status(0) {}

    void NRF24L01Plus::send_command(const uint8_t &command_word, uint8_t *data_out, const uint8_t &n,
                                    uint8_t *data_in, bool lsbyte_first) {
        auto transaction = bus.transaction(csn);
        transaction.write_read(1, &command_word, &last_status);
        if(lsbyte_first) {
            transaction.write_read_reverse(n, data_out, data_in);
        } else {
            transaction.write_read(n, data_out, data_in);
        }
    }

    void NRF24L01Plus::read_register(const uint8_t &address, uint8_t *out, bool lsbyte_first) {
        send_command(NRF_INSTRUCTION::R_REGISTER | address, nullptr, register_bytes(address), out, lsbyte_first);
    }

    void NRF24L01Plus::write_register(const uint8_t &address, uint8_t *data, bool lsbyte_first ) {
        send_command(NRF_INSTRUCTION::W_REGISTER | address, data, register_bytes(address), nullptr, lsbyte_first);
    }


    uint8_t NRF24L01Plus::register_bytes(const uint8_t &address) {
        if (address == NRF_REGISTER::RX_ADDR_P0 || address == NRF_REGISTER::RX_ADDR_P1 ||
            address == NRF_REGISTER::TX_ADDR) {
            return 5;
        }
        return 1;
    }

    void NRF24L01Plus::no_operation() {
        send_command(NRF_INSTRUCTION::RF24_NOP, nullptr, 0, nullptr);
    }

    void NRF24L01Plus::read_rx_payload(uint8_t *data) {
        send_command(NRF_INSTRUCTION::R_RX_PAYLOAD, nullptr, 6, data, true);
    }

    void NRF24L01Plus::flush_rx() {
        send_command(NRF_INSTRUCTION::FLUSH_RX);
    }

    void NRF24L01Plus::write_tx_payload(uint8_t *data, const uint8_t &size) {
        send_command(NRF_INSTRUCTION::W_TX_PAYLOAD, data, size, nullptr, true);
        send_buffered_tx_payload();
    }

    void NRF24L01Plus::flush_tx() {
        send_command(NRF_INSTRUCTION::FLUSH_TX);
    }

    void NRF24L01Plus::write_register(const uint8_t &address, uint8_t data) {
        ce.write(false);
        write_register(address, &data);
        ce.write(true);
    }

    void NRF24L01Plus::auto_retransmit(uint8_t retry_delay, uint8_t retry_count) {
        write_register(NRF_REGISTER::SETUP_RETR, retry_delay << 4 | retry_count);
    }

    void NRF24L01Plus::set_channel(uint8_t channel) {
        write_register(NRF_REGISTER::RF_CH, channel & uint8_t(0xEF));
    }

    void NRF24L01Plus::set_tx_address(uint8_t *address) {
        write_register(NRF_REGISTER::TX_ADDR, address);
    }

    void NRF24L01Plus::set_mode(uint8_t newMode) {
        if (currentMode == newMode) {
            return;
        }
        bool fromActiveState = currentMode == MODE_PRX || currentMode == MODE_PTX;

        if (fromActiveState) {
            ce.write(false);
            hwlib::wait_us(130);
        }


        uint8_t lastConfig;
        switch (newMode) {
            case 1: {
                read_register(NRF_REGISTER::CONFIG, &lastConfig);
                write_register(NRF_REGISTER::CONFIG, lastConfig & ~NRF_CONFIG::CONFIG_PRIM_RX);
                break;
            }
            case 2: {
                read_register(NRF_REGISTER::CONFIG, &lastConfig);
                write_register(NRF_REGISTER::CONFIG, lastConfig | NRF_CONFIG::CONFIG_PRIM_RX);
                ce.write(true);
                break;
            }
            case 0:
            default: {
                break;
            }

        }


    }

    void NRF24L01Plus::send_buffered_tx_payload() {
        ce.write(true);
        hwlib::wait_us(10);
        ce.write(false);
    }

    void NRF24L01Plus::power(bool value) {
        uint8_t lastConfig;
        read_register(NRF_REGISTER::CONFIG, &lastConfig);
        if (value) {
            lastConfig |= NRF_CONFIG::CONFIG_PWR_UP;
        } else {
            lastConfig &= ~NRF_CONFIG::CONFIG_PWR_UP;
        }
        write_register(NRF_REGISTER::CONFIG, &lastConfig);
    }


}

