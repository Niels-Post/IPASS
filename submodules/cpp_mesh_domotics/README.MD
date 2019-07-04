C++ Mesh networking for home automation
============================

This library provides an easy way to make microcontrollers interact with eachother in a mesh network, with the purpose of home automation.
A microcontroller can use 1 input, and 1 output module:
- An input module can use sensors (eg. buttons, rotary encoders) to provide data for the network
- An output module can drive actors (eg. leds, relays, motors) to respond to given input

Code standard: C++17

This library is part of my IPASS end project, for the first year of my HBO-ICT study at Hogeschool Utrecht in 2019.

Features 
---
- Easy to understand module structure, easy implementation
- 3 Example modules  
    - `input`  **rotary_encoder** (only works on STM32 Blue_pill MCU's):   
    Uses interrupts to read a rotary encoder. Switch is not implemented. The middle pin of the rotary encoder should be connected to Vcc.
    - `output` **led_port**:  
     The provided number of leds are turned on in the given port.
    - `output` **seven_segment**  
    Displays any provided number to a single digit seven segment display.
    
Dependencies
-----
Mesh_domotics relies on [cpp_mesh](https://github.com/Niels-Post/cpp_mesh) as mesh network implementation.

All included example modules require [hwlib](https://github.com/wovo/hwlib). However, the library itself does not.
When used without HWLIB, **NO_HWLIB** should be set before including *Makefile.inc*. 

Since this library was originally used for an embedded project, the included makefile is built for [BMPTK](http://github.com/wovo/bmptk).
However, the library can also be compiled without BMPTK, or just directly included, since it's header only. 


Installation/usage
-----
- Download the library `git clone https://github.com/Niels-Post/cpp_mesh_domotics`
- Include *Makefile.inc* from your project makefile
- Include `mesh_domotics/node.h` and create a `mesh_domotics::node` object

Building with BMPTK
----
- Make sure to set the TARGET before including *Makefile.inc*
- Include *Makefile.inc*


Building without BMPTK
----
The included makefile only sets some variables, but doesn't do any compiling itself. When building without BMPTK, some variables should be interpreted properly after including.
- HEADERS: all .hpp header files
- SOURCE: all library .cpp files
- SEARCH: the include path for header files of this library 

License Information
---
   
*Copyright Niels Post 2019.     
Distributed under the Boost Software License, Version 1.0.  
(See accompanying file LICENSE_1_0.txt or copy at  
[https://www.boost.org/LICENSE_1_0.txt](https://www.boost.org/LICENSE_1_0.txt))*
