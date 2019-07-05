//
// Created by Niels on 6/25/2019.
//
#include <single_include/catch2/catch.hpp>
#include <hwlib.hpp>
#include <link_state/node.hpp>
#include <link_state/calculator.hpp>

TEST_CASE("link_state_node") {
    hwlib::cout << "Testing link_state_node" << hwlib::endl;
}


SCENARIO("link_state_node constructs properly") {
    GIVEN("nothing") {
        WHEN("constructor is called with only template arguments") {
            link_state::node<uint8_t, uint16_t, 5> node = {};
            THEN("id is set to 0") {
                REQUIRE(node.id == 0);
            }
            AND_THEN("edge_count is 0") {
                REQUIRE(node.edge_count == 0);
            }
        }

        WHEN("constructor is called with an id argument") {
            link_state::node<uint8_t, uint16_t, 5> node = {15};
            THEN("id is set") {
                REQUIRE(node.id == 15);
            }
            AND_THEN("edge_count is 0") {
                REQUIRE(node.edge_count == 0);
            }
        }
    }

    GIVEN("array of edges, and array of costs") {
        std::array<uint8_t, 5> edges = {1, 2, 3, 4, 5};
        std::array<uint16_t, 5> costs = {10, 11, 12, 13, 14};
        WHEN("constructor is called with maximum number of edges") {
            link_state::node<uint8_t, uint16_t, 5> node = {20, edges, costs};
            THEN("id is set properly") {
                REQUIRE(node.id == 20);
            }
            AND_THEN("edges and costs are set properly") {
                REQUIRE(node.edge_count == 5);
                REQUIRE(node.edges[0] == 1);
                REQUIRE(node.edge_costs[0] == 10);
                REQUIRE(node.edges[4] == 5);
                REQUIRE(node.edge_costs[4] == 14);

            }
        }

        WHEN("constructor is called without costs") {
            link_state::node<uint8_t, uint16_t, 5> node = {20, edges};
            THEN("id is set properly") {
                REQUIRE(node.id == 20);
            }
            AND_THEN("edges are set properly") {
                REQUIRE(node.edge_count == 5);
                REQUIRE(node.edges[0] == 1);
                REQUIRE(node.edges[4] == 5);
            }
            AND_THEN("edge costs are set to 1 (default)") {
                REQUIRE(node.edge_costs[0] == 1);
                REQUIRE(node.edge_costs[4] == 1);
            }
        }
    }
}


SCENARIO("link_state calculator node editing functions work properly") {
    GIVEN("A link_state calculator") {
        link_state::calculator<uint8_t, uint16_t, 5, 30> calculator(5);

        THEN("The source node is correctly set") {
            REQUIRE(calculator.get_node(0).id == 5);
            REQUIRE(calculator.get_node(0).id != 3);
            REQUIRE(calculator.get_node_count() == 1);
        }

        WHEN("two nodes are inserted") {
            calculator.insert_replace({3});
            calculator.insert_replace({2});
            THEN("They are put in in the right order") {
                REQUIRE(calculator.get_node(1).id == 3);
                REQUIRE(calculator.get_node(2).id == 2);
            }
        }

        WHEN("two nodes are inserted with edges") {
            calculator.insert_replace({3, {1, 2, 3, 4, 5}});
            calculator.insert_replace({2, {5, 4, 3}});
            THEN("the edges are stored with the nodes") {
                REQUIRE(calculator.get_node(1).edges[0] == 1);
                REQUIRE(calculator.get_node(1).edge_count == 5);
                REQUIRE(calculator.get_node(2).edges[0] == 5);
                REQUIRE(calculator.get_node(2).edge_count == 3);
            }
        }

        WHEN("a node id is inserted twice") {
            calculator.insert_replace({3, {1, 2, 3, 4, 5}});
            calculator.insert_replace({2, {2, 4, 6}});
            calculator.insert_replace({3, {5, 4, 3}});
            THEN("the node is not added twice") {
                REQUIRE(calculator.get_node_count() == 3);
            }
            AND_THEN("the node edges are replaced") {
                REQUIRE(calculator.get_node(1).edges[0] == 5);
                REQUIRE(calculator.get_node(1).edge_count == 3);
            }
        }

    }

    GIVEN("a link_state calculator with two nodes") {
        link_state::calculator<uint8_t, uint16_t, 5, 30> calculator(5);
        calculator.insert_replace({2, {1, 2, 3, 4, 5}});
        calculator.insert_replace({3, {5, 4, 3}});
        WHEN("a node is removed") {
            REQUIRE(calculator.remove(2));
            THEN("The other element is shifted back") {
                REQUIRE(calculator.get_node(1).id == 3);
            }
            AND_THEN("The node count is adjusted") {
                REQUIRE(calculator.get_node_count() == 2);
            }
        }

        WHEN("remove is called for the source node") {
            bool result = calculator.remove(5);

            THEN("the remove fails") {
                REQUIRE(!result);
            }
            AND_THEN("the source node is not removed") {
                REQUIRE(calculator.get_node(0).id == 5);
                REQUIRE(calculator.get_node_count() == 3);
            }
        }
    }
}

