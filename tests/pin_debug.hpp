//
// Created by Niels on 6/25/2019.
//
#include <hwlib.hpp>
#ifndef IPASS_PIN_DEBUG_HPP
#define IPASS_PIN_DEBUG_HPP

class pin_debug: public hwlib::pin_out {
public:
    bool value = false;

    void write(bool x) override {
        value = x;
    }

    void flush() override {

    }
};
#endif //IPASS_PIN_DEBUG_HPP
