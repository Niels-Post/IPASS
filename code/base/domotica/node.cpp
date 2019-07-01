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
                update_input_module(primary_module, msg);
                update_input_module(secondary_module, msg);
            }


            for (uint8_t x = 0; x < 50; x++) {
                update_output_module(primary_module);
                if(x % 4 == 0) {
                    update_output_module(secondary_module);
                }
                hwlib::wait_us(1);
            }
        }
    }


    void node::update_output_module(module &m) {
        if (m.getType() != OUTPUT) {
            return;
        }
        uint8_t data[4];
        auto &out = static_cast<output_module &>(m);
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

    node::node(mesh::mesh_network &network, module primaryModule, module secondaryModule) : network(
            network), primary_module(primaryModule), secondary_module(secondaryModule) {

    }

    void node::set_primary_module(module primary_module) {
        node::primary_module = primary_module;
    }

    void node::set_secondary_module(module secondary_module) {
        node::secondary_module = secondary_module;
    }


}