//
// Created by Niels on 6/10/2019.
//

#ifndef IPASS_MESH_MESSAGE_HPP
#define IPASS_MESH_MESSAGE_HPP

#include <cstdint>

namespace mesh {
    enum message_type {
        DISCOVERY

    };

    struct mesh_message {
        uint8_t configuration;
        uint8_t sender;
        uint8_t receiver;
        uint8_t size;
        uint8_t *data;

        mesh_message(uint8_t configuration, uint8_t sender, uint8_t receiver, uint8_t size, uint8_t *data);
    };
}

#endif //IPASS_MESH_MESSAGE_HPP
