
#include "seven_segment.hpp"
#include "../../util/cout_debug.hpp"


void seven_segment::set_input(uint8_t *data) {
    uint8_t val = data[0] % 10;
    port.write(map[val]);
    port.flush();

}

seven_segment::seven_segment(uint8_t id, hwlib::port_out &port) : domotica_input_module(id), port(port) {}
