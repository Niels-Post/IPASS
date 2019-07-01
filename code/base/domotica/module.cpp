//
// Created by Niels on 6/27/2019.
//

#include "module.hpp"

namespace mesh_domotics {
    module::module(module_type type, uint8_t id) : type(type), id(id) {}

    module_type module::getType() const {
        return type;
    }

    module::module() : module(NONE, 0) {}

    input_module::input_module(uint8_t id) : module(INPUT, id) {}

    output_module::output_module(uint8_t id) : module(OUTPUT, id) {}
}