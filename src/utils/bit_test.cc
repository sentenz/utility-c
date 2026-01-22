// SPDX-License-Identifier: Apache-2.0

#include "utility-c/utils/bit.h"

#include <gtest/gtest.h>

#include <vector>
#include <string>

TEST(BitTest, Check)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      int x;
      int n;
    } in;

    struct Want
    {
      bool expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"bit 0 in 1", {1, 0}, {true}},
    {"bit 1 in 1", {1, 1}, {false}},
    {"bit 0 in 2", {2, 0}, {false}},
    {"bit 1 in 2", {2, 1}, {true}},
    {"bit 30 in INT_MAX", {INT_MAX, 30}, {true}},
    {"bit 31 in INT_MAX", {INT_MAX, 31}, {false}},  // sign bit
    {"bit 0 in 0", {0, 0}, {false}},
    {"bit 0 in -1", {-1, 0}, {true}},
    {"bit 31 in -1", {-1, 31}, {true}},
    {"bit 0 in 4", {4, 0}, {false}},
    {"bit 2 in 4", {4, 2}, {true}},
    {"bit 0 in 8", {8, 0}, {false}},
    {"bit 3 in 8", {8, 3}, {true}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Act
    auto got = bit_check(tc.in.x, tc.in.n);

    // Assert
    EXPECT_EQ(got, tc.want.expected);
  }
}

TEST(BitTest, Set)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      int initial_x;
      int n;
    } in;

    struct Want
    {
      bool return_value;
      int final_x;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"set bit 0 in 0", {0, 0}, {true, 1}},
    {"set bit 1 in 0", {0, 1}, {true, 2}},
    {"set bit 0 in 1", {1, 0}, {true, 1}},  // already set, but still true
    {"set bit 1 in 1", {1, 1}, {true, 3}},
    {"set bit 0 in -1", {-1, 0}, {true, -1}},  // already set
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    int x = tc.in.initial_x;

    // Act
    auto got = bit_set(&x, tc.in.n);

    // Assert
    EXPECT_EQ(got, tc.want.return_value);
    EXPECT_EQ(x, tc.want.final_x);
  }
}

TEST(BitTest, Unset)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      int initial_x;
      int n;
    } in;

    struct Want
    {
      bool return_value;
      int final_x;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"unset bit 0 in 1", {1, 0}, {true, 0}},
    {"unset bit 1 in 2", {2, 1}, {true, 0}},
    {"unset bit 0 in 0", {0, 0}, {true, 0}},  // already unset, but still true
    {"unset bit 1 in 1", {1, 1}, {true, 1}},  // already unset
    {"unset bit 0 in -1", {-1, 0}, {true, -2}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    int x = tc.in.initial_x;

    // Act
    auto got = bit_unset(&x, tc.in.n);

    // Assert
    EXPECT_EQ(got, tc.want.return_value);
    EXPECT_EQ(x, tc.want.final_x);
  }
}

TEST(BitTest, Toggle)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      int initial_x;
      int n;
    } in;

    struct Want
    {
      int final_x;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"toggle bit 0 in 0", {0, 0}, {1}},
    {"toggle bit 0 in 1", {1, 0}, {0}},
    {"toggle bit 1 in 0", {0, 1}, {2}},
    {"toggle bit 1 in 2", {2, 1}, {0}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    int x = tc.in.initial_x;

    // Act
    bit_toggle(&x, tc.in.n);

    // Assert
    EXPECT_EQ(x, tc.want.final_x);
  }
}
