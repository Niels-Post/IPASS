//
// Created by Niels on 6/9/2019.
//

#ifndef OOPC_OPDRACHTEN_SPI_BASE_HPP
#define OOPC_OPDRACHTEN_SPI_BASE_HPP

#include <hwlib.hpp>

namespace hwlib_ex {
    struct spi_mode {
        uint8_t clock_polarity = false;
        bool clock_phase = false;
        uint32_t half_time_ns = 1000;

        spi_mode();

        spi_mode(uint8_t clockPolarity, bool clockPhase, uint32_t halfTimeNs);
    };

    class spi_base_bus {
    protected:
        spi_mode mode;
        virtual void write_read(size_t n, const uint8_t *data_out, uint8_t *data_in);
        virtual void write_read_reverse(size_t n, const uint8_t *data_out, uint8_t *data_in);
    public:
        class spi_transaction final {
        private:
            spi_base_bus &bus;
        public:
            hwlib::pin_out &csn;
            explicit spi_transaction(spi_base_bus &bus, hwlib::pin_out &csn);

            ~spi_transaction();

            spi_transaction &write_read(size_t n, const uint8_t *data_out, uint8_t *data_in);

            spi_transaction &write_read_reverse(size_t n, const uint8_t *data_out, uint8_t *data_in);

            spi_transaction &write(size_t n, const uint8_t *data_out);

            spi_transaction &write_reverse(size_t n, const uint8_t *data_out);

            spi_transaction &read(size_t n, uint8_t *data_in);

            spi_transaction &read_reverse(size_t n, uint8_t *data_in);

            uint8_t read_byte(const uint8_t *data_out = nullptr);

            spi_transaction &write_byte(const uint8_t &byte, uint8_t *data_in = nullptr);
        };

    protected:
        virtual void onStart(spi_transaction &transaction);
        virtual void onEnd(spi_transaction &transaction);
    public:


        spi_transaction transaction(hwlib::pin_out &csn);

        explicit spi_base_bus(spi_mode mode);
    };

}

#endif //OOPC_OPDRACHTEN_SPI_BASE_HPP
