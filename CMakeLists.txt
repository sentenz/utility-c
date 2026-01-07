cmake_minimum_required(VERSION 3.21.0)

# Setup vcpkg script with CMake (note: should be placed before project() call)
set(VCPKG_ROOT "${CMAKE_CURRENT_LIST_DIR}/tools/vcpkg")
set(CMAKE_TOOLCHAIN_FILE
    "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

project(utility-c)

# Utility-c library options
option(UTILITYC_BUILD_LIB "Build the library" ON)
option(UTILITYC_BUILD_TEST "Build the tests" ON)
option(UTILITYC_BUILD_TEST_COVERAGE "Build the test coverage" ON)

if(PROJECT_IS_TOP_LEVEL)
  # Export compile commands for clang-tidy
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  set(CMAKE_VERBOSE_MAKEFILE ON)

  # Append CMAKE_MODULE_PATH with cmake modules directory
  set(CMAKE_MODULE_PATH
      "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/tools/cmake")

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

  if(PROJECT_IS_TOP_LEVEL)
    # Set target specific directories
    set_target_properties(
      ${PROJECT_NAME}
      PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/cmd/lib"
                 LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/cmd/lib")
  endif() # PROJECT_IS_TOP_LEVEL

  # vcpkg dependencies
  find_package(uriparser CONFIG REQUIRED)
  find_package(unofficial-libargon2 CONFIG REQUIRED)
  find_package(unofficial-pcre CONFIG REQUIRED)

  # Include directories
  target_include_directories(
    ${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}"
                           "${PROJECT_SOURCE_DIR}/include")

  # Define static libraries for linking
  set(static_libs
      ${static_libs} external::libmxml unofficial::argon2::libargon2
      unofficial::pcre::pcre uriparser::uriparser)

  # Link the static libraries to this target
  target_link_libraries(${PROJECT_NAME} PUBLIC m ${static_libs})
endif() # UTILITYC_BUILD_LIB

# Build tests
if(UTILITYC_BUILD_TEST)
  find_package(GTest CONFIG REQUIRED)

  # Add Test Target
  add_executable(${PROJECT_NAME}-test)

  # Set target specific directories
  set_target_properties(
    ${PROJECT_NAME}-test PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                    "${PROJECT_SOURCE_DIR}/cmd/bin")

  # Link the static libraries to this target
  target_link_libraries(${PROJECT_NAME}-test GTest::gmock GTest::gtest
                        GTest::gmock_main GTest::gtest_main ${PROJECT_NAME})
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
