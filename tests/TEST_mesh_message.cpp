//
// Created by Niels on 6/25/2019.
//

#include "catch.hpp"
#include "mesh/message.hpp"

TEST_CASE("message") {
    hwlib::cout << "Testing message" << hwlib::endl;
}

TEST_CASE("message, constructor") {
    mesh::message msg = {0x02, 0x04, 0x06, 0x08, 0x0A};

    REQUIRE(msg.type == 0x02);
    REQUIRE(msg.message_id == 0x04);
    REQUIRE(msg.sender == 0x06);
    REQUIRE(msg.receiver == 0x08);
    REQUIRE(msg.dataSize == 0x0A);
    REQUIRE(msg.connectionData[0] == 0x00);
    REQUIRE(msg.connectionData[1] == 0x00);
}

TEST_CASE("message, size no payload") {
    mesh::message msg = {0x02, 0x04, 0x06, 0x08, 0x00};
    REQUIRE(msg.size() == 7);
}

TEST_CASE("message, size with payload") {
    mesh::message msg = {0x02, 0x04, 0x06, 0x08, 0x0A};
    REQUIRE(msg.size() == 17);
}


TEST_CASE("message, to_byte_array no payload") {
    mesh::message msg = {0x02, 0x04, 0x06, 0x08, 0x00};

    uint8_t data[msg.size()];

    msg.to_byte_array(data);

    REQUIRE(data[0] == 0x02);
    REQUIRE(data[1] == 0x04);
    REQUIRE(data[2] == 0x06);
    REQUIRE(data[3] == 0x08);
    REQUIRE(data[4] == 0x00);
    REQUIRE(data[5] == 0x00);
    REQUIRE(data[6] == 0x00);
}

TEST_CASE("message, to_byte_array with payload") {
    mesh::message msg = {0x02, 0x04, 0x06, 0x08, 0x02};

    msg.data[0] = 0x0C;
    msg.data[1] = 0x0E;

    uint8_t data[msg.size()];

    msg.to_byte_array(data);

    REQUIRE(data[0] == 0x02);
    REQUIRE(data[1] == 0x04);
    REQUIRE(data[2] == 0x06);
    REQUIRE(data[3] == 0x08);
    REQUIRE(data[4] == 0x02);
    REQUIRE(data[5] == 0x0C);
    REQUIRE(data[6] == 0x0E);
    REQUIRE(data[7] == 0x00);
    REQUIRE(data[8] == 0x00);
}


TEST_CASE("message, parse without payload") {
    uint8_t data[7] = {0x03, 0x05, 0x07, 0x09, 0x00, 0x0D, 0x0F};

    mesh::message msg;
    msg.parse(7, data);
    REQUIRE(msg.type == 0x03);
    REQUIRE(msg.message_id == 0x05);
    REQUIRE(msg.sender == 0x07);
    REQUIRE(msg.receiver == 0x09);
    REQUIRE(msg.dataSize == 0x00);
    REQUIRE(msg.connectionData[0] == 0x0D);
    REQUIRE(msg.connectionData[1] == 0x0F);
}

TEST_CASE("message, parse with payload") {
    uint8_t data[9] = {0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};

    mesh::message msg;
    msg.parse(9, data);
    REQUIRE(msg.type == 0x02);
    REQUIRE(msg.message_id == 0x03);
    REQUIRE(msg.sender == 0x04);
    REQUIRE(msg.receiver == 0x05);
    REQUIRE(msg.dataSize == 0x06);
    REQUIRE(msg.data[0] == 0x07);
    REQUIRE(msg.data[1] == 0x08);
    REQUIRE(msg.connectionData[0] == 0x09);
    REQUIRE(msg.connectionData[1] == 0x0A);
}