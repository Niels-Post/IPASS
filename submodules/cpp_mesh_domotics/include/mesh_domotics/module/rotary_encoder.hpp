//
// Created by Niels on 6/27/2019.
/*
 *
 * Copyright Niels Post 2019.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 *
*/

#ifndef IPASS_ROTARY_ENCODER_HPP
#define IPASS_ROTARY_ENCODER_HPP

#define register

#include "stm32f103x6.h"

#undef register

#include <mesh_domotics/module.hpp>

/**
 * \brief EXTI9_5 interrupt handler
 *
 * External interrupt handler, used for detecting edges on pin A9, for reading the rotary encoder
 * TODO: Implement possibility for other pins
 */
void __attribute__((interrupt("IRQ"))) EXTI9_5_IRQHandler(void) asm ("EXTI9_5_IRQHandler");

/**
 * \brief Timer 2 Interrupt handler
 *
 * Used for debouncing rotary encoder input
 * TODO: implement timer choice
 */
void __attribute__((interrupt("IRQ"))) TIM2_IRQHandler(void) asm ("TIM2_IRQHandler");


namespace mesh_domotics {
    namespace modules {
        /**
         * \addtogroup example_modules
         * \ingroup mesh_domotics
         * @{
         */



        /**
         * \brief Output module for Rotary encoders, uses interrupts for accurate input
         */
        class rotary_encoder : public input_module {
            domotica_value last_value;
        public:
            /**
             * Creates a Rotary encoder input module
             * @param id Module ID, make sure this is unique within the network
             */
            rotary_encoder(uint8_t id);

            /**
             * Gets current saved value of the rotary encoder (format: 32 bit signed integer)
             * This functien doesn't adjust anything if the value is the same as the last retrieved value
             * @param data 4 byte memory space to put the current value into
             * @param force Forces the function to aquire data, even if there is no change
             * @return True if the value had changed, and was written into data
             */
            bool get_output(uint8_t data[4], bool force) override;
        };

        /**
         * @}
         */

    }
}
#endif //IPASS_ROTARY_ENCODER_HPP
