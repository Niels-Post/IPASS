cmake_minimum_required(VERSION 3.13)

set(WSL_HOMEDIR "C:/Users/Niels/Development/WSL_HWLib/HWLIB-Cmake-Tools")

include(${WSL_HOMEDIR}/HWLib_Target.cmake)


hwlib_load_auto(${CMAKE_CURRENT_SOURCE_DIR}/code HWLIB_TARGET_blue_pill blue_pill)
