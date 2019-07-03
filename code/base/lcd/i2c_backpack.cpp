//
// Created by Niels on 7/2/2019.
//

#include <hwlib.hpp>
#include "i2c_backpack.hpp"

lcd::i2c_backpack::i2c_backpack(hwlib::i2c_bus &bus, uint8_t rows, uint8_t cols, uint8_t address) : bus(bus),
                                                                                                    rows(rows),
                                                                                                    cols(cols),
                                                                                                    address(address) {
    write(0x03);
    write(0x03);
    write(0x03);
    write(0x02);

    write(BASE::FUNCTIONSET | FUNCTIONSET::_2LINE | FUNCTIONSET::_5x8DOTS | FUNCTIONSET::_4BITMODE);
    write(BASE::DISPLAYCONTROL | ONOFF::DISPLAYON);
    write(BASE::CLEARDISPLAY);
    write(BASE::ENTRYMODESET | ENTRY::ENTRYLEFT);

}


void lcd::i2c_backpack::strobe(uint8_t data) {
    bus.write(address).write(data | BITS::En | BACKLIGHT::YES);
    hwlib::wait_us(500);
    bus.write(address).write(((data & ~BITS::En) | BACKLIGHT::YES));
}

void lcd::i2c_backpack::write_four_bits(uint8_t data) {
    bus.write(address).write(data | BACKLIGHT::YES);
    strobe(data);
}

void lcd::i2c_backpack::write(uint8_t cmd, uint8_t mode) {
    write_four_bits(mode | (cmd & ((uint8_t) 0xF0)));
    write_four_bits(mode | ((cmd << 4) & ((uint8_t) 0xF0)));
}

void lcd::i2c_backpack::display_string(char *data, uint8_t n, uint8_t line) {
    if (line == 0)
        write(0x80);
    if (line == 1)
        write(0xC0);
    if (line == 2)
        write(0x94);
    if (line == 3)
        write(0xD4);

    for (size_t i = 0; i < n; i++) {
        write((uint8_t) *data++, BITS::Rs);
    }
}

void lcd::i2c_backpack::clear() {
    write(BASE::CLEARDISPLAY);
    write(BASE::RETURNHOME);
}

void lcd::i2c_backpack::set_row(uint8_t currentRow) {
    write(BASE::RETURNHOME);
    flush();
    row = currentRow;
}

void lcd::i2c_backpack::putc(char c) {
    if (positions[row] < cols) {
        buffer[row][positions[row]++] = c;
    }
}


void lcd::i2c_backpack::flush(void) {
//    write(BASE::CLEARDISPLAY);
    display_string(buffer[0], positions[0], 0);
    display_string(buffer[1], positions[1], 1);
    for (uint8_t i = 0; i < cols; i++) {
        buffer[0][i] = ' ';
        buffer[1][i] = ' ';
    }
    positions[0] = 0;
    positions[1] = 0;
}


