//
// Created by Niels on 6/27/2019.
//

#ifndef IPASS_DOMOTICA_MODULE_HPP
#define IPASS_DOMOTICA_MODULE_HPP


#include "../mesh/mesh_message.hpp"

/**
 * Indicates if a module is input or output, NONE can be used for "default" initializers
 */
enum module_type {
    NONE,
    INPUT,
    OUTPUT
};

/**
 * Used to easily "convert" data into bytes for transfer
 */
union domotica_value {
    int32_t numeric = 0;
    char data[4];
};

/**
 *  Base ADT for domotica network modules.
 *  Domotica modules either provide data for something in the network to handle, or handle the data themselves
 */
class domotica_module {
private:
    module_type type;
    uint8_t id;
protected:
public:
    /**
     * List of node_id's for the module to interact with
     * For an output module, these are the nodes it sends it's data to
     * For an input module, it is the nodes it should act on
     */
    std::array<mesh::node_id, 10> filter = {0};

    /**
     * Create a default module, mainly used to fill references that are not used
     */
    domotica_module();

    /**
     * Create a domotica module with type
     * @param type Type of the module
     * @param id Id of the module, note that this should be unique within a network
     */
    domotica_module(module_type type, uint8_t id);

    /**
     * Retrieve the type of this module
     * @return Type enum
     */
    module_type getType() const;



};


class domotica_input_module: public domotica_module {
public:
    /**
     * Create an input module
     * @param id Id of the module, note that this should be unique within a network
     */
    domotica_input_module(uint8_t id);

    /**
     * Feed an input into this module
     * Note that filter checking needs to be done before this method is called
     * Input modules should document how they interpret 4 byte data
     * @param data Data to set
     */
    virtual void set_input(uint8_t data[4]) {};
};


class domotica_output_module: public domotica_module {
public:
    /**
     * Create an output module
     * @param id Id of the module, note that this should be unique within a network
     */
    domotica_output_module(uint8_t id);

    /**
     * Check if this module has changed data available, and read it
     * @param data 4 Byte pointer to put data into if it is available
     * @param force Force the module to give data, even if it hasn't changed
     * @return True if the data had changed (or if it was forced)
     */
    virtual bool get_output(uint8_t data[4], bool force) {return false;};

};


#endif //IPASS_DOMOTICA_MODULE_HPP
