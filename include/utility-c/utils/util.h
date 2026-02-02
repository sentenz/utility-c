// SPDX-License-Identifier: Apache-2.0

#ifndef INCLUDE_UTILITY_C_UTIL_H_
#define INCLUDE_UTILITY_C_UTIL_H_

/**
 * @file util.h
 * @brief General utility functions for numeric operations.
 *
 * This module provides utility functions for working with numbers, including
 * type checking, digit counting, and floating-point comparison functions
 * that properly handle the inherent imprecision of floating-point arithmetic.
 *
 * @note Floating-point comparisons use epsilon-based techniques to handle
 *       rounding errors that occur in IEEE 754 floating-point representations.
 *
 * @code
 * // Check if a number has a fractional part
 * if (util_isFloat(3.14)) {
 *     printf("Has fractional part\n");
 * }
 *
 * // Count digits
 * int digits = util_countDigits(12345);  // Returns 5
 *
 * // Safe floating-point comparison
 * float a = 0.1f + 0.2f;
 * float b = 0.3f;
 * if (util_approximatelyEqual(a, b, 1e-6f)) {
 *     printf("a and b are approximately equal\n");
 * }
 * @endcode
 *
 * @see https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
 *
 * @copyright Copyright (c) 2023-2026 sentenz
 * @license SPDX-License-Identifier: Apache-2.0
 */

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
 * @brief Checks if a number has a fractional (decimal) component.
 *
 * Determines whether the given double value has a non-zero fractional part,
 * distinguishing between integers stored as doubles and true floating-point
 * values.
 *
 * @param[in] n The number to check.
 *
 * @return @c true if @p n has a non-zero fractional part.
 * @return @c false if @p n is effectively an integer.
 *
 * @code
 * util_isFloat(3.14);   // true
 * util_isFloat(3.0);    // false
 * util_isFloat(-2.5);   // true
 * util_isFloat(0.0);    // false
 * @endcode
 */
bool util_isFloat(const double n);

/**
 * @brief Counts the number of digits in an integer.
 *
 * Returns the count of decimal digits in the absolute value of @p n.
 *
 * @param[in] n The integer to count digits of.
 *
 * @return The number of digits in @p n (minimum 1 for zero).
 *
 * @code
 * util_countDigits(12345);   // 5
 * util_countDigits(0);       // 1
 * util_countDigits(-999);    // 3 (sign not counted)
 * @endcode
 *
 * @see util_countFloat, util_countNumber
 */
int util_countDigits(const int n);

/**
 * @brief Counts the total number of digits in a floating-point number.
 *
 * Returns the count of all decimal digits in the representation of @p n,
 * including both the integer and fractional parts.
 *
 * @param[in] n The floating-point number to count digits of.
 *
 * @return The total number of significant digits in @p n.
 *
 * @note The decimal point and sign are not counted.
 *
 * @see util_countDigits, util_countNumber
 */
int util_countFloat(const double n);

/**
 * @brief Counts digits in a number (integer or floating-point).
 *
 * Automatically determines whether to count integer or floating-point
 * digits based on whether @p n has a fractional component.
 *
 * @param[in] n The number to count digits of.
 *
 * @return The number of significant digits in @p n.
 *
 * @see util_countDigits, util_countFloat, util_isFloat
 */
int util_countNumber(const double n);

/**
 * @brief Checks if two floats are approximately equal (relative tolerance).
 *
 * Uses relative epsilon comparison, which is more suitable when comparing
 * numbers of similar magnitude. The tolerance scales with the magnitude
 * of the larger number.
 *
 * @param[in] a       The first floating-point number.
 * @param[in] b       The second floating-point number.
 * @param[in] epsilon The relative tolerance factor.
 *
 * @return @c true if @p a and @p b are within @p epsilon relative tolerance.
 * @return @c false otherwise.
 *
 * @note Use this when comparing numbers that may have accumulated
 *       floating-point errors during computation.
 *
 * @code
 * float result = some_calculation();  // Expected ~1.0
 * if (util_approximatelyEqual(result, 1.0f, 1e-5f)) {
 *     // Handle approximately equal case
 * }
 * @endcode
 *
 * @see util_essentiallyEqual, util_definitelyGreaterThan, util_definitelyLessThan
 */
bool util_approximatelyEqual(float a, float b, float epsilon);

/**
 * @brief Checks if two floats are essentially equal (stricter tolerance).
 *
 * Uses a stricter comparison than util_approximatelyEqual(). The tolerance
 * is based on the smaller of the two values, making this more conservative.
 *
 * @param[in] a       The first floating-point number.
 * @param[in] b       The second floating-point number.
 * @param[in] epsilon The relative tolerance factor.
 *
 * @return @c true if @p a and @p b are essentially equal.
 * @return @c false otherwise.
 *
 * @note Use this when you need a stricter equality check than
 *       util_approximatelyEqual().
 *
 * @see util_approximatelyEqual, util_definitelyGreaterThan, util_definitelyLessThan
 */
bool util_essentiallyEqual(float a, float b, float epsilon);

/**
 * @brief Checks if one float is definitely greater than another.
 *
 * Determines if @p a is greater than @p b by more than the tolerance
 * threshold, accounting for floating-point imprecision.
 *
 * @param[in] a       The first floating-point number.
 * @param[in] b       The second floating-point number.
 * @param[in] epsilon The minimum relative difference required.
 *
 * @return @c true if @p a is definitely greater than @p b.
 * @return @c false if @p a is less than, equal to, or only approximately
 *         greater than @p b.
 *
 * @code
 * if (util_definitelyGreaterThan(price, threshold, 1e-5f)) {
 *     printf("Price exceeds threshold\n");
 * }
 * @endcode
 *
 * @see util_definitelyLessThan, util_approximatelyEqual
 */
bool util_definitelyGreaterThan(float a, float b, float epsilon);

/**
 * @brief Checks if one float is definitely less than another.
 *
 * Determines if @p a is less than @p b by more than the tolerance
 * threshold, accounting for floating-point imprecision.
 *
 * @param[in] a       The first floating-point number.
 * @param[in] b       The second floating-point number.
 * @param[in] epsilon The minimum relative difference required.
 *
 * @return @c true if @p a is definitely less than @p b.
 * @return @c false if @p a is greater than, equal to, or only approximately
 *         less than @p b.
 *
 * @see util_definitelyGreaterThan, util_approximatelyEqual
 */
bool util_definitelyLessThan(float a, float b, float epsilon);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_UTIL_H_
