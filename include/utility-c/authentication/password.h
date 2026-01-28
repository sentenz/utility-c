// SPDX-License-Identifier: Apache-2.0

/**
 * @file password.h
 * @brief Password hashing and verification utilities.
 *
 * This module provides secure password hashing and verification functions
 * suitable for storing user credentials. The implementation uses a secure
 * hashing algorithm (typically Argon2) that is resistant to brute-force
 * attacks.
 *
 * @warning Never store passwords in plain text. Always use password_hash()
 *          to create a secure hash before storage.
 *
 * @note Password hashes are designed to be stored as strings and include
 *       all necessary parameters for verification.
 *
 * @example
 * @code
 * // Registering a new user
 * const char *password = "user_password";
 * char *hash = password_hash(password);
 * if (hash) {
 *     // Store 'hash' in database
 *     save_to_database(username, hash);
 *     free(hash);
 * }
 *
 * // Authenticating a user
 * const char *stored_hash = get_from_database(username);
 * const char *entered_password = get_user_input();
 * if (password_verify(entered_password, stored_hash) == 1) {
 *     printf("Authentication successful\n");
 * } else {
 *     printf("Invalid password\n");
 * }
 * @endcode
 *
 * @see argon.h for lower-level Argon2 functions
 */

#ifndef INCLUDE_UTILITY_C_AUTHENTICATION_PASSWORD_H_
#define INCLUDE_UTILITY_C_AUTHENTICATION_PASSWORD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Hashes a password using a secure algorithm.
 *
 * Creates a cryptographically secure hash of the provided password.
 * The hash includes a random salt and algorithm parameters, making
 * it suitable for secure storage.
 *
 * @param[in] password The null-terminated password string to hash.
 *
 * @return A newly allocated null-terminated string containing the
 *         password hash in encoded format.
 * @return NULL on failure (e.g., memory allocation error, invalid input).
 *
 * @pre @p password must be a valid null-terminated string.
 *
 * @note The caller is responsible for freeing the returned string using free().
 * @note The hash includes salt and algorithm parameters for self-contained
 *       verification.
 * @note Hash output length and format depend on the underlying algorithm.
 *
 * @warning The input password should be securely erased from memory after
 *          hashing if security requirements demand it.
 *
 * @see password_verify
 */
char *password_hash(const char *password);

/**
 * @brief Verifies a password against a stored hash.
 *
 * Compares a plain-text password against a previously generated hash
 * to determine if they match. The verification is performed in constant
 * time to prevent timing attacks.
 *
 * @param[in] password The null-terminated password to verify.
 * @param[in] hash     The null-terminated hash string to verify against
 *                     (as returned by password_hash()).
 *
 * @return 1 if the password matches the hash.
 * @return 0 if the password does not match or an error occurred.
 *
 * @pre @p password must be a valid null-terminated string.
 * @pre @p hash must be a valid hash string produced by password_hash().
 *
 * @note This function extracts the salt and parameters from the hash
 *       automatically.
 * @note Verification time is constant regardless of where the mismatch
 *       occurs, preventing timing attacks.
 *
 * @warning Do not use string comparison to verify passwords. Always use
 *          this function.
 *
 * @see password_hash
 */
int password_verify(const char *password, const char *hash);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_AUTHENTICATION_PASSWORD_H_
