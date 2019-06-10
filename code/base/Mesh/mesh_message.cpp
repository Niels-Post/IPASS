//
// Created by Niels on 6/10/2019.
//

#include "mesh_message.hpp"

mesh::mesh_message::mesh_message(uint8_t configuration, uint8_t sender, uint8_t receiver, uint8_t size, uint8_t *data)
        : configuration(configuration), sender(sender), receiver(receiver), size(size), data(data) {}
