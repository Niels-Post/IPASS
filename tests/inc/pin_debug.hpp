//
// Created by Niels on 6/25/2019.
//
#include <hwlib.hpp>

#ifndef IPASS_PIN_DEBUG_HPP
#define IPASS_PIN_DEBUG_HPP

class pin_debug : public hwlib::pin_out, public hwlib::pin_in {
public:
    bool value = false;
    bool flushed = false;

    void write(bool x) override {
        value = x;
        flushed = false;
    }

    void flush() override {
        flushed = true;
    }

    bool was_flushed() const {
        return flushed;
    }

    void direction_set_input() {}

    bool read() override {
        return value;
    }

    void refresh() override {}


};

#endif //IPASS_PIN_DEBUG_HPP
