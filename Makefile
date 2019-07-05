#
# Copyright Niels Post 2019.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# https://www.boost.org/LICENSE_1_0.txt)
#

$(shell git submodule update --init --recursive)

PROJECT := src/main

HWLIB := ./submodules/hwlib
BMPTK := ./submodules/bmptk
CATCH := ./submodules/catch2


SOURCES := src/vtable.asm
HEADERS :=

# other places to look for files for this project
SEARCH  := src/util
TARGET  := blue_pill

include submodules/i2c_lcd/Makefile.inc
include submodules/mesh/Makefile.inc
include submodules/mesh_domotics/Makefile.inc
include submodules/link_state/Makefile.inc
include submodules/nrf24l01/Makefile.inc
include submodules/spi/Makefile.inc

STM32_CLI := "/mnt/c/Program Files (x86)/STMicroelectronics/STM32 ST-LINK Utility/ST-LINK Utility/ST-LINK_CLI.exe"
RUN_PAUSE := echo "Run Complete"
DUE_BOOTMODE := cmd.exe /c @mode com3:1200

include $(HWLIB)/makefile.inc
include $(BMPTK)/Makefile.inc


#include /mnt/l/HWLib/HWLIB-Cmake-Tools/Makefile.inc


update:
	git submodule update --recursive --remote


testing:
	cd tests && $(MAKE) run








