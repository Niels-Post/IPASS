
#include <mesh_domotics/module/seven_segment.hpp>
#include <mesh_domotics/module/led_port.hpp>
#include <catch.hpp>
#include <pin_debug.hpp>

SCENARIO("Led_port gives the correct output") {
    GIVEN("A led_port module, using a por of dummy pins") {
        pin_debug pin0;
        pin_debug pin1;
        pin_debug pin2;
        pin_debug pin3;
        pin_debug pin4;
        pin_debug pin5;
        pin_debug pin6;
        pin_debug pin7;
        auto port = hwlib::port_out_from(pin0, pin1, pin2, pin3, pin4, pin5, pin6, pin7);
        auto led_port = mesh_domotics::modules::led_port(1, port);

        WHEN("A 7 is fed") {
            union mesh_domotics::domotica_value value;
            value.numeric = 7;
            led_port.set_input(value.data);
            THEN("7 pins turn on in the right direction") {
                REQUIRE(pin0.value);
                REQUIRE(pin1.value);
                REQUIRE(pin2.value);
                REQUIRE(pin3.value);
                REQUIRE(pin4.value);
                REQUIRE(pin5.value);
                REQUIRE(pin6.value);
                REQUIRE(!pin7.value);

                REQUIRE(pin0.was_flushed());
                REQUIRE(pin1.was_flushed());
                REQUIRE(pin2.was_flushed());
                REQUIRE(pin3.was_flushed());
                REQUIRE(pin4.was_flushed());
                REQUIRE(pin5.was_flushed());
                REQUIRE(pin6.was_flushed());
            }
        }

        WHEN("0 is fed") {
            union mesh_domotics::domotica_value value;
            value.numeric = 0;
            led_port.set_input(value.data);

            THEN("All pins are turned off") {
                REQUIRE(!pin0.value);
                REQUIRE(!pin1.value);
                REQUIRE(!pin2.value);
                REQUIRE(!pin3.value);
                REQUIRE(!pin4.value);
                REQUIRE(!pin5.value);
                REQUIRE(!pin6.value);
                REQUIRE(!pin7.value);

                REQUIRE(pin0.was_flushed());
                REQUIRE(pin1.was_flushed());
                REQUIRE(pin2.was_flushed());
                REQUIRE(pin3.was_flushed());
                REQUIRE(pin4.was_flushed());
                REQUIRE(pin5.was_flushed());
                REQUIRE(pin6.was_flushed());
                REQUIRE(pin7.was_flushed());
            }

        }

        WHEN("A value over max is fed") {
            union mesh_domotics::domotica_value value;
            value.numeric = 255;
            led_port.set_input(value.data);

            THEN("ALL pins turn on") {
                REQUIRE(pin0.value);
                REQUIRE(pin1.value);
                REQUIRE(pin2.value);
                REQUIRE(pin3.value);
                REQUIRE(pin4.value);
                REQUIRE(pin5.value);
                REQUIRE(pin6.value);
                REQUIRE(pin7.value);

                REQUIRE(pin0.was_flushed());
                REQUIRE(pin1.was_flushed());
                REQUIRE(pin2.was_flushed());
                REQUIRE(pin3.was_flushed());
                REQUIRE(pin4.was_flushed());
                REQUIRE(pin5.was_flushed());
                REQUIRE(pin6.was_flushed());
                REQUIRE(pin7.was_flushed());
            }


        }
    }
}

