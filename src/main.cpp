#include <hwlib.hpp>
#include <spi/bus_bitbang.hpp>
#include <nrf24l01plus/nrf24l01plus.hpp>
#include <mesh/connectivity/nrf.hpp>
#include <mesh/router/link_state_router.hpp>
#include <mesh/mesh_network.hpp>
#include <mesh_domotics/module/rotary_encoder.hpp>
#include <mesh_domotics/node.hpp>
#include <mesh_domotics/module/led_port.hpp>
#include <mesh_domotics/module/seven_segment.hpp>
#include <lcd/i2c_backpack.hpp>
#include <mesh/addon/status_lcd.hpp>
#include <spi/hardware/bus_stm32f10xxx.hpp>
#include <cout_debug.hpp>
#include <nrf24l01plus/self_test.hpp>
#include <pin_state_checker_stm32f103.hpp>


void module1_rotary_encoder(mesh::mesh_network &net) {

//    {
//        auto pinstate_checker = pin_state_checker_stm32f103();
//        pinstate_checker.assert_unset(hwlib::target::pins::a9);
//        pinstate_checker.assert_unset(hwlib::target::pins::a10);
//        if(pinstate_checker.all_successful()) {
//            auto pin = hwlib::target::pin_out(hwlib::target::pins::led);
//            pin.flush();
//        }
//    }

    auto dummy_out = mesh_domotics::output_module(0);
    mesh_domotics::modules::rotary_encoder r(1);
    r.filter[0] = 0x30;
    r.filter[1] = 0x40;
    mesh_domotics::node node(net, dummy_out, r);
    net.add_blacklist<2>({0x30, 0x40});
    node.loop();
}


void module2_lcd_display(mesh::mesh_network &net) {
//    {
//
//        auto pinstate_checker = pin_state_checker_stm32f103();
//        pinstate_checker.assert_high(hwlib::target::pins::a9);
//        pinstate_checker.assert_high(hwlib::target::pins::a10);
//        if(pinstate_checker.all_successful()) {
//            auto pin = hwlib::target::pin_out(hwlib::target::pins::led);
//            pin.flush();
//        }
//    }

    auto dummy_out = mesh_domotics::output_module(0);
    auto dummy_in = mesh_domotics::input_module(0);

    auto a9 = hwlib::target::pin_oc(hwlib::target::pins::a9);
    auto a10 = hwlib::target::pin_oc(hwlib::target::pins::a10);
    auto lcd_bus = hwlib::i2c_bus_bit_banged_scl_sda(a9, a10);
    auto lcd = lcd::i2c_backpack(lcd_bus, 2, 16);
    auto statusLcd = mesh::addons::status_lcd(lcd);
    statusLcd.setCurrentMode(mesh::addons::LINK_STATE_DISTANCES);


    std::array<mesh::message, 10> uncaught = {};
    uint8_t i = 0;
    for (;;) {
        net.update();
        net.check_new_messages(uncaught);
        if(i++ > 50) {
            i = 0;
            statusLcd.update(net);
        }
    }
}


void module3_sevendigit(mesh::mesh_network &net) {

//    {
//        auto pinstate_checker = pin_state_checker_stm32f103();
//        pinstate_checker.assert_low(hwlib::target::pins::b14);
//        pinstate_checker.assert_low(hwlib::target::pins::b15);
//        pinstate_checker.assert_low(hwlib::target::pins::a11);
//        pinstate_checker.assert_low(hwlib::target::pins::a10);
//        pinstate_checker.assert_low(hwlib::target::pins::a9);
//        pinstate_checker.assert_low(hwlib::target::pins::b13);
//        pinstate_checker.assert_low(hwlib::target::pins::b12);
//        if(pinstate_checker.all_successful()) {
//            auto pin = hwlib::target::pin_out(hwlib::target::pins::led);
//            pin.flush();
//        }
//    }

    auto dummy_in = mesh_domotics::input_module(0);

    auto _a = hwlib::target::pin_out(hwlib::target::pins::b14);
    auto _b = hwlib::target::pin_out(hwlib::target::pins::b15);
    auto _c = hwlib::target::pin_out(hwlib::target::pins::a11);
    auto _d = hwlib::target::pin_out(hwlib::target::pins::a10);
    auto _e = hwlib::target::pin_out(hwlib::target::pins::a9);
    auto _f = hwlib::target::pin_out(hwlib::target::pins::b13);
    auto _g = hwlib::target::pin_out(hwlib::target::pins::b12);

    auto port = hwlib::port_out_from(_a, _b, _c, _d, _e, _f, _g);
    mesh_domotics::modules::seven_segment s(1, port);
    s.filter[0] = 0x10;

    mesh_domotics::node node(net, s, dummy_in);
    net.add_blacklist<1>({0x10});
    node.loop();

}


int main() {
    auto ce = hwlib::target::pin_out(hwlib::target::pins::a3);
    auto bus = spi::bus_stm32f10xxx({false, false, 1});
    auto nrf = nrf24l01::nrf24l01plus(bus, hwlib::pin_out_dummy, ce);

    hwlib::wait_ms(10);

    {
        auto startup_test = nrf24l01::startup_test(nrf);
        startup_test.test_register_reset_states();
        nrf.power(true);
        startup_test.test_one_side_transmission();

        if (!startup_test.all_successful()) {
            GPIOC->CRH |= 0x2u << 20u;
            auto pin = hwlib::target::pin_out(hwlib::target::pins::led);
            hwlib::blink(pin);
        }
    }


    mesh::node_id ID = 0x20;


    mesh::connectivity::nrf mesh_connectivity(ID, nrf);
    mesh::routers::link_state router(mesh_connectivity);
    mesh::mesh_network net(mesh_connectivity, router);

    switch (ID) {
        case 0x10:
            module1_rotary_encoder(net);
            break;
        case 0x20:
            module2_lcd_display(net);
            break;
        case 0x30:
            module3_sevendigit(net);
            break;
    }
}
