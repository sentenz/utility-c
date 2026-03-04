# `tools/cmake/`

Reusable CMake helper modules for build configuration, testing, and toolchain integration.

- [1. Details](#1-details)
  - [1.1. meta_compiler_cache](#11-meta_compiler_cache)
  - [1.2. meta_conan](#12-meta_conan)
  - [1.3. meta_coverage](#13-meta_coverage)
  - [1.4. meta_gtest](#14-meta_gtest)
  - [1.5. meta_policy](#15-meta_policy)
  - [1.6. meta_sanitizers](#16-meta_sanitizers)
  - [1.7. meta_unity](#17-meta_unity)
- [2. References](#2-references)

## 1. Details

### 1.1. meta_compiler_cache

- `meta_compiler_cache.cmake`
  > Enable compiler cache using `ccache` or `sccache` to speed up recompilation by caching previous compilations and detecting when the same compilation is being done again.

- Prerequisites

  - [ccache](https://ccache.dev/) or [sccache](https://github.com/mozilla/sccache)
    > Compiler cache to speed up recompilation.

    ```bash
    sudo apt install ccache
    ```

- Usage

  ```cmake
  include(meta_compiler_cache)

  meta_compiler_cache([ENABLE <bool>] [TYPE <auto|ccache|sccache>])
  ```

- Example

  ```cmake
  meta_compiler_cache(ENABLE ON)
  meta_compiler_cache(ENABLE ON TYPE ccache)
  meta_compiler_cache(ENABLE ON TYPE sccache)
  ```

### 1.2. meta_conan

- `meta_conan.cmake`
  > Conan v2 integration helper for CMake. Ensures Conan is available, runs `conan install`, and configures the CMake toolchain.

- Prerequisites

  - [Conan](https://conan.io/) v2
    > C and C++ package manager.

    ```bash
    pip install "conan>=2"
    ```

- Usage

  ```cmake
  include(meta_conan)

  meta_conan([CREATE_INSTALL_TARGET]
             [AUTO_INSTALL <ON|OFF>]
             [USER_PRESETS_PATH <path>]
             [INSTALL_FOLDER <path>]
             [SOURCE_DIR <path>]
             [GENS <gen>...])
  ```

- Example

  ```cmake
  meta_conan(AUTO_INSTALL ON CREATE_INSTALL_TARGET)
  ```

### 1.3. meta_coverage

- `meta_coverage.cmake`
  > Enable code coverage instrumentation for projects built with GCC or Clang.

- Prerequisites

  - [gcovr](https://gcovr.com/)
    > Python-based tool for generating code coverage reports.

    ```bash
    pip install gcovr
    ```

- Usage

  ```cmake
  include(meta_coverage)

  meta_coverage([ENABLE <bool>])
  ```

- Example

  ```cmake
  meta_coverage(ENABLE ON)
  ```

### 1.4. meta_gtest

- `meta_gtest.cmake`
  > Creates or extends a Google Test (GTest)-based executable for unit and mock tests, with optional CTest discovery and data-driven test support.

- Prerequisites

  - [GTest](https://github.com/google/googletest)
    > Google's C++ testing and mocking framework.

    ```bash
    # Via Conan
    conan install . --build missing
    ```

- Usage

  ```cmake
  include(meta_gtest)

  meta_gtest([WITH_GMOCK] [WITH_MAIN] [WITH_CTEST] [WITH_DDT]
             TARGET <name>
             [ENABLE <bool>]
             [SOURCES <src>...]
             [LINK <lib>...])
  ```

- Example

  ```cmake
  meta_gtest(WITH_CTEST TARGET my_tests SOURCES test_foo.cpp test_bar.cpp LINK my_lib)
  ```

### 1.5. meta_policy

- `meta_policy.cmake`
  > Set global project properties for C++ language standard and compiler warnings.

- Usage

  ```cmake
  include(meta_policy)

  meta_policy()
  ```

- Example

  ```cmake
  meta_policy()
  ```

### 1.6. meta_sanitizers

- `meta_sanitizers.cmake`
  > Enable LeakSanitizer (LSan), AddressSanitizer (ASan), and Undefined Behavior Sanitizer (UBSan) for GCC or Clang builds.

- Usage

  ```cmake
  include(meta_sanitizers)

  meta_sanitizers([ENABLE <bool>])
  ```

- Example

  ```cmake
  meta_sanitizers(ENABLE ON)
  ```

### 1.7. meta_unity

- `meta_unity.cmake`
  > Registers Unity-based on-target unit tests with CTest using SEGGER J-Run. Each test suite (or the whole binary) is registered as a separate CTest test that invokes J-Run to flash and execute the firmware on the connected embedded target.

- Prerequisites

  - [SEGGER J-Run](https://kb.segger.com/J-Run) v8.10 or later
    > Command-line tool for automated on-target testing via J-Link.

    ```bash
    # Download and install from https://www.segger.com/downloads/jlink/
    ```

- Usage

  ```cmake
  include(meta_unity)

  meta_unity([WITH_RTT] [WITH_SEMIHOSTING]
             TARGET <name>
             [ENABLE <bool>]
             DEVICE <device>
             [INTERFACE <SWD|JTAG>]
             [SPEED <kHz>]
             [TIMEOUT <seconds>]
             [SUITES <suite>...])
  ```

- Example

  ```cmake
  meta_unity(WITH_RTT
             TARGET my_firmware
             DEVICE LPC55S16
             INTERFACE SWD
             SPEED 4000
             TIMEOUT 60
             SUITES test_leds test_sensors)
  ```

## 2. References

- MCU on Eclipse [On-Target Testing with J-Run --args feature](https://mcuoneclipse.com/2024/09/23/automated-on-target-testing-with-j-runs-args-feature/) article.
- MCU on Eclipse [On-Target Testing with CMake and CTest](https://mcuoneclipse.com/2023/12/18/modern-on-target-embedded-system-testing-with-cmake-and-ctest/) article.
- SEGGER [J-Run](https://kb.segger.com/J-Run) documentation.
