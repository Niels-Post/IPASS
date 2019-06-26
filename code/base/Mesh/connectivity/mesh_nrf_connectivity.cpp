//
// Created by Niels on 6/10/2019.
//



#include "mesh_nrf_connectivity.hpp"

#include "../../util/cout_debug.hpp"


namespace mesh {

    mesh_nrf_connectivity::mesh_nrf_connectivity(uint8_t address, nrf24l01plus &nrf) : mesh_connectivity_adapter(
            address),
                                                                                       connections{
                                                                                               mesh_nrf_connection(0),
                                                                                               mesh_nrf_connection(1),
                                                                                               mesh_nrf_connection(2),
                                                                                               mesh_nrf_connection(3),
                                                                                               mesh_nrf_connection(4),
                                                                                               mesh_nrf_connection(5)
                                                                                       },
                                                                                       nrf(nrf) {


        nrf.write_register(NRF_REGISTER::FEATURE, NRF_FEATURE::EN_DPL | NRF_FEATURE::EN_DYN_ACK);

        // Global nrf options
        nrf.rx_auto_acknowledgement(true);
        nrf.rx_set_dynamic_payload_length(true);

        // Broadcast pipe
        connections[0].setNodeId(1);
        connections[0].setNrfAddress(discovery_address);
        connections[0].setConnectionState(mesh::ACCEPTED);
        connections[0].flush(nrf);

        listen();


        nrf.mode(nrf.MODE_PRX);
    }

    bool mesh_nrf_connectivity::unicast(mesh::mesh_message &message, uint8_t next_node_id) {
        if (next_node_id == 0) {
            next_node_id = message.receiver;
        }

        auto send_pipe = getPipeByNodeId(next_node_id);
        LOG3("UNICAST_MESSAGE", message.type, message.receiver, send_pipe);

        if (send_pipe == 6) {
            LOG1("DENY_MESSAGE", "NO_PIPE");
            return false;
        }

        if (connections[send_pipe].getConnectionState() != mesh::ACCEPTED) {
            if (message.type != DISCOVERY::RESPOND && message.type != DISCOVERY::ACCEPT && message.type != DISCOVERY::DENY) {
                LOG1("DENY_MESSAGE", "NO_ACCEPT");
                return false;
            }
        }

        if (message.type == DISCOVERY::RESPOND) {
            message.connectionData[0] = connections[getPipeByNodeId(message.receiver)].getNrfAddress().address_bytes[4];
        }

        uint8_t data[message.size()];
        message.to_byte_array(data);


        if(connections[send_pipe].send_message(connections, nrf, message.size(), data)) {
            return true;
        } else {
            LOG1("DENY_MESSAGE", "SEND_FAIL");
            remove_direct_connection(message.receiver);
            return false;
        }


    }

    bool mesh_nrf_connectivity::is_message_available() {
        nrf.no_operation();
        return (nrf.last_status & NRF_STATUS::RX_DR) != 0;
    }


    bool mesh_nrf_connectivity::direct_connection_possible() {
        return getFirstFreePipe() != 6;
    }


    bool mesh_nrf_connectivity::on_discovery_present(mesh::mesh_message &origin) {
        uint8_t freePipe = getFirstFreePipe();
        if (freePipe == 6) {
            return false;
        }

        mesh_nrf_connection &freeConnection = connections[freePipe];


        freeConnection.setNodeId(origin.sender);
        freeConnection.setNrfAddress({base_address, origin.connectionData[0]});
        freeConnection.setConnectionState(mesh::RESPONDED);
        nrf.mode(nrf.MODE_NONE);
        freeConnection.flush(nrf);
        nrf.mode(nrf.MODE_PRX);

        return true;
    }


    void mesh_nrf_connectivity::remove_direct_connection(const uint8_t &address) {

        uint8_t pipe = getPipeByNodeId(address);
        if (pipe == 6 || pipe == 0) {
            return;
        }

        nrf.mode(nrf.MODE_NONE);

        mesh_nrf_connection &conn = connections[pipe];
        conn.setConnectionState(mesh::DISCONNECTED);
        conn.flush(nrf);

        listen();
        nrf.mode(nrf.MODE_PRX);
    }

