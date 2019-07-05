#
# Copyright Niels Post 2019.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# https://www.boost.org/LICENSE_1_0.txt)
#

SOURCES := src/TEST_pin_debug.cpp
SOURCES += src/TEST_domotica.cpp
SOURCES += src/TEST_spi_testing.cpp
SOURCES += src/TEST_link_state_calculator.cpp
SOURCES += src/TEST_nrf_address.cpp
SOURCES += src/TEST_nrf24l01plus.cpp
SOURCES += src/TEST_mesh_message.cpp

HEADERS := inc/pin_debug.hpp

SEARCH  := inc/ ../code/base/util/

TARGET  := native

include ../submodules/i2c_lcd/Makefile.inc
include ../submodules/link_state/Makefile.inc
include ../submodules/mesh/Makefile.inc
include ../submodules/mesh_domotics/Makefile.inc
include ../submodules/nrf24l01/Makefile.inc
include ../submodules/spi/Makefile.inc

PROJECT_CPP_FLAGS += -fexceptions

SEARCH += $(HOME)/Catch2/single_include
SEARCH += $(HOME)/Catch2/single_include/catch2


# set RELATIVE to the next higher directory
# and defer to the appropriate Makefile.* there
RELATIVE := ..

HWLIB             ?= $(HOME)/hwlib
include $(HWLIB)/makefile.inc

BMPTK             ?= $(HOME)/bmptk
include $(BMPTK)/Makefile.inc

debugtarget:
	$(info    $(DEFINES))

