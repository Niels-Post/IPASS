//
// Created by Niels on 6/19/2019.
//

#ifndef IPASS_LINK_STATE_ROUTER_HPP
#define IPASS_LINK_STATE_ROUTER_HPP

#include "../mesh_router.hpp"
#include "link_state_calculator.hpp"

using link_state::link_state_calculator;
using link_state::link_state_node;
namespace mesh { //Todo: Pagination for shitloads of nodes







    class link_state_router : public mesh_router {
        uint8_t routing_message_id = 0;

        //Link state related
        link_state_calculator<node_id, uint16_t, 5, 10> ls_calc;


        uint16_t previous_routing_messages[20] = {0};
        uint8_t previous_routing_messages_count = 0;

        bool handle_message(const node_id &sender, const uint8_t &message_id) {
            uint16_t check_value = ((sender << 8) | message_id);
            for (const uint16_t &i : previous_routing_messages) {
                if (i == check_value) {
                    return true;
                }
            }

            if (previous_routing_messages_count == 20) {
                for (uint8_t i = 0; i < 19; i++) {
                    previous_routing_messages[i] = previous_routing_messages[i + 1];
                }
                previous_routing_messages_count--;
            }
            previous_routing_messages[previous_routing_messages_count++] = check_value;
            return false;
        }

    public:
        link_state_router(mesh_connectivity_adapter &connectivity) : mesh_router(connectivity), ls_calc(connectivity.address) {
            graph_update_me();
        }


        void graph_update_me() {
            size_t count = connectivity.get_neighbour_count();
            uint8_t neighbours[count];
            connectivity.get_neighbours(neighbours);
            auto &me = ls_calc.get_node(0);
            me.edge_count = count;
            for (size_t i = 0; i < count; i++) {
                me.edges[i] = neighbours[i];
                me.edge_costs[i] = 1; //Todo actual cost
            }
        }

        void graph_update_other(const node_id &other, const mesh_message &message) {
            std::array<node_id, 5> edges = {};
            std::array<uint16_t, 5> costs = {};


            for (size_t i = 0; i < message.dataSize; i += 2) {
                edges[i / 2] = message.data[i];
                costs[i / 2] = message.data[i + 1];
            }

            ls_calc.insert_replace({other, edges, costs});
        }

        void fill_update_message(mesh_message &message) {
            auto& me = ls_calc.get_node(0);
            message.dataSize = uint8_t(me.edge_count * 2);
            for (size_t i = 0; i < me.edge_count; i++) {
                message.data[i * 2] = me.edges[i];
                message.data[i * 2 + 1] = me.edge_costs[i];
            }
        }

        void request_update() override {
            graph_update_me();
            mesh_message message = {
                    LINK_STATE_ROUTING::UPDATE_REQUEST,
                    routing_message_id++,
                    connectivity.address,
                    0,
                    0,
                    {0}
            };
            fill_update_message(message);
            connectivity.unicast_all(message);
        }

        void send_update_response(mesh_message &request_origin) {
            graph_update_me();
            mesh_message message = {
                    LINK_STATE_ROUTING::UPDATE_RESPONSE,
                    routing_message_id++,
                    connectivity.address,
                    request_origin.sender,
                    0,
                    {0}
            };
            fill_update_message(message);
            // TODO: Find Next hop.
            connectivity.unicast(message);
        }

        void on_routing_message(mesh_message &message) override {
            if (!handle_message(message.sender, message.message_id)) { //message already handled
                return;
            }


            switch (message.type) {
                case LINK_STATE_ROUTING::UPDATE_REQUEST: {
                    send_update_response(message);
                    break;
                }
                case LINK_STATE_ROUTING::UPDATE: {
                    graph_update_other(message.sender, message);
                    break;
                }
                case LINK_STATE_ROUTING::UPDATE_RESPONSE:
                    graph_update_other(message.sender, message);
                default:
                    return;
            }

            connectivity.unicast_all(message);
        }

        void on_neighbour_added(const uint8_t &address) override {
            mesh_message message = {
                    LINK_STATE_ROUTING::NEW_DIRECT_CONNECTION,
                    routing_message_id++,
                    connectivity.address,
                    0,
                    2,
                    {0}
            };
            message.data[0] = address;
            message.data[1] = 1;

            connectivity.unicast_all(message);
        }

        void on_neighbour_removed(const uint8_t &address) override {
            mesh_message message = {
                    LINK_STATE_ROUTING::REMOVE_DIRECT_CONNECTION,
                    routing_message_id++,
                    connectivity.address,
                    0,
                    2,
                    {0}
            };
            message.data[0] = address;
            message.data[1] = 1;

            connectivity.unicast_all(message);
        }



        void update_next_hops() {



        }

        node_id get_next_hop(const node_id &receiver) override {
            return 0;
        }

    };
}

#endif //IPASS_LINK_STATE_ROUTER_HPP
