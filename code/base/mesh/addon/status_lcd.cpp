//
// Created by Niels on 7/1/2019.
//

#include "status_lcd.hpp"
#include "../router/link_state_router.hpp"

mesh::status_lcd::status_lcd(mesh::mesh_network &network, lcd::i2c_backpack &lcd) : network(network), lcd(lcd), current_mode(GENERAL) {}

void mesh::status_lcd::update() {
//    network.get_connection().status();
    lcd.set_row(0);
    lcd <<  "ID: " << hwlib::hex << network.get_connection().address ;
    switch(current_mode) {
        case GENERAL:

            break;
        case LINK_STATE_DISTANCES: {
            lcd << "  distances";
            lcd.set_row(1);
            auto &router = static_cast<mesh::routers::link_state &>(network.get_router());
            for (uint8_t i = 1; i < router.get_calculator().get_node_count(); i++) {
                auto node = router.get_calculator().get_node(i);
                lcd << hwlib::hex << node.id << "(" << node.distance << ")";
            }
            break;
        }
        case LINK_STATE_NEIGHBOURS:break;
        case NETWORK_BUFFERSIZE:break;
        case BLACKLIST:break;
    }
    lcd.flush();
}

void mesh::status_lcd::setCurrentMode(mesh::display_mode currentMode) {
    current_mode = currentMode;
}