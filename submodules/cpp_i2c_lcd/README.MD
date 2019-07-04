C++ I2C LCD Backpack library
============================

A simple, basic library for interfacing with an I2C LCD "backpack".

Code standard: C++17

This library is part of my IPASS end project, for the first year of my HBO-ICT study at Hogeschool Utrecht in 2019.

Dependencies
-----
For abstraction of communication with microcontrollers, this library uses [hwlib](http://github.com/wovo/hwlib) and its bitbanged I2C implementation.

The included makefile is built for [BMPTK](http://github.com/wovo/bmptk) although this is not necessary.

Installation
-----
- Download the library `git clone http://github.com/Niels-Post/cpp_i2c_lcd`
- Include Makefile.inc from your project

Building with BMPTK
----
- Make sure to set the TARGET before including *Makefile.inc*
- Include *Makefile.inc*


Building without BMPTK
----
The included makefile only sets some variables, but doesn't do any compiling itself. When building without BMPTK, some variables should be interpreted properly after including.
- HEADERS: all .hpp header files
- SOURCES: all .cpp source files
- SEARCH: theinclude path for header files of this library 


