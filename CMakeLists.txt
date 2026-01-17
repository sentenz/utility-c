cmake_minimum_required(VERSION 3.21.0)

project(${META_PROJECT_NAME} LANGUAGES C CXX)

include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/tools/cmake")

include(meta_policy)
meta_policy()

# Set default for META_ENABLE_COMPILER_CACHE if not provided by preset
if(NOT DEFINED META_ENABLE_COMPILER_CACHE)
  set(META_ENABLE_COMPILER_CACHE OFF)
endif()

include(meta_compiler_cache)
meta_compiler_cache(ENABLE ${META_ENABLE_COMPILER_CACHE})

# Utility-c library options
option(UTILITYC_BUILD_LIB "Build the library" ON)
option(UTILITYC_BUILD_TEST "Build the tests" ON)
option(UTILITYC_BUILD_TEST_COVERAGE "Build the test coverage" ON)

if(PROJECT_IS_TOP_LEVEL)
  # Export compile commands for clang-tidy
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  set(CMAKE_VERBOSE_MAKEFILE ON)

  # Set language standards
  set(CMAKE_C_STANDARD 99)
  set(CMAKE_CXX_STANDARD 11)
endif()

# Build library
if(UTILITYC_BUILD_LIB)
  # Create library
  add_library(${PROJECT_NAME})

  target_compile_definitions(${PROJECT_NAME} PRIVATE S_HAVE_CONFIG_H S_POSIX
                                                     HAVE_CONFIG_H)

  # Include directories
  target_include_directories(
    ${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}"
                           "${PROJECT_SOURCE_DIR}/include")

  # Find dependencies (vcpkg or system)
  find_package(uriparser CONFIG REQUIRED)
  find_package(unofficial-libargon2 CONFIG REQUIRED)
  find_package(unofficial-pcre CONFIG REQUIRED)

  # Define static libraries for linking
  set(static_libs
      ${static_libs} external::libmxml unofficial::argon2::libargon2
      unofficial::pcre::pcre uriparser::uriparser)

  # Link the static libraries to this target
  target_link_libraries(${PROJECT_NAME} PUBLIC m ${static_libs})
endif() # UTILITYC_BUILD_LIB

# Build tests - meta_gtest will create the test target in subdirectories
if(UTILITYC_BUILD_TEST)
  # Find GTest package required by meta_gtest
  find_package(GTest CONFIG REQUIRED)
  
  # Enable testing for CTest integration
  include(CTest)
  enable_testing()
endif() # UTILITYC_BUILD_TEST

# Generate Test code coverage
if(UTILITYC_BUILD_TEST_COVERAGE)
  include(code_coverage)
  # Add compiler flags to generate coverage .gcda files
  append_coverage_compiler_flags()

  # Create Target to generate coverage report
  setup_target_for_coverage_lcov(
    NAME
    ${PROJECT_NAME}-coverage
    EXECUTABLE
    ${PROJECT_NAME}-test
    -j
    ${PROCESSOR_COUNT}
    DEPENDENCIES
    ${PROJECT_NAME}-test
    BASE_DIRECTORY
    "${PROJECT_SOURCE_DIR}"
    EXCLUDE
    "*_test.cc"
    "/root/*"
    "/usr/*"
    "${PROJECT_SOURCE_DIR}/build/*"
    "${PROJECT_SOURCE_DIR}/external/*"
    NO_DEMANGLE)

  # Copy Coverage Report to logs/ directory
  add_custom_command(
    TARGET ${PROJECT_NAME}-coverage
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy_directory
      ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-coverage
      "${PROJECT_SOURCE_DIR}/logs/coverage")
endif() # UTILITYC_BUILD_TEST_COVERAGE

# Add source directories
add_subdirectory(external)
add_subdirectory(src)
