
#ifndef PROJECT_HUTS_HPP
#define PROJECT_HUTS_HPP

#include <hwlib.hpp>
class huts {
public:
    static void a_niffau() {
        auto hutspin = hwlib::target::pin_out(hwlib::target::pins::a2);
        for (int i = 0; i < 20; i++) {
            hutspin.write(i % 2 == 0);
            hwlib::wait_us(5);
        }
    }
};
#endif //PROJECT_HUTS_HPP
