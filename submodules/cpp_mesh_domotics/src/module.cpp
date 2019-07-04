/*
 *
 * Copyright Niels Post 2019.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 *
*/


#include <mesh_domotics/module.hpp>

namespace mesh_domotics {
    module::module(module_type type, uint8_t id) : type(type), id(id) {}

    module_type module::getType() const {
        return type;
    }

    module::module() : module(NONE, 0) {}

    output_module::output_module(uint8_t id) : module(INPUT, id) {}

    input_module::input_module(uint8_t id) : module(OUTPUT, id) {}
}