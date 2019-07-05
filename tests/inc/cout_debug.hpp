/*
 *
 * Copyright Niels Post 2019.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 *
*/

#ifndef IPASS_COUT_DEBUG_HPP
#define IPASS_COUT_DEBUG_HPP

#include <hwlib.hpp>

class cout_debug : public hwlib::ostream {

public:

    cout_debug() {
    }

    void putc(char c) override {
        hwlib::cout << c;
    }

    void flush(void) override {

    }
};





#define DEBUG_BUILD

#ifdef DEBUG_BUILD
#define LOG(title, message) do { auto c = cout_debug(); c << hwlib::left << hwlib::setw(20) << title<< message << hwlib::endl; } while (0)
#else
#define LOG(title, message)
#endif


#endif //IPASS_COUT_DEBUG_HPP
