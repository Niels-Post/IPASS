//
// Created by Niels on 6/19/2019.
//

#ifndef IPASS_MESH_NETWORK_HPP
#define IPASS_MESH_NETWORK_HPP


#include "../util/cout_debug.hpp"
#include "connectivity_adapter.hpp"
#include "router.hpp"

namespace mesh {
    class mesh_network {
        connectivity_adapter &connection;
        router &network_router;

        std::array<node_id, 10> blacklist = {0};
        size_t blacklist_size = 0;

        uint32_t update_count = 0;
        uint32_t keepalive_interval = 1000;


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
        mesh_network(connectivity_adapter &connection, router &networkrouter) :
                connection(
                        connection),
                network_router(networkrouter) {}

        template<size_t n>
        void add_blacklist(std::array<node_id, n> list) {
            for (const node_id &node: list) {
                blacklist[blacklist_size++] = node;
            }
        }

        void discover() {
            message message = {DISCOVERY::PRESENT, 0, connection.address, 0,
                                    0}; //Todo something with a message id (singleton?)
            connection.send(message);
        }

        uint8_t check_new_messages(std::array<message, 10> &uncaught) {
            uint8_t index = 0;
            while (connection.has_message()) {
                message msg = connection.next_message();
                if (!connection.is_new_message(msg)) { //message already handled
                    continue;
                }
                if (msg.receiver == connection.address ||
                    msg.receiver == 0) { //Message is for us, or broadcast, take it
                    if (!handleMessage(msg)) {
                        uncaught[index++] = msg;
                    }
                } else { //Not for us, todo relay message (through routing)
                    uint8_t next_hop = 0;
                    if (connection.connection_state(msg.receiver) != ACCEPTED) {
                        next_hop = network_router.get_next_hop(msg.receiver);
                    } else {
                    }
                    if (!connection.send(msg, next_hop)) {
                        network_router.update_neighbours();
                    }
                }

            }
            return index;
        }

        void update() {
            if (update_count++ > keepalive_interval) {
                update_count = 0;
                message keepalive = {
                        DISCOVERY::NO_OPERATION,
                        0,
                        connection.address,
                        0
                };

                unicast_all_close_if_fail(keepalive);

                discover();
            }
        }

        void unicast_close_if_fail(message &msg, const node_id &next_hop = 0) {
            if (!connection.send(msg, next_hop)) {
                LOG("UNICAST FAILED", "");
                connection.remove_direct_connection(next_hop != 0 ? next_hop : msg.receiver);
                network_router.send_update();
            }
        }

        void unicast_all_close_if_fail(message &msg) {
            size_t count = connection.get_neighbour_count();
            node_id failed[count];

            for(size_t i = 0; i < count; i++) {
                failed[i] = 0;
            }

            if (!connection.send_all(msg, failed)) {
                for (size_t i = 0; i < connection.get_neighbour_count(); i++) {
                    if (failed[i] == 0) {
                        continue;
                    }
                    connection.remove_direct_connection(failed[i]);
                }
                network_router.send_update();
            }
        }

        void sendMessage(message &msg) {
            uint8_t nextAddress = network_router.get_next_hop(msg.receiver);
            if(nextAddress == 0) {
                return;
            }
            msg.sender = connection.address;
            if (!connection.send(msg, nextAddress)) {
                // Todo actual error handling
//                router.update_neighbours();
                LOG("failed", msg << " - " << nextAddress);
            }
        }

        bool handleMessage(message &msg) {
            if ((msg.type & 0x10) > 0) { //This is a routing message
                network_router.on_routing_message(msg);
            }

            if ((msg.type & 0x20) > 0) {
                return false;
            }

            if (is_blacklisted(msg.sender)) {
                return true;
            }


            switch (msg.type) {
                case DISCOVERY::PRESENT:
                    if (connection.connection_state(msg.sender) == DISCONNECTED) {
                        if (connection.discovery_present_received(msg)) {
                            message connectMessage = {DISCOVERY::RESPOND, 0,
                                                           connection.address,
                                                           msg.sender, 0};
                            unicast_close_if_fail(connectMessage);
                        }
                    }
                    break;
                case DISCOVERY::RESPOND: {
                    if (connection.discovery_respond_received(msg)) {

                        message finishMessage = {DISCOVERY::ACCEPT, 0, connection.address,
                                                      msg.sender, 0};
                        if (connection.send(finishMessage)) {
                            network_router.update_neighbours();
                        }

                    } else {
                        message finishMessage = {DISCOVERY::DENY, 0, connection.address,
                                                      msg.sender, 0};
                        connection.send(finishMessage);
                    }
                    break;
                }
                case DISCOVERY::ACCEPT:
                    connection.discovery_accept_received(msg);
                    network_router.initial_update();
                    break;
                case DISCOVERY::DENY:
                    if (msg.receiver == connection.address) {
                        LOG("DENYING", "");
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

        connectivity_adapter &get_connection() const {
            return connection;
        }

        router &get_router() const {
            return network_router;
        }
    };
}
#endif //IPASS_MESH_NETWORK_HPP
