// SPDX-License-Identifier: Apache-2.0

#ifndef INCLUDE_UTILITY_C_CHAR_H_
#define INCLUDE_UTILITY_C_CHAR_H_

#ifdef S_HAVE_CONFIG_H
  #include "utility-c/config.h"
#endif

#include <stdarg.h>
#include <stdio.h>
#if defined(S_POSIX)
  #include <stdbool.h>
#endif  // S_POSIX

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Checks if a string is empty.
/// @param str The string to check.
/// @return true if the string is empty, false otherwise.
bool char_isEmpty(const char *str);

/// @brief Checks if a string is valid.
/// @param str The string to check.
/// @return true if the string is valid, false otherwise.
bool char_isValid(const char *str);

/// @brief Checks if two strings are equal.
/// @param str1 The first string to compare.
/// @param str2 The second string to compare.
/// @return true if the strings are equal, false otherwise.
bool char_areEqual(const char *str1, const char *str2);

/// @brief Converts a string to an integer.
/// @param str The string to convert.
/// @return The integer value of the string.
int char_toInteger(const char *str);

/// @brief Converts an integer to a string.
/// @param n The integer to convert.
/// @return The string representation of the integer.
/// @note The caller is responsible for freeing the memory using char_free().
char *char_fromInteger(const int n);

/// @brief Converts a string to a float.
/// @param str The string to convert.
/// @return The float value of the string.
double char_toFloat(const char *str);

/// @brief Converts a float to a string.
/// @param n The float to convert.
/// @return The string representation of the float.
/// @note The caller is responsible for freeing the memory using char_free().
char *char_fromFloat(const double n);

/// @brief Converts a number to a string.
/// @param n The number to convert.
/// @return The string representation of the number.
/// @note The caller is responsible for freeing the memory using char_free().
char *char_fromNumber(const double n);

/// @brief Concatenates multiple strings.
/// @param str The first string to concatenate.
/// @param ... The additional strings to concatenate.
/// @return The concatenated string.
/// @note The caller is responsible for freeing the memory using char_free().
char *char_concats(const char *str, ...);

/// @brief Trims whitespace from a string.
/// @param str The string to trim.
/// @return The trimmed string.
/// @note The caller is responsible for freeing the memory using char_free().
char *char_trimSpace(const char *str);

/// @brief Trims non-alphanumeric characters from a string.
/// @param str The string to trim.
/// @return The trimmed string.
/// @note The caller is responsible for freeing the memory using char_free().
char *char_trimNonAlphanum(const char *str);

/// @brief Converts a string to uppercase.
/// @param str The string to convert.
/// @return The uppercase string.
/// @note The caller is responsible for freeing the memory using char_free().
char *char_toUppercase(const char *str);

/// @brief Converts a string to lowercase.
/// @param str The string to convert.
/// @return The lowercase string.
/// @note The caller is responsible for freeing the memory using char_free().
char *char_toLowercase(const char *str);

/// @brief Counts the number of occurrences of a substring in a string.
/// @param str The string to search.
/// @param sub The substring to search for.
/// @return The number of occurrences of the substring.
/// @note https://github.com/stephenmathieson/occurrences.c
size_t char_occurrences(const char *str, const char *sub);

/// @brief Replaces a substring with another substring in a string.
/// @param str The string to modify.
/// @param pre The substring to replace.
/// @param post The substring to replace with.
/// @param count The maximum number of replacements to make.
/// @return The modified string.
/// @note The caller is responsible for freeing the memory using char_free().
/// @note https://github.com/stephenmathieson/str-replace.c/blob/master/src/str-replace.c
char *char_replace(const char *str, const char *pre, const char *post, int count);

/// @brief Checks if a string contains a substring.
/// @param str The string to search.
/// @param sub The substring to search for.
/// @return true if the string contains the substring, false otherwise.
bool char_contains(const char *str, char const *sub);

/// @brief Frees the memory allocated for a string.
/// @param str The string to free.
void char_free(char *str);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_CHAR_H_
