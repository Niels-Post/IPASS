//
// Created by Niels on 7/2/2019.
//

#ifndef IPASS_I2C_BACKPACK_HPP
#define IPASS_I2C_BACKPACK_HPP

#include <cstdint>
#include <hwlib.hpp>

namespace lcd {

    struct BASE {
        static constexpr const uint8_t CLEARDISPLAY = 0x01;
        static constexpr const uint8_t RETURNHOME = 0x02;
        static constexpr const uint8_t ENTRYMODESET = 0x04;
        static constexpr const uint8_t DISPLAYCONTROL = 0x08;
        static constexpr const uint8_t CURSORSHIFT = 0x10;
        static constexpr const uint8_t FUNCTIONSET = 0x20;
        static constexpr const uint8_t SETCGRAMADDR = 0x40;
        static constexpr const uint8_t SETDDRAMADDR = 0x80;
    };

    struct ENTRY {
        static constexpr const uint8_t ENTRYRIGHT = 0x00;
        static constexpr const uint8_t ENTRYLEFT = 0x02;
        static constexpr const uint8_t ENTRYSHIFTINCREMENT = 0x01;
        static constexpr const uint8_t ENTRYSHIFTDECREMENT = 0x00;
    };

    struct ONOFF {
        static constexpr const uint8_t DISPLAYON = 0x04;
        static constexpr const uint8_t DISPLAYOFF = 0x00;
        static constexpr const uint8_t CURSORON = 0x02;
        static constexpr const uint8_t CURSOROFF = 0x00;
        static constexpr const uint8_t BLINKON = 0x01;
        static constexpr const uint8_t BLINKOFF = 0x00;
    };

    struct SHIFT {
        static constexpr const uint8_t DISPLAYMOVE = 0x08;
        static constexpr const uint8_t CURSORMOVE = 0x00;
        static constexpr const uint8_t MOVERIGHT = 0x04;
        static constexpr const uint8_t MOVELEFT = 0x00;
    };

    struct FUNCTIONSET {
        static constexpr const uint8_t _8BITMODE = 0x10;
        static constexpr const uint8_t _4BITMODE = 0x00;
        static constexpr const uint8_t _2LINE = 0x08;
        static constexpr const uint8_t _1LINE = 0x00;
        static constexpr const uint8_t _5x10DOTS = 0x04;
        static constexpr const uint8_t _5x8DOTS = 0x00;
    };

    struct BACKLIGHT {
        static constexpr const uint8_t YES = 0x08;
        static constexpr const uint8_t NO = 0x00;
    };

    struct BITS {
        static constexpr const uint8_t En = 0b00000100;
        static constexpr const uint8_t Rw = 0b00000010;
        static constexpr const uint8_t Rs = 0b00000001;
    };

    class i2c_backpack: public hwlib::ostream {
        hwlib::i2c_bus & bus;
        uint8_t rows;
        uint8_t cols;
        uint8_t address;
        uint8_t row = 0;
        char buffer[2][16] = {};
        uint8_t positions[2] = {0};
        size_t buffer_position = 0;
    public:
        i2c_backpack(hwlib::i2c_bus &bus, uint8_t rows, uint8_t cols, uint8_t address = 0x27);

        void strobe(uint8_t data);

        void write_four_bits(uint8_t data);

        void write(uint8_t cmd, uint8_t mode = 0);

        void display_string(char *data, uint8_t n, uint8_t line);

        void clear();

        void set_row(uint8_t currentRow);

    private:
        void putc(char c) override;

    public:
        void flush(void) override;
    };

}


#endif //IPASS_I2C_BACKPACK_HPP
