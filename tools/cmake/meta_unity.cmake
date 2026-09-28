include_guard(GLOBAL)

# Description:
#   Creates or extends a Unity-based embedded test executable.
#   Manages the build side: creates an add_executable target for the firmware
#   test binary and links any required libraries. The caller's toolchain file
#   controls the cross-compiler; meta_unity only handles source and link
#   configuration.
#
# Arguments:
#   One-Value
#     TARGET  - Required: CMake target name for the test executable.
#     ENABLE  - Optional: Boolean flag to enable/disable (default: ON).
#   Multi-Value
#     SOURCES - Optional: Source files to add to the test executable.
#               Intentionally optional so multiple calls can extend the same
#               target incrementally.
#     LINK    - Optional: Libraries to link into the test executable.
#
# Outputs:
#   NONE
#
# Usage:
#   meta_unity(TARGET <name>
#              [ENABLE <bool>]
#              [SOURCES <src>...]
#              [LINK <lib>...])
#
# Example:
#   meta_unity(
#     TARGET   my_firmware-test
#     SOURCES  test_leds.c test_sensors.c
#     LINK     my_firmware::leds)
function(meta_unity)
    set(options)
    set(one_value_args TARGET ENABLE)
    set(multi_value_args SOURCES LINK)
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

    # Create the executable once; subsequent calls with the same TARGET name
    # skip add_executable and only append sources and link libraries
    if(NOT TARGET "${ARG_TARGET}")
        add_executable("${ARG_TARGET}")
    endif()

    # Any SOURCES passed in this or subsequent calls are appended incrementally
    if(ARG_SOURCES)
        target_sources("${ARG_TARGET}" PRIVATE ${ARG_SOURCES})
    endif()

    # Propagate any extra link libs passed via LINK
    if(ARG_LINK)
        target_link_libraries("${ARG_TARGET}" PRIVATE ${ARG_LINK})
    endif()
endfunction()
