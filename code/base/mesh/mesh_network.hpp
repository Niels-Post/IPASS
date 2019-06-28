//
// Created by Niels on 6/19/2019.
//

#ifndef IPASS_MESH_NETWORK_HPP
#define IPASS_MESH_NETWORK_HPP


#include "../util/cout_debug.hpp"
#include "mesh_connectivity_adapter.hpp"
#include "mesh_router.hpp"

namespace mesh {
    class mesh_network {
        mesh_connectivity_adapter &connection;
        mesh_router &router;

        std::array<node_id, 10> blacklist = {0};
        size_t blacklist_size = 0;

        uint32_t update_count = 0;
        uint32_t keepalive_interval = 5000;


        bool is_blacklisted(const node_id &id) {
            for (const node_id &checkid : blacklist) {
                if (id == checkid) {
                    return true;
                }
                if (checkid == 0) {
                    break;
                }
            }
            return false;
        }



    public:
        mesh_network(mesh_connectivity_adapter &connection, mesh_router &router) :
                connection(
                        connection),
                router(router) {}

        template <size_t n>
        void add_blacklist(std::array<node_id,n> list) {
            for(const node_id &node: list) {
                blacklist[blacklist_size++] = node;
            }
        }

        void discover() {
            mesh_message message = {DISCOVERY::PRESENT, 0, connection.address, 0,
                                    0}; //Todo something with a message id (singleton?)
            connection.send(message);
        }

        uint8_t check_new_messages(std::array<mesh_message, 10> &uncaught) {
            uint8_t index = 0;
            while (connection.has_message()) {
                mesh_message msg = connection.next_message();
                if (!connection.is_new_message(msg)) { //message already handled
                    continue;
                }
                LOG("Received ", msg);
                if (msg.receiver == connection.address ||
                    msg.receiver == 0) { //Message is for us, or broadcast, take it
                    if(!handleMessage(msg) ) {
                        uncaught[index++] = msg;
                    }
                } else { //Not for us, todo relay message (through routing)
                    uint8_t next_hop = 0;
                    if (connection.connection_state(msg.receiver) != ACCEPTED) {
                        LOG("NOT DIRECT, GETTING NEXT HOP", "");
                        next_hop = router.get_next_hop(msg.receiver);
                    } else {
                        LOG("DIRECT AF, GETTING NEXT HOP", "");
                    }
                    if(!connection.send(msg, next_hop)) {
                        router.update_neighbours();
                    }
                }

            }
            return index;
        }

        void update() {
//            if(update_count%100 == 0) {
//                cout_debug c;
//                c << "." << hwlib::endl;
//            }

            if (update_count++ > keepalive_interval) {
                update_count = 0;
                mesh_message keepalive = {
                        DISCOVERY::NO_OPERATION,
                        0,
                        connection.address,
                        0
                };

                unicast_all_close_if_fail(keepalive);

                discover();
            }
        }

        void unicast_close_if_fail(mesh_message &msg, const node_id &next_hop = 0) {
            if (!connection.send(msg, next_hop)) {
                connection.remove_direct_connection(next_hop != 0 ? next_hop : msg.receiver);
                router.send_update();
            }
        }

        void unicast_all_close_if_fail(mesh_message &msg) {
            node_id failed[connection.get_neighbour_count()];

            if(!connection.send_all(msg, failed)) {
                for(size_t i = 0; i < connection.get_neighbour_count(); i++) {
                    if(failed[i] == 0) {
                        continue;
                    }
                    LOG("FAILED", failed[i]);
                    connection.remove_direct_connection(failed[i] );
                }
                router.send_update();
            }
        }

        void sendMessage(mesh_message &msg) {
            uint8_t nextAddress = router.get_next_hop(msg.receiver);
            msg.sender = connection.address;
            LOG("SENDING", msg);
            if(!connection.send(msg, nextAddress)) {
                // Todo actual error handling
//                router.update_neighbours();
            }
        }

        bool handleMessage(mesh_message &msg) {
            if ((msg.type & 0x10) > 0) { //This is a routing message
                router.on_routing_message(msg);
            }

            if((msg.type & 0x20) > 0) {
                return false;
            }

            if(is_blacklisted(msg.sender)) {
                return true;
            }


            switch (msg.type) {
                case DISCOVERY::PRESENT:
                    if (connection.connection_state(msg.sender) == DISCONNECTED) {
                        if (connection.discovery_present_received(msg)) {
                            mesh_message connectMessage = {DISCOVERY::RESPOND, 0,
                                                           connection.address,
                                                           msg.sender, 0};
                            unicast_close_if_fail(connectMessage);
                        }
                    }
                    break;
                case DISCOVERY::RESPOND: {
                    if (connection.discovery_respond_received(msg)) {

                        mesh_message finishMessage = {DISCOVERY::ACCEPT, 0, connection.address,
                                                      msg.sender, 0};
                        if (connection.send(finishMessage)) {
                            router.update_neighbours();
                        }

                    } else {
                        mesh_message finishMessage = {DISCOVERY::DENY, 0, connection.address,
                                                      msg.sender, 0};
                        connection.send(finishMessage);
                    }
                    break;
                }
                case DISCOVERY::ACCEPT:
                    connection.discovery_accept_received(msg);
                    router.initial_update();
                    break;
                case DISCOVERY::DENY:
                    if (msg.receiver == connection.address) {
                        connection.remove_direct_connection(msg.sender);
                    }
                    break;
                case DISCOVERY::NO_OPERATION:
                    break;
                default:
                    return false;
                    break;
            }
            return true;
        }
    };
}
#endif //IPASS_MESH_NETWORK_HPP
