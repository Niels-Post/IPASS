//
// Created by Niels on 6/25/2019.
//

#include <spi/bus_testing.hpp>
#include <catch.hpp>
#include <nrf24l01plus/nrf24l01plus.hpp>
#include <pin_debug.hpp>

using spi::bus_testing;

SCENARIO("nrf24l01plus") {
    GIVEN("An NRF24L01+ instance, connected using a SPI test bus and a debug ce pin") {
        auto spi = bus_testing();
        auto ce = pin_debug();
        auto nrf = nrf24l01::nrf24l01plus(spi, hwlib::pin_out_dummy, ce);

        WHEN("register_bytes is called") {
            THEN("method returns 5 on all multibyte registers") {
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::RX_ADDR_P0) == 5);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::TX_ADDR) == 5);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::RX_ADDR_P1) == 5);
            }
            AND_THEN("method returns 1 on non-multibyte registers") {
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::CONFIG) == 1);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::EN_AA) == 1);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::EN_RXADDR) == 1);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::SETUP_AW) == 1);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::SETUP_RETR) == 1);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::RF_CH) == 1);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::RF_SETUP) == 1);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::NRF_STATUS) == 1);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::OBSERVE_TX) == 1);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::RPD) == 1);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::RX_ADDR_P2) == 1);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::RX_ADDR_P3) == 1);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::RX_ADDR_P4) == 1);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::RX_ADDR_P5) == 1);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::RX_PW_P0) == 1);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::RX_PW_P1) == 1);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::RX_PW_P2) == 1);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::RX_PW_P3) == 1);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::RX_PW_P4) == 1);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::RX_PW_P5) == 1);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::FIFO_STATUS) == 1);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::DYNPD) == 1);
                REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::FEATURE) == 1);
            }
        }

        WHEN("A single command byte is sent") {
            nrf.send_command(0x2F);
            THEN("The byte is written to spi unmodified") {
                REQUIRE(spi.out_buffer_size == 1);
                REQUIRE(spi.out_buffer[0] == 0x2F);
            }
        }

        WHEN("A command with payload is sent") {
            uint8_t data[2] = {0x02, 0x04};
            nrf.send_command(0xFF, data, 2);

            THEN("The command byte and data is all written to the SPI bus") {
                REQUIRE(spi.out_buffer_size == 3);
                REQUIRE(spi.out_buffer[0] == 0xFF);
                REQUIRE(spi.out_buffer[1] == 0x02);
                REQUIRE(spi.out_buffer[2] == 0x04);
            }
        }

        WHEN("A command with payload is written LSByte first") {
            uint8_t data[2] = {0x03, 0x05};
            nrf.send_command(0x00, data, 2, nullptr, true);
            THEN("The command byte is written first") {
                REQUIRE(spi.out_buffer[0] == 0x00);
            }
            AND_THEN("The payload bytes are written in reverse order") {
                REQUIRE(spi.out_buffer[1] == 0x05);
                REQUIRE(spi.out_buffer[2] == 0x03);
            }
            AND_THEN("All bytes are written") {
                REQUIRE(spi.out_buffer_size == 3);
            }
        }

        WHEN("Data is read") {
            std::array<uint8_t, 4> data_prepare = {0x02, 0x04, 0x06, 0x08};
            spi.append_in_buffer(data_prepare);
            uint8_t data[3] = {0};
            nrf.send_command(0x09, nullptr, 3, data);

            THEN("Command byte is still written") {
                REQUIRE(spi.out_buffer[0] == 0x09);
            }
            AND_THEN("Data is read correctly") {
                REQUIRE(spi.in_buffer_size == 4);
                REQUIRE(data[0] == 0x04);
                REQUIRE(data[1] == 0x06);
                REQUIRE(data[2] == 0x08);
            }
            AND_THEN("Last status is set") {
                REQUIRE(nrf.last_status == 0x02);
            }
        }

        WHEN("Data is read LSByte first") {
            std::array<uint8_t, 4> data_prepare = {0x02, 0x04, 0x06, 0x08};
            spi.append_in_buffer(data_prepare);
            uint8_t data[3] = {0};
            nrf.send_command(0x00, nullptr, 3, data, true);

            THEN("Last_status is set correctly") {
                REQUIRE(nrf.last_status == 0x02);
            }
            AND_THEN("Data is read in reverse order") {
                REQUIRE(data[0] == 0x08);
                REQUIRE(data[1] == 0x06);
                REQUIRE(data[2] == 0x04);
                REQUIRE(spi.in_buffer_size == 4);
            }
        }

        WHEN("Chip mode is switched") {
            THEN("ce has the right state") {
                nrf.mode(nrf.MODE_NONE);
                REQUIRE(!ce.value);
                nrf.mode(nrf.MODE_PRX);
                REQUIRE(ce.value);
                REQUIRE(ce.was_flushed());
                nrf.mode(nrf.MODE_PTX);
                REQUIRE(!ce.value);
                REQUIRE(ce.was_flushed());
                nrf.mode(nrf.MODE_NONE);
                REQUIRE(!ce.value);
                REQUIRE(ce.was_flushed());
            }
        }

        WHEN("Chip mode is 'switched' to the same mode") {
            nrf.mode(nrf.MODE_PRX);
            spi.clear();
            nrf.mode(nrf.MODE_PRX);
            THEN("No SPI commands are sent") {
                REQUIRE(spi.out_buffer_size == 0);
                REQUIRE(spi.in_buffer_size == 0);
            }
        }

        WHEN("Chip mode is switched to PRX") {
            nrf.mode(nrf.MODE_PRX);
            THEN("Config register is read") {
                REQUIRE(spi.out_buffer[0] == (nrf24l01::NRF_REGISTER::CONFIG | nrf24l01::NRF_INSTRUCTION::R_REGISTER));
            }
            AND_THEN("Config register is written") {
                REQUIRE(spi.out_buffer[2] == (nrf24l01::NRF_REGISTER::CONFIG | nrf24l01::NRF_INSTRUCTION::W_REGISTER));
            }
            AND_THEN("Config register is written with the PRIM_RX bit set") {
                REQUIRE(spi.out_buffer[3] == 0x01);
            }
        }

        WHEN("Chip mode is switched to PTX") {
            std::array<uint8_t, 2> register_data = {0x00, 0xF1};
            spi.append_in_buffer(register_data);
            nrf.mode(nrf.MODE_PTX);

            THEN("Config register is read") {
                REQUIRE(spi.out_buffer[0] == (nrf24l01::NRF_REGISTER::CONFIG | nrf24l01::NRF_INSTRUCTION::R_REGISTER));
            }
            AND_THEN("Config register is written") {
                REQUIRE(spi.out_buffer[2] == (nrf24l01::NRF_REGISTER::CONFIG | nrf24l01::NRF_INSTRUCTION::W_REGISTER));
            }
            AND_THEN("Value of config register with PRIM_RX bit unset") {
                REQUIRE(spi.out_buffer[3] == 0xF0);
            }
        }

        WHEN("The chip is powered on, then off") {
            nrf.power(true);
            THEN("Config register is read") {
                REQUIRE(spi.out_buffer[0] == (nrf24l01::NRF_REGISTER::CONFIG | nrf24l01::NRF_INSTRUCTION::R_REGISTER));
            }
            AND_THEN("Config register is written") {
                REQUIRE(spi.out_buffer[2] == (nrf24l01::NRF_REGISTER::CONFIG | nrf24l01::NRF_INSTRUCTION::W_REGISTER));
            }
            AND_THEN("Value of config register with POWER bit set") {
                REQUIRE(spi.out_buffer[3] == 0x02);
            }

            spi.clear();
            nrf.power(false);
            THEN("Config register is read") {
                REQUIRE(spi.out_buffer[0] == (nrf24l01::NRF_REGISTER::CONFIG | nrf24l01::NRF_INSTRUCTION::R_REGISTER));
            }
            AND_THEN("Config register is written") {
                REQUIRE(spi.out_buffer[2] == (nrf24l01::NRF_REGISTER::CONFIG | nrf24l01::NRF_INSTRUCTION::W_REGISTER));
            }
            AND_THEN("Value of config register with POWER bit unset") {
                REQUIRE(spi.out_buffer[3] == 0x00);
            }
        }

        WHEN("A 5 byte address is set") {
            nrf24l01::address address = {0x02, 0x04, 0x06, 0x08, 0x0A};
            nrf.rx_set_address(0, address);
            THEN("All bytes are written, including command byte") {
                REQUIRE(spi.out_buffer_size == 6);
            }
            AND_THEN("address bytes are written in reverse") {
                for (size_t i = 0; i < 5; i++) {
                    REQUIRE(spi.out_buffer[i + 1] == address.address_bytes[4 - i]);
                }
            }
        }

        WHEN("A 1 byte address is set") {
            nrf24l01::address address = {0x02, 0x04, 0x06, 0x08, 0x0A};
            nrf.rx_set_address(2, address);
            THEN("Only 1 address byte is sent") {
                REQUIRE(spi.out_buffer_size == 2);
            }
            AND_THEN("The least significant adress byte is sent") {
                REQUIRE(spi.out_buffer[1] == 0x0A);

            }
        }

        WHEN("RX Address 0 is written") {
            nrf24l01::address address = {0x02, 0x04, 0x06, 0x08, 0x0A};
            nrf.rx_set_address(0, address);
            THEN("The right register address is put on the bus") {
                REQUIRE(spi.out_buffer[0] ==
                        (nrf24l01::NRF_INSTRUCTION::W_REGISTER | nrf24l01::NRF_REGISTER::RX_ADDR_P0));
            }
        }

        WHEN("RX Address 1 is written") {
            nrf24l01::address address = {0x02, 0x04, 0x06, 0x08, 0x0A};
            nrf.rx_set_address(1, address);
            THEN("The right register address is put on the bus") {
                REQUIRE(spi.out_buffer[0] ==
                        (nrf24l01::NRF_INSTRUCTION::W_REGISTER | nrf24l01::NRF_REGISTER::RX_ADDR_P1));
            }
        }

        WHEN("RX Address 2 is written") {
            nrf24l01::address address = {0x02, 0x04, 0x06, 0x08, 0x0A};
            nrf.rx_set_address(2, address);
            THEN("The right register address is put on the bus") {
                REQUIRE(spi.out_buffer[0] ==
                        (nrf24l01::NRF_INSTRUCTION::W_REGISTER | nrf24l01::NRF_REGISTER::RX_ADDR_P2));
            }
        }

        WHEN("RX Address 3 is written") {
            nrf24l01::address address = {0x02, 0x04, 0x06, 0x08, 0x0A};
            nrf.rx_set_address(3, address);
            THEN("The right register address is put on the bus") {
                REQUIRE(spi.out_buffer[0] ==
                        (nrf24l01::NRF_INSTRUCTION::W_REGISTER | nrf24l01::NRF_REGISTER::RX_ADDR_P3));
            }
        }

        WHEN("RX Address 4 is written") {
            nrf24l01::address address = {0x02, 0x04, 0x06, 0x08, 0x0A};
            nrf.rx_set_address(4, address);
            THEN("The right register address is put on the bus") {
                REQUIRE(spi.out_buffer[0] ==
                        (nrf24l01::NRF_INSTRUCTION::W_REGISTER | nrf24l01::NRF_REGISTER::RX_ADDR_P4));
            }
        }

        WHEN("RX Address 5 is written") {
            nrf24l01::address address = {0x02, 0x04, 0x06, 0x08, 0x0A};
            nrf.rx_set_address(5, address);
            THEN("The right register address is put on the bus") {
                REQUIRE(spi.out_buffer[0] ==
                        (nrf24l01::NRF_INSTRUCTION::W_REGISTER | nrf24l01::NRF_REGISTER::RX_ADDR_P5));
            }
        }

    }
}
