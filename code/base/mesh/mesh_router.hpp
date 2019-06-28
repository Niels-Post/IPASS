//
// Created by Niels on 6/10/2019.
//

#ifndef IPASS_MESH_ROUTER_HPP
#define IPASS_MESH_ROUTER_HPP


#include "mesh_connectivity_adapter.hpp"

namespace mesh {
    class mesh_router {
    protected:
        mesh_connectivity_adapter &connectivity;
    public:
        mesh_router(mesh_connectivity_adapter &connectivity) : connectivity(connectivity) {}

        virtual void update_neighbours() = 0;
        virtual void send_update() = 0;
        virtual void initial_update() = 0;
        virtual void on_routing_message(mesh_message &message) = 0;

        virtual node_id get_next_hop(const node_id &receiver) = 0;
    };
}

#endif //IPASS_MESH_ROUTER_HPP
