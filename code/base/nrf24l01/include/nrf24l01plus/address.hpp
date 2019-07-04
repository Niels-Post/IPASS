/*
 *
 * Copyright Niels Post 2019.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 *
*/

#ifndef IPASS_NRF_ADDRESS_HPP
#define IPASS_NRF_ADDRESS_HPP

#include <ostream>

namespace nrf24l01 {
    /**
     * \addtogroup nrf24l01Plus
     * @{
     */

    /**
     * \brief ADT for NRF addresses
     *
     * Implements equality checks, and some useful constructors, relating to the way addresses are set in an NRF24l01
     */
    class address {
    public:
        /// All 5 bytes of this address
        uint8_t address_bytes[5];

        /**
         * \brief Basic constructor
         *
         * Construct an NRF address using it's bytes
         * @param byteAddress Address Bytes
         */
        address(uint8_t byteAddress[5]) : address_bytes{0} {
            for (size_t i = 0; i < 5; i++) {
                address_bytes[i] = byteAddress[i];
            }
        }

        /**
         * \brief Create an empty Address
         */
        address() : address_bytes{0, 0, 0, 0, 0} {

        }

        /**
         * \brief Construct an address using seperate bytes
         * @param byte_1 First byte
         * @param byte_2 Second byte
         * @param byte_3 Third byte
         * @param byte_4 Fourth byte
         * @param byte_5 Fifth byte
         */
        address(const uint8_t &byte_1, const uint8_t &byte_2, const uint8_t &byte_3, const uint8_t &byte_4,
                const uint8_t &byte_5) : address_bytes{byte_1, byte_2, byte_3, byte_4, byte_5} {}

        /**
         * \brief Construct an address using a base address
         *
         * Uses the  first 4 bytes of the given address, and adds in a fifth byte
         * @param base Address to base on
         * @param end Fifth byte to set
         */
        address(const address &base, uint8_t end) : address_bytes{0} {
            for (size_t i = 0; i < 4; i++) {
                address_bytes[i] = base.address_bytes[i];
            }
            address_bytes[4] = end;
        }

        /**
         * \brief Check for full equality with another address
         * @param rhs Address to compare to
         * @return True if all bytes in the addresses are equal
         */
        bool operator==(const address &rhs) const {
            for (size_t i = 0; i < 5; i++) {
                if (address_bytes[i] != rhs.address_bytes[i]) {
                    return false;
                }

            }
            return true;
        }

        /**
         * \brief Compare an address to a byte value
         *
         * Returns true if the fifth byte of the address is equal to the byte value
         * @param rhs Byte value to compare to
         * @return True if the fifth byte is equal to the value
         */
        bool operator==(const uint8_t &rhs) const {
            return (address_bytes[4] == rhs);
        }

        /**
         * \brief Check for inequality with a byte value
         *
         * Returns true if the fifth byte of the address is inequal to the byte value
         * @param rhs Byte value to compare to
         * @return True if the fifth byte is inequal to the value
         */
        bool operator!=(const uint8_t &rhs) const {
            return !(*this == rhs);
        }

        /**
         * \brief Inequality operator for two nrf_addresses
         *
         * Checks for full equality of all address bytes
         * @param rhs Address to compare to
         * @return True if all address bytes are equal
         */
        bool operator!=(const address &rhs) const {
            return !(rhs == *this);
        }

        /**
         * \brief Print an address to an ostream
         *
         * Prints all address bytes in hexadecimal representation
         * @param os Stream to output to
         * @param address Address to output
         * @return The ostream, with data fed into it
         */
        friend hwlib::ostream &operator<<(hwlib::ostream &os, const address &address) {
            os << "address_bytes: " << hwlib::hex << address.address_bytes[0] << " " << address.address_bytes[1] << " "
               << address.address_bytes[2] << " " << address.address_bytes[3] << " " << address.address_bytes[4] << " "
               << hwlib::dec;
            return os;
        }
    };

    /**
     * @}
     */
}

#endif //IPASS_NRF_ADDRESS_HPP
