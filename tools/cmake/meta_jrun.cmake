include_guard(GLOBAL)

# Description:
#   Register a CTest test that flashes and executes a GTest binary on a target
#   microcontroller using SEGGER J-Run.
#
#   J-Run is a command-line utility from SEGGER that flashes a firmware image to
#   an MCU through a J-Link debug probe, executes the program, and captures its
#   semihosting output. By combining GTest's textual output protocol with J-Run,
#   test results from the physical hardware can be collected by CTest as if the
#   tests ran on the host.
#
# Arguments:
#   One-Value
#     ENABLE      - Optional: Boolean flag to enable/disable J-Run tests (default: ON).
#     TARGET      - Required: CMake executable target to flash and run.
#     DEVICE      - Required: SEGGER device identifier (e.g. STM32F407VG).
#     INTERFACE   - Optional: Debug interface (default: SWD).
#     SPEED       - Optional: J-Link clock speed in kHz (default: 4000).
#     TIMEOUT     - Optional: CTest timeout in seconds for the run (default: 120).
#
# Outputs:
#   Registers a CTest test named "<TARGET>-ontarget" that runs J-Run.
#
# Usage:
#   meta_jrun(
#       TARGET <name>
#       DEVICE <device>
#       [ENABLE <bool>]
#       [INTERFACE <iface>]
#       [SPEED <kHz>]
#       [TIMEOUT <seconds>]
#   )
#
# Example:
#   meta_jrun(TARGET my_ontarget_test DEVICE STM32F407VG SPEED 4000)
function(meta_jrun)
    set(one_value_args ENABLE TARGET DEVICE INTERFACE SPEED TIMEOUT)
    cmake_parse_arguments(PARSE_ARGV 0 ARG "" "${one_value_args}" "")

    if(DEFINED ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: Unknown arguments: ${ARG_UNPARSED_ARGUMENTS}.")
    endif()

    if(DEFINED ARG_ENABLE AND NOT ARG_ENABLE)
        return()
    endif()

    if(NOT ARG_TARGET)
        message(FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: 'TARGET' argument is required.")
    endif()

    if(NOT ARG_DEVICE)
        message(FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: 'DEVICE' argument is required.")
    endif()

    # Defaults
    if(NOT DEFINED ARG_INTERFACE OR ARG_INTERFACE STREQUAL "")
        set(ARG_INTERFACE "SWD")
    endif()

    if(NOT DEFINED ARG_SPEED OR ARG_SPEED STREQUAL "")
        set(ARG_SPEED "4000")
    endif()

    if(NOT DEFINED ARG_TIMEOUT OR ARG_TIMEOUT STREQUAL "")
        set(ARG_TIMEOUT "120")
    endif()

    # Locate J-Run executable
    find_program(meta_jrun_exe NAMES JRun jrun)
    if(NOT meta_jrun_exe)
        message(WARNING "${CMAKE_CURRENT_FUNCTION}: 'JRun' not found on PATH. On-target tests will be registered but cannot execute without the SEGGER J-Run tool.")
    endif()

    # Register CTest test
    set(meta_test_name "${ARG_TARGET}-ontarget")

    add_test(
        NAME "${meta_test_name}"
        COMMAND
            "${meta_jrun_exe}"
            --device "${ARG_DEVICE}"
            --if "${ARG_INTERFACE}"
            --speed "${ARG_SPEED}"
            "$<TARGET_FILE:${ARG_TARGET}>"
    )

    set_tests_properties("${meta_test_name}" PROPERTIES
        TIMEOUT "${ARG_TIMEOUT}"
        LABELS "ontarget"
    )
endfunction()