    void mesh_nrf_connectivity::listen() {
        // Check if there is a listen pipe already active
        for (const mesh_nrf_connection &connection : connections) {
            if (connection.getConnectionState() != mesh::DISCONNECTED && connection.getNodeId() == 0) {
                return;
            }
        }

        for (mesh_nrf_connection &empty_connection : connections) {
            if (empty_connection.getConnectionState() == mesh::DISCONNECTED) {

                nrf_address listenaddress = {base_address, address};
                bool found = true;
                while (found) {
                    listenaddress.address_bytes[4] += 2;
                    found = false;
                    for (mesh_nrf_connection &check_connection : connections) {
                        if (check_connection.getNrfAddress() == listenaddress) {
                            found = true;
                            break;
                        }
                    }
                }

                empty_connection.setNrfAddress(listenaddress);
                empty_connection.setNodeId(0);
                empty_connection.setConnectionState(mesh::WAITING);
                nrf.mode(nrf.MODE_NONE);
                empty_connection.flush(nrf);
                nrf.mode(nrf.MODE_PRX);
                return;
            }
        }


    }

    void mesh_nrf_connectivity::broadcast(mesh::mesh_message &message) {
        if(message.type != 1)
        LOG1("BROADCAST_MESSAGE", message.type);

        uint8_t bcPipe = getPipeByNodeId(0);
        if (bcPipe == 6) {
            return;
        }
        nrf.mode(nrf.MODE_PTX);

        if (message.type == DISCOVERY::PRESENT) {
            message.connectionData[0] = connections[bcPipe].getNrfAddress().address_bytes[4];
        }

        uint8_t data[message.size()];
        message.to_byte_array(data);

        connections[0].send_message(connections, nrf, message.size(), data);

        nrf.mode(nrf.MODE_PRX);
    }


    mesh::mesh_message mesh_nrf_connectivity::next_message() {
        uint8_t payload_width = nrf.rx_payload_width();
        uint8_t data[payload_width];
        nrf.rx_read_payload(data, payload_width);


        nrf.write_register(NRF_REGISTER::NRF_STATUS, NRF_STATUS::RX_DR);

        if(data[0] != 1)
        LOG2("RECEIVE_MESSAGE", data[0], data[2]);
        return mesh::mesh_message::parse(payload_width, data);
    }

    mesh::mesh_connection_state mesh_nrf_connectivity::connection_state(const uint8_t &address) {
        uint8_t pipe = getPipeByNodeId(address);
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

    bool mesh_nrf_connectivity::on_discovery_respond(mesh::mesh_message &origin) {
        uint8_t pipe_nr = getPipeByNRFAddress(origin.connectionData[0]);
        if (pipe_nr == 6 || connections[pipe_nr].getNodeId() != 0) {
            return false;
        }

        uint8_t selected_pipe = getFirstFreePipe();
        selected_pipe = selected_pipe == 6 ? pipe_nr : selected_pipe;
        if (selected_pipe == 6) {
            selected_pipe = pipe_nr;
        } else {
            connections[pipe_nr].setConnectionState(mesh::DISCONNECTED);
            connections[pipe_nr].flush(nrf);
        }

        mesh_nrf_connection &connection = connections[selected_pipe];

        connection.setConnectionState(mesh::RESPONDED);
        connection.setNodeId(origin.sender);
        connection.setNrfAddress({base_address, origin.connectionData[0]});
        connection.flush(nrf);


        listen();

        connection.setConnectionState(mesh::ACCEPTED);
        return true;
    }

    void mesh_nrf_connectivity::on_discovery_accept(mesh::mesh_message &origin) {
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

    void mesh_nrf_connectivity::unicast_all(mesh_message &message) {
        LOG2("UNICAST_MESSAGE", message.type, message.receiver);
        for (uint8_t pipe = 1; pipe < 6; pipe++) {
            if (connections[pipe].getConnectionState() != mesh::ACCEPTED) {
                LOG2("NO_CONNECTION", "", hwlib::hex << connections[pipe].getNodeId());
                continue;
            }

            if (connections[pipe].getNodeId() == message.sender) {
                continue;
            }

            uint8_t data[message.size()];
            message.to_byte_array(data);
            if(!connections[pipe].send_message(connections, nrf, message.size(), data)) {
                LOG2("REMOVING_CONN", "", connections[pipe].getNodeId());
                remove_direct_connection(connections[pipe].getNodeId());
            }
        }
    }

    size_t mesh_nrf_connectivity::get_neighbour_count() {
        size_t count = 0;
        for(mesh_nrf_connection &connection : connections) {
            if(connection.getConnectionState() == ACCEPTED) {
                count++;
            }
        }
        return count;
    }

    void mesh_nrf_connectivity::get_neighbours(uint8_t *data) {
        for(mesh_nrf_connection &connection : connections) {
            if(connection.getConnectionState() == ACCEPTED) {
                *data++ = connection.getNodeId();
            }
        }
    }
}

