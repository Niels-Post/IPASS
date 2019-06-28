//
// Created by Niels on 6/10/2019.
//



#include "mesh_nrf_connectivity.hpp"
#include "../../../base/util/cout_debug.hpp"

using nrf24l01::NRF_REGISTER;
using nrf24l01::NRF_FEATURE;
using nrf24l01::NRF_STATUS;
using nrf24l01::nrf24l01plus;
using nrf24l01::nrf_address;
namespace mesh {


    mesh_nrf_connectivity::mesh_nrf_connectivity(const node_id &address, nrf24l01plus &nrf) : mesh_connectivity_adapter(
            address),
                                                                                              connections{
                                                                                                      mesh_nrf_pipe(0),
                                                                                                      mesh_nrf_pipe(1),
                                                                                                      mesh_nrf_pipe(2),
                                                                                                      mesh_nrf_pipe(3),
                                                                                                      mesh_nrf_pipe(4),
                                                                                                      mesh_nrf_pipe(5)
                                                                                              },
                                                                                              nrf(nrf) {


        nrf.write_register(NRF_REGISTER::FEATURE, NRF_FEATURE::EN_DPL | NRF_FEATURE::EN_DYN_ACK);

        // Global nrf options
        nrf.rx_auto_acknowledgement(true);
        nrf.rx_set_dynamic_payload_length(true);

        nrf.write_register(NRF_REGISTER::SETUP_RETR, 0x0A);

        nrf.write_register(NRF_REGISTER::RF_SETUP, 8);

        // Broadcast pipe
        connections[0].setNodeId(0);
        connections[0].setNrfAddress(discovery_address);
        connections[0].setConnectionState(mesh::ACCEPTED);
        connections[0].flush(nrf);

        start_waiting();

        nrf.mode(nrf.MODE_PRX);
    }


    void mesh_nrf_connectivity::add_connection_data(mesh_message &message, node_id &next_hop) {
        switch (message.type) {
            case DISCOVERY::RESPOND:
                message.connectionData[0] = connections[getPipeByNodeId(next_hop)].getNrfAddress().address_bytes[4];
                break;
            case DISCOVERY::PRESENT:
                message.connectionData[0] = connections[getPipeByNodeId(address)].getNrfAddress().address_bytes[4];
                break;
            default:
                break;
        }
    }

    bool mesh_nrf_connectivity::send_implementation(node_id &id, uint8_t *data, size_t size) {
        size_t listen_pipe = getPipeByNodeId(id);
        return connections[listen_pipe].send_message(connections, nrf, size, data);
    }

    mesh::mesh_message mesh_nrf_connectivity::next_message() {
        buffer_messages();
        if(buffer_end != buffer_start) {
            mesh_message &msg = message_buffer[buffer_start++];
            if(buffer_start == 100) {
                buffer_start = 0;
            }
            return msg;
        }
        return {};
    }

    void mesh_nrf_connectivity::buffer_messages() {
        while((nrf.fifo_status() & uint8_t (1)) == 0) {
            uint8_t payload_width = nrf.rx_payload_width();
            uint8_t data[payload_width];
            nrf.rx_read_payload(data, payload_width);


            nrf.write_register(NRF_REGISTER::NRF_STATUS, NRF_STATUS::RX_DR);
            message_buffer[buffer_end++].parse(payload_width, data);

            if(buffer_end == 100) {
                buffer_end = 0;
            }
            if(buffer_end == (buffer_start-1)) {
                LOG("PANIIIIIEK", "Message buffer is vol");
            }
        }

    }


    bool mesh_nrf_connectivity::has_message() {
        buffer_messages();
        return buffer_start != buffer_end;
    }


    bool mesh_nrf_connectivity::discovery_present_received(mesh::mesh_message &origin) {
        uint8_t freePipe = getFirstFreePipe();
        if (freePipe == 6) {
            return false;
        }

        mesh_nrf_pipe &freeConnection = connections[freePipe];
        freeConnection.setNodeId(origin.sender);
        freeConnection.setNrfAddress({base_address, origin.connectionData[0]});
        freeConnection.setConnectionState(mesh::RESPONDED);
        freeConnection.flush(nrf);

        return true;
    }

    void mesh_nrf_connectivity::remove_direct_connection(const uint8_t &address) {
        uint8_t pipe = getPipeByNodeId(address);
        LOG("REMOVE_CONNECTION", address << " - " << pipe);
        if (pipe == 6 || pipe == 0) {
            return;
        }

        mesh_nrf_pipe &conn = connections[pipe];
        conn.setConnectionState(mesh::DISCONNECTED);
        forget_message_history_for(address);

        conn.flush(nrf);

        start_waiting();
    }




