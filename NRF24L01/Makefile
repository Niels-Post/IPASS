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
SOURCES := NRF24L01/NRF24L01Plus.cpp NRF24L01/RXPipe.cpp NRF24L01/RXPipeAll.cpp SPI/spi_base.cpp SPI/spi_bitbang.cpp

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
