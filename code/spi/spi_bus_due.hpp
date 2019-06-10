
#ifndef PROJECT_SPI_BUS_DUE_HPP
#define PROJECT_SPI_BUS_DUE_HPP

#include <hwlib.hpp>

namespace hwlib {
    class spi_bus_due {
        Spi *spi;
    public:
        spi_bus_due() {
            spi = SPI0;

            //Disable SPI interrupts
            spi->SPI_IDR = 0xFFFFFFFF;

            // Enable Clock to peripheral and clock
            PMC->PMC_PCER0 |= 1 << ID_SPI0;

            // Set clock divider todo: allow parameterization
            spi->SPI_CSR[0] |= (1 << 12);

            // Disable PIO on needed ports
            PIOA->PIO_PDR = PIO_PA25; // MISO
            PIOA->PIO_PDR = PIO_PA26; // MOSI
            PIOA->PIO_PDR = PIO_PA27; // SCK
            PIOA->PIO_PDR = PIO_PA28; // CS

            // Connect pins to the right peripheral
            PIOA->PIO_ABSR &= ~PIO_PA25; // MISO
            PIOA->PIO_ABSR &= ~PIO_PA26; // MOSI
            PIOA->PIO_ABSR &= ~PIO_PA27; // SCK
            PIOA->PIO_ABSR &= ~PIO_PA28; // CS

            // Enable SPI
            spi->SPI_CR = SPI_CR_SPIEN;

            // Put SPI into master mode, and select peripheral 0 TODO: allow multiple peripherals
            spi->SPI_MR = SPI_MR_MSTR | SPI_MR_PCS(0);


        }

    public:
        void putc(char c) {
            while ((spi->SPI_SR &= 2) != 2) {}
            spi->SPI_TDR = c;
        }

        spi_bus_due &operator<<(char c) {
            putc(c);
            return *this;
        }

        spi_bus_due &operator<<(char *c) {
            for (char *p = c; *p; p++) {
                putc(*p);
            }
            return *this;
        }
    };

    class cout_using_spi : public ostream {
        spi_bus_due bus_due;
    public:

        void putc(char c) override {
            bus_due.putc(c);
        }

        void flush() override {}

    };
}

#endif //PROJECT_SPI_BUS_DUE_HPP
