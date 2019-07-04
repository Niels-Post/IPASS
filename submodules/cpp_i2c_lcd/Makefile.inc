#
# Copyright Niels Post 2019.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# https://www.boost.org/LICENSE_1_0.txt)
#

LCD_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

SEARCH += $(LCD_DIR)include/
SOURCES += $(LCD_DIR)src/i2c_backpack.cpp
HEADERS += $(LCD_DIR)include/lcd/i2c_backpack.hpp
