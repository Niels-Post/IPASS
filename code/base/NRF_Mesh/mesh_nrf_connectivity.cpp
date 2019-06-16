//
// Created by Niels on 6/10/2019.
//

#include "mesh_nrf_connectivity.hpp"
#include "../NRF24L01/rx_pipe.hpp"
#include "../NRF24L01/rx_all_pipes.hpp"


mesh_nrf_connectivity::mesh_nrf_connectivity(nrf24l01::nrf24l01plus &nrf) : nrf(nrf) {
    nrf24l01::rx_all_pipes(nrf)
            .enabled(false)
            .auto_acknowledgement(true)
            .setDynamicPacketLength(true);

    nrf24l01::rx_pipe(nrf, 0)
            .enabled(true)
            .setAddress(discovery_address)
            .auto_acknowledgement(false);

    nrf24l01::rx_pipe(nrf, 1)
            .setAddress(base_address);
    nrf.set_mode(nrf.MODE_PRX);
}

bool mesh_nrf_connectivity::unicast(mesh::mesh_message &message, uint8_t next_address) {
    uint8_t send_pipe = address_pipe_map.get(next_address, -1);
    if (send_pipe == uint8_t(-1)) {
        return false;
    }

    nrf.set_mode(nrf.MODE_PTX);

    nrf24l01::nrf_address address = nrf24l01::rx_pipe(nrf, send_pipe)
            .enabled(false)
            .getAddress();

    nrf24l01::rx_pipe(nrf, 0).setAddress(address);

    nrf.set_tx_address(address);

    send_message(message);

    nrf.set_tx_address(discovery_address);

    nrf24l01::rx_pipe(nrf, 0).setAddress(discovery_address);
    nrf24l01::rx_pipe(nrf, send_pipe)
            .enabled(true);

    nrf.set_mode(nrf.MODE_PRX);


    return true;
}

bool mesh_nrf_connectivity::is_message_available() {
    nrf.no_operation();
    return (nrf.last_status & nrf24l01::NRF_STATUS::RX_DR) != 0;
}

mesh::mesh_message mesh_nrf_connectivity::next_message() {
    uint8_t pw = nrf.read_rx_payload_width();
    uint8_t data[pw];
    nrf.read_rx_payload(data, pw);

    uint8_t config = data[0];
    uint8_t sender = data[1];
    uint8_t receiver = data[2];
    uint8_t size = pw - 4;
    uint8_t payload[size];
    for (size_t i = 0; i < size; i++) {
        payload[i] = data[i + 4];
    }
    return {config, sender, receiver, size, payload};
}

bool mesh_nrf_connectivity::direct_connection_possible() {
    return used_pipes < 5;
}

void mesh_nrf_connectivity::add_direct_connection(const uint8_t &address) {
    if (!direct_connection_possible()) {
        return;
    }

    nrf.set_mode(nrf.MODE_NONE);
    nrf24l01::nrf_address full_address = {discovery_address, address};

    address_pipe_map.put(address, ++used_pipes);
    nrf24l01::rx_pipe(nrf, used_pipes)
            .setAddress(full_address)
            .enabled(true)
            .auto_acknowledgement(true)
            .setDynamicPacketLength(true);

    nrf.set_mode(nrf.MODE_PRX);
}

void mesh_nrf_connectivity::remove_direct_connection(const uint8_t &address) {
    uint8_t rx_pipe = address_pipe_map.remove(address, -1);
    if (rx_pipe == uint8_t(-1)) {
        return;
    }

    nrf.set_mode(nrf.MODE_NONE);


    for (size_t current_pipe = rx_pipe; current_pipe < address_pipe_map.size(); current_pipe++) {
        nrf24l01::rx_pipe(nrf, current_pipe + 1)
                .setAddress({base_address, address_pipe_map.get(current_pipe, -1)});
    }

    used_pipes--;
}

void mesh_nrf_connectivity::broadcast(mesh::mesh_message &message) {
    nrf.set_mode(nrf.MODE_PTX);
    send_message(message);
    nrf.set_mode(nrf.MODE_PRX);
}

void mesh_nrf_connectivity::send_message(mesh::mesh_message &message) {
    nrf.flush_tx();
    nrf.write_register(nrf24l01::NRF_REGISTER::NRF_STATUS, 0x60); //clear previous sent bit
    nrf.write_tx_payload((uint8_t *) &message, message.size + 4);

    do {
        nrf.no_operation();
    } while ((nrf.last_status & nrf24l01::NRF_STATUS::TX_DS) == 0);
}


