
#ifndef PROJECT_PINOUTDECORATOR_HPP
#define PROJECT_PINOUTDECORATOR_HPP

#include <hwlib.hpp>

class PinOutDecorator : public hwlib::pin_out {
    hwlib::pin_out &slave;

public:
    void write(bool x) override;

    void flush() override;

    PinOutDecorator(pin_out &slave);
};


#endif //PROJECT_PINOUTDECORATOR_HPP
