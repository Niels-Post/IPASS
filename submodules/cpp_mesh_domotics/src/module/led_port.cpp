/*
 *
 * Copyright Niels Post 2019.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 *
*/

#include <mesh_domotics/module/led_port.hpp>


namespace mesh_domotics {
    /**
     * \addtogroup mesh_domotics
     * @{
     */
    namespace modules {
        void led_port::set_input(uint8_t *data) {
            last_val.data[0] = data[0];
            last_val.data[1] = data[1];
            last_val.data[2] = data[2];
            last_val.data[3] = data[3];

            port.write((1 << last_val.numeric) - 1);
            port.flush();
        }

        led_port::led_port(uint8_t id, hwlib::port_out &port) : output_module(id), port(port) {

        }

    }
    /**
     * @}
     */
}

