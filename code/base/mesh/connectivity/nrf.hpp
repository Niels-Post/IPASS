//
// Created by Niels on 6/10/2019.
//

#ifndef IPASS_MESH_NRF_CONNECTIVITY_HPP
#define IPASS_MESH_NRF_CONNECTIVITY_HPP

#include "../../nrf24l01/nrf24l01plus.hpp"
#include "../connectivity_adapter.hpp"
#include "nrf_pipe.hpp"

namespace mesh {
    namespace connectivity {
        class nrf : public mesh::connectivity_adapter {
        private:
            std::array<message, 100> message_buffer = {};
            size_t buffer_start = 0;
            size_t buffer_end = 0;


            std::array<nrf_pipe, 6> connections;
            nrf24l01::nrf24l01plus &nrf24;
            const nrf24l01::nrf_address discovery_address = {0x70, 0x70, 0x70, 0x70, 0x70};
            const nrf24l01::nrf_address base_address = {0x72, 0x72, 0x72, 0x72, 0x70};

            uint8_t getPipeByNRFAddress(const uint8_t &nrfaddress);

            uint8_t getPipeByNodeId(const uint8_t &node_id);

            uint8_t getFirstFreePipe();

            void buffer_messages();


        public:
            nrf(const node_id &address, nrf24l01::nrf24l01plus &nrf);


        private:
            void start_waiting();

        protected:
            bool send_implementation(node_id &id, uint8_t *data, size_t size) override;

        public:

            bool has_message() override;

            mesh::message next_message() override;

            mesh::mesh_connection_state connection_state(const uint8_t &id) override;

            bool discovery_present_received(mesh::message &origin) override;

            void remove_direct_connection(const uint8_t &address) override;

            bool discovery_respond_received(mesh::message &origin) override;

            void discovery_accept_received(mesh::message &origin) override;

            size_t get_neighbour_count() override;

            void get_neighbours(uint8_t *data) override;

            void add_connection_data(message &message, node_id &next_hop) override;

            void status() override;
        };

    }
}
#endif //IPASS_MESH_NRF_CONNECTIVITY_HPP
