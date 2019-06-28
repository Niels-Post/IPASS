//
// Created by Niels on 6/27/2019.
//

#include "rotary_encoder.hpp"
#include "../../util/cout_debug.hpp"


rotary_encoder::rotary_encoder(uint8_t id, hwlib::pin_in &a, hwlib::pin_in &b) : domotica_output_module(id), a(a),
                                                                                 b(b) {}

bool rotary_encoder::get_output(uint8_t data[4]) {
    bool aVal = a.read();
    if(aVal != aLast) {
        if(b.read() != aVal) {
            currentValue.numeric++;
        } else {
            currentValue.numeric--;
        }
        data[0] = currentValue.data[0];
        data[1] = currentValue.data[1];
        data[2] = currentValue.data[2];
        data[3] = currentValue.data[3];
        aLast = aVal;
        return true;
    }
    return false;
}
