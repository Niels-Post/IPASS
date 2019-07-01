//
// Created by Niels on 6/27/2019.
//

#ifndef IPASS_DOMOTICA_NODE_HPP
#define IPASS_DOMOTICA_NODE_HPP


#include "../mesh/mesh_network.hpp"
#include "domotica_module.hpp"

/**
 * Bootstrapper class for a domotica node
 * Handles Input and Output to/from modules
 * Also calls keepalives and discovery on the network
 * */
class domotica_node {
private:
    mesh::mesh_network &network;

    domotica_input_module &input_module;
    domotica_output_module &output_module;


public:
    /**
     * Create a domotica node
     * If this node is either receive only, or send only, the empty constructor for the corresponding module can be used
     * @param network Network to connect to and interact with
     * @param inputModule Input module to use for this node. domotica_node will check for network data available and feed it to the module
     * @param outputModule  Output module to use for this node. domotica_node will continuously check for data changes and feed them to the network.
     */
    domotica_node(mesh::mesh_network &network, domotica_input_module &inputModule,
                  domotica_output_module &outputModule);

    /**
     * Change the used input module
     * @param inputModule new inputmodule
     */
    void setInputModule(domotica_input_module &inputModule);

    /**
     * Change the used output module
     * @param outputModule new outputmodule
     */
    void setOutputModule(domotica_output_module &outputModule);

    /**
     * Main loop for a domotica node (DOESN't RETURN)
     * Handles all basic network interaction (keepalives and discovery)
     */
    void __attribute__((noreturn)) loop();
};


#endif //IPASS_DOMOTICA_NODE_HPP
