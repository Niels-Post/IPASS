cmake_minimum_required(VERSION 3.13)

set(WSL_HOMEDIR "L:/HWLib/HWLIB-Cmake-Tools")


include(${WSL_HOMEDIR}/New-Version.cmake)

add_hwlib_bmptk_target(IPASS "${CMAKE_CURRENT_SOURCE_DIR}" Makefile)
