// SPDX-License-Identifier: Apache-2.0

#ifndef INCLUDE_UTILITY_C_BIT_H_
#define INCLUDE_UTILITY_C_BIT_H_

/**
 * @file bit.h
 * @brief Bit manipulation utilities.
 *
 * This module provides functions for performing bitwise operations on integers,
 * including checking, setting, unsetting, and toggling individual bits.
 *
 * @note Bit positions are zero-indexed, where position 0 represents the least
 *       significant bit (LSB).
 *
 * @code
 * int flags = 0;
 * bit_set(&flags, 0);                 // Set bit 0: flags = 0b0001
 * bit_set(&flags, 2);                 // Set bit 2: flags = 0b0101
 * bool is_set = bit_check(flags, 0);  // Returns true
 * bit_toggle(&flags, 0);              // Toggle bit 0: flags = 0b0100
 * bit_unset(&flags, 2);               // Unset bit 2: flags = 0b0000
 * @endcode
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
 * @brief Checks if a specific bit is set in an integer.
 *
 * Examines the bit at position @p n in the integer @p x to determine
 * if it is set (1) or unset (0).
 *
 * @param[in] x The integer value to check.
 * @param[in] n The zero-indexed bit position to check (0 = LSB).
 *
 * @return @c true if the bit at position @p n is set (1).
 * @return @c false if the bit at position @p n is unset (0).
 *
 * @pre @p n must be within the valid range for the integer type
 *      (typically 0 to 31 for a 32-bit integer).
 *
 * @see bit_set, bit_unset, bit_toggle
 */
bool bit_check(const int x, const int n);

/**
 * @brief Sets a specific bit in an integer.
 *
 * Sets the bit at position @p n in the integer pointed to by @p x to 1.
 *
 * @param[in,out] x Pointer to the integer to modify.
 * @param[in]     n The zero-indexed bit position to set (0 = LSB).
 *
 * @return @c true if the operation was successful.
 * @return @c false if @p x is NULL or the operation failed.
 *
 * @pre @p x must not be NULL.
 * @pre @p n must be within the valid range for the integer type.
 *
 * @post The bit at position @p n in @p *x is set to 1.
 *
 * @see bit_check, bit_unset, bit_toggle
 */
bool bit_set(int *x, const int n);

/**
 * @brief Clears (unsets) a specific bit in an integer.
 *
 * Clears the bit at position @p n in the integer pointed to by @p x to 0.
 *
 * @param[in,out] x Pointer to the integer to modify.
 * @param[in]     n The zero-indexed bit position to unset (0 = LSB).
 *
 * @return @c true if the operation was successful.
 * @return @c false if @p x is NULL or the operation failed.
 *
 * @pre @p x must not be NULL.
 * @pre @p n must be within the valid range for the integer type.
 *
 * @post The bit at position @p n in @p *x is set to 0.
 *
 * @see bit_check, bit_set, bit_toggle
 */
bool bit_unset(int *x, const int n);

/**
 * @brief Toggles a specific bit in an integer.
 *
 * Inverts the bit at position @p n in the integer pointed to by @p x.
 * If the bit is 0, it becomes 1; if it is 1, it becomes 0.
 *
 * @param[in,out] x Pointer to the integer to modify.
 * @param[in]     n The zero-indexed bit position to toggle (0 = LSB).
 *
 * @pre @p x must not be NULL.
 * @pre @p n must be within the valid range for the integer type.
 *
 * @post The bit at position @p n in @p *x is inverted.
 *
 * @see bit_check, bit_set, bit_unset
 */
void bit_toggle(int *x, const int n);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_BIT_H_
