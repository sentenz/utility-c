// SPDX-License-Identifier: Apache-2.0

#include "utility-c/regex.h"

#include "gtest/gtest.h"

TEST(regex, match) {
  // TODO(Sentenz) Create unit tests for os functions
  GTEST_SKIP();
  typedef struct s_test {
    char *in[2];
    int want;
    int got;
  } test_t;

  test_t test[3] = {
      {.in = {"^[a-zA-Z0-9]*$", "Teststring123"}, .want = 0},  // check if it an alphanumeric
      {.in   = {"^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?"
                  "[0-9][0-9]?)$",
                "192.168.0.200"},
       .want = 0},  // check if it is an IPv4 address
      {.in   = {"^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?"
                  "[0-9][0-9]?)$",
                "192.168.0.256"},
       .want = -1}};  // check if it is an IPv4 address

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = regex_match(test[i].in[0], test[i].in[1]);
    EXPECT_EQ(test[i].got, test[i].want);
  }
}

TEST(regex, selectStringBetweenPatterns) {
  typedef struct s_test {
    char *in[3];
    char *want;
    char *got;
  } test_t;

  test_t test[3] = {{.in   = {"this is 192.168.0.200 my IP Address", "this is ", "my IP Address"},
                     .want = "192.168.0.200 "},
                    {.in   = {"i have dev-container-001 as a Hostname ", "i have", "as a Hostname"},
                     .want = " dev-container-001 "},
                    // FIXME test not working (arithmetic operators need \)
                    {.in = {"7!+cos(pi)*2=5042", "7!+", "*2=5042"}, .want = NULL}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = regex_selectStringBetweenPatterns(test[i].in[0], test[i].in[1], test[i].in[2]);
    EXPECT_STREQ(test[i].got, test[i].want);
    free(test[i].got);
  }
}
