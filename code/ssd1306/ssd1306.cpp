//
// Created by Niels on 7/1/2019.
//

#include "ssd1306.hpp"


ssd1306::ssd1306::ssd1306(hwlib::i2c_bus &i2C) : i2c(i2C) {
    auto trans = i2c.write(OLED_ADDRESS);
    trans.write(0);

    trans.write(FUNDAMENTAL::DISPLAY_OFF);

    trans.write(HARDWARE::SET_MUX_RATIO);
    trans.write(0x3F);

    trans.write(HARDWARE::SET_DISPLAY_OFFSET);
    trans.write(0x00);

    trans.write(HARDWARE::SET_DISPLAY_START);


    trans.write(HARDWARE::SET_COM_PINS);
    trans.write(0x12);


    trans.write(FUNDAMENTAL::SET_CONTRAST);
    trans.write(0x7f);


    trans.write(FUNDAMENTAL::DISPLAY_ON_RAM);

    trans.write(FUNDAMENTAL::DISPLAY_NO_INVERSE);


    trans.write(TIMING::SET_DISPLAY_CLOCK_DIVIDE);
    trans.write(0x80);

    trans.write(CHARGE_BUMP::SET_CHARGE_BUMP);
    trans.write(0x14);

    trans.write(TIMING::SET_PRE_CHARGE_PERIOD);
    trans.write(0x22);

    trans.write(TIMING::SET_VCOMH_DESELECT);
    trans.write(0x30);

    trans.write(FUNDAMENTAL::DISPLAY_ON);
}

void ssd1306::ssd1306::write_data(size_t n, uint8_t *data) {
    auto write_trans = i2c.write(OLED_ADDRESS);
    write_trans.write(CONTROL::DATA);

    for (size_t i = 0; i < n; i++) {
        write_trans.write(*data++);
    }
}

void ssd1306::ssd1306::write_command(uint8_t command, size_t n, uint8_t *data) {
    auto write_trans = i2c.write(OLED_ADDRESS);
    write_trans.write(CONTROL::CONTINUATION);
    write_trans.write(command);

    for (size_t i = 0; i < n; i++) {
        write_trans.write(1u << 7u);
        write_trans.write(*data++);
    }
}

void ssd1306::ssd1306::write_command_stream(uint8_t command, size_t n, uint8_t *data) {
    auto write_trans = i2c.write(OLED_ADDRESS);
    write_trans.write(0);
    write_trans.write(command);

    for (size_t i = 0; i < n; i++) {
        write_trans.write(*data++);
    }
}
