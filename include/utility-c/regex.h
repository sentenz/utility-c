// SPDX-License-Identifier: Apache-2.0

#ifndef INCLUDE_UTILITY_C_REGEX_H_
#define INCLUDE_UTILITY_C_REGEX_H_

#ifdef S_HAVE_CONFIG_H
  #include "utility-c/config.h"
#endif

#include <stdarg.h>
#ifdef S_POSIX
  #include <stdbool.h>
#endif  // S_POSIX
#include <stdio.h>

#define OVECCOUNT 30 /* should be a multiple of 3 */

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Matches a regular expression pattern against a string.
/// @param pattern The regular expression pattern to match.
/// @param str The string to match against.
/// @return true if the pattern matches the string, false otherwise.
/// @note https://github.com/vmg/pcre/blob/master/pcredemo.c
bool regex_match(const char *pattern, const char *str);

/// @brief Finds the first occurrence of a regular expression pattern in a string.
/// @param pattern The regular expression pattern to search for.
/// @param str The string to search in.
/// @return A pointer to the first occurrence of the pattern in the string, or NULL if not found.
/// @note The caller is responsible for freeing the memory using free().
char *regex_find(const char *pattern, const char *str);

/// @brief Selects a substring between two regular expression patterns in a string.
/// @param str The string to search in.
/// @param leftPattern The left pattern to search for.
/// @param rightPattern The right pattern to search for.
/// @return A pointer to the selected substring, or NULL if not found.
/// @note The caller is responsible for freeing the memory using free().
char *regex_selectStringBetweenPatterns(const char *str,
                                        const char *leftPattern,
                                        const char *rightPattern);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_REGEX_H_
