// SPDX-License-Identifier: Apache-2.0

#include "src/crypto/argon2.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <argon2.h>

int argon2_generateSalt(unsigned char *salt) {
  // seed the random number generator with the current time
  srand(time(NULL));

  for (int i = 0; i < ARGON2_SALT_SIZE; ++i) {
    salt[i] = (char)rand() % 256;
  }

  return 0;
}

int argon2_extractSalt(unsigned char *salt, const char *encoded) {
  return memcpy(salt, encoded + sizeof(uint32_t), ARGON2_SALT_SIZE) == 0 ? 0 : -1;
}

int argon2_hashPassword(const char *password, unsigned char *salt, unsigned char *encoded) {
  if (password == NULL) {
    return -1;
  }

  unsigned char hash[ARGON2_HASH_SIZE];
  return argon2_hash(ARGON2_HASH_ITERATIONS,
                     ARGON2_HASH_MEMORY,
                     ARGON2_HASH_THREADS,
                     password,
                     strlen(password),
                     salt,
                     ARGON2_SALT_SIZE,
                     hash,
                     ARGON2_HASH_SIZE,
                     encoded,
                     ARGON2_ENCODED_SIZE,
                     Argon2_i,
                     ARGON2_VERSION_10);
}

int argon2_compareHashAndPassword(const char *password, const char *encoded) {
  if (password == NULL) {
    return -1;
  }

  return argon2_verify(encoded, password, strlen(password), Argon2_i);
}
