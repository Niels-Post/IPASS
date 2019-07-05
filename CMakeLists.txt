cmake_minimum_required(VERSION 3.13)

set(WSL_HOMEDIR "C:/Users/niels/Development/WSL_HWLib/HWLIB-Cmake-Tools")
include(${WSL_HOMEDIR}/HWLib_Target.cmake)

#include(${WSL_HOMEDIR}/New-Version.cmake)

#add_hwlib_bmptk_target(IPASS "${CMAKE_CURRENT_SOURCE_DIR}" Makefile)


add_hwlib_target(tests ${CMAKE_CURRENT_SOURCE_DIR}/tests HWLIB_TARGET_native native)
target_include_directories(tests
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/submodules/i2c_lcd/include
        ${CMAKE_CURRENT_SOURCE_DIR}/submodules/link_state/include
        ${CMAKE_CURRENT_SOURCE_DIR}/submodules/mesh/include
        ${CMAKE_CURRENT_SOURCE_DIR}/submodules/mesh_domotics/include
        ${CMAKE_CURRENT_SOURCE_DIR}/submodules/spi/include
        ${CMAKE_CURRENT_SOURCE_DIR}/submodules/nrf24l01/include
        )