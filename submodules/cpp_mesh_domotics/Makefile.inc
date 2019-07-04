#
# Copyright Niels Post 2019.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# https://www.boost.org/LICENSE_1_0.txt)
#

MESH_DOMOTICS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

SEARCH +=  $(MESH_DOMOTICS_DIR)include/

HEADERS += $(MESH_DOMOTICS_DIR)include/mesh_domotics/module.hpp
HEADERS += $(MESH_DOMOTICS_DIR)include/mesh_domotics/node.hpp


SOURCES += $(MESH_DOMOTICS_DIR)src/module.cpp
SOURCES += $(MESH_DOMOTICS_DIR)src/node.cpp




# The rotary encoder module is dependent on the STM32's interrupts, and can therefore not be compiled on another platform
ifndef ($(NO_HWLIB))
HEADERS += $(MESH_DOMOTICS_DIR)include/mesh_domotics/module/led_port.hpp
HEADERS += $(MESH_DOMOTICS_DIR)include/mesh_domotics/module/seven_segment.hpp
SOURCES += $(MESH_DOMOTICS_DIR)src/module/led_port.cpp
SOURCES += $(MESH_DOMOTICS_DIR)src/module/seven_segment.cpp
ifeq ($(TARGET),blue_pill)
SOURCES += $(MESH_DOMOTICS_DIR)src/module/rotary_encoder.cpp
HEADERS += $(MESH_DOMOTICS_DIR)include/mesh_domotics/module/rotary_encoder.hpp
endif
endif