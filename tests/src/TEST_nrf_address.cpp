//
// Created by Niels on 6/25/2019.
//
#include <hwlib.hpp>
#include <catch.hpp>
#include <nrf24l01plus/address.hpp>

using nrf24l01::address;

TEST_CASE("address") {
    hwlib::cout << "Testing address" << hwlib::endl;
}

SCENARIO("address constructor works properly") {
    GIVEN("nothing") {
        WHEN("The default constructor is called") {
            address empty_constructor;
            THEN("All address bytes are set to 0") {
                for (uint8_t address_byte : empty_constructor.address_bytes) {
                    REQUIRE(address_byte == 0);
                }
            }
        }

        WHEN("The 5 argument constructor is called") {
            address constructor_5(2, 4, 6, 8, 10);
            THEN("All bytes are set correctly") {
                REQUIRE(constructor_5.address_bytes[0] == 2);
                REQUIRE(constructor_5.address_bytes[1] == 4);
                REQUIRE(constructor_5.address_bytes[2] == 6);
                REQUIRE(constructor_5.address_bytes[3] == 8);
                REQUIRE(constructor_5.address_bytes[4] == 10);
            }
        }

        WHEN("The base address constructor is called using another address") {
            auto base_address = address(0x48, 0x50, 0x52, 0x54, 0x56);
            auto new_address = address(base_address, 0x70);
            THEN("The first 4 bytes of the base address are used") {
                REQUIRE(new_address.address_bytes[0] == 0x48);
                REQUIRE(new_address.address_bytes[1] == 0x50);
                REQUIRE(new_address.address_bytes[2] == 0x52);
                REQUIRE(new_address.address_bytes[3] == 0x54);
            }
            AND_THEN("The last byte is read from the method argument") {
                REQUIRE(new_address.address_bytes[4] == 0x70);
            }
        }
    }
}

TEST_CASE("address, equality and inequality operators work") {
    GIVEN("3 addresses (1,2 and 3), of which 1 and 2 are completely equal, and 3 differs by one byte") {
        address address1 = {0x48, 0x50, 0x52, 0x54, 0x56};
        address address2 = {0x48, 0x50, 0x52, 0x54, 0x56};
        address address3 = {0x48, 0x50, 0x00, 0x54, 0x56};
        WHEN("Address 1 and 2 are compared") {
            THEN("equality operator returns true") {
                REQUIRE(address1 == address2);
            }
            AND_THEN("inequality operator returns false") {
                REQUIRE(!(address1 != address2));
            }
        }

        WHEN("address 1 and 3 are compared") {
            THEN("equality operator returns false") {
                REQUIRE(!(address1 == address3));
            }
            AND_THEN("inequality operator returns true") {
                REQUIRE(address1 != address3);
            }
        }

        WHEN("An address is compared to it's LSByte") {
            THEN("equality operator returns true") {
                REQUIRE(address1 == 0x56);
            }
            AND_THEN("inequality operator returns false") {

                REQUIRE(!(address1 != 0x56));
            }
        }

        WHEN("An address is compared to a different LSByte") {
            THEN("equality operator returns false") {
                REQUIRE(!(address1 == 0x64));
            }
            AND_THEN("inequality operator returns true") {

                REQUIRE(address1 != 0x64);
            }
        }
    }
}