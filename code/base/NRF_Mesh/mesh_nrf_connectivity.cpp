//
// Created by Niels on 6/10/2019.
//

#include "mesh_nrf_connectivity.hpp"
#include "../NRF24L01/rx_pipe.hpp"
#include "../NRF24L01/rx_all_pipes.hpp"
#include "../Util/huts.hpp"

using nrf24l01::NRF_REGISTER;
using nrf24l01::NRF_FEATURE;
using nrf24l01::NRF_STATUS;
using nrf24l01::rx_all_pipes;
using nrf24l01::rx_pipe;
using nrf24l01::nrf24l01plus;
using nrf24l01::nrf_address;

mesh_nrf_connectivity::mesh_nrf_connectivity(uint8_t address, nrf24l01plus &nrf) : mesh_connectivity_adapter(address),
                                                                                   nrf(nrf) {

    nrf.write_register(NRF_REGISTER::FEATURE, NRF_FEATURE::EN_DPL | NRF_FEATURE::EN_DYN_ACK);

    nrf.set_tx_address(discovery_address);

    rx_all_pipes(nrf)
            .auto_acknowledgement(true)
            .setDynamicPacketLength(true);

    rx_pipe(nrf, 0)
            .setAddress(discovery_address);

    rx_pipe(nrf, 1)
            .enabled(false)
            .setAddress({base_address});

    listen();


    nrf.set_mode(nrf.MODE_PRX);
}

bool mesh_nrf_connectivity::unicast(mesh::mesh_message &message, uint8_t next_address) {
    if (next_address == 0) {
        next_address = message.receiver;
    }

    auto send_pipe = getPipeByAddress(next_address);
    if (send_pipe == 6) {
        return false;
    }


    nrf.set_mode(nrf.MODE_PTX);

    rx_pipe(nrf, send_pipe)
            .enabled(false);

    nrf_address address = {base_address, pipe_nrf_addresses[send_pipe]};

    rx_pipe(nrf, 0).setAddress(address);

    nrf.set_tx_address(address);

    send_message(message, false);

    nrf.set_tx_address(discovery_address);

    rx_pipe(nrf, 0).setAddress(discovery_address);

    rx_pipe(nrf, send_pipe)
            .enabled(true);

    nrf.set_mode(nrf.MODE_PRX);


    return true;
}

bool mesh_nrf_connectivity::is_message_available() {
    nrf.no_operation();
    return (nrf.last_status & NRF_STATUS::RX_DR) != 0;
}


bool mesh_nrf_connectivity::direct_connection_possible() {
    return next_pipe < 6;
}


bool mesh_nrf_connectivity::connect(mesh::mesh_message &origin) {
    if (!direct_connection_possible()) {
        return false;
    }

    nrf.set_mode(nrf.MODE_NONE);

    pipe_addresses[next_pipe] = origin.sender;
    pipe_nrf_addresses[next_pipe] = origin.connectionData[0];

    rx_pipe(nrf, next_pipe)
            .setAddress({base_address, origin.connectionData[0]})
            .enabled(true);
    next_pipe++;

    nrf.set_mode(nrf.MODE_PRX);
    return true;
}


void mesh_nrf_connectivity::remove_direct_connection(const uint8_t &address) {

    uint8_t pipe = getPipeByAddress(address);
    if (pipe == 6 || pipe == 0) {
        return;
    }

    nrf.set_mode(nrf.MODE_NONE);


    for (uint8_t i = pipe; i < next_pipe - 1; i++) {
        pipe_addresses[i] = pipe_addresses[i + 1];
        pipe_nrf_addresses[i] = pipe_nrf_addresses[i + 1];
        accepted[i] = accepted[i+1];

        rx_pipe(nrf, i)
                .setAddress({base_address, pipe_nrf_addresses[i]});
    }

    next_pipe--;

    rx_pipe(nrf, next_pipe)
            .enabled(false);
    accepted[next_pipe] = false;

    if (next_pipe == 5) {
        listen();
    }

    nrf.set_mode(nrf.MODE_PRX);
}

