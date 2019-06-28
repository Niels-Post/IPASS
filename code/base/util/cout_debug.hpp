
//
// Created by Niels on 6/24/2019.
//

#ifndef IPASS_COUT_DEBUG_HPP
#define IPASS_COUT_DEBUG_HPP

#include <hwlib.hpp>



#define DEBUG_UART

#ifdef DEBUG_SPI
#include "../spi/spi_bitbang.hpp"
class cout_debug : public hwlib::ostream {
    hwlib::target::pin_out csn;
    hwlib::target::pin_out clk;
    hwlib::target::pin_out mosi;
    hwlib::target::pin_in miso;
    hwlib_ex::spi_bitbang bus;

public:

    cout_debug() :
            csn{hwlib::target::pins::a8},
            clk{hwlib::target::pins::a9},
            mosi{hwlib::target::pins::a10},
            miso{hwlib::target::pins::a11},
            bus{clk, mosi, miso, {false, false, 1}} {
    }

    void putc(char c) override {
        bus.transaction(csn).write_byte(c);
    }

    void flush(void) override {

    }
};
#endif

#ifdef DEBUG_UART
class cout_debug : public hwlib::ostream {
    hwlib::target::pin_out uart;

public:

    cout_debug() :
            uart{hwlib::target::pins::a8} {
    }

    void putc(char c) override {
        hwlib::uart_putc_bit_banged_pin(c, uart);
    }

    void flush(void) override {

    }
};
#endif


#ifndef DEBUG_SPI
#ifndef DEBUG_UART
class cout_debug : public hwlib::ostream {

public:

    cout_debug()  {
    }

    void putc(char c) override {
        hwlib::cout << c;
    }

    void flush(void) override {

    }
};
#endif
#endif




#define DEBUG_BUILD

#ifdef DEBUG_BUILD
#define LOG(title, message) do { auto c = cout_debug(); c << hwlib::left << hwlib::setw(20) << title<< message << hwlib::endl; } while (0)
#else
#define LOG(title)
#define LOG1(title, param1)
#define LOG2(title, param1, param2)
#define LOG3(title, param1, param2, param3)
#endif


#endif //IPASS_COUT_DEBUG_HPP
