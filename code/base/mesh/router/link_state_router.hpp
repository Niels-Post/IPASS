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
        bool is_updated = false;

        //Link state related
        link_state_calculator<node_id, uint16_t, 5, 10> ls_calc;


        void graph_update_other(const node_id &other, const mesh_message &message) {
            std::array<node_id, 5> edges = {};
            std::array<uint16_t, 5> costs = {};


            for (size_t i = 0; i < message.dataSize / 2; i++) {
                edges[i] = message.data[i * 2];
                costs[i] = message.data[i * 2 + 1];
            }


            ls_calc.insert_replace({other, edges, costs});
            is_updated = false;
        }

        void fill_update_message(mesh_message &message) {
            auto &me = ls_calc.get_node(0);
            message.dataSize = uint8_t(me.edge_count * 2);
            for (size_t i = 0; i < me.edge_count; i++) {
                message.data[i * 2] = me.edges[i];
                message.data[i * 2 + 1] = me.edge_costs[i];
            }
            is_updated = false;
        }

    public:
        link_state_router(mesh_connectivity_adapter &connectivity) : mesh_router(connectivity),
                                                                     ls_calc(connectivity.address) {
            update_neighbours();
        }

        void update_neighbours() override {
            connectivity.status();
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

        void send_update() override {
            update_neighbours();
            mesh_message update_message(
                    LINK_STATE_ROUTING::UPDATE,
                    0,
                    connectivity.address,
                    0
            );
            fill_update_message(update_message);
            connectivity.send_all(update_message);

        }


        void initial_update() override {
            update_neighbours();
            mesh_message message = {
                    LINK_STATE_ROUTING::UPDATE_REQUEST,
                    0,
                    connectivity.address,
                    0
            };
            fill_update_message(message);
            connectivity.send_all(message);
        }

        void on_routing_message(mesh_message &message) override {
            switch (message.type) {
                case LINK_STATE_ROUTING::UPDATE_REQUEST: {
                    LOG("RECEIVED INITIAL_UPDATE", "");
                    graph_update_other(message.sender, message);
                    cout_debug c;
                    c << "Current known nodes:" << hwlib::endl;
                    for (size_t i = 0; i < ls_calc.get_node_count(); i++) {
                        auto &node = ls_calc.get_node(i);
                        c << node << hwlib::endl;
                    }
                    send_update();
//                    send_update_response(message);
                    break;
                }
                case LINK_STATE_ROUTING::UPDATE: {
                    LOG("RECEIVED UPDATE", "");
                    graph_update_other(message.sender, message);
                    cout_debug c;
                    c << "Current known nodes:" << hwlib::endl;
                    for (size_t i = 0; i < ls_calc.get_node_count(); i++) {
                        auto &node = ls_calc.get_node(i);
                        c << node << hwlib::endl;
                    }
                    break;
                }
            }

            connectivity.send_all(message);

        }

        node_id get_next_hop(const node_id &receiver) override {
            if (!is_updated) {
                LOG("RUNNING ALGO", "");
                is_updated = true;
                ls_calc.setup();
                ls_calc.loop();
            }
            LOG("ALGO GAAVE", ls_calc.get_next_hop(receiver));
            return ls_calc.get_next_hop(receiver);
        }

    };
}

#endif //IPASS_LINK_STATE_ROUTER_HPP
