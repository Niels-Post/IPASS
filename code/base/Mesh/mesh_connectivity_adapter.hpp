//
// Created by Niels on 6/10/2019.
//

#ifndef IPASS_MESH_CONNECTIVITY_ADAPTER_HPP
#define IPASS_MESH_CONNECTIVITY_ADAPTER_HPP

#include <hwlib.hpp>

#include "mesh_message.hpp"



namespace mesh {



    enum mesh_connection_state {
        DISCONNECTED,
        WAITING,
        RESPONDED,
        ACCEPTED
    };

    class mesh_connectivity_adapter {
    public:
        uint8_t address;

        mesh_connectivity_adapter(uint8_t address) : address(address) {}

        virtual void broadcast(mesh_message &message) = 0;

        virtual bool unicast(mesh_message &message, uint8_t next_address = 0) = 0;

        virtual bool is_message_available() = 0;

        virtual mesh_message next_message() = 0;

        virtual bool direct_connection_possible() = 0;

        virtual mesh_connection_state connection_state(const uint8_t &address) = 0;

        virtual bool discovery_respond(mesh::mesh_message &origin) = 0;

        virtual bool on_discovery_respond(mesh::mesh_message &origin) = 0;

        virtual void on_discovery_accept(mesh::mesh_message &origin) = 0;



        virtual void remove_direct_connection(const uint8_t &address) = 0;
    };

}
#endif //IPASS_MESH_CONNECTIVITY_ADAPTER_HPP
