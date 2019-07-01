//
// Created by Niels on 6/28/2019.
//

#ifndef IPASS_SPI_BLUE_PILL_HPP
#define IPASS_SPI_BLUE_PILL_HPP

#include <hwlib.hpp>
#include "spi_base.hpp"

class spi_blue_pill : public hwlib_ex::spi_base_bus {
    bool start;
    uint8_t data_out_empty[32] = {0};

public:
    spi_blue_pill(hwlib_ex::spi_mode mode);

private:

    void write_read(size_t n, const uint8_t *data_out, uint8_t *data_in) override;

protected:
    void onStart(spi_transaction &transaction) override;

    void onEnd(spi_transaction &transaction) override;

};


#endif //IPASS_SPI_BLUE_PILL_HPP
