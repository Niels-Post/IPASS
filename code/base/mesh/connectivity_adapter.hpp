//
// Created by Niels on 6/10/2019.
//

#ifndef IPASS_MESH_CONNECTIVITY_ADAPTER_HPP
#define IPASS_MESH_CONNECTIVITY_ADAPTER_HPP

#include "message.hpp"
#include "definitions.hpp"

namespace mesh {
    /**
     * \defgroup connectivity_adapters
     * \ingroup mesh_networking
     * \brief Set of currently implemented mesh connectivity adapters
     *
     * Each adapter enables mesh networking over a specific connection method
     */

    /**
     * \addtogroup mesh_networking
     * @{
     */


    class connectivity_adapter {
        uint16_t previous_messages[20] = {0};
        uint8_t previous_messages_count = 0;
        uint8_t current_message_id = 0;
    protected:

        void add_message_id(message &msg);

        virtual void add_connection_data(message &msg, node_id &next_hop) = 0;

        virtual bool send_implementation(node_id &id, uint8_t *data, size_t size) = 0;

    public:
        node_id address;

        // All methods implemented in connectivity_adapter itself

        explicit connectivity_adapter(const node_id &address);

        void forget_message_history_for(const node_id &id);

        bool is_new_message(const message &msg);

        bool send(message &message, node_id next_hop = 0);

        bool send_all(message &message, node_id *failed_addresses = nullptr);


        // Connection-type specific methods

        virtual bool has_message() = 0;

        virtual message next_message() = 0;

        virtual mesh_connection_state connection_state(const node_id &id) = 0;

        virtual size_t get_neighbour_count() = 0;

        virtual void get_neighbours(uint8_t data[]) = 0;

        virtual bool discovery_present_received(mesh::message &origin) = 0;

        virtual bool discovery_respond_received(mesh::message &origin) = 0;

        virtual void discovery_accept_received(mesh::message &origin) = 0;

        virtual void remove_direct_connection(const uint8_t &address) = 0;

        virtual void status() = 0;
    };

    /**
     * @}
     */

}
#endif //IPASS_MESH_CONNECTIVITY_ADAPTER_HPP
