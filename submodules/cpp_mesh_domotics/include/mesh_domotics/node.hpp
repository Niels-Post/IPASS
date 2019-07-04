/*
 *
 * Copyright Niels Post 2019.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 *
*/

#ifndef IPASS_DOMOTICA_NODE_HPP
#define IPASS_DOMOTICA_NODE_HPP


#include <mesh/mesh_network.hpp>
#include <mesh_domotics/module.hpp>

/**
 * \defgroup mesh_domotics Mesh Domotics
 * \brief Home automation-like functionality using a mesh network
 *
 * Provides an abstract interface for making simple hardware modules that serve as:
 *  - Output-Module, to provide sensor data for other nodes in the network
 *  - Input-Module, to perform hardware-based actions based on provided sensor data
 */




namespace mesh_domotics {

    /**
     * \addtogroup mesh_domotics
     * @{
     */

/**
 * \brief Bootstrapper class for a domotica node.
 *
 * Handles Input and Output to/from modules.
 * Also calls keepalives and discovery on the network
 * */
    class node {
    private:
        mesh::mesh_network &network;

        output_module &outputmodule;
        input_module &inputmodule;
        uint8_t output_update_count = 0;
    public:


        /**
         * \brief Create a domotica node
         *
         * If this node is either receive only, or send only, the empty constructor for the corresponding module can be used
         * Depending on the module types, node will either feed data from the network into the module, or feed data from the module into the network
         * @param network Network to connect to and interact with
         * @param inputModule Primary module to use for this node
         * @param outputModule  Secondary module to use
         */
        node(mesh::mesh_network &network, output_module &inputModule,
             input_module &outputModule);


        /**
         * \brief Update the output module.
         *
         * Feeds received data into the output module if it's filter matches the sender of the data
         * @param msg Message to update from
         */
        void update_output_module(mesh::message &msg);


        /**
         * \brief Update the input module.
         *
         * Checks for any changed data, and sends it to all nodes in the input module's filter
         */
        void update_input_module();

        /**
         * \brief Main loop for a domotica node
         *
         * (DOESN't RETURN)
         * Handles all basic network interaction (keepalives and discovery)
         */
        void loop();


    };

    /**
     * @}
     */

}
#endif //IPASS_DOMOTICA_NODE_HPP