    void mesh_nrf_connectivity::start_waiting() {
        // Check if there is a listen pipe already active
        for (size_t i = 1; i < 6; i++) {
            if (connections[i].getConnectionState() == mesh::WAITING) {
                return;
            }
        }

        for (mesh_nrf_pipe &empty_connection : connections) {
            if (empty_connection.getConnectionState() == mesh::DISCONNECTED) {
                nrf_address listenaddress = {base_address, address};
                bool found = true;
                while (found) {
                    listenaddress.address_bytes[4] += 2;
                    found = false;
                    for (mesh_nrf_pipe &check_connection : connections) {
                        if (check_connection.getNrfAddress() == listenaddress) {
                            found = true;
                            break;
                        }
                    }
                }

                empty_connection.setNrfAddress(listenaddress);
                empty_connection.setNodeId(address);
                empty_connection.setConnectionState(mesh::WAITING);
                empty_connection.flush(nrf);
                return;
            }
        }


    }

    mesh::mesh_connection_state mesh_nrf_connectivity::connection_state(const node_id &id) {
        size_t pipe = getPipeByNodeId(id);
        return pipe == 6 ? mesh::DISCONNECTED : connections[pipe].getConnectionState();
    }

    uint8_t mesh_nrf_connectivity::getPipeByNRFAddress(const uint8_t &nrfaddress) {
        for (int i = 0; i < 6; i++) {
            if (connections[i].getNrfAddress().address_bytes[4] == nrfaddress) {
                return i;
            }
        }
        return 6;
    }

    uint8_t mesh_nrf_connectivity::getPipeByNodeId(const uint8_t &node_id) {
        for (int i = 0; i < 6; i++) {
            if (connections[i].getNodeId() == node_id) {
                return i;
            }
        }
        return 6;
    }

    bool mesh_nrf_connectivity::discovery_respond_received(mesh::mesh_message &origin) {
        uint8_t pipe_nr = getPipeByNRFAddress(origin.connectionData[0]);
        if (pipe_nr == 6 || connections[pipe_nr].getNodeId() != address) {
            return false;
        }

        if(getPipeByNodeId(origin.sender) != 6) {
            remove_direct_connection(origin.sender);
        }


        mesh_nrf_pipe &connection = connections[pipe_nr];

        LOG("NEW_CONNECTION_IN", origin.sender);
        connection.setConnectionState(mesh::ACCEPTED);
        connection.setNodeId(origin.sender);
        connection.flush(nrf);


        start_waiting();

        return true;
    }

    void mesh_nrf_connectivity::discovery_accept_received(mesh::mesh_message &origin) {

        LOG("NEW_CONNECTION_OUT", origin.sender);
        connections[getPipeByNodeId(origin.sender)].setConnectionState(mesh::ACCEPTED);
    }

    uint8_t mesh_nrf_connectivity::getFirstFreePipe() {
        for (int i = 0; i < 6; i++) {
            if (connections[i].getConnectionState() == mesh::DISCONNECTED) {
                return i;
            }
        }
        return 6;
    }

    size_t mesh_nrf_connectivity::get_neighbour_count() {
        size_t count = 0;
        for (size_t i = 1; i < connections.size(); i++) {
            if (connections[i].getConnectionState() == ACCEPTED) {
                count++;
            }
        }
        return count;
    }

    void mesh_nrf_connectivity::get_neighbours(uint8_t *data) {
        for (size_t i = 1; i < connections.size(); i++) {

            if (connections[i].getConnectionState() == ACCEPTED) {
                *data++ = connections[i].getNodeId();
            }
        }
    }

    void mesh_nrf_connectivity::status() {
        LOG("Connection status:", "");
        for(size_t i = 0; i < 6; i++) {
            LOG(i, connections[i] );
        }

        nrf_address test = nrf.rx_get_address(0);
        LOG("RX0", test);
        test = nrf.tx_get_address();
        LOG("TX", test);
        LOG("status", hwlib::bin << nrf.last_status);
        LOG("fifo", hwlib::bin << nrf.fifo_status());
        uint8_t dat;
        nrf.read_register(NRF_REGISTER::EN_RXADDR, &dat);
        LOG("EN_RX", hwlib::bin << dat);



    }
}

