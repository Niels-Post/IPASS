cmake_minimum_required(VERSION 3.13)

set(WSL_HOMEDIR "L:/HWLib/HWLIB_Cmake_Tools")

include(L:/HWLib/HWLIB_Cmake_Tools/HWLIB_Target.cmake)

#hwlib_load_auto(${CMAKE_CURRENT_SOURCE_DIR}/Week_1)
#hwlib_load_auto(${CMAKE_CURRENT_SOURCE_DIR}/DIT HWLIB_TARGET_arduino_due arduino_due /dev/ttyS3)
#hwlib_load_auto(${CMAKE_CURRENT_SOURCE_DIR}/Week_2 HWLIB_TARGET_native native)
#hwlib_load_auto(${CMAKE_CURRENT_SOURCE_DIR}/Week_3 HWLIB_TARGET_native native)
#hwlib_load_auto(${CMAKE_CURRENT_SOURCE_DIR}/Week_4 HWLIB_TARGET_arduino_due arduino_due /dev/ttyS3)
#hwlib_load_auto(${CMAKE_CURRENT_SOURCE_DIR}/Week_5 HWLIB_TARGET_arduino_due arduino_due /dev/ttyS3)
#
#hwlib_load_auto(${CMAKE_CURRENT_SOURCE_DIR}/IPASS HWLIB_TARGET_arduino_due arduino_due /dev/ttyS3)
hwlib_load_auto(${CMAKE_CURRENT_SOURCE_DIR}/Eindopdracht_OOPC HWLIB_TARGET_my_first_dev_board my_first_dev_board /dev/ttyS3)
hwlib_load_auto(${CMAKE_CURRENT_SOURCE_DIR}/IPASS HWLIB_TARGET_blue_pill blue_pill)

add_executable(test Oefententamen/code/Opdracht_1.cpp)
add_executable(test2 Oefententamen/code/Opdracht_2.cpp)