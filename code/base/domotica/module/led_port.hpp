

#ifndef IPASS_LED_PORT_HPP
#define IPASS_LED_PORT_HPP


#include "../module.hpp"


namespace mesh_domotics {
    namespace modules {
        /**
         * \addtogroup example_modules
         * \ingroup mesh_domotics
         * @{
         */

        /**
         * \brief Domotica input module for a row of pin_outs.
         *
         * Takes the input value as an integer, and enables that amount of pins in the given port.
         * Pins are enabled lsbit first
         */
        class led_port : public input_module {
            hwlib::port_out &port;
            union domotica_value last_val;
        public:
            /**
             * \brief Creates a LED port
             *
             * @param id Module ID to use, make sure this is unique within the network
             * @param port Port of pin_outs to enable/disable on input
             */
            led_port(uint8_t id, hwlib::port_out &port);

        public:
            /**
             * \brief Turns on the amount of leds in the numeric value of data
             *
             * @param data Data to process
             */
            void set_input(uint8_t *data) override;
        };

        /**
         * @}
         */
    }
}
#endif //IPASS_LED_PORT_HPP
