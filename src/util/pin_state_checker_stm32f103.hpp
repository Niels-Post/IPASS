//
// Created by Niels on 7/5/2019.
//

#ifndef IPASS_PIN_STATE_CHECKER_STM32F103_HPP
#define IPASS_PIN_STATE_CHECKER_STM32F103_HPP

class pin_state_checker_stm32f103 {
    bool success_all = true;

    uint32_t APB2ENR_old = 0;
    uint32_t port_config_old = 0;
    uint32_t ODR_old = 0;
    uint32_t current_pin_mask = 0;
    hwlib::target::pin_info_type current_pin_info = {};
    GPIO_TypeDef *current_port = nullptr;
    uint32_t &port_config = APB2ENR_old; // Just initial value

    void prepare(const hwlib::target::pins &pin) {
        current_pin_info = hwlib::target::pin_info(pin);
        APB2ENR_old = RCC->APB2ENR;
        RCC->APB2ENR |= 0x00000004u << current_pin_info.port;
        auto port_address = APB2PERIPH_BASE + 0x0800 + (current_pin_info.port * 0x0400);
        current_port = ((GPIO_TypeDef *) port_address);
        port_config = current_pin_info.pin < 8 ? current_port->CRL : current_port->CRH;
        current_pin_mask = 1u << current_pin_info.pin;
        ODR_old = current_port->ODR;
        port_config_old = port_config;
        port_config &= ~(0xFu << (current_pin_info.pin*4u));
    }

    void restore(){
        current_port->ODR = ODR_old;
        port_config = port_config_old;
        RCC->APB2ENR = APB2ENR_old;
    }
public:



    void assert_high(const hwlib::target::pins &pin) {
        prepare(pin);
        port_config |= (0x4u << (current_pin_info.pin*4u));
        hwlib::wait_ms(10);
        if((current_port->IDR & current_pin_mask) == 0) {
            LOG("Line high assertion failed: ", "pin(" << current_pin_info.port << "," << current_pin_info.pin << ")." << "Required: HIGH but was LOW");
            success_all = false;
        }
        restore();
    }

    void assert_low(const hwlib::target::pins &pin) {
        prepare(pin);
        port_config |= (0x4u << (current_pin_info.pin*4u));
        hwlib::wait_ms(10);
        if((current_port->IDR & current_pin_mask) > 0) {
            LOG("Line low assertion failed: ", "pin(" << current_pin_info.port << "," << current_pin_info.pin << ")." << "Required: LOW but was HIGH");
            success_all = false;
        }
        restore();
    }

    void assert_unset(const hwlib::target::pins &pin) {
        prepare(pin);
        port_config = (0x8u << (current_pin_info.pin*4u));

        current_port->ODR &=  ~current_pin_mask ;
        hwlib::wait_ms(10);

        LOG("test", hwlib::bin << current_port->IDR);
        if((current_port->IDR & current_pin_mask) > 0) {
            LOG("Line unset assertion failed: ", "pin(" << current_pin_info.port << "," << current_pin_info.pin << ")." << "Required: LOW but was HIGH");
            success_all = false;
        }


        current_port->ODR |= current_pin_mask ;
        hwlib::wait_ms(10);

        if((current_port->IDR & current_pin_mask) == 0) {
            LOG("Line unset assertion failed: ", "pin(" << current_pin_info.port << "," << current_pin_info.pin << ")." << "Required: HIGH but was LOW");
            success_all = false;
        }

        restore();
    }



    bool all_successful() {
        return success_all;
    }
};

#endif //IPASS_PIN_STATE_CHECKER_STM32F103_HPP
