#include <hwlib.hpp>



#include "NRF24L01/nrf24l01plus.hpp"
#include "NRF24L01/NRF24L01_Definitions.hpp"
#include "NRF24L01/rx_all_pipes.hpp"
#include "NRF24L01/rx_pipe.hpp"
#include "SPI/spi_bitbang.hpp"
#include "NRF_Mesh/mesh_nrf_connectivity.hpp"
#include "Util/huts.hpp"

using namespace nrf24l01;



void transmit(mesh_nrf_connectivity &connectivity) {
    mesh::mesh_message test = {12, 0, 0x0F, 0, nullptr};

    connectivity.add_direct_connection(0x0F);
    for(;;) {
        connectivity.unicast(test, test.receiver);
        test.sender++;
        hwlib::wait_ms(100);
    }

}

void toontje() {}

void receive(mesh_nrf_connectivity &connectivity){
    connectivity.add_direct_connection(0x0F);
    for(;;) {
       if(connectivity.is_message_available()) {
            connectivity.next_message();
            hwlib::wait_ms(100);
            huts::a_niffau();
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

    auto nrf = nrf24l01plus(bus, csn, ce);
    hwlib::wait_ms(2000);
    nrf.power(true);
    auto mesh_connectivity = mesh_nrf_connectivity(nrf);

    transmit(mesh_connectivity);
//    receive(mesh_connectivity);

//    nrf.auto_retransmit(0, 0);
//    rx_all_pipes(nrf)
//            .auto_acknowledgement(false)
//            .enabled(false)
//            .setDynamicPacketLength(false);
//
//    nrf.set_channel(8);
//
//
//    uint8_t address[5] = {0x78, 0x78, 0x78, 0x78, 0x78};
//    rx_pipe(nrf, 0)
//            .enabled(true)
//            .setPayloadWidth(6)
//            .setAddress(address);
//    nrf.set_tx_address(address);
//
//
////    transmit(nrf, ce);
//    receive(nrf,ce);



}
