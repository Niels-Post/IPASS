//
// Created by Niels on 6/27/2019.
//

#ifndef IPASS_DOMOTICA_NODE_HPP
#define IPASS_DOMOTICA_NODE_HPP


#include "../mesh/mesh_network.hpp"
#include "domotica_module.hpp"

class domotica_node {
private:
    mesh::mesh_network &network;

    domotica_input_module &input_module;
    domotica_output_module &output_module;


public:
    domotica_node(mesh::mesh_network &network, domotica_input_module &inputModule,
                  domotica_output_module &outputModule);

    void setInputModule(domotica_input_module &inputModule);

    void setOutputModule(domotica_output_module &outputModule);

    void loop();

};


#endif //IPASS_DOMOTICA_NODE_HPP
