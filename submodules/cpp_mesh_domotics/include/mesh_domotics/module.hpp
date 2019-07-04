/*
 *
 * Copyright Niels Post 2019.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 *
*/

#ifndef IPASS_DOMOTICA_MODULE_HPP
#define IPASS_DOMOTICA_MODULE_HPP


#include <mesh/message.hpp>

/**
 * \defgroup example_modules Example Domotics Modules
 * \ingroup mesh_domotics
 * \brief Collection of example mesh_domotics modules
 */

namespace mesh_domotics {
    /**
     * \addtogroup mesh_domotics
     * @{
     */

/**
 * \brief Indicates if a module is input or output, NONE can be used for "default" initializers
 */
    enum module_type {
        NONE,
        INPUT,
        OUTPUT
    };

    /**
     * \brief Used to easily "convert" data into bytes for transfer
     */
    union domotica_value {
        /// Numeric Value
        int32_t numeric = 0;
        /// Value of Characters
        char characters[4];
        /// Value of bytes
        uint8_t data[4];
    };

/**
 *  \brief Base ADT for domotica network modules.
 *
 *  Domotica modules either provide data for something in the network to handle, or handle the data themselves
 */
    class module {
    private:
        module_type type;
        uint8_t id;
    protected:
    public:
        /**
         * \brief List of node_id's for the module to interact with.
         *
         * For an output module, these are the nodes it sends it's data to
         * For an input module, it is the nodes it should act on
         */
        std::array<mesh::node_id, 10> filter = {0};

        /**
         * \brief Create a default module, mainly used to fill references that are not used
         */
        module();

        /**
         * \brief Create a domotica module with type
         *
         * @param type Type of the module
         * @param id Id of the module, note that this should be unique within a network
         */
        module(module_type type, uint8_t id);

        /**
         * \brief Retrieve the type of this module
         *
         * @return Type enum
         */
        module_type getType() const;


    };


    /**
     * \brief Domotica input module, should respond to input from the network
     */
    class output_module : public module {
    public:
        /**
         * \brief Create an input module
         *
         * @param id Id of the module, note that this should be unique within a network
         */
        output_module(uint8_t id);

        /**
         * \brief Feed an input into this module
         *
         * Note that filter checking needs to be done before this method is called
         * Input modules should document how they interpret 4 byte data
         * @param data Data to set
         */
        virtual void set_input(uint8_t data[4]) {};
    };


    /**
     * \brief Domotica output module, should provide output for the network
     */
    class input_module : public module {
    public:
        /**
         * Create an output module
         * @param id Id of the module, note that this should be unique within a network
         */
        input_module(uint8_t id);

        /**
         * Check if this module has changed data available, and read it
         * @param data 4 Byte pointer to put data into if it is available
         * @param force Force the module to give data, even if it hasn't changed
         * @return True if the data had changed (or if it was forced)
         */
        virtual bool get_output(uint8_t data[4], bool force) {
            return false;
        };

    };

    /**
     * @}
     */

}
#endif //IPASS_DOMOTICA_MODULE_HPP
