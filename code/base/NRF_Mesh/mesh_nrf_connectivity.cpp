//
// Created by Niels on 6/10/2019.
//

#include "mesh_nrf_connectivity.hpp"
#include "../Util/huts.hpp"

using nrf24l01::NRF_REGISTER;
using nrf24l01::NRF_FEATURE;
using nrf24l01::NRF_STATUS;
using nrf24l01::nrf24l01plus;
using nrf24l01::nrf_address;


mesh_nrf_connectivity::mesh_nrf_connectivity(uint8_t address, nrf24l01plus &nrf) : connections{
        mesh_nrf_connection(0),
        mesh_nrf_connection(1),
        mesh_nrf_connection(2),
        mesh_nrf_connection(3),
        mesh_nrf_connection(4),
        mesh_nrf_connection(5)
}, mesh_connectivity_adapter(address),
                                                                                   nrf(nrf) {


    nrf.write_register(NRF_REGISTER::FEATURE, NRF_FEATURE::EN_DPL | NRF_FEATURE::EN_DYN_ACK);

    // Global nrf options
    nrf.rx_auto_acknowledgement(true);
    nrf.rx_set_dynamic_payload_length(true);

    // Broadcast pipe
    connections[0].setNrfAddress(discovery_address);
    connections[0].flush(nrf);

    listen();


    nrf.mode(nrf.MODE_PRX);
}

bool mesh_nrf_connectivity::unicast(mesh::mesh_message &message, uint8_t next_address) {
    if (next_address == 0) {
        next_address = message.receiver;
    }

    auto send_pipe = getPipeByNodeId(next_address);
    if (send_pipe == 6 || (connections[send_pipe].getConnectionState() != mesh::ACCEPTED && message.configuration == mesh::DISCOVERY_RESPOND)) {
        return false;
    }

    if(message.configuration == mesh::DISCOVERY_RESPOND) {
            message.connectionData[0] = connections[getPipeByNodeId(message.receiver)].getNrfAddress().address_bytes[4];
    }

    uint8_t data[message.size()];
    message.to_byte_array(data);



    connections[send_pipe].send_message(connections, nrf, message.size(), data);

    return true;
}

bool mesh_nrf_connectivity::is_message_available() {
    nrf.no_operation();
    return (nrf.last_status & NRF_STATUS::RX_DR) != 0;
}


bool mesh_nrf_connectivity::direct_connection_possible() {
    for(mesh_nrf_connection &connection : connections) {
        if(connection.getConnectionState() == mesh::DISCONNECTED)
        {
            return true;
        }
    }
    return false;
}


bool mesh_nrf_connectivity::discovery_respond(mesh::mesh_message &origin) {
    if (!direct_connection_possible()) {
        return false;
    }



    uint8_t freePipe = getFirstFreePipe();
    if(freePipe == 6)
    {
        return false;
    }
    mesh_nrf_connection freeConnection = connections[freePipe];


    freeConnection.setChipId(origin.sender);
    freeConnection.setNrfAddress({base_address, origin.connectionData[0]});

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

    mesh_nrf_connection conn = connections[pipe];
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
            empty_connection.setChipId(0);
            empty_connection.setConnectionState(mesh::WAITING);
            nrf.mode(nrf.MODE_NONE);
            empty_connection.flush(nrf);
            nrf.mode(nrf.MODE_PRX);

        }
    }


}

void mesh_nrf_connectivity::broadcast(mesh::mesh_message &message) {
    nrf.mode(nrf.MODE_PTX);

    uint8_t bcPipe = getPipeByNodeId(0);
    if(bcPipe == 6) {
        return;
    }

    if(message.configuration == mesh::DISCOVERY_PRESENT) {
        message.connectionData[0] = connections[bcPipe].getNrfAddress().address_bytes[4];
    }

    nrf.mode(nrf.MODE_PRX);
}


mesh::mesh_message mesh_nrf_connectivity::next_message() {
    uint8_t payload_width = nrf.rx_payload_width();
    uint8_t data[payload_width];
    nrf.rx_read_payload(data, payload_width);


    nrf.write_register(NRF_REGISTER::NRF_STATUS, NRF_STATUS::RX_DR);

    return mesh::mesh_message::parse(payload_width, data);
}

mesh::mesh_connection_state mesh_nrf_connectivity::connection_state(const uint8_t &address) {
    uint8_t pipe = getPipeByNodeId(address);
    return pipe == 6 ? mesh::DISCONNECTED : connections[pipe].getConnectionState();
}

uint8_t mesh_nrf_connectivity::getPipeByNRFAddress(const uint8_t &nrfaddress) {
    for (int i = 0; i < 6; ++i) {
        if(connections[i].getNrfAddress().address_bytes[4] == nrfaddress) {
            return i;
        }
    }
    return 6;
}

uint8_t mesh_nrf_connectivity::getPipeByNodeId(const uint8_t &node_id) {
    for (int i = 0; i < 6; ++i) {
        if(connections[i].getNodeId() == node_id) {
            return i;
        }
    }
    return 6;
}

bool mesh_nrf_connectivity::on_discovery_respond(mesh::mesh_message &origin) {
    uint8_t pipe_nr = getPipeByNRFAddress(origin.connectionData[0]);
    if (pipe_nr == 6 || pipe_addresses[pipe_nr] != 0) {
        return false;
    }

    if (next_pipe < 5) {
        pipe_addresses[next_pipe] = origin.sender;
        pipe_nrf_addresses[next_pipe] = origin.connectionData[0];

        rx_pipe(nrf, next_pipe)
                .setAddress({base_address, pipe_nrf_addresses[next_pipe]})
                .enabled(true);


    }

    next_pipe++;
    listen();

    return true;
}

void mesh_nrf_connectivity::on_discovery_accept(mesh::mesh_message &origin) {
    connections[getPipeByNodeId(origin.sender)].setConnectionState(mesh::ACCEPTED);
}

uint8_t mesh_nrf_connectivity::getFirstFreePipe() {
    for (int i = 0; i < 6; ++i) {
        if(connections[i].getConnectionState() == mesh::DISCONNECTED) {
            return i;
        }
    }
    return 6;
}


