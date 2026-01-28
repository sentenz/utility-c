// SPDX-License-Identifier: Apache-2.0

/**
 * @file char.h
 * @brief String manipulation utilities.
 *
 * This module provides a comprehensive set of functions for string operations
 * including validation, conversion, transformation, and memory management.
 *
 * @note All functions returning dynamically allocated strings require the caller
 *       to free the memory using char_free() unless otherwise specified.
 *
 * @warning Functions may return NULL on memory allocation failure or invalid input.
 *
 * @example
 * @code
 * // String validation
 * if (char_isValid(input) && !char_isEmpty(input)) {
 *     // Process the string
 * }
 *
 * // String transformation
 * char *upper = char_toUppercase("hello");
 * char *trimmed = char_trimSpace("  hello  ");
 * char_free(upper);
 * char_free(trimmed);
 *
 * // String concatenation
 * char *path = char_concats("/home", "/", "user", "/", "file.txt", NULL);
 * char_free(path);
 * @endcode
 */

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

/**
 * @brief Checks if a string is empty.
 *
 * A string is considered empty if it is NULL, has zero length, or contains
 * only the null terminator.
 *
 * @param[in] str The null-terminated string to check.
 *
 * @return @c true if @p str is NULL or has zero length.
 * @return @c false if @p str contains one or more characters.
 *
 * @see char_isValid
 */
bool char_isEmpty(const char *str);

/**
 * @brief Checks if a string is valid (non-NULL and non-empty).
 *
 * A string is considered valid if it is not NULL and contains at least
 * one character before the null terminator.
 *
 * @param[in] str The null-terminated string to check.
 *
 * @return @c true if @p str is non-NULL and non-empty.
 * @return @c false if @p str is NULL or empty.
 *
 * @see char_isEmpty
 */
bool char_isValid(const char *str);

/**
 * @brief Compares two strings for equality.
 *
 * Performs a case-sensitive comparison of two null-terminated strings.
 *
 * @param[in] str1 The first null-terminated string to compare.
 * @param[in] str2 The second null-terminated string to compare.
 *
 * @return @c true if both strings are identical (same content and length).
 * @return @c false if the strings differ or if either is NULL.
 *
 * @note Two NULL strings are not considered equal.
 */
bool char_areEqual(const char *str1, const char *str2);

/**
 * @brief Checks if a string contains a substring.
 *
 * Searches for the first occurrence of @p sub within @p str.
 *
 * @param[in] str The null-terminated string to search in.
 * @param[in] sub The null-terminated substring to search for.
 *
 * @return @c true if @p sub is found within @p str.
 * @return @c false if @p sub is not found, or if either parameter is NULL.
 *
 * @see char_occurrences
 */
bool char_contains(const char *str, char const *sub);

/**
 * @brief Converts a string to an integer.
 *
 * Parses the initial portion of @p str as a signed decimal integer.
 *
 * @param[in] str The null-terminated string to convert.
 *
 * @return The integer value parsed from @p str.
 * @return 0 if @p str is NULL, empty, or contains no valid digits.
 *
 * @note Leading whitespace is skipped. Conversion stops at the first
 *       non-numeric character.
 *
 * @see char_fromInteger, char_toFloat
 */
int char_toInteger(const char *str);

/**
 * @brief Converts an integer to a string.
 *
 * Creates a new null-terminated string containing the decimal representation
 * of the integer @p n.
 *
 * @param[in] n The integer to convert.
 *
 * @return A newly allocated string containing the integer representation.
 * @return NULL on memory allocation failure.
 *
 * @note The caller is responsible for freeing the memory using char_free().
 *
 * @see char_toInteger, char_fromFloat, char_fromNumber
 */
char *char_fromInteger(const int n);

/**
 * @brief Converts a string to a floating-point number.
 *
 * Parses the initial portion of @p str as a double-precision floating-point
 * number.
 *
 * @param[in] str The null-terminated string to convert.
 *
 * @return The double value parsed from @p str.
 * @return 0.0 if @p str is NULL, empty, or contains no valid digits.
 *
 * @note Leading whitespace is skipped. Conversion stops at the first
 *       character that cannot be part of a floating-point number.
 *
 * @see char_fromFloat, char_toInteger
 */
double char_toFloat(const char *str);

/**
 * @brief Converts a floating-point number to a string.
 *
 * Creates a new null-terminated string containing the decimal representation
 * of the floating-point number @p n.
 *
 * @param[in] n The double value to convert.
 *
 * @return A newly allocated string containing the floating-point representation.
 * @return NULL on memory allocation failure.
 *
 * @note The caller is responsible for freeing the memory using char_free().
 *
 * @see char_toFloat, char_fromInteger, char_fromNumber
 */
char *char_fromFloat(const double n);

/**
 * @brief Converts a number (integer or floating-point) to a string.
 *
 * Automatically determines whether to format the number as an integer or
 * floating-point based on whether @p n has a fractional component.
 *
 * @param[in] n The number to convert.
 *
 * @return A newly allocated string containing the number representation.
 * @return NULL on memory allocation failure.
 *
 * @note The caller is responsible for freeing the memory using char_free().
 *
 * @see char_fromInteger, char_fromFloat
 */
