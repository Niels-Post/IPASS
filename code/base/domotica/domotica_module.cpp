//
// Created by Niels on 6/27/2019.
//

#include "domotica_module.hpp"

domotica_module::domotica_module(module_type type, uint8_t id) : type(type), id(id) {}

module_type domotica_module::getType() const {
    return type;
}

domotica_module::domotica_module(): domotica_module(NONE,0) {}

domotica_input_module::domotica_input_module(uint8_t id) : domotica_module(INPUT, id) {}

domotica_output_module::domotica_output_module(uint8_t id) : domotica_module(OUTPUT, id) {}
