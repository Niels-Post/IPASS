/*
 *
 * Copyright Niels Post 2019.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 *
*/

#include <mesh_domotics/module/seven_segment.hpp>

namespace mesh_domotics {
    namespace modules {
        void seven_segment::set_input(uint8_t *data) {
            uint8_t val = data[0] % 10;
            port.write(map[val]);
            port.flush();

        }

        seven_segment::seven_segment(uint8_t id, hwlib::port_out &port) : output_module(id), port(port) {}

    }
}
