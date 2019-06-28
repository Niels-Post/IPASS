cmake_minimum_required(VERSION 3.13)

set(WSL_HOMEDIR "C:/Users/niels/Development/WSL_HWLib/HWLIB-Cmake-Tools")

include(${WSL_HOMEDIR}/HWLib_Target.cmake)


hwlib_load_auto(${CMAKE_CURRENT_SOURCE_DIR}/code HWLIB_TARGET_blue_pill blue_pill)

add_hwlib_target(tests ${CMAKE_CURRENT_SOURCE_DIR}/tests HWLIB_TARGET_native native)
target_include_directories(tests
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/code/base
        ${CMAKE_CURRENT_SOURCE_DIR}/code/base/mesh
        ${CMAKE_CURRENT_SOURCE_DIR}/code/base/nrf24l01
        ${CMAKE_CURRENT_SOURCE_DIR}/code/base/spi)
