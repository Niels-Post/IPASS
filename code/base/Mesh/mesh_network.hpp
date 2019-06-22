//
// Created by Niels on 6/19/2019.
//

#ifndef IPASS_MESH_NETWORK_HPP
#define IPASS_MESH_NETWORK_HPP

#include "mesh_message.hpp"
#include "mesh_router.hpp"
#include "mesh_connectivity_adapter.hpp"
#include "../Util/huts.hpp"

namespace mesh {
    class mesh_network {
        uint8_t current_conversation_id = 0;
        mesh_connectivity_adapter &connection;
        mesh_router &router;

    public:
        mesh_network(mesh_connectivity_adapter &connection, mesh_router &router) :
                connection(
                        connection),
                router(router) {}

        void discover() {
            huts::a_niffau();
            mesh_message message = {DISCOVERY_PRESENT, ++current_conversation_id, connection.address, 0, 0};
            connection.broadcast(message);
        }

        void update() {
            while (connection.is_message_available()) {
                mesh_message msg = connection.next_message();
                if (msg.receiver == connection.address ||
                    msg.receiver == 0) { //Message is for us, or broadcast, take it
                    handleMessage(msg);
                } else { //Not for us, todo relay message (through routing)

                }

            }
        }

        void sendMessage(mesh_message &msg) {

            //todo get router next hop
            uint8_t nextAddress = msg.receiver;
            if(connection.connection_state(msg.receiver) == ACCEPTED) {
                connection.unicast(msg, nextAddress);
            }
        }

        void handleMessage(mesh_message &msg) {
            switch (msg.configuration) {
                case DISCOVERY_PRESENT:
                    if (connection.connection_state(msg.sender) == DISCONNECTED) {
                        //todo implement orderly waiting
                        if (connection.on_discovery_present(msg)) {
                            huts::a_niffau();
                            huts::a_niffau();
                            huts::a_niffau();
                            mesh_message connectMessage = {DISCOVERY_RESPOND, ++current_conversation_id,
                                                           connection.address,
                                                           msg.sender, 0};
                            connection.unicast(connectMessage);
                        }

                    }
                    break;
                case DISCOVERY_RESPOND: {
                    if (connection.on_discovery_respond(msg)) {
                        mesh_message finishMessage = {DISCOVERY_ACCEPT, ++current_conversation_id, connection.address,
                                                      msg.sender, 0};
                        connection.unicast(finishMessage);
                    } else {
                        mesh_message finishMessage = {DISCOVERY_DENY, ++current_conversation_id, connection.address,
                                                      msg.sender, 0};
                        connection.unicast(finishMessage);
                    }
                    break;
                }
                case DISCOVERY_ACCEPT:
                    break;
                case DISCOVERY_DENY:
                    if (msg.receiver == connection.address) {
                        connection.remove_direct_connection(msg.sender);
                    }
                    break;
            }
        }
    };
}
#endif //IPASS_MESH_NETWORK_HPP
