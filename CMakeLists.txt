cmake_minimum_required(VERSION 3.15)

set(PICO_SDK_FETCH_FROM_GIT on)
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)
project ( test1 )


# Link Time Optimization (LTO)
include("cmake/StandardProjectSettings.cmake")

set(WARNINGS_AS_ERRORS False)
# Compiler warnings
include("cmake/CompilerWarnings.cmake")
include("cmake/Sanitizers.cmake")
include("cmake/mcu_command_targets.cmake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra ")


set(OPENOCD_PATH "/home/calle/src/openocd" CACHE STRING "Where to find openocd")
set(OPENOCD_INTERFACE "cmsis-dap" CACHE STRING "Interface to use ")

pico_sdk_init()

add_library(project_options INTERFACE)
#target_compile_features(project_options INTERFACE cxx_std_17)

set(ENABLE_CLANG_TIDY OFF)
include("cmake/StaticAnalyzers.cmake")

add_library(project_warnings INTERFACE)
set_project_warnings( project_warnings )

#add_library(project_sanitizer INTERFACE)
#enable_sanitizers(project_sanitizer)




include_directories(
include
)

add_subdirectory(src)

#enable_testing()
#add_subdirectory(tests)
