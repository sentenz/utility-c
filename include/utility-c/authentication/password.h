// SPDX-License-Identifier: Apache-2.0

#ifndef INCLUDE_UTILITY_C_AUTHENTICATION_PASSWORD_H_
#define INCLUDE_UTILITY_C_AUTHENTICATION_PASSWORD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Hashes a password using a secure algorithm.
/// @param password The password to hash.
/// @return A pointer to the hashed password.
/// @note The caller is responsible for freeing the memory using free().
char *password_hash(const char *password);

/// @brief Verifies a password against a hashed password.
/// @param password The password to verify.
/// @param hash The hashed password to verify against.
/// @return 1 if the password is correct, 0 otherwise.
int password_verify(const char *password, const char *hash);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_AUTHENTICATION_PASSWORD_H_
