// SPDX-License-Identifier: Apache-2.0

#ifndef INCLUDE_UTILITY_C_CONFIG_H_
#define INCLUDE_UTILITY_C_CONFIG_H_

/**
 * @file config.h
 * @brief Project configuration and platform detection macros.
 *
 * This header provides configuration macros, platform detection, and
 * common definitions used throughout the utility-c library. It handles
 * differences between operating systems and environments.
 *
 * Platform Macros:
 * - S_UNIX  : Defined when compiling on Unix-like systems
 * - S_LINUX : Defined when compiling on Linux
 * - S_POSIX : Defined when POSIX APIs are available (Unix or Linux)
 *
 * Standard Definitions:
 * - UNUSED(x) : Suppresses unused parameter warnings
 * - NULL      : Null pointer constant
 * - NAME_MAX  : Maximum filename length
 * - PATH_MAX  : Maximum path length
 *
 * @note This header should be included first in source files that need
 *       platform-specific behavior.
 *
 * @warning Modifying these definitions may break compatibility across
 *          platforms.
 *
 * @copyright Copyright (c) 2023-2026 sentenz
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Suppresses compiler warnings for unused parameters.
 *
 * Use this macro to explicitly mark function parameters as intentionally
 * unused, preventing compiler warnings.
 *
 * @param x The unused parameter.
 *
 * @code
 * void callback(void *ctx, int event) {
 *     UNUSED(ctx);  // Context not needed in this implementation
 *     handle_event(event);
 * }
 * @endcode
 */
#if !defined(UNUSED)
  #define UNUSED(x) (void)(x)
#endif

/**
 * @brief Null pointer constant.
 *
 * Provides a portable definition of NULL if not already defined.
 */
#if !defined(NULL)
  #define NULL (void*)0
#endif

/**
 * @brief Maximum length of a filename.
 *
 * Defines the maximum number of bytes in a filename (not including
 * the path components). Default is 64 if not defined by the system.
 */
#if !defined(NAME_MAX)
  #define NAME_MAX 64
#endif

// === Project-Specific Definitions ===============================================================

/**
 * @brief Null character constant.
 *
 * Provides a named constant for the null terminator character.
 */
#if !defined(S_NULL_CHAR) && !defined(S_NULL_CHAR)
  #define S_NULL_CHAR '\0'
#endif

/**
 * @brief Unix platform detection.
 *
 * Defined when compiling on a Unix-like operating system.
 */
#if defined(__unix__) && !defined(S_UNIX)
  #define S_UNIX
#endif

/**
 * @brief Linux platform detection.
 *
 * Defined when compiling on Linux.
 */
#if defined(__linux__) && !defined(S_LINUX)
  #define S_LINUX
#endif

/**
 * @brief POSIX compatibility detection.
 *
 * Defined when POSIX APIs are available (Unix or Linux systems).
 * Used to conditionally include POSIX-specific headers and features.
 */
#if (defined(S_UNIX) || defined(S_LINUX)) && !defined(S_POSIX)
  #define S_POSIX
#endif

/**
 * @brief File system module activation flag.
 *
 * Enables file system operations throughout the library.
 */
#if !defined(S_FS)
  #define S_FS
#endif

/* Regex library selection (PCRE or PCRE2)
 * Uncomment ONE of the following to select the regex library:
 * #define S_PCRE
 * #define S_PCRE2
 * #define PCRE2_CODE_UNIT_WIDTH 8
 */

/**
 * @brief Maximum path length.
 *
 * On non-Linux systems, defaults to _POSIX_PATH_MAX.
 */
#if !defined(S_LINUX)
  #define PATH_MAX _POSIX_PATH_MAX
#endif

// === Compile-Time Checks ========================================================================

/**
 * @brief Verifies HAVE_CONFIG_H is defined for PCRE support.
 *
 * The PCRE and PCRE2 libraries require HAVE_CONFIG_H to be defined
 * for proper compilation.
 */
#if !defined(HAVE_CONFIG_H)
  #error "Macro 'HAVE_CONFIG_H' is needed in regex PCRE and PRCE2!"
#endif

/**
 * @brief Ensures only one regex library is selected.
 *
 * Both S_PCRE and S_PCRE2 cannot be defined simultaneously.
 */
#if defined(S_PCRE) && defined(S_PCRE2)
  #error "Define only one regex library!"
#endif

/**
 * @brief Verifies file system module is enabled.
 *
 * The S_FS macro must be defined for file system operations.
 */
#if !defined(S_FS)
  #error "Check the file system activation macro 'S_FS'!"
#endif

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_CONFIG_H_
