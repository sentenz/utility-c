// SPDX-License-Identifier: Apache-2.0

#ifndef INCLUDE_UTILITY_C_CONFIG_H_
#define INCLUDE_UTILITY_C_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Standard definitions
 */

/* Ignore unused parameter */
#if !defined(UNUSED)
  #define UNUSED(x) (void)(x)
#endif

/* Define NULL pointer */
#if !defined(NULL)
  #define NULL (void*)0
#endif

/* Define maximum length of file name */
#if !defined(NAME_MAX)
  #define NAME_MAX 64
#endif

/*
 * Specific definitions
 */

/* Define NULL character */
#if !defined(S_NULL_CHAR) && !defined(S_NULL_CHAR)
  #define S_NULL_CHAR '\0'
#endif

/* Define unix */
#if defined(__unix__) && !defined(S_UNIX)
  #define S_UNIX
#endif

/* Define linux */
#if defined(__linux__) && !defined(S_LINUX)
  #define S_LINUX
#endif

/* Define posix */
#if (defined(S_UNIX) || defined(S_LINUX)) && !defined(S_POSIX)
  #define S_POSIX
#endif

/* Define file system */
#if !defined(S_FS)
  #define S_FS
#endif

/* Define regex library PCRE */
#if !defined(S_PCRE)
  #define S_PCRE
#endif

/* Define regex library PCRE2 */
// #undef S_PCRE2
// #undef PCRE2_CODE_UNIT_WIDTH 8

/* Define maximum length of path */
#if !defined(S_LINUX)
  #define PATH_MAX _POSIX_PATH_MAX
#endif

/*
 * Macro check
 */

/* This macro checks the include module of regex */
#if !defined(HAVE_CONFIG_H)
  #error "Macro 'HAVE_CONFIG_H' is needed in regex PCRE and PRCE2!"
#endif

/* Check regex module */
#if defined(S_PCRE) && defined(S_PCRE2)
  #error "Define only one regex library!"
#endif

/* This macro checks file system module */
#if !defined(S_FS)
  #error "Check the file system activation macro 'S_FS'!"
#endif

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_CONFIG_H_
