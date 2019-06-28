//
// Created by Niels on 6/27/2019.
//

#ifndef IPASS_LED_PORT_HPP
#define IPASS_LED_PORT_HPP


#include "../domotica_module.hpp"

class led_port: public domotica_input_module {
    hwlib::port_out &port;
    union domotica_value last_val;
public:
    led_port(uint8_t id, hwlib::port_out &port);

public:
    void set_input(uint8_t *data) override;
};


#endif //IPASS_LED_PORT_HPP
