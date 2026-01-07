// SPDX-License-Identifier: Apache-2.0

#include "utility-c/authentication/password.h"

#include "src/crypto/argon2.h"

char *password_hash(const char *password) {
  unsigned char salt[ARGON2_SALT_SIZE];
  if (argon2_generateSalt(salt) != 0) {
    return NULL;
  }

  unsigned char encoded[ARGON2_ENCODED_SIZE];
  if (argon2_hashPassword(password, salt, encoded) != 0) {
    return NULL;
  }

  return strdup(encoded);
}

int password_verify(const char *password, const char *hash) {
  return argon2_compareHashAndPassword(password, hash);
}
