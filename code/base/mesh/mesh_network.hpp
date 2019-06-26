//
// Created by Niels on 6/19/2019.
//

#ifndef IPASS_MESH_NETWORK_HPP
#define IPASS_MESH_NETWORK_HPP


#include "../Util/cout_debug.hpp"

namespace mesh {
    class mesh_network {
        mesh_connectivity_adapter &connection;
        mesh_router &router;

    public:
        mesh_network(mesh_connectivity_adapter &connection, mesh_router &router) :
                connection(
                        connection),
                router(router) {}

        void discover() {
            mesh_message message = {DISCOVERY::PRESENT, 0, connection.address, 0, 0}; //Todo something with a message id (singleton?)
            connection.broadcast(message);
        }

        void update() {
            while (connection.is_message_available()) {
                mesh_message msg = connection.next_message();
                if((msg.type & 0x10) > 0) { //This is a routing message
                    LOG("RECEIVE_ROUTING_MSG");
                    router.on_routing_message(msg);
                }
                if (msg.receiver == connection.address ||
                    msg.receiver == 0) { //Message is for us, or broadcast, take it
                    handleMessage(msg);
                } else { //Not for us, todo relay message (through routing)
                    uint8_t next_hop = 0;
                    if(connection.connection_state(msg.receiver) != ACCEPTED) {
                        next_hop = router.get_next_hop(msg.receiver);
                    }
                    connection.unicast(msg);
                }

            }
        }

        void sendMessage(mesh_message &msg) {

            //todo get router next hop
            uint8_t nextAddress = msg.receiver;
            if (connection.connection_state(msg.receiver) == ACCEPTED) {
                connection.unicast(msg, nextAddress);
            }
        }

        void handleMessage(mesh_message &msg) {
            switch (msg.type) {
                case DISCOVERY::PRESENT:

                    if (connection.connection_state(msg.sender) == DISCONNECTED) {
                        if (connection.on_discovery_present(msg)) {
                            mesh_message connectMessage = {DISCOVERY::RESPOND, 0,
                                                           connection.address,
                                                           msg.sender, 0};
                            connection.unicast(connectMessage);
                        }
                    }
                    break;
                case DISCOVERY::RESPOND: {
                    if (connection.on_discovery_respond(msg)) {

                        mesh_message finishMessage = {DISCOVERY::ACCEPT, 0, connection.address,
                                                      msg.sender, 0};
                        if(connection.unicast(finishMessage)) {
                            LOG2("NEW_CONNECTION", "", hwlib::hex << msg.sender);
                        } else {
                            LOG1("ACCEPT_FAIL", hwlib::hex << msg.sender);
                        }

                    } else {
                        mesh_message finishMessage = {DISCOVERY::DENY, 0, connection.address,
                                                      msg.sender, 0};
                        connection.unicast(finishMessage);
                    }
                    break;
                }
                case DISCOVERY::ACCEPT:
                    connection.on_discovery_accept(msg);
                    LOG2("NEW_CONNECTION", "",  hwlib::hex << msg.sender);
                    break;
                case DISCOVERY::DENY:
                    if (msg.receiver == connection.address) {
                        connection.remove_direct_connection(msg.sender);
                    }
                    break;
                case DISCOVERY::NO_OPERATION:break;
            }
        }
    };
}
#endif //IPASS_MESH_NETWORK_HPP
