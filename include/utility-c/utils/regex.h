// SPDX-License-Identifier: Apache-2.0

/**
 * @file regex.h
 * @brief Regular expression pattern matching utilities.
 *
 * This module provides functions for regular expression operations including
 * pattern matching, searching, and substring extraction using PCRE (Perl
 * Compatible Regular Expressions).
 *
 * @note This module uses PCRE/PCRE2 library for regex operations.
 * @note Regular expression syntax follows PCRE standards.
 *
 * @warning Complex patterns may impact performance on large strings.
 *
 * @example
 * @code
 * // Simple pattern matching
 * if (regex_match("^[a-z]+$", "hello")) {
 *     printf("String contains only lowercase letters\n");
 * }
 *
 * // Extract first match
 * char *match = regex_find("\\d+", "Price: 123 USD");
 * if (match) {
 *     printf("Found number: %s\n", match);  // Prints "123"
 *     free(match);
 * }
 *
 * // Extract text between patterns
 * char *content = regex_between("<title>Hello</title>", "<title>", "</title>");
 * if (content) {
 *     printf("Title: %s\n", content);  // Prints "Hello"
 *     free(content);
 * }
 * @endcode
 *
 * @see https://www.pcre.org/original/doc/html/pcrepattern.html for pattern syntax
 */

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

/**
 * @brief Output vector count for PCRE matching.
 *
 * Defines the size of the output vector used for capturing substrings.
 * Should be a multiple of 3 as required by PCRE.
 */
#define OVECCOUNT 30

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Tests if a regular expression pattern matches a string.
 *
 * Attempts to match the entire string against the given pattern.
 *
 * @param[in] pattern The PCRE regular expression pattern.
 * @param[in] str     The null-terminated string to match against.
 *
 * @return @c true if @p pattern matches @p str.
 * @return @c false if no match is found or on error.
 *
 * @pre @p pattern must be a valid PCRE regular expression.
 * @pre @p str must be a valid null-terminated string.
 *
 * @note Based on https://github.com/vmg/pcre/blob/master/pcredemo.c
 *
 * @example
 * @code
 * // Email validation
 * bool valid = regex_match("^[\\w.-]+@[\\w.-]+\\.[a-z]{2,}$", email);
 *
 * // Check for digits
 * bool has_digits = regex_match(".*\\d.*", input);
 * @endcode
 *
 * @see regex_find
 */
bool regex_match(const char *pattern, const char *str);

/**
 * @brief Finds the first occurrence of a pattern in a string.
 *
 * Searches for the first substring matching the given pattern and returns
 * a copy of the matched text.
 *
 * @param[in] pattern The PCRE regular expression pattern.
 * @param[in] str     The null-terminated string to search in.
 *
 * @return A newly allocated string containing the first match.
 * @return NULL if no match is found, pattern is invalid, or memory
 *         allocation fails.
 *
 * @pre @p pattern must be a valid PCRE regular expression.
 * @pre @p str must be a valid null-terminated string.
 *
 * @note The caller is responsible for freeing the returned string using free().
 *
 * @example
 * @code
 * // Extract first number from text
 * char *num = regex_find("[0-9]+", "Order #12345 placed");
 * // num = "12345"
 * free(num);
 *
 * // Extract first word
 * char *word = regex_find("\\b\\w+\\b", "  Hello, World!  ");
 * // word = "Hello"
 * free(word);
 * @endcode
 *
 * @see regex_match, regex_between
 */
char *regex_find(const char *pattern, const char *str);

/**
 * @brief Extracts text between two patterns.
 *
 * Finds the substring that occurs between the first occurrence of
 * @p leftPattern and the first subsequent occurrence of @p rightPattern.
 *
 * @param[in] str          The null-terminated string to search in.
 * @param[in] leftPattern  The PCRE pattern marking the start delimiter.
 * @param[in] rightPattern The PCRE pattern marking the end delimiter.
 *
 * @return A newly allocated string containing the text between the patterns.
 * @return NULL if either pattern is not found, they don't appear in the
 *         correct order, or memory allocation fails.
 *
 * @pre @p leftPattern and @p rightPattern must be valid PCRE patterns.
 * @pre @p str must contain both patterns with @p leftPattern appearing
 *      before @p rightPattern.
 *
 * @note The caller is responsible for freeing the returned string using free().
 * @note The delimiter patterns themselves are not included in the result.
 *
 * @example
 * @code
 * // Extract HTML tag content
 * char *title = regex_between(
 *     "<html><title>My Page</title></html>",
 *     "<title>",
 *     "</title>"
 * );
 * // title = "My Page"
 * free(title);
 *
 * // Extract quoted string
 * char *quoted = regex_between("Say \"Hello\" to me", "\"", "\"");
 * // quoted = "Hello"
 * free(quoted);
 * @endcode
 *
 * @see regex_find
 */
char *regex_between(const char *str,
                    const char *leftPattern,
                    const char *rightPattern);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_REGEX_H_
