# SPDX-License-Identifier: Apache-2.0

# ARM Cortex-M bare-metal cross-compilation toolchain.
#
# Targets : Cortex-M0/M0+/M3/M4/M4F/M7 and compatible ARM bare-metal devices.
# Requires: arm-none-eabi-gcc toolchain present on PATH or under CMAKE_SYSROOT.
#
# Overridable cache variables (set via -D on the CMake command line or preset):
#   META_TARGET_CPU        - ARM CPU identifier (default: cortex-m4)
#   META_TARGET_FPU        - FPU identifier      (default: fpv4-sp-d16)
#   META_TARGET_FLOAT_ABI  - Float ABI           (default: hard)

set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Let CMake probe the compiler via a static library to avoid linking issues
# with an incomplete sysroot during the try_compile phase.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# ── Toolchain executables ────────────────────────────────────────────────────

find_program(meta_arm_c_compiler   NAMES arm-none-eabi-gcc   DOC "ARM C compiler")
find_program(meta_arm_cxx_compiler NAMES arm-none-eabi-g++   DOC "ARM C++ compiler")
find_program(meta_arm_asm_compiler NAMES arm-none-eabi-gcc   DOC "ARM assembler (gcc front-end)")

if(meta_arm_c_compiler)
    set(CMAKE_C_COMPILER   "${meta_arm_c_compiler}"   CACHE FILEPATH "ARM C compiler"   FORCE)
endif()

if(meta_arm_cxx_compiler)
    set(CMAKE_CXX_COMPILER "${meta_arm_cxx_compiler}" CACHE FILEPATH "ARM C++ compiler" FORCE)
endif()

if(meta_arm_asm_compiler)
    set(CMAKE_ASM_COMPILER "${meta_arm_asm_compiler}" CACHE FILEPATH "ARM assembler"    FORCE)
endif()

set(CMAKE_AR     arm-none-eabi-ar     CACHE FILEPATH "ARM archiver")
set(CMAKE_RANLIB arm-none-eabi-ranlib CACHE FILEPATH "ARM ranlib")

# ── CPU and FPU configuration ────────────────────────────────────────────────

if(NOT DEFINED META_TARGET_CPU)
    set(META_TARGET_CPU "cortex-m4" CACHE STRING "ARM target CPU (e.g., cortex-m4, cortex-m7)")
endif()

if(NOT DEFINED META_TARGET_FPU)
    set(META_TARGET_FPU "fpv4-sp-d16" CACHE STRING "ARM FPU (e.g., fpv4-sp-d16, fpv5-d16)")
endif()

if(NOT DEFINED META_TARGET_FLOAT_ABI)
    set(META_TARGET_FLOAT_ABI "hard" CACHE STRING "ARM float ABI: soft, softfp, or hard")
endif()

# ── Compiler flags ───────────────────────────────────────────────────────────

set(meta_arm_cpu_flags
    "-mcpu=${META_TARGET_CPU}"
    "-mthumb"
    "-mfpu=${META_TARGET_FPU}"
    "-mfloat-abi=${META_TARGET_FLOAT_ABI}"
)
list(JOIN meta_arm_cpu_flags " " meta_arm_cpu_flags_str)

set(CMAKE_C_FLAGS_INIT          "${meta_arm_cpu_flags_str}" CACHE STRING "ARM C flags")
set(CMAKE_CXX_FLAGS_INIT        "${meta_arm_cpu_flags_str}" CACHE STRING "ARM C++ flags")
set(CMAKE_ASM_FLAGS_INIT        "${meta_arm_cpu_flags_str}" CACHE STRING "ARM ASM flags")

# Use semihosting so that GTest output (printf) is forwarded over the
# ARM debug interface (via J-Link / J-Run) to the host console.
set(CMAKE_EXE_LINKER_FLAGS_INIT "-specs=rdimon.specs -lrdimon" CACHE STRING "ARM linker flags")

# ── Search path restrictions ─────────────────────────────────────────────────
# Restrict CMake's search paths so that host libraries are never picked up
# when cross-compiling.

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
