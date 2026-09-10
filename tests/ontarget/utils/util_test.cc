// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>

#include <climits>
#include <string>
#include <vector>

#include "utility-c/utils/util.h"

TEST(UtilTest, IsFloat)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      double n;
    } in;

    struct Want
    {
      bool expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"integer-zero",     {0.0},         {false}},
    {"integer-positive", {5.0},         {false}},
    {"integer-negative", {-3.0},        {false}},
    {"float-positive",   {3.14},        {true}},
    {"float-negative",   {-2.5},        {true}},
    {"float-small",      {0.001},       {true}},
    {"large-integer",    {1000000.0},   {false}},
    {"large-float",      {1000000.123}, {true}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Act
    bool got = util_isFloat(tc.in.n);

    // Assert
    EXPECT_EQ(got, tc.want.expected);
  }
}

TEST(UtilTest, CountDigits)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      int n;
    } in;

    struct Want
    {
      int expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"zero",             {0},          {1}},
    {"single-digit",     {5},          {1}},
    {"two-digits",       {42},         {2}},
    {"three-digits",     {123},        {3}},
    {"negative",         {-56},        {2}},
    {"large-number",     {123456789},  {9}},
    {"max-int",          {2147483647}, {10}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Act
    int got = util_countDigits(tc.in.n);

    // Assert
    EXPECT_EQ(got, tc.want.expected);
  }
}

TEST(UtilTest, CountFloat)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      double n;
    } in;

    struct Want
    {
      int expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"one-decimal",    {3.1},       {2}},
    {"two-decimals",   {3.14},      {3}},
    {"three-decimals", {3.141},     {4}},
    {"negative-float", {-2.5},      {2}},
    {"small-float",    {0.001},     {4}},
    {"trailing-zero",  {1.100},     {2}},
    {"many-decimals",  {3.1415927}, {8}},
    {"integer-input",  {5.0},       {0}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Act
    int got = util_countFloat(tc.in.n);

    // Assert
    EXPECT_EQ(got, tc.want.expected);
  }
}

TEST(UtilTest, ApproximatelyEqual)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      float a;
      float b;
      float epsilon;
    } in;

    struct Want
    {
      bool expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"equal",          {1.0f,  1.0f,    0.01f}, {true}},
    {"close",          {1.0f,  1.001f,  0.01f}, {true}},
    {"not-close",      {1.0f,  1.1f,    0.01f}, {false}},
    {"negative-equal", {-1.0f, -1.0f,   0.01f}, {true}},
    {"negative-close", {-1.0f, -1.001f, 0.01f}, {true}},
    {"zero-epsilon",   {1.0f,  1.0001f, 0.0f},  {false}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Act
    bool got = util_approximatelyEqual(tc.in.a, tc.in.b, tc.in.epsilon);

    // Assert
    EXPECT_EQ(got, tc.want.expected);
  }
}

TEST(UtilTest, DefinitelyGreaterThan)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      float a;
      float b;
      float epsilon;
    } in;

    struct Want
    {
      bool expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"clearly-greater",   {1.1f,  1.0f,  0.01f}, {true}},
    {"not-greater",       {1.001f, 1.0f, 0.01f}, {false}},
    {"equal",             {1.0f,  1.0f,  0.01f}, {false}},
    {"negative-greater",  {-1.0f, -1.1f, 0.01f}, {true}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Act
    bool got = util_definitelyGreaterThan(tc.in.a, tc.in.b, tc.in.epsilon);

    // Assert
    EXPECT_EQ(got, tc.want.expected);
  }
}

TEST(UtilTest, DefinitelyLessThan)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      float a;
      float b;
      float epsilon;
    } in;

    struct Want
    {
      bool expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"clearly-less",   {0.9f,  1.0f,  0.01f}, {true}},
    {"not-less",       {1.001f, 1.0f, 0.01f}, {false}},
    {"equal",          {1.0f,  1.0f,  0.01f}, {false}},
    {"negative-less",  {-1.1f, -1.0f, 0.01f}, {true}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Act
    bool got = util_definitelyLessThan(tc.in.a, tc.in.b, tc.in.epsilon);

    // Assert
    EXPECT_EQ(got, tc.want.expected);
  }
}
