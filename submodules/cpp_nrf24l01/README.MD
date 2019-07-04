C++ NRF24L01+ Library
============================

Bare-metal library for using the NRF24L01+ tranceiver on microcontrollers. 

Code standard: C++17.

This library is part of my IPASS end project, for the first year of my HBO-ICT study at Hogeschool Utrecht in 2019.


Features 
---
- All registers addresses and register bitmasks in a definition file
- Methods for the most used functions
- Setters for RX pipe attributes per pipe, as well as for all receive pipes at once
- Supports Auto_Acknowledge, Dynamic Payload Width, and NOACK transactions


Dependencies
-----
For SPI communication with the module, this library requires [cpp_spi](https://github.com/Niels-Post/cpp_spi) 

For pin-level abstraction, [hwlib](https://github.com/wovo/hwlib) is used.

Since this library was originally used for an embedded project, the included makefile is written for [BMPTK](http://github.com/wovo/bmptk).
However, the library can also be compiled without BMPTK, for which instructions are below.


Installation/usage
-----
- Download the library `git clone https://github.com/Niels-Post/cpp_nrf24l01`
- Include *Makefile.inc* from your project
- Include `nrf24l01/nrf24l01plus.hpp`

Building with BMPTK
----
- Just Include *Makefile.inc* :)


Building without BMPTK
----
The included makefile only sets some variables, but doesn't do any compiling itself. When building without BMPTK, some variables should be interpreted properly after including.
- HEADERS: all .hpp header files
- SEARCH: the include path for header files of this library 

The header files can also be dropped into your own project, make sure to adjust their include directives though.


License Information
---
   
*Copyright Niels Post 2019.     
Distributed under the Boost Software License, Version 1.0.  
(See accompanying file LICENSE_1_0.txt or copy at  
[https://www.boost.org/LICENSE_1_0.txt](https://www.boost.org/LICENSE_1_0.txt))*
  
 