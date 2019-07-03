//
// Created by Niels on 6/27/2019.
//

#ifndef IPASS_DOMOTICA_NODE_HPP
#define IPASS_DOMOTICA_NODE_HPP


#include "../mesh/mesh_network.hpp"
#include "module.hpp"

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

        input_module &inputmodule;
        output_module &outputmodule;
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
        node(mesh::mesh_network &network, input_module &inputModule,
             output_module &outputModule);


        /**
         * \brief Update athe input module.
         *
         * Feeds received data into the input module, if it's filter matches the sender of the data
         * @param msg Message to update from
         */
        void update_input_module(mesh::message &msg);


        /**
         * \brief Update the output module.
         *
         * Checks for any changed data, and sends it to all nodes in filter
         */
        void update_output_module();

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
