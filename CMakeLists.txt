cmake_minimum_required(VERSION 3.15)

set(PICO_SDK_FETCH_FROM_GIT on)
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

project(Rp2040_freertos)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 20)
set(OPENOCD_PATH "/home/calle/src/openocd" CACHE STRING "Where to find openocd")
set(OPENOCD_INTERFACE "cmsis-dap" CACHE STRING "Interface to use ")

include(mcu_command_targets)
pico_sdk_init()



set(WARNINGS_AS_ERRORS false)
include("cmake/StandardProjectSettings.cmake")

#set(ENABLE_CLANG_TIDY ON)
#include("cmake/StaticAnalyzers.cmake")

include_directories(

)

add_subdirectory(src)
