include_guard(GLOBAL)

# Description:
#   Set global project properties for languages (C/C++ Standard) and compiler warnings.
#
# Arguments:
#   NONE
#
# Outputs:
#   NONE
#
# Usage:
#   meta_policy()
#
# Example:
#   meta_policy()
function(meta_policy)
    # Global Language Properties for C
    set(CMAKE_C_STANDARD 99 CACHE STRING "C standard to be used")
    set(CMAKE_C_STANDARD_REQUIRED ON CACHE BOOL "Require C standard")
    set(CMAKE_C_EXTENSIONS OFF CACHE BOOL "Disable C extensions")

    # Global Language Properties for C++
    set(CMAKE_CXX_STANDARD 11 CACHE STRING "C++ standard to be used")
    set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "Require C++ standard")
    set(CMAKE_CXX_EXTENSIONS OFF CACHE BOOL "Disable C++ extensions")

    # Global Compiler Warnings
    if(MSVC)
        add_compile_options(/W4 /permissive-)
    else()
        set(meta_warning_flags -Wall -Wextra -Wpedantic)
        foreach(warning_flag IN LISTS meta_warning_flags)
            add_compile_options(
                $<$<COMPILE_LANG_AND_ID:C,GNU>:${warning_flag}>
                $<$<COMPILE_LANG_AND_ID:C,Clang>:${warning_flag}>
                $<$<COMPILE_LANG_AND_ID:C,AppleClang>:${warning_flag}>
                $<$<COMPILE_LANG_AND_ID:CXX,GNU>:${warning_flag}>
                $<$<COMPILE_LANG_AND_ID:CXX,Clang>:${warning_flag}>
                $<$<COMPILE_LANG_AND_ID:CXX,AppleClang>:${warning_flag}>
            )
        endforeach()
    endif()
endfunction()
