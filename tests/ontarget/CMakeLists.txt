# ── On-Target Tests ──────────────────────────────────────────────────────────────────────────────
#
# Registers CTest tests that flash and execute the cross-compiled GTest binary on the target MCU
# via SEGGER J-Run. The test executable (${PROJECT_NAME}-test) is built by the normal src/ build
# infrastructure using the ARM cross-compiler specified by the toolchain file.
#
# Prerequisites:
#   - ARM GNU Embedded Toolchain (arm-none-eabi-gcc / arm-none-eabi-g++) on PATH.
#   - SEGGER J-Link software with the JRun utility on PATH.
#   - META_JRUN_DEVICE cache variable set to the target MCU (e.g. STM32F407VG).

include(meta_jrun)
meta_jrun(
    TARGET ${PROJECT_NAME}-test
    DEVICE "${META_JRUN_DEVICE}"
)
