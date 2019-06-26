//
// Created by Niels on 6/25/2019.
//
#include "catch.hpp"
#include <hwlib.hpp>
#include "mesh/router/link_state_calculator.hpp"


TEST_CASE("link_state_node") {
    hwlib::cout << "Testing link_state_node" << hwlib::endl;
}

TEST_CASE("link_state_node, empty constructor") {
    link_state::link_state_node<uint8_t, uint16_t, 5> node = {};

    REQUIRE(node.id == 0);
    REQUIRE(node.edge_count == 0);
}

TEST_CASE("link_state_node, constructor with id") {
    link_state::link_state_node<uint8_t, uint16_t, 5> node = {15};

    REQUIRE(node.id == 15);
    REQUIRE(node.edge_count == 0);
}

TEST_CASE("link_state_node, constructor with max edges") {
    std::array<uint8_t, 5> edges = {1,2,3,4,5};
    std::array<uint16_t, 5> costs = {10,11,12,13,14};

    link_state::link_state_node<uint8_t, uint16_t, 5> node = {20, edges, costs};

    REQUIRE(node.id == 20);
    REQUIRE(node.edge_count == 5);


    REQUIRE(node.edges[0] == 1);
    REQUIRE(node.edge_costs[0] == 10);

    REQUIRE(node.edges[4] == 5);
    REQUIRE(node.edge_costs[4] == 14);
}

TEST_CASE("link_state_node, constructor without costs") {
    std::array<uint8_t, 5> edges = {1,2,3,4,5};

    link_state::link_state_node<uint8_t, uint16_t, 5> node = {20, edges};

    REQUIRE(node.id == 20);
    REQUIRE(node.edge_count == 5);


    REQUIRE(node.edges[0] == 1);
    REQUIRE(node.edge_costs[0] == 1);

    REQUIRE(node.edges[4] == 5);
    REQUIRE(node.edge_costs[4] == 1);
}


TEST_CASE("link_state_calculator") {
    hwlib::cout << "Testing link_state_calculator" << hwlib::endl;
}

TEST_CASE("link_state_calculator, constructor initializes source node") {
    link_state::link_state_calculator<uint8_t, uint16_t,5,30> calculator(5);

    REQUIRE(calculator.get_node(0).id == 5);
    REQUIRE(calculator.get_node(0).id != 3);
    REQUIRE(calculator.get_node_count() == 1);
}

TEST_CASE("link_state_calculator, get_node retrieves the correct node") {
    link_state::link_state_calculator<uint8_t, uint16_t,5,30> calculator(5);

    calculator.insert_replace({ 3});
    calculator.insert_replace({ 2});

    REQUIRE(calculator.get_node(1).id == 3);
    REQUIRE(calculator.get_node(2).id == 2);
}


TEST_CASE("link_state_calculator, edge data is stored with nodes  ") {
    link_state::link_state_calculator<uint8_t, uint16_t,5,30> calculator(5);
    calculator.insert_replace({3, {1,2,3,4,5}});
    calculator.insert_replace({2, {5,4,3}});

    REQUIRE(calculator.get_node(1).edges[0] == 1);
    REQUIRE(calculator.get_node(1).edge_count == 5);
    REQUIRE(calculator.get_node(2).edges[0] == 5);
    REQUIRE(calculator.get_node(2).edge_count == 3);
}


TEST_CASE("link_state_calculator, insert_replace") {
    link_state::link_state_calculator<uint8_t, uint16_t,5,30> calculator(5);
    calculator.insert_replace({3, {1,2,3,4,5}});
    calculator.insert_replace({2, {5,4,3}});
    REQUIRE(calculator.get_node_count() == 3);
    calculator.insert_replace({3, {5,4,3}});
    REQUIRE(calculator.get_node_count() == 3);
    REQUIRE(calculator.get_node(1).edges[0] == 5);
    REQUIRE(calculator.get_node(1).edge_count == 3);
}


TEST_CASE("link_state_calculator, remove shifts remaining elements back") {
    link_state::link_state_calculator<uint8_t, uint16_t, 5, 30> calculator(1);
    calculator.insert_replace({2, {1,2,3,4,5}});
    calculator.insert_replace({3, {5,4,3}});

    REQUIRE(calculator.remove(2));

    REQUIRE(calculator.get_node_count() == 2);
    REQUIRE(calculator.get_node(1).id == 3);
}

TEST_CASE("link_state_calculator, remove doesn't remove source element") {
    link_state::link_state_calculator<uint8_t, uint16_t, 5, 30> calculator(1);
    calculator.insert_replace({3, {5,4,3}});
    REQUIRE(!calculator.remove(1));

    REQUIRE(calculator.get_node_count() == 2);
    REQUIRE(calculator.get_node(0).id == 1);
}