char *char_fromNumber(const double n);

/**
 * @brief Concatenates multiple strings into a single string.
 *
 * Joins a variable number of strings together. The argument list must be
 * terminated with NULL.
 *
 * @param[in] str The first null-terminated string to concatenate.
 * @param[in] ... Additional null-terminated strings, terminated by NULL.
 *
 * @return A newly allocated string containing all concatenated strings.
 * @return NULL on memory allocation failure or if @p str is NULL.
 *
 * @warning The variadic argument list MUST be terminated with NULL.
 *
 * @note The caller is responsible for freeing the memory using char_free().
 *
 * @example
 * @code
 * char *result = char_concats("Hello", " ", "World", "!", NULL);
 * // result = "Hello World!"
 * char_free(result);
 * @endcode
 */
char *char_concats(const char *str, ...);

/**
 * @brief Trims leading and trailing whitespace from a string.
 *
 * Creates a new string with all leading and trailing whitespace characters
 * (spaces, tabs, newlines) removed.
 *
 * @param[in] str The null-terminated string to trim.
 *
 * @return A newly allocated trimmed string.
 * @return NULL on memory allocation failure or if @p str is NULL.
 *
 * @note The caller is responsible for freeing the memory using char_free().
 *
 * @see char_trimNonAlphanum
 */
char *char_trimSpace(const char *str);

/**
 * @brief Trims leading and trailing non-alphanumeric characters from a string.
 *
 * Creates a new string with all leading and trailing non-alphanumeric
 * characters removed. Alphanumeric characters are [a-zA-Z0-9].
 *
 * @param[in] str The null-terminated string to trim.
 *
 * @return A newly allocated trimmed string.
 * @return NULL on memory allocation failure or if @p str is NULL.
 *
 * @note The caller is responsible for freeing the memory using char_free().
 *
 * @see char_trimSpace
 */
char *char_trimNonAlphanum(const char *str);

/**
 * @brief Converts a string to uppercase.
 *
 * Creates a new string with all alphabetic characters converted to uppercase.
 * Non-alphabetic characters remain unchanged.
 *
 * @param[in] str The null-terminated string to convert.
 *
 * @return A newly allocated uppercase string.
 * @return NULL on memory allocation failure or if @p str is NULL.
 *
 * @note The caller is responsible for freeing the memory using char_free().
 *
 * @see char_toLowercase
 */
char *char_toUppercase(const char *str);

/**
 * @brief Converts a string to lowercase.
 *
 * Creates a new string with all alphabetic characters converted to lowercase.
 * Non-alphabetic characters remain unchanged.
 *
 * @param[in] str The null-terminated string to convert.
 *
 * @return A newly allocated lowercase string.
 * @return NULL on memory allocation failure or if @p str is NULL.
 *
 * @note The caller is responsible for freeing the memory using char_free().
 *
 * @see char_toUppercase
 */
char *char_toLowercase(const char *str);

/**
 * @brief Counts the number of occurrences of a substring in a string.
 *
 * Counts non-overlapping occurrences of @p sub within @p str.
 *
 * @param[in] str The null-terminated string to search in.
 * @param[in] sub The null-terminated substring to count.
 *
 * @return The number of non-overlapping occurrences of @p sub in @p str.
 * @return 0 if @p str or @p sub is NULL, or if @p sub is empty.
 *
 * @note Based on https://github.com/stephenmathieson/occurrences.c
 *
 * @see char_contains, char_replace
 */
size_t char_occurrences(const char *str, const char *sub);

/**
 * @brief Replaces occurrences of a substring with another substring.
 *
 * Creates a new string with up to @p count occurrences of @p pre replaced
 * by @p post.
 *
 * @param[in] str   The null-terminated source string.
 * @param[in] pre   The null-terminated substring to find and replace.
 * @param[in] post  The null-terminated substring to replace @p pre with.
 * @param[in] count The maximum number of replacements to perform.
 *                  Use -1 to replace all occurrences.
 *
 * @return A newly allocated string with replacements applied.
 * @return NULL on memory allocation failure or if any parameter is NULL.
 *
 * @note The caller is responsible for freeing the memory using char_free().
 * @note Based on https://github.com/stephenmathieson/str-replace.c
 *
 * @example
 * @code
 * char *result = char_replace("hello world", "world", "there", 1);
 * // result = "hello there"
 * char_free(result);
 * @endcode
 *
 * @see char_occurrences, char_contains
 */
char *char_replace(const char *str, const char *pre, const char *post, int count);

/**
 * @brief Frees memory allocated for a string by char module functions.
 *
 * Safely frees a string that was allocated by any char_* function.
 * It is safe to pass NULL to this function.
 *
 * @param[in] str The string to free, or NULL.
 *
 * @post @p str is no longer valid and must not be used.
 *
 * @note This function should be used to free strings returned by `char_*()`.
 */
void char_free(char *str);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_CHAR_H_
