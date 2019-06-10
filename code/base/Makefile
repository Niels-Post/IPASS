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
SOURCES := Mesh/mesh_connectivity_adapter.cpp Mesh/mesh_message.cpp Mesh/mesh_node.cpp Mesh/mesh_router.cpp NRF24L01/nrf24l01plus.cpp NRF24L01/rx_all_pipes.cpp NRF24L01/rx_pipe.cpp NRF_Mesh/mesh_nrf_connectivity.cpp SPI/spi_base.cpp SPI/spi_bitbang.cpp

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
