//
// Created by Niels on 6/27/2019.
//

#ifndef IPASS_DOMOTICA_MODULE_HPP
#define IPASS_DOMOTICA_MODULE_HPP


#include "../mesh/mesh_message.hpp"

enum module_type {
    NONE,
    INPUT,
    OUTPUT
};

union domotica_value {
    int32_t numeric = 0;
    char data[4];
};

class domotica_module {
private:
    module_type type;
    uint8_t id;
protected:
public:
    std::array<mesh::node_id, 10> filter = {0};

    domotica_module();

    domotica_module(module_type type, uint8_t id);

    module_type getType() const;



};


class domotica_input_module: public domotica_module {
public:
    domotica_input_module(uint8_t id);

    virtual void set_input(uint8_t data[4]) {};
};


class domotica_output_module: public domotica_module {
public:
    domotica_output_module(uint8_t id);

    virtual bool get_output(uint8_t data[4]) {return false;};

};


#endif //IPASS_DOMOTICA_MODULE_HPP
