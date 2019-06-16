//
// Created by Niels on 6/10/2019.
//

#ifndef IPASS_MESH_NRF_CONNECTIVITY_HPP
#define IPASS_MESH_NRF_CONNECTIVITY_HPP

#include "../Mesh/mesh_connectivity_adapter.hpp"
#include "../NRF24L01/nrf24l01plus.hpp"
#include "../Util/Map.hpp"

class mesh_nrf_connectivity: mesh::mesh_connectivity_adapter {
private:
    Map<uint8_t, uint8_t, 5> address_pipe_map;
    const nrf24l01::nrf_address discovery_address = {0x70, 0x70, 0x70, 0x70, 0x70};
    const nrf24l01::nrf_address base_address = {0x72, 0x72, 0x72, 0x72, 0x70};
    nrf24l01::nrf24l01plus &nrf;
    uint8_t used_pipes = 0;
public:
    mesh_nrf_connectivity(nrf24l01::nrf24l01plus &nrf);


private:
    void send_message(mesh::mesh_message &message);
public:


    void broadcast(mesh::mesh_message &message) override;
    bool unicast(mesh::mesh_message &message, uint8_t next_address) override;

    bool is_message_available() override;

    mesh::mesh_message next_message() override;

    bool direct_connection_possible() override;

    void add_direct_connection(const uint8_t &address) override;

    void remove_direct_connection(const uint8_t &address) override;
};


#endif //IPASS_MESH_NRF_CONNECTIVITY_HPP
