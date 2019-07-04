#
# Copyright Niels Post 2019.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# https://www.boost.org/LICENSE_1_0.txt)
#

SPI_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

SEARCH += $(SPI_DIR)include/

HEADERS += $(SPI_DIR)include/spi/bus_base.hpp
HEADERS += $(SPI_DIR)include/spi/bus_bitbang.hpp
HEADERS += $(SPI_DIR)include/spi/bus_testing.hpp

SOURCES += $(SPI_DIR)src/bus_base.cpp
SOURCES += $(SPI_DIR)src/bus_bitbang.cpp

ifeq ($(TARGET),blue_pill)
HEADERS += $(SPI_DIR)include/spi/hardware/bus_stm32f10xxx.hpp
SOURCES += $(SPI_DIR)src/hardware/bus_stm32f10xxx.cpp
endif