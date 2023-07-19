cmake_minimum_required(VERSION 3.15)

set(TARGET_NAME "Test1")
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(${CMAKE_SOURCE_DIR}/cmake/freertos.cmake)

freertos_support(
 DESTINATION "${CMAKE_SOURCE_DIR}/external/"
 )


###########
# Sources that compiles
###########
set( SRC main.cpp)



###########
# Add exectutable
###########
add_executable(${TARGET_NAME} ${SRC} )


target_include_directories(${TARGET_NAME}
   PRIVATE
   ${CMAKE_SOURCE_DIR}/external/FreeRtos/include
   ${CMAKE_SOURCE_DIR}/external/FreeRtos/portable/GCC/ARM_CM0
   ${CMAKE_SOURCE_DIR}/config

 )



###########
# Library linking
###########

pico_add_extra_outputs(${TARGET_NAME})

#example_auto_set_url(hello_serial)
# stdio via USB
pico_enable_stdio_usb(${TARGET_NAME} 1)
pico_enable_stdio_uart(${TARGET_NAME} 1)

target_link_libraries(${TARGET_NAME}
PRIVATE
#project_options
#project_warnings
pico_stdlib
#freertos_kernel
#freertos_kernel_port
)

make_openocd_target(
  NAME ${TARGET_NAME}
  OPENOCD_PATH ${OPENOCD_PATH}
  OPENOCD_INTERFACE ${OPENOCD_INTERFACE}
  OPENOCD_TARGET rp2040
  )
