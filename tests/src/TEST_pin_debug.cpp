
#include <single_include/catch2/catch.hpp>
#include <pin_debug.hpp>


SCENARIO("debug_pin stores state correctly") {
    GIVEN("A debug pin") {
        pin_debug pin;

        WHEN("True is written through write()") {
            pin.write(true);
            THEN("The value changes to true") {
                REQUIRE(pin.value);
            }
            AND_THEN("Read() returns true") {
                REQUIRE(pin.read());
            }
            AND_THEN("was_flushed returns false") {
                REQUIRE(!pin.was_flushed());
            }
        }

        WHEN("Flush is called") {
            pin.flush();
            THEN("Was flushed is true") {
                REQUIRE(pin.was_flushed());
            }
        }

        WHEN("False is written through write()") {
            pin.write(false);
            THEN("The value changes to false") {
                REQUIRE(!pin.value);
            }
            AND_THEN("Read() returns false") {
                REQUIRE(!pin.read());
            }
            AND_THEN("was_flushed returns false") {
                REQUIRE(!pin.was_flushed());
            }
        }

        WHEN("Value is manually written as true") {
            pin.value = true;
            THEN("read() returns true") {
                REQUIRE(pin.read());
            }
        }

        WHEN("Value is manually written as false") {
            pin.value = false;
            THEN("read() returns false") {
                REQUIRE(!pin.read());
            }
        }
    }
}

SCENARIO("debug_pin is usable both as output as input") {
    GIVEN("A debug pin") {
        pin_debug pin;
        WHEN("it's cast to an output pin") {
#pragma GCC diagnostic ignored "-Wunused-variable"
            hwlib::pin_out &pin_out = pin;
            THEN("No compile error occurs") {
                REQUIRE(true);
            }
        }

        WHEN("it's cast to an input pin") {
#pragma GCC diagnostic ignored "-Wunused-variable"
            hwlib::pin_in &pin_in = pin;
            THEN("No compile error occurs") {
                REQUIRE(true);
            }
        }
    }
}
