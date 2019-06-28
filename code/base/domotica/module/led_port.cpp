//
// Created by Niels on 6/27/2019.
//

#include "led_port.hpp"

void led_port::set_input(uint8_t *data) {
    last_val.data[0] = data[0];
    last_val.data[1] = data[1];
    last_val.data[2] = data[2];
    last_val.data[3] = data[3];

    port.write((1 << last_val.numeric/2) -1 );
    port.flush();
}

led_port::led_port(uint8_t id, hwlib::port_out &port) : domotica_input_module(id), port(port) {

}
