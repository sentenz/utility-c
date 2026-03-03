include_guard(GLOBAL)

# Description:
#   Registers Unity-based on-target unit tests with CTest using SEGGER J-Run.
#   Each test suite (or the whole binary) is registered as a separate CTest test
#   that invokes J-Run to flash and execute the firmware on the connected embedded target.
#
# Arguments:
#   Options
#     WITH_RTT         - Use SEGGER RTT for test output communication (default if no output method specified).
#     WITH_SEMIHOSTING - Use semihosting for test output communication.
#   One-Value
#     TARGET           - Required: CMake target name (executable) to flash and test.
#     ENABLE           - Optional: Boolean flag to enable/disable registration (default: ON).
#     DEVICE           - Required: Target device name for J-Run (e.g., LPC55S16).
#     INTERFACE        - Optional: Debug interface type (SWD or JTAG, default: SWD).
#     SPEED            - Optional: Interface speed in kHz (default: 4000).
#     TIMEOUT          - Optional: CTest timeout in seconds per test case (default: 60).
#   Multi-Value
#     SUITES           - Optional: Unity test suite names to register as individual CTest tests.
#                        Each suite is passed to the firmware via J-Run --args for test filtering.
#                        If omitted, a single CTest test is registered for the entire binary.
#
# Outputs:
#   NONE
#
# Usage:
#   meta_unity([WITH_RTT] [WITH_SEMIHOSTING]
#             TARGET <name>
#             [ENABLE <bool>]
#             DEVICE <device>
#             [INTERFACE <SWD|JTAG>]
#             [SPEED <kHz>]
#             [TIMEOUT <seconds>]
#             [SUITES <suite>...])
#
# Example:
#   meta_unity(WITH_RTT
#             TARGET my_firmware
#             DEVICE LPC55S16
#             INTERFACE SWD
#             SPEED 4000
#             TIMEOUT 60
#             SUITES test_leds test_sensors)
function(meta_unity)
    set(options WITH_RTT WITH_SEMIHOSTING)
    set(one_value_args TARGET ENABLE DEVICE INTERFACE SPEED TIMEOUT)
    set(multi_value_args SUITES)
    cmake_parse_arguments(PARSE_ARGV 0 ARG "${options}" "${one_value_args}" "${multi_value_args}")

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

    if(ARG_WITH_RTT AND ARG_WITH_SEMIHOSTING)
        message(FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: 'WITH_RTT' and 'WITH_SEMIHOSTING' are mutually exclusive.")
    endif()

    # Ensure testing is enabled at the top-level so CTest can run registered tests
    if(NOT BUILD_TESTING)
        message(WARNING "${CMAKE_CURRENT_FUNCTION}: 'BUILD_TESTING' must be enabled e.g., include(CTest).")
    endif()

    # Find J-Run executable; result is cached so repeated calls reuse the detected path
    find_program(meta_unity_jrun_exe NAMES JRun jrun)
    if(NOT meta_unity_jrun_exe)
        message(WARNING "${CMAKE_CURRENT_FUNCTION}: 'JRun' not found. On-target Unity tests will be disabled.")
        return()
    endif()

    # Apply defaults
    if(NOT ARG_INTERFACE)
        set(ARG_INTERFACE "SWD")
    endif()

    if(NOT ARG_SPEED)
        set(ARG_SPEED 4000)
    endif()

    if(NOT ARG_TIMEOUT)
        set(ARG_TIMEOUT 60)
    endif()

    # Build base J-Run arguments
    set(meta_jrun_args
        --device "${ARG_DEVICE}"
        --if "${ARG_INTERFACE}"
        --speed "${ARG_SPEED}"
    )

    # Add output method flag; RTT is the default when neither option is given
    if(ARG_WITH_SEMIHOSTING)
        list(APPEND meta_jrun_args --semihosting)
    else()
        list(APPEND meta_jrun_args --rtt)
    endif()

    # Resolve ELF path from the build target via a generator expression
    set(meta_elf_file "$<TARGET_FILE:${ARG_TARGET}>")

    if(ARG_SUITES)
        # Register each suite as a separate CTest test; the suite name is forwarded to
        # the firmware via J-Run --args so Unity can select and run only that suite.
        # The firmware must pass argc/argv from main() to a Unity runner that checks
        # argv[1] and calls the matching suite function (e.g., RunTests_<suite>()).
        foreach(meta_suite IN LISTS ARG_SUITES)
            add_test(
                NAME "${ARG_TARGET}.${meta_suite}"
                COMMAND "${meta_unity_jrun_exe}" ${meta_jrun_args} --args "${meta_suite}" "${meta_elf_file}"
            )
            set_tests_properties("${ARG_TARGET}.${meta_suite}" PROPERTIES TIMEOUT "${ARG_TIMEOUT}")
        endforeach()
    else()
        # Register a single CTest test for the whole target binary
        add_test(
            NAME "${ARG_TARGET}"
            COMMAND "${meta_unity_jrun_exe}" ${meta_jrun_args} "${meta_elf_file}"
        )
        set_tests_properties("${ARG_TARGET}" PROPERTIES TIMEOUT "${ARG_TIMEOUT}")
    endif()
endfunction()
