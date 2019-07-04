#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2016
#
# This file is in the public domain.
#
#############################################################################

# source files in this project (main.cpp is automatically assumed)
SOURCES := src/TEST_pin_debug.cpp src/TEST_domotica.cpp src/TEST_spi_testing.cpp src/TEST_link_state_calculator.cpp src/TEST_nrf_address.cpp src/TEST_nrf24l01plus.cpp src/TEST_mesh_message.cpp

# header files in this project
HEADERS := inc/pin_debug.hpp

# other places to look for files for this project
SEARCH  := inc/ ../code/base/util/
TARGET  := native

include ../code/base/mesh/Makefile.inc
include ../code/base/mesh_domotics/Makefile.inc
include ../code/base/link_state/Makefile.inc
include ../code/base/nrf24l01/Makefile.inc
include ../code/base/spi/Makefile.inc

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

