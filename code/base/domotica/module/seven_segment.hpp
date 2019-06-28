
#ifndef PROJECT_SEVEN_SEGMENT_HPP
#define PROJECT_SEVEN_SEGMENT_HPP


#include "../domotica_module.hpp"

class seven_segment: public domotica_input_module{
    hwlib::port_out &port;
    uint8_t map[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};


public:

    seven_segment(uint8_t id, hwlib::port_out &port);

    void set_input(uint8_t *data) override;
};


#endif //PROJECT_SEVEN_SEGMENT_HPP
