// SPDX-License-Identifier: Apache-2.0

#ifndef INCLUDE_UTILITY_C_BIT_H_
#define INCLUDE_UTILITY_C_BIT_H_

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

/// @brief Checks if a bit is set.
/// @param x The integer to check.
/// @param n The bit position to check.
/// @return true if the bit is set, false otherwise.
bool bit_check(const int x, const int n);

/// @brief Sets a bit.
/// @param x A pointer to the integer to modify.
/// @param n The bit position to set.
/// @return true if the bit was set, false otherwise.
bool bit_set(int *x, const int n);

/// @brief Unsets a bit.
/// @param x A pointer to the integer to modify.
/// @param n The bit position to unset.
/// @return true if the bit was unset, false otherwise.
bool bit_unset(int *x, const int n);

/// @brief Toggles a bit.
/// @param x A pointer to the integer to modify.
/// @param n The bit position to toggle.
void bit_toggle(int *x, const int n);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_BIT_H_
