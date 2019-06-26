//
// Created by Niels on 6/10/2019.
//

#ifndef IPASS_MESH_NRF_CONNECTIVITY_HPP
#define IPASS_MESH_NRF_CONNECTIVITY_HPP

#include <hwlib.hpp>
#include "../../nrf24l01/nrf24l01plus.hpp"
#include "../mesh_connectivity_adapter.hpp"
#include "mesh_nrf_connection.hpp"
#include "../../nrf24l01/nrf_address.hpp"

using nrf24l01::NRF_REGISTER;
using nrf24l01::NRF_FEATURE;
using nrf24l01::NRF_STATUS;
using nrf24l01::nrf24l01plus;
using nrf24l01::nrf_address;

namespace mesh {
    class mesh_nrf_connectivity : public mesh::mesh_connectivity_adapter {
    private:
        std::array<mesh_nrf_connection, 6> connections;
        nrf24l01plus &nrf;
        const nrf_address discovery_address = {0x70, 0x70, 0x70, 0x70, 0x70};
        const nrf_address base_address = {0x72, 0x72, 0x72, 0x72, 0x70};

        uint8_t getPipeByNRFAddress(const uint8_t &nrfaddress);

        uint8_t getPipeByNodeId(const uint8_t &node_id);

        uint8_t getFirstFreePipe();


    public:
        mesh_nrf_connectivity(uint8_t address, nrf24l01plus &nrf);


    private:
        void listen();

    public:


        void broadcast(mesh::mesh_message &message) override;

        bool unicast(mesh::mesh_message &message, uint8_t next_node_id) override;

        void unicast_all(mesh_message &message) override;

        bool is_message_available() override;

        mesh::mesh_message next_message() override;

        bool direct_connection_possible() override;

        mesh::mesh_connection_state connection_state(const uint8_t &address) override;

        bool on_discovery_present(mesh::mesh_message &origin) override;

        void remove_direct_connection(const uint8_t &address) override;

        bool on_discovery_respond(mesh::mesh_message &origin) override;

        void on_discovery_accept(mesh::mesh_message &origin) override;

        size_t get_neighbour_count() override;

        void get_neighbours(uint8_t *data) override;
    };

}
#endif //IPASS_MESH_NRF_CONNECTIVITY_HPP