SCENARIO("link_state_calculator, max distance is calculated properly") {
    GIVEN("a calculator with cost size of 1 byte") {
        link_state::calculator<uint8_t, uint8_t, 5, 30> calculator8(1);
        THEN("The maximum distance is equal to 0xFF") {
            REQUIRE(calculator8.max_distance == 0xFF);
        }
    }

    GIVEN("a calculator with cost size of 2 bytes") {
        link_state::calculator<uint8_t, uint16_t, 5, 30> calculator16(1);
        THEN("The maximum distance is equal to 0xFFFF") {
            REQUIRE(calculator16.max_distance == 0xFFFF);
        }
    }

    GIVEN("a calculator with cost size of 4 bytes") {
        link_state::calculator<uint8_t, uint32_t, 5, 30> calculator32(1);
        THEN("The maximum distance is equal to 0xFFFFFFFF") {
            REQUIRE(calculator32.max_distance == 0xFFFFFFFF);
        }
    }

    GIVEN("a calculator with cost size of 8 bytes") {
        link_state::calculator<uint8_t, uint64_t, 5, 30> calculator64(1);
        THEN("The maximum distance is equal to 0xFFFFFFFF") {
            REQUIRE(calculator64.max_distance == 0xFFFFFFFFFFFFFFFF);
        }
    }
}

/**
 * Following tests are all (at least partially) based on the Test graph, of which the schematic can be found in the same folder as this test file
 */


SCENARIO("Link_state_calculator test graph, schematic of this graph can be found in the same folder") {
    GIVEN("a link_state_calculator containing the test graph") {
        link_state::calculator<uint8_t, uint8_t, 5, 30> calculator(0xFF);
        calculator.insert_replace({0xFF, {0xE1, 0xE2, 0xE3}, {1, 3, 5}});    //source_node
        calculator.insert_replace({0xE1, {0xFF}, {1}});                      //node 1
        calculator.insert_replace({0xE2, {0xFF, 0xE3, 0xE4}, {3, 1, 4}});    //node 2
        calculator.insert_replace({0xE3, {0xFF, 0xE2, 0xE4}, {5, 1, 2}});             //node 3
        calculator.insert_replace({0xE4, {0xE2, 0xE3}, {4, 2}});             //node 4
        calculator.insert_replace({0xE5, {}, {}});                           //node 5

        WHEN("setup is called") {
            calculator.setup();
            THEN("Only the shortest path of the source node is known") {
                REQUIRE(calculator.get_node(0).shortest_path_known);
                REQUIRE(!calculator.get_node(1).shortest_path_known);
                REQUIRE(!calculator.get_node(2).shortest_path_known);
                REQUIRE(!calculator.get_node(3).shortest_path_known);
                REQUIRE(!calculator.get_node(4).shortest_path_known);
                REQUIRE(!calculator.get_node(5).shortest_path_known);
            }
            AND_THEN("Initial distances of direct neighbours are set to their costs") {
                REQUIRE(calculator.get_node(0).distance == 0);
                REQUIRE(calculator.get_node(1).distance == 1);
                REQUIRE(calculator.get_node(2).distance == 3);
                REQUIRE(calculator.get_node(3).distance == 5);
            }
            AND_THEN("All remaining distances are set to the maximum distance") {
                REQUIRE(calculator.get_node(4).distance == calculator.max_distance);
                REQUIRE(calculator.get_node(5).distance == calculator.max_distance);
            }
        }

        WHEN("setup and loop are called") {
            calculator.setup();
            calculator.loop();

            THEN("All reachable nodes have shortest_path_known true") {
                REQUIRE(calculator.get_node(0).shortest_path_known);
                REQUIRE(calculator.get_node(1).shortest_path_known);
                REQUIRE(calculator.get_node(2).shortest_path_known);
                REQUIRE(calculator.get_node(3).shortest_path_known);
                REQUIRE(calculator.get_node(4).shortest_path_known);
            }
            AND_THEN("Unreachable nodes have shortest_path_known false") {
                REQUIRE(!calculator.get_node(5).shortest_path_known);
            }
            AND_THEN("Distances for reachable nodes are calculated") {
                REQUIRE(calculator.get_node(0).distance == 0);
                REQUIRE(calculator.get_node(1).distance == 1);
                REQUIRE(calculator.get_node(2).distance == 3);
                REQUIRE(calculator.get_node(3).distance == 4);
                REQUIRE(calculator.get_node(4).distance == 6);
            }
            AND_THEN("Distances for unreachable nodes are set to the maximum distance") {
                REQUIRE(calculator.get_node(5).distance == calculator.max_distance);
            }
            AND_THEN("Previous_nodes are set for the shortest possible path") {
                REQUIRE(calculator.get_node(1).previous_node == 0xFF);
                REQUIRE(calculator.get_node(2).previous_node == 0xFF);
                REQUIRE(calculator.get_node(3).previous_node == 0xE2);
                REQUIRE(calculator.get_node(4).previous_node == 0xE3);
            }
            AND_THEN("Previous_node is set to 0 for unreachable nodes") {
                REQUIRE(calculator.get_node(5).previous_node == 0);
            }
            AND_THEN("next_hops for reachable nodes is calculated properly") {
                REQUIRE(calculator.get_next_hop(0xE1) == 0xE1);
                REQUIRE(calculator.get_next_hop(0xE2) == 0xE2);
                REQUIRE(calculator.get_next_hop(0xE3) == 0xE2);
                REQUIRE(calculator.get_next_hop(0xE4) == 0xE2);
            }
            AND_THEN("next_hop returns 0 for unreachable nodes") {
                REQUIRE(calculator.get_next_hop(0xE5) == 0);
            }
        }
    }
}
