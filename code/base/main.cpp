#include <hwlib.hpp>
#include "spi/spi_bitbang.hpp"
#include "nrf24l01/nrf24l01plus.hpp"
#include "mesh/connectivity/nrf.hpp"
#include "mesh/router/link_state_router.hpp"
#include "mesh/mesh_network.hpp"
#include "domotica/module/rotary_encoder.hpp"
#include "domotica/node.hpp"
#include "domotica/module/led_port.hpp"
#include "domotica/module/seven_segment.hpp"
#include "spi/spi_blue_pill.hpp"
#include "lcd/i2c_backpack.hpp"
#include "mesh/addon/status_lcd.hpp"

int main() {




    auto ce = hwlib::target::pin_out(hwlib::target::pins::a4);
    auto bus = spi_blue_pill({false, false, 1});
    auto nrf = nrf24l01::nrf24l01plus(bus, hwlib::pin_out_dummy, ce);

    LOG("", "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    LOG("PROGRAM_STARTING", "");
    hwlib::wait_ms(100);
    nrf.power(true);
    LOG("NRF_POWER_ON", "");

    mesh::node_id ID = 0x20;
    LOG("MY_ID", ID);


    mesh::connectivity::nrf mesh_connectivity(ID, nrf);
    mesh::routers::link_state router(mesh_connectivity);
    mesh::mesh_network net(mesh_connectivity, router);


    mesh_domotics::input_module dummy_in = {0};
    mesh_domotics::output_module dummy_out = {0};

    mesh_domotics::node node(net, dummy_in, dummy_out);



    switch (ID) {
        case 0x10: {
            mesh_domotics::modules::rotary_encoder r(1);
            r.filter[0] = 0x30;
            r.filter[1] = 0x40;
            node.set_output_module(r);
            net.add_blacklist<2>({0x30, 0x40});
            break;
        }
        case 0x20: {
//            auto a9 = hwlib::target::pin_oc(hwlib::target::pins::a9);
//            auto a10 = hwlib::target::pin_oc(hwlib::target::pins::a10);
//            auto lcd_bus = hwlib::i2c_bus_bit_banged_scl_sda(a9, a10);
//            auto lcd = lcd::i2c_backpack(lcd_bus, 2, 16);
//
//            lcd << "HALLO";
//            lcd.flush();
//
//            mesh::status_lcd status(net, lcd);
//            status.setCurrentMode(mesh::LINK_STATE_DISTANCES);
//            uint8_t i = 0;
//            for(;;) {
//                net.update();
//                if(i > 25) {
//                    i = 0;
//                    status.update();
//                }
//                i++;
//            }
            break;
        }

        case 0x30: {
            auto _a = hwlib::target::pin_out(hwlib::target::pins::b14);
            auto _b = hwlib::target::pin_out(hwlib::target::pins::b15);
            auto _c = hwlib::target::pin_out(hwlib::target::pins::a11);
            auto _d = hwlib::target::pin_out(hwlib::target::pins::a10);
            auto _e = hwlib::target::pin_out(hwlib::target::pins::a9);
            auto _f = hwlib::target::pin_out(hwlib::target::pins::b13);
            auto _g = hwlib::target::pin_out(hwlib::target::pins::b12);

            auto port = hwlib::port_out_from(_a, _b, _c, _d, _e, _f, _g);
            mesh_domotics::modules::seven_segment s(1, port);
            node.set_input_module(s);
            net.add_blacklist<1>({0x10});
            break;
        }
        case 0x40: {
            auto a = hwlib::target::pin_out(hwlib::target::pins::a9);
            auto b = hwlib::target::pin_out(hwlib::target::pins::a10);
            auto c = hwlib::target::pin_out(hwlib::target::pins::a11);

            auto port = hwlib::port_out_from(a, b, c);
            mesh_domotics::modules::led_port l(2, port);
            node.set_input_module(l);
        }


    }

    if (ID == 0x10) { //Rotary Encoder module

    }

    node.loop();
}
