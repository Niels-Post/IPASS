//
// Created by Niels on 6/10/2019.
//

#ifndef IPASS_MESH_CONNECTIVITY_ADAPTER_HPP
#define IPASS_MESH_CONNECTIVITY_ADAPTER_HPP

#include "mesh_message.hpp"
#include "mesh_definitions.hpp"
#include "../util/cout_debug.hpp"

namespace mesh {


    class mesh_connectivity_adapter {
        uint16_t previous_messages[20] = {0};
        uint8_t previous_messages_count = 0;
        uint16_t current_message_id = 0;
    protected:

        void add_message_id(mesh_message &msg) {
            if (msg.sender == address && msg.message_id == 0) {
                msg.message_id = current_message_id++;
            }
        }

        virtual void add_connection_data(mesh_message &msg, node_id &next_hop)=0;

        virtual bool send_implementation(node_id &id, uint8_t *data, size_t size) = 0;
    public:
        node_id address;

        mesh_connectivity_adapter(const node_id &address) : address(address) {}

        void forget_message_history_for(const node_id &id) {
            for (size_t i = 0; i < previous_messages_count; i++) {
                if ((previous_messages[i] >> 8) == id) {
                    for (uint8_t j = i; j < previous_messages_count; j++) {
                        previous_messages[i] = previous_messages[i + 1];
                    }
                    previous_messages_count--;
                }
            }

        }

        bool is_new_message(const mesh_message &msg) {
            uint16_t check_value = ((msg.sender << 8) | msg.message_id);
            for (size_t i = 0; i < previous_messages_count; i++) {
                if (previous_messages[i] == check_value) {
                    return false;
                }
            }

            if (previous_messages_count == 20) {
                for (uint8_t i = 0; i < 19; i++) {
                    previous_messages[i] = previous_messages[i + 1];
                }
                previous_messages_count--;
            }
            previous_messages[previous_messages_count++] = check_value;
            return true;
        }


        virtual bool send(mesh_message &message, node_id next_hop = 0) {
            if (next_hop == 0) next_hop = message.receiver;


            mesh_connection_state state = connection_state(next_hop);
            if(state == DISCONNECTED) {
                LOG("CONN_DISC", hwlib::hex << next_hop);
            } else if ( message.receiver != 0 &&
                        state != mesh::ACCEPTED
                       && message.type != DISCOVERY::RESPOND
                       && message.type != DISCOVERY::ACCEPT
                       && message.type != DISCOVERY::DENY) {
                LOG("BAD_STATE", state << " - " << message.receiver);
                return false;
            }

            add_message_id(message);
            add_connection_data(message, next_hop);


            LOG("Sending", message);

            uint8_t message_bytes[message.size()];
            message.to_byte_array(message_bytes);


            for(uint8_t fail_count = 0; fail_count < 5; fail_count++) {
                if(send_implementation(next_hop, message_bytes, message.size())) {
                    return true;
                }
                hwlib::wait_ms(1);
                has_message();

            }

            return false;
        }

        virtual bool send_all(mesh_message &message, node_id *failed_addresses = nullptr) {
            bool all_successful = true;
            node_id neighbours[get_neighbour_count()];
            get_neighbours(neighbours);

            add_message_id(message);

            for (size_t i = 0; i < get_neighbour_count(); i++) {
                if (connection_state(neighbours[i]) == mesh::ACCEPTED && message.sender != neighbours[i]) {
                    mesh_message copy_msg = message; //Make a copy with neighbour-specific changes
                    if (!send(copy_msg, neighbours[i])) {
                        if(failed_addresses != nullptr) {
                            *failed_addresses++ = neighbours[i];
                        }
                        all_successful = false;
                    }

                }

            }
            return all_successful;
        };

        virtual bool has_message() = 0;

        virtual mesh_message next_message() = 0;


        virtual mesh_connection_state connection_state(const node_id &id) = 0;

        virtual size_t get_neighbour_count() = 0;

        virtual void get_neighbours(uint8_t data[]) = 0;

        virtual bool discovery_present_received(mesh::mesh_message &origin) = 0;

        virtual bool discovery_respond_received(mesh::mesh_message &origin) = 0;

        virtual void discovery_accept_received(mesh::mesh_message &origin) = 0;


        virtual void remove_direct_connection(const uint8_t &address) = 0;

        virtual void status() = 0;
    };

}
#endif //IPASS_MESH_CONNECTIVITY_ADAPTER_HPP
