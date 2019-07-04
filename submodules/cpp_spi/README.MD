C++ SPI library
============================

Library for SPI communication. Can be used with any platform.

This library is part of my IPASS end project, for the first year of my HBO-ICT study at Hogeschool Utrecht in 2019.


Features 
---
- Base BitBanged SPI implementation
- Includes a testing bus, which can be used to check input/output in Unit tests.

Included
---
- Basic BitBang implementation
- Hardware implementation for the STM32 BluePill, using DMA

Dependencies
-----
For pin abstraction [hwlib](https://github.com/wovo/hwlib) is required.

When using hardware SPI buses, HWLIB requires [BMPTK](https://github.com/wovo/bmptk) to correctly place the device header files.

Installation/usage
-----
- Download the library `git clone https://github.com/Niels-Post/cpp_spi`
- Make sure to set TARGET before including Makefile.inc
- Include *Makefile.inc* from your project
- Include the bus you'd like to use
- Start a transaction using bus.transaction(csn). When using a hardware bus, csn can be left out.

Building with BMPTK
----
- Just Include *Makefile.inc* :)


Building without BMPTK
----
The included makefile only sets some variables, but doesn't do any compiling itself. When building without BMPTK, some variables should be interpreted properly after including.
- HEADERS: all .hpp header files
- SOURCES: all library .cpp files
- SEARCH: the include path for header files of this library 

Note that when building without BMPTK, the included hardware buses may not work.


License Information
---
   
*Copyright Niels Post 2019.     
Distributed under the Boost Software License, Version 1.0.  
(See accompanying file LICENSE_1_0.txt or copy at  
[https://www.boost.org/LICENSE_1_0.txt](https://www.boost.org/LICENSE_1_0.txt))*
  
 