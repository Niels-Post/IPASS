//
// Created by Niels on 7/2/2019.
//

#ifndef IPASS_I2C_BACKPACK_HPP
#define IPASS_I2C_BACKPACK_HPP

#include <cstdint>
#include <hwlib.hpp>

namespace lcd {

    /**
     * \defgroup i2c_backpack I2C LCD Backpack Library
     * \brief Simple library for controlling an LCD library using an I2C backpack
     *
     * This is about the most basic implementation you can get, it can only clear the screen and write text to it. \n
     * An operator<< is implemented to enable the use of functions like setw.\n
     * Since I couldn't actually find the datasheet of this module, I only have the command list, but don't have understanding for all of them.\n
     * Therefore the documentation is lacking.\n
     * This class is based on https://github.com/DzikuVx/WinguWeatherRaspberryPi/blob/master/lcddriver.py\n
     */

    /**
     * \addtogroup i2c_backpack
     * @{
     */

    /// \brief Basic commands for the I2C backpack
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

    /// \brief Values for the BASE::ENTRYMODESET command
    struct ENTRY {
        static constexpr const uint8_t ENTRYRIGHT = 0x00;
        static constexpr const uint8_t ENTRYLEFT = 0x02;
        static constexpr const uint8_t ENTRYSHIFTINCREMENT = 0x01;
        static constexpr const uint8_t ENTRYSHIFTDECREMENT = 0x00;
    };

    /// \brief Commands for interacting with ON-Off Settings
    struct ONOFF {
        static constexpr const uint8_t DISPLAYON = 0x04;
        static constexpr const uint8_t DISPLAYOFF = 0x00;
        static constexpr const uint8_t CURSORON = 0x02;
        static constexpr const uint8_t CURSOROFF = 0x00;
        static constexpr const uint8_t BLINKON = 0x01;
        static constexpr const uint8_t BLINKOFF = 0x00;
    };

    /// \brief Commands for moving the cursor
    struct SHIFT {
        static constexpr const uint8_t DISPLAYMOVE = 0x08;
        static constexpr const uint8_t CURSORMOVE = 0x00;
        static constexpr const uint8_t MOVERIGHT = 0x04;
        static constexpr const uint8_t MOVELEFT = 0x00;
    };

    /// \brief Commands for setting operating modes
    struct FUNCTIONSET {
        static constexpr const uint8_t _8BITMODE = 0x10;
        static constexpr const uint8_t _4BITMODE = 0x00;
        static constexpr const uint8_t _2LINE = 0x08;
        static constexpr const uint8_t _1LINE = 0x00;
        static constexpr const uint8_t _5x10DOTS = 0x04;
        static constexpr const uint8_t _5x8DOTS = 0x00;
    };

    /// \brief Bit values for enabling/disabling the backlight
    struct BACKLIGHT {
        static constexpr const uint8_t YES = 0x08;
        static constexpr const uint8_t NO = 0x00;
    };

    /// \brief Bit values for commands
    struct BITS {
        static constexpr const uint8_t En = 0b00000100;
        static constexpr const uint8_t Rw = 0b00000010;
        static constexpr const uint8_t Rs = 0b00000001;
    };

    /**
     * \brief Library for an i2c LCD backpack
     *
     * This class only supports 2 row, 16 column LCD's for now.
     */
    class i2c_backpack : public hwlib::ostream {
        /// I2C bus to use to communicate with the LCD
        hwlib::i2c_bus &bus;
        /// Number of rows, not really used right now, except for argument validation
        uint8_t rows;
        /// Number of columns, also not really used
        uint8_t cols;
        /// I2C address of the module to connect to (default is 0x27)
        uint8_t address;
        /// Number of the row this module is currently writing on
        uint8_t row = 0;
        /// Character buffer
        char buffer[2][16] = {};
        /// Current writing positions in character buffer
        uint8_t positions[2] = {0};
    public:
        /**
         * \brief Create an I2C backpack
         * @param bus  Bus to connect to backpack through
         * @param rows number of rows the LCD has
         * @param cols number of columns the LCD has
         * @param address Address of the LCD display
         */
        i2c_backpack(hwlib::i2c_bus &bus, uint8_t rows, uint8_t cols, uint8_t address = 0x27);

        /**
         * \brief Not sure what this does exactly. It transmits data to the LCD.
         * @param data Data to send
         */
        void strobe(uint8_t data);

        /**
         * \brief Also not sure about this. For some reason it duplicates a 4 bit command, and sends it twice, together in one byte....
         * @param data Data to send
         */
        void write_four_bits(uint8_t data);

        /**
         * \brief Write a command to the backpack
         * @param cmd  Command to write
         * @param mode Read/write mode,
         */
        void write(uint8_t cmd, uint8_t mode = 0);

        /**
         * \brief Display a string of characters on the LCD
         *
         * @param data Pointer to array of characters to write
         * @param n Size of array
         * @param line Line number to write to
         */
        void display_string(char *data, uint8_t n, uint8_t line);

        /**
         * \bref Clear the display
         */
        void clear();

        /**
         * \brief Set the row to write on
         *
         * Note that this only influences the ostream operators, not display_string() since it immediately writes to a specified row
         * @param currentRow The new row
         */
        void set_row(uint8_t currentRow);

    private:
        /**
         * \brief Put a single character into the buffer, used for the ostream implementation
         *
         * @param c Character to buffer
         */
        void putc(char c) override;

    public:
        /**
         * \brief Flush the buffer eq. display buffer to screen
         */
        void flush(void) override;
    };

    /**
     * @}
     */

}


#endif //IPASS_I2C_BACKPACK_HPP
