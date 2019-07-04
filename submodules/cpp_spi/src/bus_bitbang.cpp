/*
 *
 * Copyright Niels Post 2019.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 *
*/

#include <spi/bus_bitbang.hpp>


spi::bus_bitbang::bus_bitbang(hwlib::pin_out &_sclk, hwlib::pin_out &_mosi, hwlib::pin_in &_miso,
                              const spi::spi_mode &mode)
        : spi_base_bus(mode), sclk(_sclk),
          mosi(_mosi), miso(_miso) {
    sclk.write(mode.clock_polarity);
    mosi.write(false);
}


void spi::bus_bitbang::write_read(size_t n, const uint8_t *data_out, uint8_t *data_in) {
    for (uint8_t i = 0; i < n; ++i) {
        uint8_t d =
                (data_out == nullptr)
                ? 0
                : *data_out++;

        write_read_byte(d);

        if (data_in != nullptr) {
            *data_in++ = d;
        }
    }
    wait_half_period();
}

void spi::bus_bitbang::write_read_reverse(size_t n, const uint8_t *data_out, uint8_t *data_in) {

    if (data_in != nullptr) {
        data_in += n;
    }
    if (data_out != nullptr) {
        data_out += n;
    }
    for (uint8_t i = 0; i < n; ++i) {
        uint8_t d =
                (data_out == nullptr)
                ? 0
                : *--data_out;

        write_read_byte(d);

        if (data_in != nullptr) {
            *--data_in = d;
        }

    }
//    wait_half_period();
}


void spi::bus_bitbang::wait_half_period() {
    hwlib::wait_ns(mode.half_time_ns);
}

void spi::bus_bitbang::write_read_byte(uint8_t &d) {
    if (mode.clock_phase) {
        for (uint_fast8_t j = 0; j < 8; ++j) {
            sclk.write(!mode.clock_polarity);
            mosi.write((d & 0x80) != 0);
//            wait_half_period();
//            wait_half_period();
            sclk.write(mode.clock_polarity);
            d = d << 1;
            if (miso.read()) {
                d |= 0x01;
            }
        }
    } else {
        for (uint_fast8_t j = 0; j < 8; ++j) {
            mosi.write((d & 0x80) != 0);
//            wait_half_period();
            sclk.write(!mode.clock_polarity);
//            wait_half_period();
            d = d << 1;
            if (miso.read()) {
                d |= 0x01;
            }
            sclk.write(mode.clock_polarity);
        }
    }
}

