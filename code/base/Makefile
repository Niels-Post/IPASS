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
SOURCES := lcd/i2c_backpack.cpp mesh/addon/status_lcd.cpp domotica/module.cpp domotica/module/led_port.cpp domotica/module/rotary_encoder.cpp domotica/module/seven_segment.cpp domotica/node.cpp mesh/connectivity/nrf.cpp mesh/connectivity/nrf_pipe.cpp mesh/connectivity_adapter.cpp mesh/router/link_state_router.cpp spi/spi_base.cpp spi/spi_bitbang.cpp spi/spi_blue_pill.cpp vtable.asm

# header files in this project
HEADERS :=

# other places to look for files for this project
SEARCH  :=
TARGET  := blue_pill







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

