//
// Created by Niels on 6/25/2019.
//

#ifndef IPASS_NRF_ADDRESS_HPP
#define IPASS_NRF_ADDRESS_HPP

namespace nrf24l01 {
    class nrf_address {
    public:
        uint8_t address_bytes[5];

        nrf_address(uint8_t byteAddress[5]) : address_bytes{0} {
            for (size_t i = 0; i < 5; i++) {
                address_bytes[i] = byteAddress[i];
            }
        }

        nrf_address() : address_bytes{0, 0, 0, 0, 0} {

        }

        nrf_address(const uint8_t &byte_1, const uint8_t &byte_2, const uint8_t &byte_3, const uint8_t &byte_4,
                    const uint8_t &byte_5) : address_bytes{byte_1, byte_2, byte_3, byte_4, byte_5} {}

        nrf_address(const nrf_address &base, uint8_t end) : address_bytes{0} {
            for (size_t i = 0; i < 4; i++) {
                address_bytes[i] = base.address_bytes[i];
            }
            address_bytes[4] = end;
        }

        bool operator==(const nrf_address &rhs) const {
            for (size_t i = 0; i < 5; i++) {
                if (address_bytes[i] != rhs.address_bytes[i]) {
                    return false;
                }

            }
            return true;
        }

        bool operator==(const uint8_t &rhs) const {
            return (address_bytes[4] == rhs);
        }

        bool operator!=(const uint8_t &rhs) const {
            return !(*this == rhs);
        }

        bool operator!=(const nrf_address &rhs) const {
            return !(rhs == *this);
        }
    };
}

#endif //IPASS_NRF_ADDRESS_HPP
