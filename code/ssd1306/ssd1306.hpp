//
// Created by Niels on 7/1/2019.
//

#ifndef IPASS_SSD1306_HPP
#define IPASS_SSD1306_HPP

#include <hwlib.hpp>


#define OLED_ADDRESS 0x3B

namespace ssd1306 {
    struct CONTROL {
        static constexpr const uint8_t CONTINUATION = 0x80;
        static constexpr const uint8_t DATA = 0x40;
    };

    struct FUNDAMENTAL {
        static constexpr const uint8_t SET_CONTRAST = 0x81;
        static constexpr const uint8_t DISPLAY_ON_RAM = 0xA4;
        static constexpr const uint8_t ENTIRE_DISPLAY_ON = 0xA5;
        static constexpr const uint8_t DISPLAY_NO_INVERSE = 0xA6;
        static constexpr const uint8_t DISPLAY_INVERSE = 0xA7;
        static constexpr const uint8_t DISPLAY_ON = 0xAE;
        static constexpr const uint8_t DISPLAY_OFF = 0xAF;
    };

    struct SCROLLING {
        static constexpr const uint8_t SETUP_SCROLL_RIGHT= 0x26;
        static constexpr const uint8_t SETUP_SCROLL_LEFT = 0x27;
        static constexpr const uint8_t SETUP_SCROLL_VERTICAL_RIGHT =  0x29;
        static constexpr const uint8_t SETUP_SCROLL_VERTICAL_LEFT =  0x2A;
        static constexpr const uint8_t SCROLL_DEACTIVATE =  0x2E;
        static constexpr const uint8_t SCROLL_ACTIVATE =  0x2F;
        static constexpr const uint8_t SET_SCROLL_Y_AREA =  0xA3;
    };

    struct ADRESSING {
        static constexpr const uint8_t SET_MODE = 0x20;
        static constexpr const uint8_t MODE_HORIZONTAL = 0x0;
        static constexpr const uint8_t MODE_VERTICAL = 0x1;
        static constexpr const uint8_t MODE_PAGE = 0x2;
        static constexpr const uint8_t SET_COLUMN_RANGE = 0x21;
        static constexpr const uint8_t SET_PAGE_RANGE = 0x22;
    };

    struct HARDWARE {
        static constexpr const uint8_t SET_DISPLAY_START = 0x40;
        static constexpr const uint8_t SET_SEGMENT_REMAP_0 = 0xA0;
        static constexpr const uint8_t SET_SEGMENT_REMAP_127 = 0xA1;
        static constexpr const uint8_t SET_MUX_RATIO = 0xA8;
        static constexpr const uint8_t SET_COM_OUTPUT_DIRECTION_FORWARD = 0xC0;
        static constexpr const uint8_t SET_COM_OUTPUT_DIRECTION_BACKWARD = 0xC8;
        static constexpr const uint8_t SET_DISPLAY_OFFSET = 0xD3;
        static constexpr const uint8_t SET_COM_PINS = 0xDA;
    };

    struct TIMING {
        static constexpr const uint8_t SET_DISPLAY_CLOCK_DIVIDE = 0xD5;
        static constexpr const uint8_t SET_PRE_CHARGE_PERIOD = 0xD9;
        static constexpr const uint8_t SET_VCOMH_DESELECT = 0xDB;
        static constexpr const uint8_t NOP = 0xE3;
    };

    struct CHARGE_BUMP {
        static constexpr const uint8_t SET_CHARGE_BUMP = 0x8D;
    };

    class ssd1306 {
        hwlib::i2c_bus &i2c;
    public:

        ssd1306(hwlib::i2c_bus &i2C);

        void write_data(size_t n, uint8_t *data);

        void write_command(uint8_t command, size_t n, uint8_t *data);
        void write_command_stream(uint8_t command, size_t n, uint8_t *data);

    };
}


#endif //IPASS_SSD1306_HPP
