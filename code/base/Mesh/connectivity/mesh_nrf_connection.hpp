//
// Created by Niels on 6/20/2019.
//

#ifndef IPASS_MESH_NRF_CONNECTION_HPP
#define IPASS_MESH_NRF_CONNECTION_HPP


#include <hwlib.hpp>
#include "../../nrf24l01/nrf24l01plus.hpp"
#include "../../nrf24l01/NRF24L01_Definitions.hpp"
#include "../mesh_definitions.hpp"
#include "../../nrf24l01/nrf_address.hpp"

namespace mesh {
    class mesh_nrf_connection {
        mesh::mesh_connection_state connection_state = mesh::DISCONNECTED;
        uint8_t pipe_number;
        uint8_t node_id = 0;
        nrf24l01::nrf_address nrfaddress;

    public:
        mesh_nrf_connection(uint8_t pipeNumber) : pipe_number(pipeNumber) {}

        void flush(nrf24l01::nrf24l01plus &nrf);

        bool send_message(std::array<mesh_nrf_connection, 6> &all_pipes, nrf24l01::nrf24l01plus &nrf, uint8_t n,
                          uint8_t *data);

        void setConnectionState(mesh::mesh_connection_state cS);

        void setNodeId(uint8_t nodeId);

        void setNrfAddress(const nrf24l01::nrf_address &nrfAddress);

        mesh::mesh_connection_state getConnectionState() const;

        uint8_t getNodeId() const;

        const nrf24l01::nrf_address &getNrfAddress() const;
    };
}

#endif //IPASS_MESH_NRF_CONNECTION_HPP
