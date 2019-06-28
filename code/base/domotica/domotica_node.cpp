//
// Created by Niels on 6/27/2019.
//

#include "domotica_node.hpp"


void domotica_node::loop() {
    std::array<mesh::mesh_message, 10> uncaughtMessages = {};
    for (;;) {
        uint8_t data[4];
        network.update();
        for (uint8_t i = 0; i < network.check_new_messages(uncaughtMessages); i++) {
            LOG("SOMETHING UNCAUGHT", "");
            mesh::mesh_message &msg = uncaughtMessages[i];
            if (msg.type == mesh::DOMOTICA::DATA) {
                data[0] = msg.data[0];
                data[1] = msg.data[1];
                data[2] = msg.data[2];
                data[3] = msg.data[3];
                input_module.set_input(data);
            }
        }


        for(uint8_t x = 0; x < 15; x++) {
            if (output_module.get_output(data)) {
                mesh::mesh_message msg(
                        mesh::DOMOTICA::DATA,
                        0,
                        0,
                        0,
                        4
                );
                msg.data[0] = data[0];
                msg.data[1] = data[1];
                msg.data[2] = data[2];
                msg.data[3] = data[3];
                for (uint8_t i = 0; i < 10; i++) {
                    if (output_module.filter[i] == 0) {
                        break;
                    }
                    msg.receiver = output_module.filter[i];
                    network.sendMessage(msg);
                }
            }
            hwlib::wait_us(1);
        }
    }
}


void domotica_node::setInputModule(domotica_input_module &inputModule) {
    input_module = inputModule;
}

void domotica_node::setOutputModule(domotica_output_module &outputModule) {
    output_module = outputModule;
}

domotica_node::domotica_node(mesh::mesh_network &network, domotica_input_module &inputModule,
                             domotica_output_module &outputModule) : network(network), input_module(inputModule),
                                                                     output_module(outputModule) {}
