#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <vector>

#include "utility-c/utils/regex.h"

TEST(RegexTest, Match)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      const char *pattern;
      const char *str;
    } in;
    struct Want
    {
      bool expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"simple-match", {"abc", "abc"}, true},
    {"simple-no-match", {"abc", "ab"}, false},
    {"digits-match", {"[0-9]+", "12345"}, true},
    {"digits-no-match", {"[0-9]+", "abc"}, false},
    {"word-boundary", {"foo", "bar foo baz"}, true},
    {"empty-pattern", {"", ""}, true},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // No setup needed

    // Act
    bool got = regex_match(tc.in.pattern, tc.in.str);

    // Assert
    EXPECT_EQ(got, tc.want.expected);
  }
}

TEST(RegexTest, Find)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      const char *pattern;
      const char *str;
    } in;
    struct Want
    {
      const char *expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"find-digits", {"[0-9]+", "abc123def"}, {"123"}},
    {"find-word", {"foo", "bar foo baz"}, {"foo"}},
    {"find-prefix", {"^abc", "abcdef"}, {"abc"}},
    {"no-match", {"xyz", "abcdef"}, {NULL}},
    {"empty-string", {"abc", ""}, {NULL}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // No setup needed

    // Act
    std::unique_ptr<char, decltype(&free)> got(regex_find(tc.in.pattern, tc.in.str), free);

    // Assert
    if (tc.want.expected == NULL)
    {
      EXPECT_EQ(got.get(), nullptr);
    }
    else
    {
      ASSERT_NE(got.get(), nullptr);
      EXPECT_STREQ(got.get(), tc.want.expected);
    }
  }
}

TEST(RegexTest, Between)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      const char *str;
      const char *left;
      const char *right;
    } in;
    struct Want
    {
      const char *expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"simple-between", {"start middle end", "start ", " end"}, {"middle"}},
    {"no-left", {"middle end", "start ", " end"}, {NULL}},
    {"no-right", {"start middle", "start ", " end"}, {NULL}},
    {"empty-between", {"start  end", "start ", " end"}, {""}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // No setup needed

    // Act
    std::unique_ptr<char, decltype(&free)> got(regex_between(tc.in.str, tc.in.left, tc.in.right), free);

    // Assert
    if (tc.want.expected == NULL)
    {
      EXPECT_EQ(got.get(), nullptr);
    }
    else
    {
      ASSERT_NE(got.get(), nullptr);
      EXPECT_STREQ(got.get(), tc.want.expected);
    }
  }
}
