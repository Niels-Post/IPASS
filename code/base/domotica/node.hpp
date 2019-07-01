//
// Created by Niels on 6/27/2019.
//

#ifndef IPASS_DOMOTICA_NODE_HPP
#define IPASS_DOMOTICA_NODE_HPP


#include "../mesh/mesh_network.hpp"
#include "module.hpp"

/**
 * \defgroup \title mesh_domotics
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
 * Bootstrapper class for a domotica node.
 * Handles Input and Output to/from modules.
 * Also calls keepalives and discovery on the network
 * */
    class node {
    private:
        mesh::mesh_network &network;


        module primary_module;
        module secondary_module;


    public:
        /**
         * Create a domotica node
         * If this node is either receive only, or send only, the empty constructor for the corresponding module can be used
         * Depending on the module types, node will either feed data from the network into the module, or feed data from the module into the network
         * @param network Network to connect to and interact with
         * @param primaryModule Primary module to use for this node
         * @param secondaryModule  Secondary module to use
         */
        node(mesh::mesh_network &network, module primaryModule = {},
                      module secondaryModule = {});


        /**
         * Update a module (if it is an input). Feeds received data into the input module, if it's filter matches the sender of the data
         * @param m Module to update
         */
        void update_input_module(module &m, mesh::message &msg);


        /**
         * Update a module (if it is an output). Checks for any changed data, and sends it to all nodes in filter
         * @param m Module to update
         */
        void update_output_module(module &m);

        /**
         * Main loop for a domotica node (DOESN't RETURN)
         * Handles all basic network interaction (keepalives and discovery)
         */
        void __attribute__((noreturn)) loop();

        /**
         * Change this node's primary module
         * @param primary_module New primary module
         */
        void set_primary_module(module primary_module);

        /**
         * Change this node's secondary module
         * @param secondary_module New secondary module
         */
        void set_secondary_module(module secondary_module);
    };

    /**
     * @}
     */

}
#endif //IPASS_DOMOTICA_NODE_HPP
