include_guard(GLOBAL)

# Description:
#   Registers a GTest-based executable as a CTest test executed on a physical
#   target via SEGGER J-Run.  J-Run flashes the cross-compiled ELF binary
#   through a J-Link probe and captures the test output (printf / semihosting).
#   The entire GTest binary is registered as a single CTest test to minimise
#   flash/run cycles on the hardware.
#
# Arguments:
#   Options
#     ENABLE      - Optional: Boolean flag to enable/disable J-Run (default: ON).
#   One-Value
#     TARGET      - Required: CMake executable target to register with CTest.
#     DEVICE      - Required: SEGGER device name (e.g., "STM32F407VG").
#     INTERFACE   - Optional: J-Link connection interface (default: SWD).
#     SPEED       - Optional: J-Link interface speed in kHz (default: auto).
#
# Outputs:
#   Registers a CTest test that invokes JRun to flash and run the ELF on the
#   target, and configures GTEST_OUTPUT so results are saved as JUnit XML.
#
# Usage:
#   meta_jrun([ENABLE <bool>]
#             TARGET <target>
#             DEVICE <device>
#             [INTERFACE <interface>]
#             [SPEED <speed>])
#
# Example:
#   meta_jrun(TARGET my_tests DEVICE "STM32F407VG" INTERFACE SWD SPEED auto)
function(meta_jrun)
    set(options)
    set(one_value_args ENABLE TARGET DEVICE INTERFACE SPEED)
    set(multi_value_args)
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

    # Apply defaults
    if(NOT ARG_INTERFACE)
        set(ARG_INTERFACE "SWD")
    endif()

    if(NOT ARG_SPEED)
        set(ARG_SPEED "auto")
    endif()

    # Locate JRun on PATH or in common SEGGER J-Link install locations
    find_program(
        meta_jrun_exe
        NAMES JRun JRun.exe
        PATHS
            "$ENV{SEGGER_PATH}/JLink"
            "/opt/SEGGER/JLink"
            "/usr/bin"
            "C:/Program Files/SEGGER/JLink"
            "C:/Program Files (x86)/SEGGER/JLink"
        DOC "Path to the SEGGER JRun executable"
    )

    if(NOT meta_jrun_exe)
        message(WARNING
            "${CMAKE_CURRENT_FUNCTION}: JRun not found. "
            "On-target tests will not be registered with CTest. "
            "Install SEGGER J-Link or set the SEGGER_PATH environment variable."
        )
        return()
    endif()

    message(STATUS "${CMAKE_CURRENT_FUNCTION}: Found JRun at ${meta_jrun_exe}")

    # Verify the target exists
    if(NOT TARGET "${ARG_TARGET}")
        message(FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: Target '${ARG_TARGET}' does not exist.")
    endif()

    # Warn when testing is not properly enabled
    if(NOT BUILD_TESTING)
        message(WARNING "${CMAKE_CURRENT_FUNCTION}: 'BUILD_TESTING' is not set; include(CTest) first.")
    endif()

    # Register the GTest binary as a single CTest entry executed via J-Run.
    # Running the whole binary as one test avoids the flash overhead that would
    # occur if each GTest case were registered as an individual CTest test.
    add_test(
        NAME "${ARG_TARGET}"
        COMMAND
            "${meta_jrun_exe}"
            --device   "${ARG_DEVICE}"
            --if       "${ARG_INTERFACE}"
            --speed    "${ARG_SPEED}"
            $<TARGET_FILE:${ARG_TARGET}>
    )

    # Direct GTest to write JUnit-compatible XML alongside the other test logs
    set_tests_properties("${ARG_TARGET}" PROPERTIES
        ENVIRONMENT
            "GTEST_OUTPUT=xml:${CMAKE_BINARY_DIR}/test-results/${ARG_TARGET}.xml"
    )

    message(STATUS
        "${CMAKE_CURRENT_FUNCTION}: Registered CTest test '${ARG_TARGET}' "
        "for device '${ARG_DEVICE}' via J-Run."
    )
endfunction()
