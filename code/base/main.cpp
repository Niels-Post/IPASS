#include <hwlib.hpp>
#include "NRF24L01/nrf24l01plus.hpp"
#include "NRF_Mesh/mesh_nrf_connectivity.hpp"
#include "Mesh/router/rip_router.hpp"

#include "Mesh/mesh_network.hpp"
#include "Util/huts.hpp"

using namespace nrf24l01;








int main() {
    auto ce = hwlib::target::pin_out(hwlib::target::pins::a1);
    auto csn = hwlib::target::pin_out(hwlib::target::pins::a2);
    auto sclk = hwlib::target::pin_out(hwlib::target::pins::a3);
    auto mosi = hwlib::target::pin_out(hwlib::target::pins::a4);
    auto miso = hwlib::target::pin_in(hwlib::target::pins::a5);
    auto bus = hwlib_ex::spi_bitbang(sclk, mosi, miso, csn, hwlib_ex::spi_mode(false, false, 1));



    auto nrf = nrf24l01plus(bus, csn, ce);
    hwlib::wait_ms(2000);
    huts::a_niffau();
    nrf.power(true);


    mesh_nrf_connectivity mesh_connectivity(0x8F,nrf);
    mesh::rip_router router;

    mesh::mesh_network net(mesh_connectivity, router);


    uint64_t count = 0;
    for(;;) {
        net.update();
        if(count > 500) {
            count = 0;
            net.discover();
        }
        if(count == 250) {

        }
        hwlib::wait_ms(10);
        count++;
    }

//    receive(nrf);




}
