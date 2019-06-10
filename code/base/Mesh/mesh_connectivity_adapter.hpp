//
// Created by Niels on 6/10/2019.
//

#ifndef IPASS_MESH_CONNECTIVITY_ADAPTER_HPP
#define IPASS_MESH_CONNECTIVITY_ADAPTER_HPP
#include <hwlib.hpp>

#include "mesh_message.hpp"
namespace mesh {
    class mesh_connectivity_adapter {
    public:
        virtual void broadcast(mesh_message &message) = 0;
        virtual bool unicast(mesh_message &message, uint8_t next_address) = 0;
        virtual bool is_message_available() = 0;
        virtual mesh_message first_message() = 0;

        virtual bool direct_connection_possible() = 0;
        virtual void add_direct_connection(const uint8_t &address) = 0;
        virtual void remove_direct_connection(const uint8_t &address) = 0;
    };

}
#endif //IPASS_MESH_CONNECTIVITY_ADAPTER_HPP
