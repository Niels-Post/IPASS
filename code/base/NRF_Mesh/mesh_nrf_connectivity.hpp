//
// Created by Niels on 6/10/2019.
//

#ifndef IPASS_MESH_NRF_CONNECTIVITY_HPP
#define IPASS_MESH_NRF_CONNECTIVITY_HPP

#include "../Mesh/mesh_connectivity_adapter.hpp"
#include "../NRF24L01/nrf24l01plus.hpp"
#include "../Util/Map.hpp"
#include "mesh_nrf_connection.hpp"

typedef uint8_t mesh_address;





class mesh_nrf_connectivity : public mesh::mesh_connectivity_adapter {
private:
    std::array<mesh_nrf_connection,6> connections;
    const nrf24l01::nrf_address discovery_address = {0x70, 0x70, 0x70, 0x70, 0x70};
    const nrf24l01::nrf_address base_address = {0x72, 0x72, 0x72, 0x72, 0x70};


    nrf24l01::nrf24l01plus &nrf;

    uint8_t getPipeByNRFAddress(const uint8_t &nrfaddress);
    uint8_t getPipeByNodeId(const uint8_t &node_id);
    uint8_t getFirstFreePipe();
public:
    mesh_nrf_connectivity(uint8_t address, nrf24l01::nrf24l01plus &nrf);



private:
    void listen();

public:


    void broadcast(mesh::mesh_message &message) override;

    bool unicast(mesh::mesh_message &message, uint8_t next_address) override;

    bool is_message_available() override;

    mesh::mesh_message next_message() override;

    bool direct_connection_possible() override;


    mesh::mesh_connection_state connection_state(const uint8_t &address) override;


    bool on_discovery_present(mesh::mesh_message &origin) override;

    void remove_direct_connection(const uint8_t &address) override;

    bool on_discovery_respond(mesh::mesh_message &origin) override;

    void on_discovery_accept(mesh::mesh_message &origin) override;
};



#endif //IPASS_MESH_NRF_CONNECTIVITY_HPP
