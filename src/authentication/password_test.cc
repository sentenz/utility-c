// SPDX-License-Identifier: Apache-2.0

#include "utility-c/authentication/password.h"

#include "gtest/gtest.h"

static void teardown(char *str) {
  if (str != NULL) {
    free(str);
  }
}

TEST(password, hash) {
  typedef struct s_test {
    char *in;
    int want;
    int got;
  } test_t;

  test_t tests[6] = {{.in = "password\0", .want = 0},
                     {.in = "⌘{čřžŧ¶'`[łĐŧđĐ¶\0", .want = 0},
                     {.in = "  \0", .want = 0},
                     {.in = "\0", .want = 0},
                     {.in = "", .want = 0},
                     {.in = NULL, .want = -1}};

  for (auto &test : tests) {
    char *hash = password_hash(test.in);
    test.got   = password_verify(test.in, hash);
    EXPECT_EQ(test.got, test.want);

    teardown(hash);
  }
}

TEST(password, verify) {
  // TODO(Sentenz) Create unit tests for password functions
  GTEST_SKIP();
}
