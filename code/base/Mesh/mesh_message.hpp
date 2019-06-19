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
        DISCOVERY_CONNECT,
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

    };
}

#endif //IPASS_MESH_MESSAGE_HPP
