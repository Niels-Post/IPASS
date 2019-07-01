//
// Created by Niels on 6/25/2019.
//
#include <hwlib.hpp>
#include "catch.hpp"
#include "nrf24l01/nrf_address.hpp"

using nrf24l01::nrf_address;

TEST_CASE("nrf_address") {
    hwlib::cout << "Testing nrf_address" << hwlib::endl;
}

TEST_CASE("nrf_address, empty constructor") {

    nrf_address empty_constructor;

    for (uint8_t address_byte : empty_constructor.address_bytes) {
        REQUIRE(address_byte == 0);
    }
}

TEST_CASE("nrf_address, 5 argument constructor") {
    nrf_address constructor_5(2, 4, 6, 8, 10);
    REQUIRE(constructor_5.address_bytes[0] == 2);
    REQUIRE(constructor_5.address_bytes[1] == 4);
    REQUIRE(constructor_5.address_bytes[2] == 6);
    REQUIRE(constructor_5.address_bytes[3] == 8);
    REQUIRE(constructor_5.address_bytes[4] == 10);
}

TEST_CASE("nrf_address, base address constructor") {
    auto base_address = nrf_address(0x48, 0x50, 0x52, 0x54, 0x56);
    auto new_address = nrf_address(base_address, 0x70);


    REQUIRE(new_address.address_bytes[0] == 0x48);
    REQUIRE(new_address.address_bytes[1] == 0x50);
    REQUIRE(new_address.address_bytes[2] == 0x52);
    REQUIRE(new_address.address_bytes[3] == 0x54);
    REQUIRE(new_address.address_bytes[4] == 0x70);
}

TEST_CASE("nrf_address, equality rhs nrf_address") {
    nrf_address address1 = {0x48, 0x50, 0x52, 0x54, 0x56};
    nrf_address address2 = {0x48, 0x50, 0x52, 0x54, 0x56};
    nrf_address address3 = {0x48, 0x50, 0x00, 0x54, 0x56};


    CHECK(address1 == address2);
    CHECK(!(address1 == address3));
}


TEST_CASE("nrf_address, inequality rhs nrf_address") {
    auto address1 = nrf_address(0x00, 0x04, 0x08, 0x10, 0xF2);
    auto address2 = nrf_address(0x00, 0x04, 0x10, 0x10, 0xF2);
    auto address3 = nrf_address(0x00, 0x04, 0x08, 0x10, 0xF2);

    REQUIRE(address1 != address2);
    REQUIRE(!(address1 != address3));
}


TEST_CASE("nrf_address, equality rhs uint8_t") {
    auto base_address = nrf_address(0x48, 0x50, 0x52, 0x54, 0x56);


    REQUIRE(base_address == 0x56);
    REQUIRE(!(base_address == 0x3));
}


TEST_CASE("nrf_address, inequality rhs uint8_t") {
    auto base_address = nrf_address(0x48, 0x50, 0x52, 0x54, 0x56);

    REQUIRE(base_address != 0x0);
    REQUIRE(!(base_address != 0x56));
}