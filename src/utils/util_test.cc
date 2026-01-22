// SPDX-License-Identifier: Apache-2.0

#include "utility-c/util.h"

#include "gtest/gtest.h"

TEST(util, isFloat) {
  typedef struct s_test {
    double in;
    bool want;
    bool got;
  } test_t;

  test_t test[6] = {{.in = 0.000, .want = false},
                    {.in = 3.14159, .want = true},
                    {.in = -3.14159, .want = true},
                    {.in = 11235, .want = false},
                    {.in = -11235, .want = false},
                    {.in = 11235813, .want = false}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = util_isFloat(test[i].in);
    EXPECT_EQ(test[i].got, test[i].want);
  }
}

TEST(util, countDigits) {
  typedef struct s_test {
    int in;
    int want;
    int got;
  } test_t;

  test_t test[4] = {{.in = 0, .want = 1},
                    {.in = 11235, .want = 5},
                    {.in = -11235, .want = 5},
                    {.in = 11235813, .want = 8}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = util_countDigits(test[i].in);
    EXPECT_EQ(test[i].got, test[i].want);
  }
}

TEST(util, countFloat) {
  typedef struct s_test {
    double in;
    int want;
    int got;
  } test_t;

  test_t test[5] = {{.in = 0.000, .want = 0},
                    {.in = 333, .want = 0},
                    {.in = 0.00001, .want = 6},
                    {.in = 3.14159, .want = 6},
                    {.in = -3.14159, .want = 6}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = util_countFloat(test[i].in);
    EXPECT_EQ(test[i].got, test[i].want);
  }
}

TEST(util, countNumber) {
  typedef struct s_test {
    double in;
    int want;
    int got;
  } test_t;

  test_t test[5] = {{.in = 0.000, .want = 1},
                    {.in = 333, .want = 3},
                    {.in = 0.00001, .want = 6},
                    {.in = 3.14159, .want = 6},
                    {.in = -3.14159, .want = 6}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = util_countNumber(test[i].in);
    EXPECT_EQ(test[i].got, test[i].want);
  }
}

TEST(util, approximatelyEqual) {
  typedef struct s_test {
    float in[3];
    bool want;
    bool got;
  } test_t;

  test_t test[3] = {{.in = {95.0, 100.0, 0.05}, .want = true},
                    {.in = {94.9, 100.0, 0.05}, .want = false},
                    {.in = {10.0, 20.0, 0.5}, .want = true}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = util_approximatelyEqual(test[i].in[0], test[i].in[1], test[i].in[2]);
    EXPECT_EQ(test[i].got, test[i].want);
  }
}

TEST(util, essentiallyEqual) {
  typedef struct s_test {
    float in[3];
    bool want;
    bool got;
  } test_t;

  test_t test[3] = {{.in = {95.0, 100.0, 0.05}, .want = false},
                    {.in = {95.5, 100.0, 0.05}, .want = true},
                    {.in = {10.1, 15.1, 0.5}, .want = true}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = util_essentiallyEqual(test[i].in[0], test[i].in[1], test[i].in[2]);
    EXPECT_EQ(test[i].got, test[i].want);
  }
}

TEST(util, definitelyGreaterThan) {
  typedef struct s_test {
    float in[3];
    bool want;
    bool got;
  } test_t;

  test_t test[3] = {{.in = {100.0, 94.9, 0.05}, .want = true},
                    {.in = {100.0, 95.0, 0.05}, .want = false},
                    {.in = {100, 49.9, 0.5}, .want = true}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = util_definitelyGreaterThan(test[i].in[0], test[i].in[1], test[i].in[2]);
    EXPECT_EQ(test[i].got, test[i].want);
  }
}

TEST(util, definitelyLessThan) {
  typedef struct s_test {
    float in[3];
    bool want;
    bool got;
  } test_t;

  test_t test[3] = {{.in = {94.9, 100.0, 0.05}, .want = true},
                    {.in = {95.0, 100.0, 0.05}, .want = false},
                    {.in = {24.9, 50.0, 0.5}, .want = true}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = util_definitelyLessThan(test[i].in[0], test[i].in[1], test[i].in[2]);
    EXPECT_EQ(test[i].got, test[i].want);
  }
}
