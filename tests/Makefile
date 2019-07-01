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
SOURCES := TEST_spi_testing.cpp TEST_link_state_calculator.cpp TEST_nrf_address.cpp TEST_nrf24l01plus.cpp TEST_mesh_message.cpp  mesh/connectivity/mesh_nrf_pipe.cpp mesh/connectivity/mesh_nrf_connectivity.cpp spi/spi_base.cpp spi/spi_bitbang.cpp spi/spi_blue_pill.cpp domotica/module/rotary_encoder.cpp domotica/module/led_port.cpp domotica/module/seven_segment.cpp domotica/domotica_module.cpp domotica/domotica_node.cpp link_state/calculator.cpp link_state/node.cpp mesh/router/link_state_router.cpp mesh/connectivity_adapter.cpp

# header files in this project
HEADERS :=

# other places to look for files for this project
SEARCH  :=
TARGET  := native




PROJECT_CPP_FLAGS += -fexceptions
SEARCH += $(HOME)/Catch2/single_include
SEARCH += $(HOME)/Catch2/single_include/catch2

STM32_CLI := "/mnt/c/Program Files (x86)/STMicroelectronics/STM32 ST-LINK Utility/ST-LINK Utility/ST-LINK_CLI.exe"

RUN_PAUSE := echo "Run Complete"
DUE_BOOTMODE := cmd.exe /c @mode com3:1200

# set RELATIVE to the next higher directory
# and defer to the appropriate Makefile.* there
RELATIVE := ..

HWLIB             ?= $(HOME)/hwlib
include $(HWLIB)/makefile.inc

BMPTK             ?= $(HOME)/bmptk
include $(BMPTK)/Makefile.inc

debugtarget:
	$(info    $(DEFINES))