void mesh_nrf_connectivity::listen() {
    if (next_pipe >= 6) {
        return;
    }
    uint8_t listenaddress = address;
    bool found = true;
    while (found) {
        listenaddress += 2;
        found = false;
        for (uint8_t i = 0; i < next_pipe; i++) {
            if (pipe_nrf_addresses[i] == listenaddress) {
                found = true;
                break;
            }
        }
    }

    pipe_nrf_addresses[5] = listenaddress;
    pipe_addresses[5] = 0;

    nrf.set_mode(nrf.MODE_NONE);
    rx_pipe(nrf, 5)
            .setAddress({base_address, listenaddress})
            .enabled(true);
    nrf.set_mode(nrf.MODE_PRX);
}

void mesh_nrf_connectivity::broadcast(mesh::mesh_message &message) {
    nrf.set_mode(nrf.MODE_PTX);
    send_message(message, true);
    nrf.set_mode(nrf.MODE_PRX);
}

void mesh_nrf_connectivity::send_message(mesh::mesh_message &message, const bool &broadcast) {
    uint8_t payload[message.size()];

    payload[0] = uint8_t(message.configuration);
    payload[1] = message.conversation_id;
    payload[2] = message.sender;
    payload[3] = message.receiver;
    payload[4] = message.dataSize;

    for (size_t i = 0; i < message.dataSize; i++) {
        payload[i + 5] = message.data[i];
    }

    //Todo if needed set ConnData, first implement protocol

    switch (message.configuration) {
        case mesh::DISCOVERY_PRESENT:
            if (next_pipe >= 6) { // We can't accept new connections at the moment todo cleanup / keepalives
                return;
            } else {
                message.connectionData[0] = pipe_nrf_addresses[5];
            }
            break;
        case mesh::DISCOVERY_CONNECT:
            message.connectionData[0] = pipe_nrf_addresses[getPipeByAddress(message.receiver)];
            break;
        case mesh::DISCOVERY_ACCEPT:
            break;
        case mesh::DISCOVERY_DENY:
            break;
    }

    payload[message.size() - 2] = message.connectionData[0];
    payload[message.size() - 1] = message.connectionData[1];

    nrf.flush_tx();
    nrf.write_register(NRF_REGISTER::NRF_STATUS, 0x60); //clear previous sent bit
    nrf.write_tx_payload(payload, message.size(), broadcast);

    do {
        nrf.no_operation();
    } while ((nrf.last_status & NRF_STATUS::TX_DS) == 0);
}

mesh::mesh_message mesh_nrf_connectivity::next_message() {
    uint8_t payload_width = nrf.read_rx_payload_width();
    uint8_t data[payload_width];
    nrf.read_rx_payload(data, payload_width);

    std::array<uint8_t, 25> payload = {};
    for (size_t i = 0; i < data[4]; i++) {
        payload[i] = data[i + 4];
    }

    std::array<uint8_t, 2> connectionData = {};
    connectionData[0] = data[payload_width - 2];
    connectionData[1] = data[payload_width - 1];

    nrf.write_register(NRF_REGISTER::NRF_STATUS, NRF_STATUS::RX_DR);

    return {mesh::message_type(data[0]), data[1], data[2], data[3], data[4], payload, connectionData};
}

mesh::mesh_connection_state mesh_nrf_connectivity::connection_state(const uint8_t &address) {
    uint8_t pipe = getPipeByAddress(address);
    if(pipe == 6) {
        return mesh::DISCONNECTED;
    }

    if(accepted[pipe]) {
        return mesh::ACCEPTED;
    } else {
        return mesh::CONNECTED;
    }
}

uint8_t mesh_nrf_connectivity::getPipeByNRFAddress(const uint8_t &nrfaddress) {
    for (size_t i = 0; i < sizeof(pipe_nrf_addresses); i++) {
        if (pipe_nrf_addresses[i] == nrfaddress) {
            return i;
        }
    }
    return 6;
}

uint8_t mesh_nrf_connectivity::getPipeByAddress(const uint8_t &address) {
    for (size_t i = 0; i < sizeof(pipe_addresses); i++) {
        if (pipe_addresses[i] == address) {
            return i;
        }
    }
    return 6;
}

bool mesh_nrf_connectivity::onConnect(mesh::mesh_message &origin) {
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

void mesh_nrf_connectivity::onAccept(mesh::mesh_message &origin) {
    accepted[getPipeByAddress(origin.sender)] = true;
}








