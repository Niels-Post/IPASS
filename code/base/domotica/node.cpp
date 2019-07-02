//
// Created by Niels on 6/27/2019.
//

#include "node.hpp"

namespace mesh_domotics {



    void node::loop() {
        std::array<mesh::message, 10> uncaughtMessages = {};
        for (;;) {
            network.update();
            uint8_t newMsg = network.check_new_messages(uncaughtMessages);
            for (uint8_t i = 0; i < newMsg; i++) {
                mesh::message &msg = uncaughtMessages[i];
                update_input_module(msg);
            }

            hwlib::wait_ms(5);
                update_output_module();
        }

    }


    void node::update_output_module() {
        uint8_t data[4];
        if (outputmodule.get_output(data, ++output_update_count > 50)) {
            mesh::message msg(
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
                if (outputmodule.filter[i] == 0) {
                    break;
                }
                msg.receiver = outputmodule.filter[i];
                network.sendMessage(msg);
            }
        }

        if(output_update_count > 50) {
            output_update_count = 0;
        }
    }

    void node::update_input_module(mesh::message &msg) {
        bool filter_ok = false;
        for (uint8_t i = 0; i < 10; i++) {
            if (msg.sender == inputmodule.filter[i]) {
                filter_ok = true;
                break;
            }
        }
        if (!filter_ok) {
            return;
        }

        uint8_t data[4];

        if (msg.type == mesh::DOMOTICA::DATA) {
            data[0] = msg.data[0];
            data[1] = msg.data[1];
            data[2] = msg.data[2];
            data[3] = msg.data[3];
            inputmodule.set_input(data);
        }
    }

    node::node(mesh::mesh_network &network, input_module &inputModule, output_module &outputModule) : network(
            network), inputmodule(inputModule), outputmodule(outputModule) {

    }

    void node::set_input_module(input_module &input) {
        inputmodule = input;
    }

    void node::set_output_module(output_module &output) {
        outputmodule = output;
    }



}