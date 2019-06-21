//
// Created by Niels on 6/10/2019.
//

#ifndef IPASS_MESH_MESSAGE_HPP
#define IPASS_MESH_MESSAGE_HPP

#include <cstdint>
#include <type_traits>


namespace mesh {


    enum message_type : uint8_t {
        DISCOVERY_PRESENT,
        DISCOVERY_RESPOND,
        DISCOVERY_ACCEPT,
        DISCOVERY_DENY
    };


    struct mesh_message {
        size_t size() {
            return dataSize + 7;
        }
        message_type configuration;
        uint8_t conversation_id;
        uint8_t sender;
        uint8_t receiver;
        size_t dataSize;
        std::array<uint8_t, 25> data = {0};
        std::array<uint8_t, 2> connectionData = {0};

        mesh_message(message_type configuration, uint8_t messageId, uint8_t sender, uint8_t receiver, size_t dataSize,
                     const std::array<uint8_t, 25> &data = {0}, const std::array<uint8_t, 2> &connectionData = {0}) : configuration(
                configuration), conversation_id(messageId), sender(sender), receiver(receiver), dataSize(dataSize),
                                                                                                          data(data),
                                                                                                          connectionData(
                                                                                                                  connectionData) {}

       void to_byte_array(uint8_t out[]) {
           uint8_t payload[size()];

           payload[0] = uint8_t(configuration);
           payload[1] = conversation_id;
           payload[2] = sender;
           payload[3] = receiver;
           payload[4] = dataSize;

           for (size_t i = 0; i < dataSize; i++) {
               payload[i + 5] = data[i];
           }
           payload[size() - 2] = connectionData[0];
           payload[size() - 1] = connectionData[1];
        }

        static mesh_message parse(size_t n, const uint8_t in[]) {
            std::array<uint8_t, 25> data = {};

            for (size_t i = 0; i < (n-7); i++) {
                data[i] = in[i + 4];
            }

            std::array<uint8_t, 2> connectionData = {};
            connectionData[0] = in[n - 2];
            connectionData[1] = in[n - 1];
            return {mesh::message_type(data[0]), data[1], data[2], data[3], data[4], data, connectionData};
        }

    };
}

#endif //IPASS_MESH_MESSAGE_HPP
