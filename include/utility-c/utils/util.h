// SPDX-License-Identifier: Apache-2.0

#ifndef INCLUDE_UTILITY_C_UTIL_H_
#define INCLUDE_UTILITY_C_UTIL_H_

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

/// @brief Checks if a given number is a floating point number.
/// @param n The number to check.
/// @return true if the number is a floating point number, false otherwise.
bool util_isFloat(const double n);

/// @brief Counts the number of digits in an integer.
/// @param n The integer to count digits of.
/// @return The number of digits in the integer.
int util_countDigits(const int n);

/// @brief Counts the number of digits in a floating point number.
/// @param n The floating point number to count digits of.
/// @return The number of digits in the floating point number.
int util_countFloat(const double n);

/// @brief Counts the number of digits in a number.
/// @param n The number to count digits of.
/// @return The number of digits in the number.
int util_countNumber(const double n);

/// @brief Checks if two floating point numbers are approximately equal.
/// @param a The first floating point number.
/// @param b The second floating point number.
/// @param epsilon The maximum difference between the two numbers to consider them equal.
/// @return true if the two numbers are approximately equal, false otherwise.
bool util_approximatelyEqual(float a, float b, float epsilon);

/// @brief Checks if two floating point numbers are essentially equal.
/// @param a The first floating point number.
/// @param b The second floating point number.
/// @param epsilon The maximum difference between the two numbers to consider them equal.
/// @return true if the two numbers are essentially equal, false otherwise.
bool util_essentiallyEqual(float a, float b, float epsilon);

/// @brief Checks if one floating point number is definitely greater than another.
/// @param a The first floating point number.
/// @param b The second floating point number.
/// @param epsilon The minimum difference between the two numbers to consider a greater than b.
/// @return true if a is definitely greater than b, false otherwise.
bool util_definitelyGreaterThan(float a, float b, float epsilon);

/// @brief Checks if one floating point number is definitely less than another.
/// @param a The first floating point number.
/// @param b The second floating point number.
/// @param epsilon The minimum difference between the two numbers to consider a less than b.
/// @return true if a is definitely less than b, false otherwise.
bool util_definitelyLessThan(float a, float b, float epsilon);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_UTIL_H_
