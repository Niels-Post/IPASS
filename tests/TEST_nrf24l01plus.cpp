//
// Created by Niels on 6/25/2019.
//

#include "spi/spi_testing.hpp"
#include "catch.hpp"
#include "nrf24l01/nrf24l01plus.hpp"
#include "pin_debug.hpp"

using spi_ex::spi_testing;

TEST_CASE("nrf24l01plus") {
    hwlib::cout << "Testing nrf24l01plus" << hwlib::endl;
}

TEST_CASE("nrf24l01plus, register bytes") {
    auto spi = spi_testing();
    auto nrf = nrf24l01::nrf24l01plus(spi, hwlib::pin_out_dummy, hwlib::pin_out_dummy);
    REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::RX_ADDR_P0) == 5);
    REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::TX_ADDR) == 5);
    REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::RX_ADDR_P1) == 5);
    REQUIRE(nrf.register_bytes(nrf24l01::NRF_REGISTER::NRF_STATUS) == 1);
}

TEST_CASE("nrf24l01plus, command, no payload") {
    auto spi = spi_testing();
    auto nrf = nrf24l01::nrf24l01plus(spi, hwlib::pin_out_dummy, hwlib::pin_out_dummy);

    nrf.send_command(0x2F);

    REQUIRE(spi.out_buffer_size == 1);
    REQUIRE(spi.out_buffer[0] == 0x2F);
}

TEST_CASE("nrf24l01plus, command, writing payload") {
    auto spi = spi_testing();
    auto nrf = nrf24l01::nrf24l01plus(spi, hwlib::pin_out_dummy, hwlib::pin_out_dummy);

    uint8_t data[2] = {0x02, 0x04};


    nrf.send_command(0xFF, data, 2);

    REQUIRE(spi.out_buffer_size == 3);
    REQUIRE(spi.out_buffer[0] == 0xFF);
    REQUIRE(spi.out_buffer[1] == 0x02);
    REQUIRE(spi.out_buffer[2] == 0x04);
}

TEST_CASE("nrf24l01plus, command, writing payload reversed") {
    auto spi = spi_testing();
    auto nrf = nrf24l01::nrf24l01plus(spi, hwlib::pin_out_dummy, hwlib::pin_out_dummy);

    uint8_t data[2] = {0x03, 0x05};

    nrf.send_command(0x00, data, 2, nullptr, true);

    REQUIRE(spi.out_buffer_size == 3);
    REQUIRE(spi.out_buffer[0] == 0x00);
    REQUIRE(spi.out_buffer[1] == 0x05);
    REQUIRE(spi.out_buffer[2] == 0x03);
}

TEST_CASE("nrf24l01plus, command, reading payload") {
    auto spi = spi_testing();
    auto nrf = nrf24l01::nrf24l01plus(spi, hwlib::pin_out_dummy, hwlib::pin_out_dummy);


    std::array<uint8_t, 4> data_prepare = {0x02, 0x04, 0x06, 0x08};
    spi.append_in_buffer(data_prepare);

    uint8_t data[3] = {0};

    nrf.send_command(0x00, nullptr, 3, data);

    REQUIRE(spi.in_buffer_size == 4);
    REQUIRE(nrf.last_status == 0x02);
    REQUIRE(data[0] == 0x04);
    REQUIRE(data[1] == 0x06);
    REQUIRE(data[2] == 0x08);
}

TEST_CASE("nrf24l01plus, command, reading payload reversed") {
    auto spi = spi_testing();
    auto nrf = nrf24l01::nrf24l01plus(spi, hwlib::pin_out_dummy, hwlib::pin_out_dummy);


    std::array<uint8_t, 4> data_prepare = {0x02, 0x04, 0x06, 0x08};
    spi.append_in_buffer(data_prepare);

    uint8_t data[3] = {0};

    nrf.send_command(0x00, nullptr, 3, data, true);

    REQUIRE(spi.in_buffer_size == 4);
    REQUIRE(nrf.last_status == 0x02);
    REQUIRE(data[0] == 0x08);
    REQUIRE(data[1] == 0x06);
    REQUIRE(data[2] == 0x04);
}

TEST_CASE("nrf24l01plus, switching mode, chip enable state") {
    auto spi = spi_testing();
    auto ce = pin_debug();
    auto nrf = nrf24l01::nrf24l01plus(spi, hwlib::pin_out_dummy, ce);

    nrf.mode(nrf.MODE_NONE);
    REQUIRE(!ce.value);
    nrf.mode(nrf.MODE_PRX);
    REQUIRE(ce.value);
    nrf.mode(nrf.MODE_PTX);
    REQUIRE(!ce.value);
    nrf.mode(nrf.MODE_NONE);
    REQUIRE(!ce.value);
}

TEST_CASE("nrf24l01plus, switching mode, switching to same mode") {
    auto spi = spi_testing();
    auto nrf = nrf24l01::nrf24l01plus(spi, hwlib::pin_out_dummy, hwlib::pin_out_dummy);

    nrf.mode(nrf.MODE_PRX);
    spi.clear();

    nrf.mode(nrf.MODE_PRX);


    REQUIRE(spi.out_buffer_size == 0);
    REQUIRE(spi.in_buffer_size == 0);
}

