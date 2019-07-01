//
// Created by Niels on 6/27/2019.
//

#ifndef IPASS_LED_PORT_HPP
#define IPASS_LED_PORT_HPP


#include "../domotica_module.hpp"

/**
 * Domotica input module for a row of pin_outs.
 * Takes tha input value as an integer, and enables that amount of pins in the given port.
 * Pins are enabled lsbit first
 */
class led_port: public domotica_input_module {
    hwlib::port_out &port;
    union domotica_value last_val;
public:
    /**
     * Creates a LED port
     * @param id Module ID to use, make sure this is unique within the network
     * @param port Port of pin_outs to enable/disable on input
     */
    led_port(uint8_t id, hwlib::port_out &port);

public:
    void set_input(uint8_t *data) override;
};


#endif //IPASS_LED_PORT_HPP
