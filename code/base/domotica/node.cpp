//
// Created by Niels on 6/27/2019.
//

#include "node.hpp"

namespace mesh_domotics {



    void node::loop() {
        std::array<mesh::message, 10> uncaughtMessages = {};
        for (;;) {
            network.update();
            for (uint8_t i = 0; i < network.check_new_messages(uncaughtMessages); i++) {
                LOG("SOMETHING UNCAUGHT", "");
                mesh::message &msg = uncaughtMessages[i];
                update_input_module(inputmodule, msg);
            }

            hwlib::wait_ms(1);
            for (uint8_t x = 0; x < 50; x++) {
                update_output_module(outputmodule);
                hwlib::wait_us(1);
            }
        }

    }


    void node::update_output_module(output_module &m) {
        if (m.getType() != OUTPUT) {
            return;
        }
        uint8_t data[4];
        auto &out = m;
        if (out.get_output(data, true)) {
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
                if (out.filter[i] == 0) {
                    break;
                }
                msg.receiver = out.filter[i];
                network.sendMessage(msg);
            }
        }
    }

    void node::update_input_module(module &m, mesh::message &msg) {
        if (m.getType() != INPUT) {
            return;
        }
        bool filter_ok = false;
        for (uint8_t i = 0; i < 10; i++) {
            if (msg.sender == m.filter[i]) {
                filter_ok = true;
                break;
            }
        }
        if (!filter_ok) {
            return;
        }

        uint8_t data[4];
        auto &inp = static_cast<input_module &>(m);

        if (msg.type == mesh::DOMOTICA::DATA) {
            data[0] = msg.data[0];
            data[1] = msg.data[1];
            data[2] = msg.data[2];
            data[3] = msg.data[3];
            inp.set_input(data);
        }
    }

    node::node(mesh::mesh_network &network, input_module &inputModule, output_module &outputModule) : network(
            network), inputmodule(inputModule), outputmodule(outputModule) {

    }

    void node::set_input_module(input_module &primary_module) {
        node::inputmodule = primary_module;
    }

    void node::set_output_module(output_module &secondary_module) {
        node::outputmodule = secondary_module;
    }



}