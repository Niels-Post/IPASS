//
// Created by Niels on 6/24/2019.
//

#ifndef IPASS_SPI_TESTING_HPP
#define IPASS_SPI_TESTING_HPP

#include "spi_base.hpp"

class spi_testing : public hwlib_ex::spi_base_bus {
public:
    std::array<uint8_t, 128> out_buffer = {0};
    std::array<uint8_t, 128> in_buffer = {0};
    size_t out_buffer_size = 0;
    size_t in_buffer_index = 0;
    size_t in_buffer_size = 0;

    spi_testing() : spi_base_bus(hwlib_ex::spi_mode(false, false, 1)) {}

protected:
    void write_read(size_t n, const uint8_t *data_out, uint8_t *data_in) override {

        for (size_t i = 0; i < n; i++) {
            if (data_out != nullptr) {
                out_buffer[out_buffer_size] = *data_out++;
            }
            out_buffer_size++;
            if (data_in != nullptr) {
                *data_in++ = in_buffer[in_buffer_index];
            }
            in_buffer_index++;

        }
    }

public:
    template<size_t n>
    void append_in_buffer(const std::array<uint8_t, n> &to_add) {
        for (const uint8_t &val : to_add) {
            in_buffer[in_buffer_size++] = val;
        }
    }

    template<size_t n>
    bool match(const std::array<uint8_t, n> &match_array, bool is_out) {
        std::array<uint8_t, 128> buffer = is_out ? out_buffer : in_buffer;
        for (size_t i = 0; i < n; i++) {
            if (match_array[i] != buffer[i]) {
                return false;
            }
        }
        return true;
    }


    void clear() {
        for (uint8_t &val : out_buffer) {
            val = 0;
        }
        for (uint8_t &val : in_buffer) {
            val = 0;
        }
        out_buffer_size = 0;
        in_buffer_index = 0;
        in_buffer_size = 0;
    }

};


#endif //IPASS_SPI_TESTING_HPP
