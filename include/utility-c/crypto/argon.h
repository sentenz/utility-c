// SPDX-License-Identifier: Apache-2.0

#ifndef INCLUDE_UTILITY_C_CRYPTO_ARGON_H_
#define INCLUDE_UTILITY_C_CRYPTO_ARGON_H_

/**
 * @file argon.h
 * @brief Argon2 password hashing functions.
 *
 * This module provides low-level functions for the Argon2 password hashing
 * algorithm, which won the Password Hashing Competition in 2015. Argon2 is
 * designed to be resistant to GPU-based attacks and side-channel attacks.
 *
 * Algorithm Parameters:
 * - Salt size: 16 bytes (128 bits) - sufficient randomness
 * - Hash size: 32 bytes (256 bits) - secure output length
 * - Iterations: 2 - time cost (increase for slower but more secure hashing)
 * - Memory: 8 KB - memory cost (increase for better GPU resistance)
 * - Threads: 1 - parallelism factor
 *
 * @note For most use cases, prefer the higher-level password.h interface.
 * @note This implementation uses Argon2id variant for optimal security.
 *
 * @warning Incorrect use of cryptographic functions can compromise security.
 *          Ensure you understand the parameters before modifying defaults.
 *
 * @code
 * unsigned char salt[ARGON2_SALT_SIZE];
 * unsigned char encoded[ARGON2_ENCODED_SIZE];
 *
 * // Generate salt and hash password
 * if (argon2_generateSalt(salt) == 0) {
 *     if (argon2_hashPassword("my_password", salt, encoded) == 0) {
 *         // Store 'encoded' in database
 *         printf("Hash: %s\n", encoded);
 *     }
 * }
 *
 * // Later, verify password
 * if (argon2_compareHashAndPassword("my_password", (char *)encoded) == 0) {
 *     printf("Password verified!\n");
 * }
 * @endcode
 *
 * @see https://github.com/P-H-C/phc-winner-argon2
 * @see https://datatracker.ietf.org/doc/html/rfc9106
 *
 * @copyright Copyright (c) 2023-2026 sentenz
 * @license SPDX-License-Identifier: Apache-2.0
 */

/**
 * @brief Size of the salt in bytes.
 *
 * 16 bytes (128 bits) provides sufficient randomness for unique salts.
 */
#define ARGON2_SALT_SIZE 16

/**
 * @brief Size of the raw hash output in bytes.
 *
 * 32 bytes (256 bits) provides a secure hash length.
 */
#define ARGON2_HASH_SIZE 32

/**
 * @brief Number of iterations (time cost).
 *
 * Higher values increase computation time, making brute-force attacks
 * more expensive. Default of 2 balances security and performance.
 */
#define ARGON2_HASH_ITERATIONS 2

/**
 * @brief Memory cost in kibibytes.
 *
 * Higher values require more memory, making parallel attacks on GPUs
 * more difficult. Default of 8 KB is suitable for resource-constrained
 * environments.
 */
#define ARGON2_HASH_MEMORY 8

/**
 * @brief Parallelism factor (number of threads).
 *
 * Number of parallel threads to use during hashing.
 */
#define ARGON2_HASH_THREADS 1

/**
 * @brief Maximum size of the encoded hash string.
 *
 * The encoded format includes algorithm identifier, version, parameters,
 * salt, and hash in a single string.
 */
#define ARGON2_ENCODED_SIZE 108

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Generates a cryptographically secure random salt.
 *
 * Fills the provided buffer with random bytes suitable for use as a
 * password salt. Uses the system's cryptographic random number generator.
 *
 * @param[out] salt Buffer to store the generated salt.
 *                  Must be at least ARGON2_SALT_SIZE bytes.
 *
 * @retval 0  Success - salt buffer filled with random bytes.
 * @retval -1 Failure (e.g., random source unavailable).
 *
 * @pre @p salt must point to a buffer of at least ARGON2_SALT_SIZE bytes.
 *
 * @post On success, @p salt contains ARGON2_SALT_SIZE random bytes.
 *
 * @see argon2_hashPassword, argon2_extractSalt
 */
int argon2_generateSalt(unsigned char *salt);

/**
 * @brief Extracts the salt from an encoded Argon2 hash.
 *
 * Parses the encoded hash string and extracts the salt component.
 * This is useful when you need to rehash with the same salt for
 * comparison or migration purposes.
 *
 * @param[out] salt    Buffer to store the extracted salt.
 *                     Must be at least ARGON2_SALT_SIZE bytes.
 * @param[in]  encoded The null-terminated encoded Argon2 hash string.
 *
 * @retval 0  Success - salt extracted to buffer.
 * @retval -1 Failure (e.g., invalid encoded format).
 *
 * @pre @p salt must point to a buffer of at least ARGON2_SALT_SIZE bytes.
 * @pre @p encoded must be a valid Argon2 encoded hash string.
 *
 * @see argon2_generateSalt, argon2_hashPassword
 */
int argon2_extractSalt(unsigned char *salt, const char *encoded);

/**
 * @brief Hashes a password using Argon2id.
 *
 * Computes the Argon2id hash of the password with the provided salt
 * and outputs the result in encoded format suitable for storage.
 *
 * @param[in]  password The null-terminated password to hash.
 * @param[in]  salt     The salt to use (ARGON2_SALT_SIZE bytes).
 * @param[out] encoded  Buffer to store the encoded hash.
 *                      Must be at least ARGON2_ENCODED_SIZE bytes.
 *
 * @retval 0  Success - password hashed and stored in encoded buffer.
 * @retval -1 Failure (e.g., memory allocation error).
 *
 * @pre @p password must be a valid null-terminated string.
 * @pre @p salt must contain ARGON2_SALT_SIZE bytes.
 * @pre @p encoded must point to a buffer of at least ARGON2_ENCODED_SIZE bytes.
 *
 * @post On success, @p encoded contains the null-terminated encoded hash.
 *
 * @note The encoded output format is:
 *       $argon2id$v=19$m=8,t=2,p=1$<salt>$<hash>
 *
 * @see argon2_generateSalt, argon2_compareHashAndPassword
 */
int argon2_hashPassword(const char *password, unsigned char *salt, unsigned char *encoded);

/**
 * @brief Verifies a password against an encoded Argon2 hash.
 *
 * Compares the provided password against the encoded hash by extracting
 * the salt and parameters from the hash, recomputing the hash, and
 * comparing in constant time.
 *
 * @param[in] password The null-terminated password to verify.
 * @param[in] encoded  The null-terminated encoded Argon2 hash.
 *
 * @retval 0  Password matches the hash (verification successful).
 * @retval -1 Password does not match or an error occurred.
 *
 * @pre @p password must be a valid null-terminated string.
 * @pre @p encoded must be a valid Argon2 encoded hash string.
 *
 * @note The comparison is performed in constant time to prevent
 *       timing attacks.
 *
 * @see argon2_hashPassword
 */
int argon2_compareHashAndPassword(const char *password, const char *encoded);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_CRYPTO_ARGON_H_
