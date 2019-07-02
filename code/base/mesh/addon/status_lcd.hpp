//
// Created by Niels on 7/1/2019.
//

#ifndef IPASS_STATUS_LCD_HPP
#define IPASS_STATUS_LCD_HPP


#include <hwlib.hpp>
#include "../mesh_network.hpp"
#include "../router.hpp"
#include "../../lcd/i2c_backpack.hpp"

namespace mesh {

    enum display_mode {
        GENERAL,
        LINK_STATE_DISTANCES,
        LINK_STATE_NEIGHBOURS,
        NETWORK_BUFFERSIZE,
        BLACKLIST,
    };

    class status_lcd {
        mesh::mesh_network &network;
        lcd::i2c_backpack &lcd;
        display_mode current_mode;
    public:
        status_lcd(mesh_network &network, lcd::i2c_backpack &lcd);

        void update();

        void setCurrentMode(display_mode currentMode);
    };

}


#endif //IPASS_STATUS_LCD_HPP
