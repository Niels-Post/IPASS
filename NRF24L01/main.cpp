#include <hwlib.hpp>
#include "NRF24L01/NRF24L01Plus.hpp"
#include "NRF24L01/NRF24L01_Definitions.hpp"
#include "NRF24L01/RXPipeAll.hpp"
#include "NRF24L01/RXPipe.hpp"
#include "SPI/spi_bitbang.hpp"

using namespace NRF24L01;

void transmit(NRF24L01Plus &nrf, hwlib::pin_out &ce) {
    nrf.set_mode(nrf.MODE_PTX);
    nrf.power(true);

//    uint8_t data_receive;
    uint8_t data[6] = {'H', 'a', 'l', 'l', 'o', 'a'};
    for (;;) {
        nrf.write_tx_payload(data, 6);
        nrf.read_register(NRF_REGISTER::FIFO_STATUS);
        hwlib::wait_ms(5);
    }
}


void receive(NRF24L01Plus &nrf, hwlib::pin_out &ce) {
    nrf.set_mode(nrf.MODE_PRX);
    nrf.power(true);
    uint8_t status_check;
    uint8_t data[32];
    for (;;) {
        nrf.read_register(NRF_REGISTER::FIFO_STATUS, &status_check);
        if ((status_check & NRF_FIFO_STATUS::RX_EMPTY) == 0) {
            nrf.read_rx_payload(data);
        }
        hwlib::wait_ms(10);
    }
}


int main() {
    auto ce = hwlib::target::pin_out(hwlib::target::pins::a1);
    auto csn = hwlib::target::pin_out(hwlib::target::pins::a2);
    auto sclk = hwlib::target::pin_out(hwlib::target::pins::a3);
    auto mosi = hwlib::target::pin_out(hwlib::target::pins::a4);
    auto miso = hwlib::target::pin_in(hwlib::target::pins::a5);
    auto bus = hwlib_ex::spi_bitbang(sclk, mosi, miso, csn, hwlib_ex::spi_mode(false, false, 1));

    auto nrf = NRF24L01Plus(bus, csn, ce);
    hwlib::wait_ms(2000);

    nrf.auto_retransmit(0, 0);
    RXPipeAll(nrf)
            .auto_acknowledgement(false)
            .enabled(false)
            .setDynamicPacketLength(false);

    nrf.set_channel(8);


    uint8_t address[5] = {0x78, 0x78, 0x78, 0x78, 0x78};
    RXPipe(nrf, 0)
            .enabled(true)
            .setPayloadWidth(6)
            .setAddress(address);
    nrf.set_tx_address(address);


//    transmit(nrf, ce);
    receive(nrf,ce);



}
