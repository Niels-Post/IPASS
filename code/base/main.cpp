#include <hwlib.hpp>
#include "spi/spi_bitbang.hpp"
#include "nrf24l01/nrf24l01plus.hpp"
#include "mesh/connectivity/mesh_nrf_connectivity.hpp"
#include "mesh/router/link_state_router.hpp"
#include "mesh/mesh_network.hpp"

#include "util/cout_debug.hpp"


int main() {
    auto ce = hwlib::target::pin_out(hwlib::target::pins::a1);
    auto csn = hwlib::target::pin_out(hwlib::target::pins::a2);
    auto sclk = hwlib::target::pin_out(hwlib::target::pins::a3);
    auto mosi = hwlib::target::pin_out(hwlib::target::pins::a4);
    auto miso = hwlib::target::pin_in(hwlib::target::pins::a5);
    auto bus = hwlib_ex::spi_bitbang(sclk, mosi, miso, hwlib_ex::spi_mode(false, false, 1));

    auto nrf = nrf24l01::nrf24l01plus(bus, csn, ce);

    LOG("PROGRAM_STARTING");


    hwlib::wait_ms(100);
    nrf.power(true);

    LOG("NRF_POWER_ON");

    mesh::mesh_nrf_connectivity mesh_connectivity(0x2F, nrf);
    mesh::link_state_router router(mesh_connectivity);

    mesh::mesh_network net(mesh_connectivity, router);


    uint64_t count = 0;
    for (;;) {
        net.update();
        if (count > 500) {
            count = 0;
            net.discover();
            mesh::mesh_message msg (mesh::DISCOVERY::NO_OPERATION, 0, mesh_connectivity.address, 0, 0);
            mesh_connectivity.unicast_all(msg);
        }


        hwlib::wait_ms(10);
        count++;
    }

//    receive(nrf);




}
