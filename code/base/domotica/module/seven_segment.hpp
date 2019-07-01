
#ifndef PROJECT_SEVEN_SEGMENT_HPP
#define PROJECT_SEVEN_SEGMENT_HPP


#include "../domotica_module.hpp"

/**
 * Domotica input module for a seven segment 1 digit display
 * Uses a port as output
 * TODO: allow more digits
 */
class seven_segment: public domotica_input_module{
    hwlib::port_out &port;
    uint8_t map[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};


public:
    /**
     * Create a seven segment display module
     * @param id Module ID, make sure this is unique within a network
     * @param port Port to use for the pin_outs, which should be filled with pin_outs in order from pin A to G
     */
    seven_segment(uint8_t id, hwlib::port_out &port);

    /**
     * Set the state of the seven segment display (uses the first byte of data as an uint8, since we can only have 10 values anyway)
     * @param data Pointer to the value to use
     */
    void set_input(uint8_t *data) override;
};


#endif //PROJECT_SEVEN_SEGMENT_HPP
