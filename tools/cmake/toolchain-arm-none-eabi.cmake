# SPDX-License-Identifier: Apache-2.0

# Description:
#   CMake toolchain file for cross-compiling with the ARM GNU Embedded Toolchain
#   (arm-none-eabi-gcc / arm-none-eabi-g++). Configures the compiler, linker, and
#   system root for bare-metal Cortex-M targets.
#
#   Cache variables that callers may override:
#     META_MCU_FLAGS   - Semicolon-separated list of CPU / FPU flags
#                        (default: -mcpu=cortex-m4;-mthumb)
#     META_LINKER_SCRIPT - Path to a device-specific linker script (optional)
#
# Usage:
#   cmake -DCMAKE_TOOLCHAIN_FILE=tools/cmake/toolchain-arm-none-eabi.cmake ..

cmake_minimum_required(VERSION 3.28)

# ── Target system description ────────────────────────────────────────────────────────────────────

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# ── Toolchain executables ────────────────────────────────────────────────────────────────────────

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(CMAKE_SIZE arm-none-eabi-size)

# ── Search path configuration ────────────────────────────────────────────────────────────────────

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# ── Target MCU flags ─────────────────────────────────────────────────────────────────────────────

set(META_MCU_FLAGS "-mcpu=cortex-m4;-mthumb" CACHE STRING "CPU / FPU flags for the target MCU")

# ── Compiler / linker flags ──────────────────────────────────────────────────────────────────────

string(REPLACE ";" " " meta_mcu_flags_str "${META_MCU_FLAGS}")

set(CMAKE_C_FLAGS_INIT "${meta_mcu_flags_str} -ffunction-sections -fdata-sections -specs=nosys.specs -specs=nano.specs")
set(CMAKE_CXX_FLAGS_INIT "${meta_mcu_flags_str} -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -specs=nosys.specs -specs=nano.specs")
set(CMAKE_ASM_FLAGS_INIT "${meta_mcu_flags_str}")

set(CMAKE_EXE_LINKER_FLAGS_INIT "${meta_mcu_flags_str} -Wl,--gc-sections -specs=nosys.specs -specs=nano.specs")

# Optional linker script
set(META_LINKER_SCRIPT "" CACHE FILEPATH "Path to a device-specific linker script")
if(META_LINKER_SCRIPT)
    set(CMAKE_EXE_LINKER_FLAGS_INIT "${CMAKE_EXE_LINKER_FLAGS_INIT} -T${META_LINKER_SCRIPT}")
endif()

# ── Disable compiler tests for bare-metal ────────────────────────────────────────────────────────

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
