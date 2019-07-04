/*
 *
 * Copyright Niels Post 2019.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 *
*/

#include <spi/bus_base.hpp>

namespace spi {
    spi_base_bus::spi_transaction &
    spi_base_bus::spi_transaction::write_read_reverse(size_t n, const uint8_t *data_out, uint8_t *data_in) {
        bus.write_read_reverse(n, data_out, data_in);
        return *this;
    }

    spi_base_bus::spi_transaction &
    spi_base_bus::spi_transaction::write(size_t n, const uint8_t *data_out) {
        bus.write_read(n, data_out, nullptr);
        return *this;
    }

    spi_base_bus::spi_transaction &
    spi_base_bus::spi_transaction::write_reverse(size_t n, const uint8_t *data_out) {
        bus.write_read_reverse(n, data_out, nullptr);
        return *this;
    }

    spi_base_bus::spi_transaction &
    spi_base_bus::spi_transaction::read(size_t n, uint8_t *data_in) {
        bus.write_read(n, nullptr, data_in);
        return *this;
    }

    spi_base_bus::spi_transaction &
    spi_base_bus::spi_transaction::read_reverse(size_t n, uint8_t *data_in) {
        bus.write_read_reverse(n, nullptr, data_in);
        return *this;
    }

    spi_base_bus::spi_transaction &
    spi_base_bus::spi_transaction::write_read(size_t n, const uint8_t *data_out, uint8_t *data_in) {
        bus.write_read(n, data_out, data_in);
        return *this;
    }

    spi_base_bus::spi_transaction::spi_transaction(spi_base_bus &bus, hwlib::pin_out &csn) : bus(bus), csn(csn) {
        bus.onStart(*this);
    }

    spi_base_bus::spi_transaction::~spi_transaction() {
        bus.onEnd(*this);
    }

    uint8_t spi_base_bus::spi_transaction::read_byte(const uint8_t *data_out) {
        uint8_t value[1];
        write_read(1, data_out, value);
        return *value;
    }

    spi_base_bus::spi_transaction &spi_base_bus::spi_transaction::write_byte(const uint8_t &byte, uint8_t *data_in) {
        uint8_t temp = byte;
        return write_read(1, &temp, data_in);
    }

    spi_base_bus::spi_base_bus(const spi_mode mode) : mode(mode) {}

    spi_base_bus::spi_transaction spi_base_bus::transaction(hwlib::pin_out &csn) {
        return spi_base_bus::spi_transaction(*this, csn);
    }

    void spi_base_bus::write_read(size_t n, const uint8_t *data_out, uint8_t *data_in) {
        uint8_t outbuffer[n];
        uint8_t inbuffer[n];
        if (data_out != nullptr) {
            for (size_t i = 0; i < n; i++) {
                outbuffer[i] = data_out[n - 1 - i];
            }
        }

        write_read_reverse(n, outbuffer, inbuffer);

        if (data_in != nullptr) {
            for (size_t i = 0; i < n; i++) {
                data_in[i] = inbuffer[n - 1 - i];
            }
        }
    }

    void spi_base_bus::write_read_reverse(size_t n, const uint8_t *data_out, uint8_t *data_in) {
        uint8_t outbuffer[n];
        uint8_t inbuffer[n];
        if (data_out != nullptr) {
            for (size_t i = 0; i < n; i++) {
                outbuffer[i] = data_out[n - 1 - i];
            }
        }

        write_read(n, outbuffer, inbuffer);

        if (data_in != nullptr) {
            for (size_t i = 0; i < n; i++) {
                data_in[i] = inbuffer[n - 1 - i];
            }
        }
    }

    void spi_base_bus::onStart(spi::spi_base_bus::spi_transaction &transaction) {
        transaction.csn.write(false);
    }

    void spi_base_bus::onEnd(spi::spi_base_bus::spi_transaction &transaction) {
        transaction.csn.write(true);
    }

    spi_mode::spi_mode(bool clockPolarity, bool clockPhase, uint32_t halfTimeNs) : clock_polarity(clockPolarity),
                                                                                   clock_phase(clockPhase),
                                                                                   half_time_ns(halfTimeNs) {}

    spi_mode::spi_mode() = default;
}
