//
// Created by Niels on 6/27/2019.
//

#ifndef IPASS_ROTARY_ENCODER_HPP
#define IPASS_ROTARY_ENCODER_HPP

#include <hwlib.hpp>
#include "../domotica_module.hpp"

class rotary_encoder: public domotica_output_module {



    hwlib::pin_in &a, &b;
    bool aLast;
public:
    rotary_encoder(uint8_t id, hwlib::pin_in &a, hwlib::pin_in &b);

    union domotica_value currentValue;

    bool get_output(uint8_t data[4]) override;
};


#endif //IPASS_ROTARY_ENCODER_HPP
