cmake_minimum_required(VERSION 3.13)

set(WSL_HOMEDIR "L:/HWLib/HWLIB_Cmake_Tools")

include(${WSL_HOMEDIR}/HWLib_Target.cmake)


hwlib_load_auto(${CMAKE_CURRENT_SOURCE_DIR}/code HWLIB_TARGET_blue_pill blue_pill)