TEST_CASE("nrf24l01plus, switching mode to prx, commands") {
    auto spi = spi_testing();
    auto nrf = nrf24l01::nrf24l01plus(spi, hwlib::pin_out_dummy, hwlib::pin_out_dummy);


    nrf.mode(nrf.MODE_NONE);

    nrf.mode(nrf.MODE_PRX);


    REQUIRE(spi.out_buffer[0] == (nrf24l01::NRF_REGISTER::CONFIG | nrf24l01::NRF_INSTRUCTION::R_REGISTER));
    REQUIRE(spi.out_buffer[2] == (nrf24l01::NRF_REGISTER::CONFIG | nrf24l01::NRF_INSTRUCTION::W_REGISTER));
    REQUIRE(spi.out_buffer[3] == 0x01);
}

TEST_CASE("nrf24l01plus, switching mode to ptx, commands") {
    auto spi = spi_testing();
    auto nrf = nrf24l01::nrf24l01plus(spi, hwlib::pin_out_dummy, hwlib::pin_out_dummy);


    nrf.mode(nrf.MODE_NONE);
    std::array<uint8_t, 2> register_data = {0x00, 0xF1};
    spi.append_in_buffer(register_data);

    nrf.mode(nrf.MODE_PTX);


    REQUIRE(spi.out_buffer[0] == (nrf24l01::NRF_REGISTER::CONFIG | nrf24l01::NRF_INSTRUCTION::R_REGISTER));
    REQUIRE(spi.out_buffer[2] == (nrf24l01::NRF_REGISTER::CONFIG | nrf24l01::NRF_INSTRUCTION::W_REGISTER));
    REQUIRE(spi.out_buffer[3] == 0xF0);
}

TEST_CASE("nrf24l01plus, switching power") {
    auto spi = spi_testing();
    auto nrf = nrf24l01::nrf24l01plus(spi, hwlib::pin_out_dummy, hwlib::pin_out_dummy);


    nrf.power(true);
    REQUIRE(spi.out_buffer[3] == 0x02);
    spi.clear();
    nrf.power(false);
    REQUIRE(spi.out_buffer[3] == 0x00);
    REQUIRE(spi.out_buffer_size == 4);
}

TEST_CASE("nrf24l01plus, setting rx_address can set a 5 byte address, and does so in reverse") {
    auto spi = spi_testing();
    auto nrf = nrf24l01::nrf24l01plus(spi, hwlib::pin_out_dummy, hwlib::pin_out_dummy);

    nrf24l01::nrf_address address = {0x02, 0x04, 0x06, 0x08, 0x0A};
    nrf.rx_set_address(0, address);

    REQUIRE(spi.out_buffer_size == 6);
    for (size_t i = 0; i < 5; i++) {
        REQUIRE(spi.out_buffer[i + 1] == address.address_bytes[4 - i]);
    }
}

TEST_CASE("nrf24l01plus, setting rx_address can set a 1 byte address") {
    auto spi = spi_testing();
    auto nrf = nrf24l01::nrf24l01plus(spi, hwlib::pin_out_dummy, hwlib::pin_out_dummy);

    nrf24l01::nrf_address address = {0x02, 0x04, 0x06, 0x08, 0x0A};
    nrf.rx_set_address(2, address);

    REQUIRE(spi.out_buffer_size == 2);
    REQUIRE(spi.out_buffer[1] == 0x0A);
}

TEST_CASE("nrf24l01, setting rx_address writes to the correct registers") {
    auto spi = spi_testing();
    auto nrf = nrf24l01::nrf24l01plus(spi, hwlib::pin_out_dummy, hwlib::pin_out_dummy);

    nrf24l01::nrf_address address = {0x02, 0x04, 0x06, 0x08, 0x0A};

    nrf.rx_set_address(0, address);
    REQUIRE(spi.out_buffer[0] == (nrf24l01::NRF_INSTRUCTION::W_REGISTER | nrf24l01::NRF_REGISTER::RX_ADDR_P0));
    spi.clear();
    nrf.rx_set_address(1, address);
    REQUIRE(spi.out_buffer[0] == (nrf24l01::NRF_INSTRUCTION::W_REGISTER | nrf24l01::NRF_REGISTER::RX_ADDR_P1));
    spi.clear();
    nrf.rx_set_address(2, address);
    REQUIRE(spi.out_buffer[0] == (nrf24l01::NRF_INSTRUCTION::W_REGISTER | nrf24l01::NRF_REGISTER::RX_ADDR_P2));
    spi.clear();
    nrf.rx_set_address(3, address);
    REQUIRE(spi.out_buffer[0] == (nrf24l01::NRF_INSTRUCTION::W_REGISTER | nrf24l01::NRF_REGISTER::RX_ADDR_P3));
    spi.clear();
    nrf.rx_set_address(4, address);
    REQUIRE(spi.out_buffer[0] == (nrf24l01::NRF_INSTRUCTION::W_REGISTER | nrf24l01::NRF_REGISTER::RX_ADDR_P4));
    spi.clear();
    nrf.rx_set_address(5, address);
    REQUIRE(spi.out_buffer[0] == (nrf24l01::NRF_INSTRUCTION::W_REGISTER | nrf24l01::NRF_REGISTER::RX_ADDR_P5));
    spi.clear();


}

