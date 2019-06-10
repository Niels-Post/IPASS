#include <hwlib.hpp>
#include "spi_bus_due.hpp"

namespace hwlib {

    class spi_bus_hardware {
        Spi *spi;
    public:
        spi_bus_hardware() {
            spi = SPI0;
            hwlib::kitt
            //Disable SPI interrupts
            spi->SPI_IDR = 0xFFFFFFFF;

            // Enable Clock to peripheral and clock
            PMC->PMC_PCER0 |= 1 << ID_PIOA;
            PMC->PMC_PCER0 |= 1 << ID_SPI0;

            // Set clock divider todo: allow parameterization
            spi->SPI_CSR[0] |= (1 << 12);

            // Disable PIO on needed ports
            PIOA->PIO_PDR = PIO_PA25;
            PIOA->PIO_PDR = PIO_PA26;
            PIOA->PIO_PDR = PIO_PA27;
            PIOA->PIO_PDR = PIO_PA28;
            PIOA->PIO_PDR = PIO_PA29;
            PIOA->PIO_PDR = PIO_PB21;
            PIOA->PIO_PDR = PIO_PB23;


            // Connect pins to the right peripheral
            PIOA->PIO_ABSR &= ~PIO_PA25;
            PIOA->PIO_ABSR &= ~PIO_PA26;
            PIOA->PIO_ABSR &= ~PIO_PA27;
            PIOA->PIO_ABSR &= ~PIO_PA28;
            PIOA->PIO_ABSR &= ~PIO_PA29;
            PIOA->PIO_ABSR |= PIO_PB21;
            PIOA->PIO_ABSR |= PIO_PB23;


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

        spi_bus_hardware &operator<<(char c) {
            putc(c);
            return *this;
        }

        spi_bus_hardware &operator<<(char *c) {
            for (char *p = c; *p; p++) {
                putc(*p);
            }
            return *this;
        }
    };
}


void print(uint32_t data) {
    for (int i = 31; i >= 0; i--) {
        hwlib::cout << ((data >> i) & 1);
        if ((i) % 8 == 0)
            hwlib::cout << " ";
    }
}

void print_status(Spi *spi) {
    print(spi->SPI_WPSR);
    hwlib::cout << "-";
    print(spi->SPI_SR);
    hwlib::cout << "-";
    print(spi->SPI_RDR);
    hwlib::cout << "-";
    print(spi->SPI_MR);
    hwlib::cout << hwlib::endl;
}

int main() {
    hwlib::cout_using_spi cout;
    hwlib::wait_ms(3000);
    for (;;) {
        cout << "test";
        hwlib::wait_ms(1000);
    }

}
