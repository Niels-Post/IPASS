#include <hwlib.hpp>
#include "nrf24l01plus.hpp"

namespace nrf24l01 {

    nrf24l01plus::nrf24l01plus(hwlib_ex::spi_base_bus &bus, hwlib::pin_out &csn, hwlib::pin_out &ce) : bus(bus),
                                                                                               csn(csn),
                                                                                               ce(ce), last_status(0) {}

    void nrf24l01plus::send_command(const uint8_t &command_word, const uint8_t *data_out, const uint8_t &n,
                                    uint8_t *data_in, bool lsbyte_first) {
        auto transaction = bus.transaction(csn);
        transaction.write_read(1, &command_word, &last_status);
        if(lsbyte_first) {
            transaction.write_read_reverse(n, data_out, data_in);
        } else {
            transaction.write_read(n, data_out, data_in);
        }
    }

    void nrf24l01plus::read_register(const uint8_t &address, uint8_t *out, bool lsbyte_first) {
        send_command(NRF_INSTRUCTION::R_REGISTER | address, nullptr, register_bytes(address), out, lsbyte_first);
    }

    void nrf24l01plus::write_register(const uint8_t &address, const uint8_t *data, bool lsbyte_first ) {
        send_command(NRF_INSTRUCTION::W_REGISTER | address, data, register_bytes(address), nullptr, lsbyte_first);
    }


    uint8_t nrf24l01plus::register_bytes(const uint8_t &address) {
        if (address == NRF_REGISTER::RX_ADDR_P0 || address == NRF_REGISTER::RX_ADDR_P1 ||
            address == NRF_REGISTER::TX_ADDR) {
            return 5;
        }
        return 1;
    }

    void nrf24l01plus::no_operation() {
        send_command(NRF_INSTRUCTION::RF24_NOP, nullptr, 0, nullptr);
    }

    void nrf24l01plus::read_rx_payload(uint8_t *data, const uint8_t payload_width) {

        send_command(NRF_INSTRUCTION::R_RX_PAYLOAD, nullptr, payload_width, data, true);
    }

    void nrf24l01plus::flush_rx() {
        send_command(NRF_INSTRUCTION::FLUSH_RX);
    }

    void nrf24l01plus::write_tx_payload(uint8_t *data, const uint8_t &size) {
        send_command(NRF_INSTRUCTION::W_TX_PAYLOAD, data, size, nullptr, true);
        send_buffered_tx_payload();
    }

    void nrf24l01plus::flush_tx() {
        send_command(NRF_INSTRUCTION::FLUSH_TX);
    }

    void nrf24l01plus::write_register(const uint8_t &address, uint8_t data) {
        write_register(address, &data);
    }

    void nrf24l01plus::auto_retransmit(uint8_t retry_delay, uint8_t retry_count) {
        write_register(NRF_REGISTER::SETUP_RETR, retry_delay << 4 | retry_count);
    }

    void nrf24l01plus::set_channel(uint8_t channel) {
        write_register(NRF_REGISTER::RF_CH, channel & uint8_t(0xEF));
    }

    void nrf24l01plus::set_tx_address(const nrf_address &address) {
        write_register(NRF_REGISTER::TX_ADDR, address.address_bytes);
    }

    void nrf24l01plus::set_mode(uint8_t newMode) {
        if (currentMode == newMode) {
            return;
        }
        bool fromActiveState = currentMode == MODE_PRX || currentMode == MODE_PTX;

        currentMode = newMode;
        if (fromActiveState) {
            ce.write(false);
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
                hwlib::wait_us(130);
                break;

            case 0:
            default: {
                break;
            }

        }


    }

    void nrf24l01plus::send_buffered_tx_payload() {
        ce.write(true);
        hwlib::wait_us(10);
        ce.write(false);
    }

    void nrf24l01plus::power(bool value) {
        uint8_t lastConfig;
        read_register(NRF_REGISTER::CONFIG, &lastConfig);
        if (value) {
            lastConfig |= NRF_CONFIG::CONFIG_PWR_UP;
        } else {
            lastConfig &= ~NRF_CONFIG::CONFIG_PWR_UP;
        }
        write_register(NRF_REGISTER::CONFIG, &lastConfig);
    }

    uint8_t nrf24l01plus::fifo_status() {
        uint8_t status;
        read_register(NRF_REGISTER::FIFO_STATUS, &status);
        return status;
    }

    uint8_t nrf24l01plus::read_rx_payload_width() {
        uint8_t pw;
        send_command(nrf24l01::NRF_INSTRUCTION::R_RX_PL_WID, nullptr, 1, &pw);
        return pw;
    }



}

