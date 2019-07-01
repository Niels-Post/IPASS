//
// Created by Niels on 6/22/2019.
//

#ifndef IPASS_MESH_MESSAGE_TYPES_HPP
#define IPASS_MESH_MESSAGE_TYPES_HPP

namespace mesh {

    enum mesh_connection_state {
        DISCONNECTED,
        WAITING,
        RESPONDED,
        ACCEPTED
    };

    struct DISCOVERY {
        static constexpr const uint8_t NO_OPERATION = 0x00;
        static constexpr const uint8_t PRESENT = 0x01;
        static constexpr const uint8_t RESPOND = 0x02;
        static constexpr const uint8_t ACCEPT = 0x03;
        static constexpr const uint8_t DENY = 0x04;

    };

    struct LINK_STATE_ROUTING {
        static constexpr const uint8_t UPDATE_REQUEST = 0x10;
        static constexpr const uint8_t UPDATE = 0x11;
    };

    struct DOMOTICA {
        static constexpr const uint8_t DATA = 0x20;
    };

}

#endif //IPASS_MESH_MESSAGE_TYPES_HPP
