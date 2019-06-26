//
// Created by Niels on 6/19/2019.
//

#ifndef IPASS_LINK_STATE_ROUTER_HPP
#define IPASS_LINK_STATE_ROUTER_HPP

#include "../mesh_router.hpp"

namespace mesh { //Todo: Pagination for shitloads of nodes

    template<typename id_type, typename cost_type, size_t max_edges>
    struct link_state_node {
        std::array<id_type,max_edges> edges;
        std::array<cost_type,max_edges> edge_costs = {1};
        uint8_t edge_count = 0;
        id_type id;
        id_type previous_node;
        cost_type distance = 0;
        bool id_definitive = false;
        link_state_node(id_type id) : id(id) {}

        link_state_node() : id(0) {}

        link_state_node(const std::array<id_type, max_edges> edges, const std::array<cost_type, max_edges> edgecosts,id_type id) : edges(edges), edge_costs(edgecosts), id(id) {
            for(const id_type &edge_id : edges) {
                if(edge_id == 0) {
                    return;
                }
                edge_count++;
            }
        }

        link_state_node(const std::array<id_type, max_edges> &edges, id_type id) : link_state_node(edges, {1}, id) {
        }



    };

    template<typename id_type , typename cost_type, size_t max_edges, size_t max_nodes, cost_type max_distance>
    class link_state_calculator {
    public:

        std::array<link_state_node<id_type, cost_type, max_edges>, max_nodes> nodes = {};
        size_t node_count = 0;

        size_t get_index_by_id(const node_id &id) {
            for(size_t i = 0; i < node_count; i++) {
                 if(nodes[i].id == id) {
                     return i;
                 }
            }
            return node_count;
        }

        void insert_replace(link_state_node<id_type, cost_type, max_edges> node) {
            size_t existing_node = get_index_by_id(node.id);
            nodes[existing_node] = node;

            if(existing_node == node_count) {
                node_count++;
            }
        }

        void remove(const id_type &id) {
            size_t node_index = get_index_by_id(id);
            if(node_index != node_count) {
                node_count--;
                for(size_t i = node_index; i < node_count; i++) {
                    nodes[i] = nodes[i+1];
                }
            }
        }

        void setup() {
            nodes[0].definitive = true;

            for(size_t i = 1; i < node_count; i++) {
                nodes[i].definitive = false;
                for(size_t j = 0; j < nodes[i].edge_count; j++) {
                    if(nodes[i].edges[j].other_node == nodes[0].id) {
                        nodes[i].distance = nodes[i].edges[j].cost;
                        nodes[i].previous_node = nodes[0].id;
                    } else {
                        nodes[i].distance = max_distance;
                    }
                }
            }
        }

        void loop() {
            for(size_t definitive_node_count = 1; definitive_node_count < node_count; definitive_node_count++) {
                cost_type min_distance = max_distance;
                size_t min_distance_node;
                for(size_t i = 1; i < node_count; i++) {
                    if(nodes[i].definitive) {
                        continue;
                    }

                    if(nodes[i].distance < min_distance) {
                        min_distance = nodes[i].distance;
                        min_distance_node = i;
                    }
                }

                if(min_distance == max_distance) { //Fucked, ABORT todo: request routing update
                    return;
                }

                link_state_node<id_type, cost_type, max_edges>& node = nodes[min_distance_node];

                for(size_t edge_id = 0; edge_id < node.edge_count; edge_id++) {
                    auto neighbour_index = get_index_by_id(node.edges[edge_id]);
                    if(neighbour_index == node_count) {
                        // We don't know this node, todo request routing update, propagate through previous_hops to find next_hop
                        continue;
                    }

                    link_state_node<id_type, cost_type, max_edges>& neighbour = nodes[neighbour_index];

                    if(neighbour.id_definitive) {
                        continue;
                    }

                    if((node.distance + nodes.edge_costs[edge_id]) < neighbour.distance) {
                        neighbour.distance = (node.distance + nodes.edge_costs[edge_id]);
                        neighbour.previous_node = node;
                    }
                    neighbour.id_definitive = true;

                }
            }

        }
    };





    class link_state_router : public mesh_router {
        uint8_t routing_message_id = 0;

        //Link state related
        link_state_calculator<node_id, uint16_t, 5, 10, 0xFFFF> ls_calc = {};


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
        link_state_router(mesh_connectivity_adapter &connectivity) : mesh_router(connectivity) {

            ls_calc.insert_replace({{}, connectivity.address});
            graph_update_me();
        }


        void graph_update_me() {
            size_t count = connectivity.get_neighbour_count();
            uint8_t neighbours[count];
            connectivity.get_neighbours(neighbours);
            auto &me = ls_calc.nodes[0];
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

            ls_calc.insert_replace({edges, costs, other});
        }

        void fill_update_message(mesh_message &message) {
            auto& me = ls_calc.nodes[0];
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
