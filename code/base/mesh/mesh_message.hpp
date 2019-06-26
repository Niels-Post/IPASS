//
// Created by Niels on 6/10/2019.
//

#ifndef IPASS_MESH_MESSAGE_HPP
#define IPASS_MESH_MESSAGE_HPP

#include <hwlib.hpp>
#include "mesh_definitions.hpp"

namespace mesh {

    typedef uint8_t message_type;
    typedef uint8_t node_id;

    struct mesh_message {
        size_t size() {
            return dataSize + 7;
        }

        message_type type;
        uint8_t message_id;
        uint8_t sender;
        uint8_t receiver;
        uint8_t dataSize;
        std::array<uint8_t, 25> data = {0};
        std::array<uint8_t, 2> connectionData = {0};

        mesh_message(message_type messageType, uint8_t messageId, node_id sender, node_id receiver, size_t dataSize = 0,
                     const std::array<uint8_t, 25> &data = {0}, const std::array<uint8_t, 2> &connectionData = {0})
                : type(
                messageType), message_id(messageId), sender(sender), receiver(receiver), dataSize(dataSize),
                  data(data),
                  connectionData(
                          connectionData) {}

        void to_byte_array(uint8_t out[]) {
            out[0] = type;
            out[1] = message_id;
            out[2] = sender;
            out[3] = receiver;
            out[4] = dataSize;

            for (size_t i = 0; i < dataSize; i++) {
                out[i + 5] = data[i];
            }
            out[size() - 2] = connectionData[0];
            out[size() - 1] = connectionData[1];
        }

        static mesh_message parse(size_t n, const uint8_t in[]) {
            if(n < 7) {
                return {0, 0, 0, 0, 0};
            }
            std::array<uint8_t, 25> data = {};

            for (size_t i = 0; i < (n - 7); i++) {
                data[i] = in[i + 5];
            }

            std::array<uint8_t, 2> connectionData = {};
            connectionData[0] = in[n - 2];
            connectionData[1] = in[n - 1];
            return {in[0], in[1], in[2], in[3], in[4], data, connectionData};
        }

    };
}

#endif //IPASS_MESH_MESSAGE_HPP
