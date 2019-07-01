//
// Created by Niels on 6/10/2019.
//

#ifndef IPASS_MESH_MESSAGE_HPP
#define IPASS_MESH_MESSAGE_HPP

#include <hwlib.hpp>
#include <ostream>
#include "definitions.hpp"

namespace mesh {

    typedef uint8_t message_type;
    typedef uint8_t node_id;

    struct message {
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

        message() : type(0), message_id(0), sender(0), receiver(0), dataSize(0) {}

        message(message_type messageType, uint8_t messageId, node_id sender, node_id receiver, size_t dataSize = 0,
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

        bool parse(size_t n, const uint8_t in[]) {
            data.fill(0);
            connectionData.fill(0);


            if (n < 7) {
                return false;
            }

            type = in[0];
            message_id = in[1];
            sender = in[2];
            receiver = in[3];
            dataSize = in[4];


            for (size_t i = 0; i < (n - 7); i++) {
                data[i] = in[i + 5];
            }
            connectionData[0] = in[n - 2];
            connectionData[1] = in[n - 1];
            return true;
        }

        friend hwlib::ostream &operator<<(hwlib::ostream &os, const message &message) {
            os << "type:" << message.type << " message_id:" << message.message_id << " sender:" << hwlib::hex
               << message.sender
               << " receiver: " << message.receiver << " dataSize:" << hwlib::dec << message.dataSize
               << " connectionData:"
               << message.connectionData[0] << " - ";

            for (size_t i = 0; i < message.dataSize; i++) {
                os << message.data[i] << " ";
            }
            return os;
        }

    };
}

#endif //IPASS_MESH_MESSAGE_HPP
