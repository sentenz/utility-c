include_guard(GLOBAL)

# Description:
#   Find multiple CMake packages at once with specified configurations.
#
# Arguments:
#   Multi-value
#     PACKAGES - List of package names to find (e.g., "GTest", "uriparser")
#   Options
#     CONFIG   - Use CONFIG mode for all packages (default: OFF)
#     REQUIRED - Mark all packages as REQUIRED (default: OFF)
#
# Outputs:
#   NONE
#
# Usage:
#   meta_find_packages(PACKAGES <pkg1> <pkg2> ... [CONFIG] [REQUIRED])
#
# Example:
#   meta_find_packages(PACKAGES GTest Boost REQUIRED CONFIG)
#   meta_find_packages(PACKAGES uriparser unofficial-libargon2 CONFIG REQUIRED)
function(meta_find_packages)
    set(options CONFIG REQUIRED)
    set(multi_value_args PACKAGES)
    cmake_parse_arguments(PARSE_ARGV 0 ARG "${options}" "" "${multi_value_args}")

    if(DEFINED ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: Unknown arguments: ${ARG_UNPARSED_ARGUMENTS}.")
    endif()

    if(NOT ARG_PACKAGES)
        message(FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: 'PACKAGES' argument is required.")
    endif()

    # Build the find_package arguments
    set(find_args "")
    if(ARG_CONFIG)
        list(APPEND find_args CONFIG)
    endif()
    if(ARG_REQUIRED)
        list(APPEND find_args REQUIRED)
    endif()

    # Find each package with the specified arguments
    foreach(package IN LISTS ARG_PACKAGES)
        find_package(${package} ${find_args})
    endforeach()
endfunction()
