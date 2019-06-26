//
// Created by Niels on 6/25/2019.
//

#ifndef IPASS_LINK_STATE_CALCULATOR_HPP
#define IPASS_LINK_STATE_CALCULATOR_HPP
namespace link_state {
    template<typename id_type, typename cost_type, size_t max_edges>
    struct link_state_node {
        id_type id;
        std::array<id_type, max_edges> edges;
        std::array<cost_type, max_edges> edge_costs = {1};
        uint8_t edge_count = 0;
        id_type previous_node;
        cost_type distance = 0;
        bool shortest_path_known = false;

        link_state_node(id_type id) : id(id) {}

        link_state_node() : id(0) {}

        link_state_node(id_type id, const std::array<id_type, max_edges> edges,
                        const std::array<cost_type, max_edges> edgecosts
        ) : id(id), edges(edges), edge_costs(edgecosts) {
            for (const id_type &edge_id : edges) {
                if (edge_id == 0) {
                    return;
                }
                edge_count++;
            }
        }

        link_state_node(id_type id, const std::array<id_type, max_edges> &edges) : link_state_node(id, edges, {0}) {
            edge_costs.fill(1);
        }


    };

    template<typename id_type, typename cost_type, size_t max_edges, size_t max_nodes>
    class link_state_calculator {
    private:
        std::array<link_state_node<id_type, cost_type, max_edges>, max_nodes> nodes = {};
        size_t node_count = 0;
    public:
        cost_type max_distance;

        link_state_calculator(id_type source_id) {
            cost_type temp = 0;
            for (size_t i = 0; i < sizeof(cost_type); i++) {
                temp <<= 8;
                temp |= 0xFF;
            }
            max_distance = temp;
            nodes[0].id = source_id;
            nodes[0].distance = 0;
            node_count++;
        }

        link_state_node<id_type, cost_type, max_edges> &get_node(const size_t &index) {
            return nodes[index];
        }

        size_t get_node_count() const {
            return node_count;
        }


        size_t get_index_by_id(const id_type &id) {
            for (size_t i = 0; i < node_count; i++) {
                if (nodes[i].id == id) {
                    return i;
                }
            }
            return node_count;
        }

        void insert_replace(link_state_node<id_type, cost_type, max_edges> node) {
            size_t existing_node = get_index_by_id(node.id);
            nodes[existing_node] = node;

            if (existing_node == node_count) {
                node_count++;
            }
        }

        bool remove(const id_type &id) {
            size_t node_index = get_index_by_id(id);
            if (node_index != node_count && node_index != 0) {
                node_count--;
                for (size_t i = node_index; i < node_count; i++) {
                    nodes[i] = nodes[i + 1];
                }
                nodes[node_count] = {};
                return true;
            }
            return false;
        }

        id_type get_next_hop(const id_type &id) {
            size_t node_index = get_index_by_id(id);
            if (node_index == node_count) {
                return 0;
            }

            auto node = get_node(node_index);
            while (node.previous_node != nodes[0].id) {

                node_index = get_index_by_id(node.previous_node);
                if (node.distance == max_distance) {
                    hwlib::cout << "uhh wth1" << hwlib::endl;
                    return 0;
                }
                if (!node.shortest_path_known) {
                    hwlib::cout << "uhh wth2" << hwlib::endl;
                    return 0;
                }
                if (node.previous_node == 0) {
                    hwlib::cout << "uhh wth3" << hwlib::endl;
                    return 0;
                }
                if (node_index == node_count) { //Either this node is unreachable, or link state hasn't been run yet since latest change todo auto-run?
                    hwlib::cout << "uhh wth4" << hwlib::hex << node.previous_node << hwlib::endl;
                    for(auto & node2 : nodes) {
                        hwlib::cout <<  hwlib::hex << node2.id << hwlib::endl;
                    }
                    return 0;
                }


                node = get_node(node_index);

            }
            return node.id;
        }

        void setup() {
            link_state_node<id_type, cost_type, max_edges> &source_node = nodes[0];
            source_node.shortest_path_known = true;

            for (size_t i = 1; i < node_count; i++) {
                link_state_node<id_type, cost_type, max_edges> &node = nodes[i];
                node.shortest_path_known = false;
                node.distance = max_distance;
                for (size_t j = 0; j < node.edge_count; j++) {
                    if (node.edges[j] == source_node.id && node.edge_costs[j] < node.distance) {
                        node.distance = node.edge_costs[j];
                        node.previous_node = source_node.id;
                        break;
                    }
                }
            }
        }

        void loop() {
            for (size_t definitive_node_count = 1; definitive_node_count < node_count; definitive_node_count++) {
                cost_type min_distance = max_distance;
                size_t min_distance_node;
                for (size_t i = 1; i < node_count; i++) {
                    link_state_node<id_type, cost_type, max_edges> &node = nodes[i];
                    if (node.shortest_path_known) {
                        continue;
                    }

                    if (node.distance < min_distance) {
                        min_distance = node.distance;
                        min_distance_node = i;
                    }
                }

                if (min_distance == max_distance) { //Fucked, ABORT todo: request routing update
                    return;
                } else {
                }

                link_state_node<id_type, cost_type, max_edges> &node = nodes[min_distance_node];


                for (size_t edge_id = 0; edge_id < node.edge_count; edge_id++) {
                    auto neighbour_index = get_index_by_id(node.edges[edge_id]);
                    if (neighbour_index == node_count) {
                        // We don't know this node, todo request routing update, propagate through previous_hops to find next_hop
                        continue;
                    }

                    link_state_node<id_type, cost_type, max_edges> &neighbour = nodes[neighbour_index];

                    if (neighbour.shortest_path_known) {
                        continue;
                    }

                    if ((node.distance + node.edge_costs[edge_id]) < neighbour.distance) {
                        neighbour.distance = (node.distance + node.edge_costs[edge_id]);
                        neighbour.previous_node = node.id;
                    }

                }

                node.shortest_path_known = true;
            }

        }
    };
}
#endif //IPASS_LINK_STATE_CALCULATOR_HPP
