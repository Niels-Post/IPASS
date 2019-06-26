//
// Created by Niels on 6/20/2019.
//

#include "mesh_nrf_connection.hpp"

using nrf24l01::nrf24l01plus;
using std::array;
using nrf24l01::NRF_REGISTER;

namespace mesh {
    void mesh_nrf_connection::flush(nrf24l01plus &nrf) {
        switch (connection_state) {
            case mesh::DISCONNECTED:
                nrf.rx_enabled(pipe_number, false);
                break;
            case mesh::WAITING:
            case mesh::RESPONDED:
            case mesh::ACCEPTED:
                nrf.rx_set_address(pipe_number, nrf_address);
                nrf.rx_enabled(pipe_number, true);
                if (pipe_number == 0) {
                    nrf.tx_set_address(nrf_address);
                }
                break;
        }
    }

    bool mesh_nrf_connection::send_message(array<mesh_nrf_connection, 6> &all_pipes, nrf24l01plus &nrf,
                                           uint8_t n,
                                           uint8_t *data) {
        nrf.mode(nrf.MODE_PTX);

        uint8_t old_pipe = pipe_number;
        if (pipe_number != 0) {
            nrf.rx_enabled(pipe_number, false);
            pipe_number = 0;
            flush(nrf);
        }
        nrf.tx_flush();
        nrf.write_register(NRF_REGISTER::NRF_STATUS, 0x60); //clear previous sent bit
        nrf.tx_write_payload(data, n, old_pipe == 0);
        bool success = true;
        do {
            nrf.no_operation();
            if((nrf.last_status & nrf24l01::NRF_STATUS::MAX_RT) > 0) {
                nrf.write_register(NRF_REGISTER::NRF_STATUS, 0x10); // Clear Max RT
                success = false;
                break;
            }
        } while ((nrf.last_status & nrf24l01::NRF_STATUS::TX_DS ) == 0); // TODO Check for max RT


        if (old_pipe != 0) {
            pipe_number = old_pipe;
            flush(nrf);
            all_pipes[0].flush(nrf);
        }
        nrf.mode(nrf.MODE_PRX);
        return success;
    }

    void mesh_nrf_connection::setConnectionState(mesh::mesh_connection_state cS) {
        mesh_nrf_connection::connection_state = cS;
    }

    void mesh_nrf_connection::setNodeId(uint8_t nodeId) {
        node_id = nodeId;
    }

    void mesh_nrf_connection::setNrfAddress(const nrf24l01::nrf_address &nrfAddress) {
        nrf_address = nrfAddress;
    }

    mesh::mesh_connection_state mesh_nrf_connection::getConnectionState() const {
        return connection_state;
    }

    uint8_t mesh_nrf_connection::getNodeId() const {
        return node_id;
    }

    const nrf24l01::nrf_address &mesh_nrf_connection::getNrfAddress() const {
        return nrf_address;
    }
}