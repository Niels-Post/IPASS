#
# Copyright Niels Post 2019.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# https://www.boost.org/LICENSE_1_0.txt)
#

NRF24L01DIR := $(dir $(lastword $(MAKEFILE_LIST)))

SEARCH += $(NRF24L01DIR)include/

HEADERS += $(NRF24L01DIR)include/nrf24l01plus/address.hpp
HEADERS += $(NRF24L01DIR)include/nrf24l01plus/definitions.hpp
HEADERS += $(NRF24L01DIR)include/nrf24l01plus/nrf24l01plus.hpp