SCENARIO("Seven segment display gives the correct output") {
    GIVEN("A seven segment display, connected to a port of debug pins") {
        pin_debug pina;
        pin_debug pinb;
        pin_debug pinc;
        pin_debug pind;
        pin_debug pine;
        pin_debug pinf;
        pin_debug ping;
        auto port = hwlib::port_out_from(pina, pinb, pinc, pind, pine, pinf, ping);
        mesh_domotics::modules::seven_segment display(1, port);

        mesh_domotics::domotica_value val;

        WHEN("Data is 0") {
            val.numeric = 0;
            display.set_input(val.data);
            THEN("ALl pins except G are on") {
                REQUIRE(pina.value);
                REQUIRE(pinb.value);
                REQUIRE(pinc.value);
                REQUIRE(pind.value);
                REQUIRE(pine.value);
                REQUIRE(pinf.value);
                REQUIRE(!ping.value);
            }
        }

        WHEN("Data is 1") {
            val.numeric = 1;
            display.set_input(val.data);
            THEN("Only pin B and C are on") {
                REQUIRE(!pina.value);
                REQUIRE(pinb.value);
                REQUIRE(pinc.value);
                REQUIRE(!pind.value);
                REQUIRE(!pine.value);
                REQUIRE(!pinf.value);
                REQUIRE(!ping.value);
            }
        }

        WHEN("Data is 2") {
            val.numeric = 2;
            display.set_input(val.data);
            THEN("Only pins ABGED are on") {
                REQUIRE(pina.value);
                REQUIRE(pinb.value);
                REQUIRE(!pinc.value);
                REQUIRE(pind.value);
                REQUIRE(pine.value);
                REQUIRE(!pinf.value);
                REQUIRE(ping.value);
            }
        }

        WHEN("Data is 3") {
            val.numeric = 3;
            display.set_input(val.data);
            THEN("Only pins ABGCD are on") {
                REQUIRE(pina.value);
                REQUIRE(pinb.value);
                REQUIRE(pinc.value);
                REQUIRE(pind.value);
                REQUIRE(!pine.value);
                REQUIRE(!pinf.value);
                REQUIRE(ping.value);
            }
        }

        WHEN("Data is 4") {
            val.numeric = 4;
            display.set_input(val.data);
            THEN("Only pins ABGED are on") {
                REQUIRE(!pina.value);
                REQUIRE(pinb.value);
                REQUIRE(pinc.value);
                REQUIRE(!pind.value);
                REQUIRE(!pine.value);
                REQUIRE(pinf.value);
                REQUIRE(ping.value);
            }
        }

        WHEN("Data is 5") {
            val.numeric = 5;
            display.set_input(val.data);
            THEN("Only pins AFGCD") {
                REQUIRE(pina.value);
                REQUIRE(!pinb.value);
                REQUIRE(pinc.value);
                REQUIRE(pind.value);
                REQUIRE(!pine.value);
                REQUIRE(pinf.value);
                REQUIRE(ping.value);
            }
        }

        WHEN("Data is 6") {
            val.numeric = 6;
            display.set_input(val.data);
            THEN("Only pins ACDEFG are on") {
                REQUIRE(pina.value);
                REQUIRE(!pinb.value);
                REQUIRE(pinc.value);
                REQUIRE(pind.value);
                REQUIRE(pine.value);
                REQUIRE(pinf.value);
                REQUIRE(ping.value);
            }
        }

        WHEN("Data is 7") {
            val.numeric = 7;
            display.set_input(val.data);
            THEN("Only pins ABC are on") {
                REQUIRE(pina.value);
                REQUIRE(pinb.value);
                REQUIRE(pinc.value);
                REQUIRE(!pind.value);
                REQUIRE(!pine.value);
                REQUIRE(!pinf.value);
                REQUIRE(!ping.value);
            }
        }

        WHEN("Data is 8") {
            val.numeric = 8;
            display.set_input(val.data);
            THEN("All Pins are on") {
                REQUIRE(pina.value);
                REQUIRE(pinb.value);
                REQUIRE(pinc.value);
                REQUIRE(pind.value);
                REQUIRE(pine.value);
                REQUIRE(pinf.value);
                REQUIRE(ping.value);
            }
        }

        WHEN("Data is 9") {
            val.numeric = 9;
            display.set_input(val.data);
            THEN("Only pins ABCDFG are on") {
                REQUIRE(pina.value);
                REQUIRE(pinb.value);
                REQUIRE(pinc.value);
                REQUIRE(pind.value);
                REQUIRE(!pine.value);
                REQUIRE(pinf.value);
                REQUIRE(ping.value);
            }
        }


        WHEN("Data is above 9") {
            val.numeric = 25;
            display.set_input(val.data);
            THEN("Remainder of division by 10 is used instead") {
                REQUIRE(pina.value);
                REQUIRE(!pinb.value);
                REQUIRE(pinc.value);
                REQUIRE(pind.value);
                REQUIRE(!pine.value);
                REQUIRE(pinf.value);
                REQUIRE(ping.value);
            }
        }
    }
}