TEST_CASE("link_state_calculator, max distance is calculated properly") {
    link_state::link_state_calculator<uint8_t, uint8_t, 5, 30> calculator8(1);
    REQUIRE(calculator8.max_distance == 0xFF);

    link_state::link_state_calculator<uint8_t, uint16_t, 5, 30> calculator16(1);
    REQUIRE(calculator16.max_distance == 0xFFFF);

    link_state::link_state_calculator<uint8_t, uint32_t, 5, 30> calculator32(1);
    REQUIRE(calculator32.max_distance == 0xFFFFFFFF);

    link_state::link_state_calculator<uint8_t, uint64_t, 5, 30> calculator64(1);
    REQUIRE(calculator64.max_distance == 0xFFFFFFFFFFFFFFFF);
}

/**
 * Following tests are all (at least partially) based on the Test graph, of which the schematic can be found in the same folder as this test file
 */

TEST_CASE("link_state_calculator, setup sets all neighbours' distance to their costs") {
    link_state::link_state_calculator<uint8_t, uint8_t, 5, 30> calculator(0xFF);


    calculator.insert_replace({0xFF, {0xE1, 0xE2}, {1, 3}});    //source_node
    calculator.insert_replace({0xE1, {0xFF}, {1}});             //node 1
    calculator.insert_replace({0xE2, {0xFF, 0xE4}, {3, 4}});    //node 2
    calculator.insert_replace({0xE4, {0xE2}, {4}});             //node 4

    calculator.setup();


    REQUIRE(calculator.get_node(0).id == 0xFF);
    REQUIRE(calculator.get_node(1).id == 0xE1);
    REQUIRE(calculator.get_node(2).id == 0xE2);
    REQUIRE(calculator.get_node(3).id == 0xE4);



    REQUIRE(calculator.get_node(0).shortest_path_known);
    REQUIRE(!calculator.get_node(1).shortest_path_known);
    REQUIRE(!calculator.get_node(2).shortest_path_known);
    REQUIRE(!calculator.get_node(3).shortest_path_known);

    REQUIRE(calculator.get_node(0).distance == 0);
    REQUIRE(calculator.get_node(1).distance == 1);
    REQUIRE(calculator.get_node(2).distance == 3);
    REQUIRE(calculator.get_node(3).distance == 0xFF);

}


TEST_CASE("link_state_calculator, big test case, tests full functionality of link state calculations (see ") {
    link_state::link_state_calculator<uint8_t, uint8_t, 5, 30> calculator(0xFF);


    calculator.insert_replace({0xFF, {0xE1, 0xE2, 0xE3}, {1, 3, 5}});    //source_node
    calculator.insert_replace({0xE1, {0xFF}, {1}});                      //node 1
    calculator.insert_replace({0xE2, {0xFF, 0xE3, 0xE4}, {3, 1, 4}});    //node 2
    calculator.insert_replace({0xE3, {0xFF, 0xE2, 0xE4}, {5, 1, 2}});             //node 3
    calculator.insert_replace({0xE4, {0xE2, 0xE3}, {4, 2}});             //node 4
    calculator.insert_replace({0xE5, {}, {}});                           //node 5

    calculator.setup();
    calculator.loop();

    REQUIRE(calculator.get_node(0).id == 0xFF);
    REQUIRE(calculator.get_node(1).id == 0xE1);
    REQUIRE(calculator.get_node(2).id == 0xE2);
    REQUIRE(calculator.get_node(3).id == 0xE3);
    REQUIRE(calculator.get_node(4).id == 0xE4);
    REQUIRE(calculator.get_node(5).id == 0xE5);

    REQUIRE(calculator.get_node(0).shortest_path_known);
    REQUIRE(calculator.get_node(1).shortest_path_known);
    REQUIRE(calculator.get_node(2).shortest_path_known);
    REQUIRE(calculator.get_node(3).shortest_path_known);
    REQUIRE(calculator.get_node(4).shortest_path_known);
    REQUIRE(!calculator.get_node(5).shortest_path_known);

    REQUIRE(calculator.get_node(0).distance == 0);
    REQUIRE(calculator.get_node(1).distance == 1);
    REQUIRE(calculator.get_node(2).distance == 3);
    REQUIRE(calculator.get_node(3).distance == 4);
    REQUIRE(calculator.get_node(4).distance == 6);
    REQUIRE(calculator.get_node(5).distance == 0xFF);


    REQUIRE(calculator.get_node(1).previous_node == 0xFF);
    REQUIRE(calculator.get_node(2).previous_node == 0xFF);
    REQUIRE(calculator.get_node(3).previous_node == 0xE2);
    REQUIRE(calculator.get_node(4).previous_node == 0xE3);
    REQUIRE(calculator.get_node(5).previous_node == 0);


    REQUIRE(calculator.get_next_hop(0xE1) == 0xE1);
    REQUIRE(calculator.get_next_hop(0xE2) == 0xE2);
    REQUIRE(calculator.get_next_hop(0xE3) == 0xE2);
    REQUIRE(calculator.get_next_hop(0xE4) == 0xE2);
    REQUIRE(calculator.get_next_hop(0xE5) == 0);

}
