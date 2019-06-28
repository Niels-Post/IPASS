//
// Created by Niels on 6/20/2019.
//

#ifndef IPASS_MESH_NRF_PIPE_HPP
#define IPASS_MESH_NRF_PIPE_HPP


#include <hwlib.hpp>
#include <ostream>
#include "../../nrf24l01/nrf24l01plus.hpp"
#include "../../nrf24l01/NRF24L01_Definitions.hpp"
#include "../mesh_definitions.hpp"
#include "../mesh_message.hpp"

namespace mesh {
    class mesh_nrf_pipe {
        mesh::mesh_connection_state connection_state = mesh::DISCONNECTED;
        uint8_t pipe_number;
        node_id connected_node = 0;
        nrf24l01::nrf_address nrf_address;

    public:
        mesh_nrf_pipe(uint8_t pipeNumber) : pipe_number(pipeNumber) {}

        void flush(nrf24l01::nrf24l01plus &nrf);

        bool send_message(std::array<mesh_nrf_pipe, 6> &all_pipes, nrf24l01::nrf24l01plus &nrf, uint8_t n,
                          uint8_t *data);

        void setConnectionState(mesh::mesh_connection_state cS);

        void setNodeId(uint8_t nodeId);

        void setNrfAddress(const nrf24l01::nrf_address &nrfAddress);

        mesh::mesh_connection_state getConnectionState() const;

        uint8_t getNodeId() const;

        const nrf24l01::nrf_address &getNrfAddress() const;

        friend hwlib::ostream &operator<<(hwlib::ostream &os, const mesh_nrf_pipe &pipe);
    };
}

#endif //IPASS_MESH_NRF_PIPE_HPP
