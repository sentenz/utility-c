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
#     DEVICE           - Required: Target device name for J-Run (e.g., LPC55S16).
#     ENABLE           - Optional: Boolean flag to enable/disable registration (default: ON).
#     SPEED            - Optional: Interface speed in kHz (default: 4000).
#     TIMEOUT          - Optional: CTest timeout in seconds per test case (default: 60).
#     INTERFACE        - Optional: Debug interface type (SWD or JTAG, default: SWD).
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

    if(ARG_WITH_RTT AND ARG_WITH_SEMIHOSTING)
        message(FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: 'WITH_RTT' and 'WITH_SEMIHOSTING' are mutually exclusive.")
    endif()

    if(NOT ARG_DEVICE)
        message(FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: 'DEVICE' argument is required.")
    endif()

    # Validate that the specified TARGET exists and is an executable
    if(NOT ARG_TARGET)
        message(FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: 'TARGET' argument is required.")
    endif()
    if(NOT TARGET "${ARG_TARGET}")
        message(FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: 'TARGET' must be an existing CMake target, but '${ARG_TARGET}' was not found.")
    endif()
    get_target_property(_meta_unity_target_type "${ARG_TARGET}" TYPE)
    if(NOT _meta_unity_target_type STREQUAL "EXECUTABLE")
        message(FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: 'TARGET' must be an EXECUTABLE target, but '${ARG_TARGET}' is of type '${_meta_unity_target_type}'.")
    endif()

    # Ensure CTest infrastructure is enabled for add_test()
    if(NOT CMAKE_TESTING_ENABLED)
      enable_testing()
    endif()

    # Find J-Run executable
    if(NOT meta_unity_jrun_exe)
        find_program(meta_unity_jrun_exe NAMES JRun jrun)
    endif()
    if(NOT meta_unity_jrun_exe)
        message(WARNING "${CMAKE_CURRENT_FUNCTION}: 'JRun' not found. On-target Unity tests will be disabled.")
        return()
    endif()

    # Apply defaults and validate arguments
    if(NOT ARG_INTERFACE)
        set(ARG_INTERFACE "SWD")
    endif()
    string(TOUPPER "${ARG_INTERFACE}" ARG_INTERFACE)
    set(_meta_allowed_interfaces "SWD" "JTAG")
    if(NOT ARG_INTERFACE IN_LIST _meta_allowed_interfaces)
        message(FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: Invalid argument of 'INTERFACE' with '${ARG_INTERFACE}'. Allowed values are '${_meta_allowed_interfaces}'.")
    endif()

    if(NOT ARG_SPEED)
        set(ARG_SPEED 4000)
    endif()
    if(NOT ARG_SPEED MATCHES "^[1-9][0-9]*$")
        message(FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: 'SPEED' must be a positive integer in kHz, but got '${ARG_SPEED}'.")
    endif()

    if(NOT ARG_TIMEOUT)
        set(ARG_TIMEOUT 60)
    endif()
    if(NOT ARG_TIMEOUT MATCHES "^[1-9][0-9]*$")
        message(FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: 'TIMEOUT' must be a positive integer in seconds, but got '${ARG_TIMEOUT}'.")
    endif()

    # Build base J-Run arguments
    set(meta_jrun_args
        --device "${ARG_DEVICE}"
        --if "${ARG_INTERFACE}"
        --speed "${ARG_SPEED}"
    )

    # Add output method flag, RTT is the default if no method is specified
    if(ARG_WITH_SEMIHOSTING)
        list(APPEND meta_jrun_args --semihosting)
    else()
        list(APPEND meta_jrun_args --rtt)
    endif()

    # Resolve ELF path from the build target via a generator expression
    set(meta_elf_file "$<TARGET_FILE:${ARG_TARGET}>")

    if(ARG_SUITES)
        set(_meta_unity_suites ${ARG_SUITES})
        list(REMOVE_DUPLICATES _meta_unity_suites)
        list(REMOVE_ITEM _meta_unity_suites "")

        # Register each suite as a separate CTest test, the suite name is forwarded to the firmware via J-Run --args so Unity can select and run only that suite
        foreach(meta_suite IN LISTS _meta_unity_suites)
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
