#include <hwlib.hpp>
#include "spi/spi_bitbang.hpp"
#include "nrf24l01/nrf24l01plus.hpp"
#include "mesh/connectivity/mesh_nrf_connectivity.hpp"
#include "mesh/router/link_state_router.hpp"
#include "mesh/mesh_network.hpp"

#include "util/cout_debug.hpp"
#include "domotica/module/rotary_encoder.hpp"
#include "domotica/domotica_node.hpp"
#include "domotica/module/led_port.hpp"
#include "domotica/module/seven_segment.hpp"
#include "spi/spi_blue_pill.hpp"






void standard(nrf24l01::nrf24l01plus & nrf, mesh::node_id id) {
    LOG("MY_ID", id);
    mesh::mesh_nrf_connectivity mesh_connectivity(id, nrf);
    mesh::link_state_router router(mesh_connectivity);
    mesh::mesh_network net(mesh_connectivity, router);

//    if(id == 0x20) {
//        net.add_blacklist<2>({0x40});
//    }
//    if(id == 0x40) {
//        net.add_blacklist<1>({0x20});
//    }


    domotica_input_module d1(0);
    domotica_output_module d2(0);
    domotica_node node(net,d1,d2);


    node.loop();
}


void rotary(nrf24l01::nrf24l01plus & nrf, mesh::node_id id) {
    LOG("MY_ID", id);
    mesh::mesh_nrf_connectivity mesh_connectivity(id, nrf);
    mesh::link_state_router router(mesh_connectivity);
    mesh::mesh_network net(mesh_connectivity, router);

    auto a = hwlib::target::pin_in(hwlib::target::pins::a9);
    auto b = hwlib::target::pin_in(hwlib::target::pins::a10);

    rotary_encoder r(1);
    r.filter[0] = 0x30;
    net.add_blacklist<1>({0x30});
    domotica_input_module d(0);

    domotica_node node(net,d,r);

    node.loop();


}

void port(nrf24l01::nrf24l01plus & nrf, mesh::node_id id) {
    LOG("MY_ID", id);
    mesh::mesh_nrf_connectivity mesh_connectivity(id, nrf);
    mesh::link_state_router router(mesh_connectivity);
    mesh::mesh_network net(mesh_connectivity, router);

    auto a = hwlib::target::pin_out(hwlib::target::pins::a9);
    auto b = hwlib::target::pin_out(hwlib::target::pins::a10);
    auto c = hwlib::target::pin_out(hwlib::target::pins::a11);


    auto port = hwlib::port_out_from(a, b, c);

    net.add_blacklist<1>({0x10});
    led_port l(2, port);
    domotica_output_module d(0);

    domotica_node node(net,l,d);

    node.loop();
}


void sevensegment(nrf24l01::nrf24l01plus & nrf, mesh::node_id id) {
    LOG("MY_ID", id);
    mesh::mesh_nrf_connectivity mesh_connectivity(id, nrf);
    mesh::link_state_router router(mesh_connectivity);
    mesh::mesh_network net(mesh_connectivity, router);

    auto _a = hwlib::target::pin_out(hwlib::target::pins::b14);
    auto _b = hwlib::target::pin_out(hwlib::target::pins::b15);
    auto _c = hwlib::target::pin_out(hwlib::target::pins::a11);
    auto _d = hwlib::target::pin_out(hwlib::target::pins::a10);
    auto _e = hwlib::target::pin_out(hwlib::target::pins::a9);
    auto _f = hwlib::target::pin_out(hwlib::target::pins::b13);
    auto _g = hwlib::target::pin_out(hwlib::target::pins::b12);

    auto port = hwlib::port_out_from(_a, _b, _c, _d, _e, _f, _g);


    net.add_blacklist<1>({0x10});
    seven_segment s( 3, port);
    domotica_output_module d(0);

    domotica_node node(net,s,d);

    node.loop();
    EXTI9_5_IRQHandler();
}





int main() {


    auto ce = hwlib::target::pin_out(hwlib::target::pins::a4);
    auto bus = spi_blue_pill({false,false,1});
    auto nrf = nrf24l01::nrf24l01plus(bus, hwlib::pin_out_dummy, ce);
    LOG("", "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    LOG("PROGRAM_STARTING", "");
    hwlib::wait_ms(100);
    nrf.power(true);
    LOG("NRF_POWER_ON", "");
//    rotary(nrf, 0x10);
//    standard(nrf, 0x20);
    sevensegment(nrf, 0x30);
//    port(nrf, 0x30);
//    standard(nrf, 0x40);



}
