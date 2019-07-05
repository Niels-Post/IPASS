//
// Created by Niels on 6/25/2019.
//

#include <single_include/catch2/catch.hpp>
#include <mesh/message.hpp>

SCENARIO("Mesh message constructor works properly") {
    GIVEN("nothing") {
        WHEN("The constructor is called with only require arguments") {
            mesh::message msg = {0x02, 0x04, 0x06, 0x08, 0x0A};
            THEN("All fields are set correctly") {
                REQUIRE(msg.type == 0x02);
                REQUIRE(msg.message_id == 0x04);
                REQUIRE(msg.sender == 0x06);
                REQUIRE(msg.receiver == 0x08);
                REQUIRE(msg.dataSize == 0x0A);
                REQUIRE(msg.connectionData[0] == 0x00);
                REQUIRE(msg.connectionData[1] == 0x00);
            }
        }

        WHEN("The constructor is called with data size 0") {
            mesh::message msg = {0x02, 0x04, 0x06, 0x08, 0x00};
            THEN("The message byte size is calculated correctly") {
                REQUIRE(msg.size() == 7);
            }
        }

        WHEN("The constructor is called with data size > 0") {
            mesh::message msg = {0x02, 0x04, 0x06, 0x08, 0x0A};
            THEN("The message byte size is calculated correctly") {
                REQUIRE(msg.size() == 17);
            }
        }
    }
}

SCENARIO("message to_byte_array works propery") {
    GIVEN("A dummy message") {
        mesh::message msg = {0x02, 0x04, 0x06, 0x08, 0x00};

        WHEN("payload size is 0") {
            uint8_t data[msg.size()];
            msg.to_byte_array(data);
            THEN("Bytes are formed properly") {
                REQUIRE(data[0] == 0x02);
                REQUIRE(data[1] == 0x04);
                REQUIRE(data[2] == 0x06);
                REQUIRE(data[3] == 0x08);
                REQUIRE(data[4] == 0x00);
                REQUIRE(data[5] == 0x00);
                REQUIRE(data[6] == 0x00);
            }
        }

        WHEN("payload size is > 0") {
            msg.dataSize = 2;
            msg.data[0] = 0x0C;
            msg.data[1] = 0x0E;
            uint8_t data[msg.size()];
            msg.to_byte_array(data);
            THEN("The data is added to the message correctly") {
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
        }
    }
}

SCENARIO("Message parsing works properly") {
    mesh::message msg;
    GIVEN("an empty message") {
        WHEN("data is given without payload") {
            uint8_t data[7] = {0x03, 0x05, 0x07, 0x09, 0x00, 0x0D, 0x0F};
            msg.parse(7, data);
            THEN("All fields are set properly") {
                REQUIRE(msg.type == 0x03);
                REQUIRE(msg.message_id == 0x05);
                REQUIRE(msg.sender == 0x07);
                REQUIRE(msg.receiver == 0x09);
                REQUIRE(msg.dataSize == 0x00);
                REQUIRE(msg.connectionData[0] == 0x0D);
                REQUIRE(msg.connectionData[1] == 0x0F);
            }
        }

        WHEN("data is given with payload") {
            uint8_t data[9] = {0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
            msg.parse(9, data);
            THEN("All fields are set properly") {
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
        }
    }
}
