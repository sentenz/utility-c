// SPDX-License-Identifier: Apache-2.0

#ifndef SRC_CRYPTO_ARGON2_H_
#define SRC_CRYPTO_ARGON2_H_

#define ARGON2_SALT_SIZE       16
#define ARGON2_HASH_SIZE       32
#define ARGON2_HASH_ITERATIONS 2
#define ARGON2_HASH_MEMORY     8
#define ARGON2_HASH_THREADS    1
#define ARGON2_ENCODED_SIZE    108

/// @brief Generates a random salt for use with Argon2.
/// @param salt A pointer to the buffer to store the salt in.
/// @return 0 on success, -1 on failure.
int argon2_generateSalt(unsigned char *salt);

/// @brief Extracts the salt from an encoded Argon2 hash.
/// @param salt A pointer to the buffer to store the salt in.
/// @param encoded The encoded Argon2 hash.
/// @return 0 on success, -1 on failure.
int argon2_extractSalt(unsigned char *salt, const char *encoded);

/// @brief Hashes a password using Argon2.
/// @param password The password to hash.
/// @param salt The salt to use for hashing.
/// @param encoded A pointer to the buffer to store the encoded hash in.
/// @return 0 on success, -1 on failure.
int argon2_hashPassword(const char *password, unsigned char *salt, unsigned char *encoded);

/// @brief Compares a password against an encoded Argon2 hash.
/// @param password The password to compare.
/// @param encoded The encoded Argon2 hash to compare against.
/// @return 0 if the password is correct, -1 otherwise.
int argon2_compareHashAndPassword(const char *password, const char *encoded);

#endif  // SRC_CRYPTO_ARGON2_H_
