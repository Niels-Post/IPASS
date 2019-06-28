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



//void run(mesh::node_id id) {
//
//
//
//    switch (id) {
//        case 0x10: {
//            net.add_blacklist<1>({0x20});
//            auto a = hwlib::target::pin_in(hwlib::target::pins::a9);
//            auto b = hwlib::target::pin_in(hwlib::target::pins::a10);
//            rotary_encoder r(2,a,b);
//            r.filter[0] = 0x30;
//            uint64_t count = 0;
//            uint8_t data[4];
//            for (;;) {
//                if(r.get_output(data)) {
//                    mesh::mesh_message msg(
//                                mesh::DOMOTICA::DATA,
//                                0,
//                                0,
//                                0,
//                                4
//                            );
//                    msg.data[0] = data[0];
//                    msg.data[1] = data[1];
//                    msg.data[2] = data[2];
//                    msg.data[3] = data[3];
//                    for(uint8_t i = 0; i < 10; i++) {
//                        if(r.filter[i] == 0) {
//                            break;
//                        }
//                        msg.receiver = r.filter[i];
//                        net.sendMessage(msg);
//                    }
//                }
//                net.update();
//                if (count > 5000) {
//                    count = 0;
//                    net.discover();
//                }
//                hwlib::wait_ms(2);
//
//
//                count++;
//            }
//            break;
//        }
//        case 0x20: {
//
//            net.add_blacklist<2>({0x10, 0x30});
//            uint64_t count = 0;
//            for (;;) {
//                net.update();
//                if (count > 5000) {
//                    count = 0;
//                    net.discover();
//                }
//                hwlib::wait_ms(2);
//
//
//                count++;
//            }
//        }
//        case 0x30: {
//            net.add_blacklist<2>({0x20, 0x40});
//            uint64_t count = 0;
//            for (;;) {
//                net.update();
//                if (count > 5000) {
//                    count = 0;
//                    net.discover();
//                }
//                hwlib::wait_ms(2);
//
//
//                count++;
//            }
//        }
//        case 0x40: {
//            net.add_blacklist<1>({0x30});
//            uint64_t count = 0;
//            for (;;) {
//                net.update();
//                if (count > 5000) {
//                    count = 0;
//                    net.discover();
//                }
//                hwlib::wait_ms(2);
//
//
//                count++;
//            }
//        }
//    }
//}



void standard(nrf24l01::nrf24l01plus & nrf, mesh::node_id id) {
    LOG("MY_ID", id);
    mesh::mesh_nrf_connectivity mesh_connectivity(id, nrf);
    mesh::link_state_router router(mesh_connectivity);
    mesh::mesh_network net(mesh_connectivity, router);

    if(id == 0x20) {
        net.add_blacklist<2>({0x40});
    }
    if(id == 0x40) {
        net.add_blacklist<1>({0x20});
    }


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

    rotary_encoder r(1, a, b);
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
}


int main() {
    auto ce = hwlib::target::pin_out(hwlib::target::pins::a1);
    auto csn = hwlib::target::pin_out(hwlib::target::pins::a2);
    auto sclk = hwlib::target::pin_out(hwlib::target::pins::a3);
    auto mosi = hwlib::target::pin_out(hwlib::target::pins::a4);
    auto miso = hwlib::target::pin_in(hwlib::target::pins::a5);

    auto bus = hwlib_ex::spi_bitbang(sclk, mosi, miso, hwlib_ex::spi_mode(false, false, 1));

    auto nrf = nrf24l01::nrf24l01plus(bus, csn, ce);


    LOG("", "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    LOG("PROGRAM_STARTING", "");


    hwlib::wait_ms(100);
    nrf.power(true);

    LOG("NRF_POWER_ON", "");

    rotary(nrf, 0x10);
//    standard(nrf, 0x20);
//    port(nrf, 0x30);
//    standard(nrf, 0x40);
//    sevensegment(nrf, 0x30);

//    run(0x10);
//    run(0x20);
//    run(0x30);
//    run(0x40);

//    auto a = hwlib::target::pin_in(hwlib::target::pins::a9);
//    auto b = hwlib::target::pin_in(hwlib::target::pins::a10);
//
//    rotary_encoder r(1, a, b);
//    cout_debug c;
//    uint8_t data[4];
//    for (;;) {
//        if (r.get_output(data)) {
//            c << data[0] << " " << data[1] << " " << data[2] << " " << data[3] << hwlib::endl;
//        }
//    }


//    receive(nrf);




}
