//
// Created by Niels on 6/9/2019.
//

#ifndef OOPC_OPDRACHTEN_SPI_BITBANG_HPP
#define OOPC_OPDRACHTEN_SPI_BITBANG_HPP

#include "spi_base.hpp"

namespace hwlib_ex {
    class spi_bitbang : public spi_base_bus {
    protected:
        hwlib::pin_direct_from_out_t sclk;
        hwlib::pin_direct_from_out_t mosi;
        hwlib::pin_direct_from_in_t miso;
    public:
        spi_bitbang(hwlib::pin_out &_sclk, hwlib::pin_out &_mosi, hwlib::pin_in &_miso,
                    const hwlib_ex::spi_mode &mode);

    protected:
        void wait_half_period();

        void write_read_byte(uint8_t &d);

        void write_read(size_t n, const uint8_t *data_out, uint8_t *data_in) override;

        void write_read_reverse(size_t n, const uint8_t *data_out, uint8_t *data_in) override;

    };
}

#endif //OOPC_OPDRACHTEN_SPI_BITBANG_HPP
