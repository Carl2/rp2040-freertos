###############################################################################
#                             Make a flash target                             #
#   This will make a flash target fot the executable target
###############################################################################
function(make_mcu_cmd_target)



  cmake_parse_arguments(TARGET "" "NAME" "" ${ARGN})

  add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND arm-none-eabi-objcopy  -O ihex ${TARGET_NAME} ${TARGET_NAME}.hex
    VERBATIM
  )

  get_target_property(BIN_DIR ${TARGET_NAME} BINARY_DIR)

  add_custom_target(flash
    COMMAND bash -c "st-flash --format=ihex write ${BIN_DIR}/${TARGET_NAME}.hex"
    VERBATIM
    )





endfunction()


function(make_openocd_target)
  cmake_parse_arguments(TARGET "" "NAME;OPENOCD_PATH;OPENOCD_INTERFACE;OPENOCD_TARGET" "" ${ARGN})

  get_target_property(BIN_DIR ${TARGET_NAME} BINARY_DIR)

  message(STATUS "${CMAKE_SOURCE_DIR}/tools/openocd/run_openocd.sh \
-o ${TARGET_OPENOCD_PATH} \
-i ${TARGET_OPENOCD_INTERFACE} \
-t ${TARGET_OPENOCD_TARGET} \
-e ${BIN_DIR}/${TARGET_NAME}")

  add_custom_target(ocd_flash
    COMMAND bash -c "${CMAKE_SOURCE_DIR}/tools/openocd/run_openocd.sh -o ${TARGET_OPENOCD_PATH} \
-i ${TARGET_OPENOCD_INTERFACE} \
-t ${TARGET_OPENOCD_TARGET} \
-e ${BIN_DIR}/${TARGET_NAME}"
    VERBATIM
  )

  add_custom_target(debug
    COMMAND bash -c "${CMAKE_SOURCE_DIR}/tools/openocd/run_openocd.sh -o ${TARGET_OPENOCD_PATH} \
-i ${TARGET_OPENOCD_INTERFACE} \
-t ${TARGET_OPENOCD_TARGET}"
    VERBATIM
  )

  #############################################################################
  #                            Use a big terminal!                            #
  #############################################################################

    add_custom_target(run_debug
      COMMAND bash -c "${CMAKE_SOURCE_DIR}/tools/openocd/run_debug.sh \
${CMAKE_SOURCE_DIR}/tools/openocd/run_openocd.sh \
${TARGET_OPENOCD_PATH} \
${TARGET_OPENOCD_INTERFACE} \
${TARGET_OPENOCD_TARGET} \
${BIN_DIR}/${TARGET_NAME} \
${CMAKE_SOURCE_DIR}/tools/gdb/ocd_debug.gdb"
 VERBATIM
)

###############################################################################
#                                    GDBgui                                   #
###############################################################################
  add_custom_target(gui_debug
    COMMAND bash -c "${CMAKE_SOURCE_DIR}/tools/openocd/gdbgui.sh \
${CMAKE_SOURCE_DIR}/tools/openocd/run_openocd.sh \
${TARGET_OPENOCD_PATH} \
${TARGET_OPENOCD_INTERFACE} \
${TARGET_OPENOCD_TARGET} \
${BIN_DIR}/${TARGET_NAME} \
${CMAKE_SOURCE_DIR}/tools/gdb/gdbgui.gdb \
${CMAKE_SOURCE_DIR}"
 VERBATIM
)

endfunction(make_openocd_target)
