
#include "PinOutDecorator.hpp"

PinOutDecorator::PinOutDecorator(hwlib::pin_out &slave) : slave(slave) {}

void PinOutDecorator::write(bool x) {
    slave.write(!x);
}

void PinOutDecorator::flush() {
    slave.flush();
}
