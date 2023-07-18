#!/usr/bin/env bash

#tmux new-session -s Debug \; select-layout tiled \; split-window -h  "echo \"Hello\" " \; split-window -h  "echo \"World\" "
#/home/calle/git/PH_WIP/tools/openocd/run_openocd.sh -o /home/calle/src/openocd -i picoprobe -t stm32f1x -e /home/calle/git/PH_WIP/build/src/FSSPH/FSSPH

RUN_EXEC_CMD=$1
TARGET_OPENOCD_PATH=$2
TARGET_OPENOCD_INTERFACE=$3
TARGET_OPENOCD_TARGET=$4
TARGET_BIN=$5
GDB_CFG=$6
PRJ_DIR=$7

#gdbgui -g "arm-none-eabi-gdb -se /home/calle/git/PH_WIP/build/src/FSSPH/FSSPH -x /home/calle/git/PH_WIP/tools/gdb/gdbgui.gdb"
cmd="gdbgui -g \"arm-none-eabi-gdb -se ${TARGET_BIN} -x ${GDB_CFG}\" --project ${PRJ_DIR}"

echo "${cmd}"

tmux new-session \
"${RUN_EXEC_CMD} -o \
 ${TARGET_OPENOCD_PATH} -i\
 ${TARGET_OPENOCD_INTERFACE} -t \
 ${TARGET_OPENOCD_TARGET}" \; \
 split-window -h -p 50 " ${cmd} " \;
#--args ${TARGET_BIN}"
