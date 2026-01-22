#include <gtest/gtest.h>

#include <vector>
#include <string>

#include "utility-c/utils/char.h"

TEST(CharTest, IsEmpty)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      const char *str;
    } in;

    struct Want
    {
      bool empty;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"null", {nullptr}, {true}},
    {"empty", {""}, {true}},
    {"non-empty", {"abc"}, {false}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    auto got = char_isEmpty(tc.in.str);

    // Assert
    EXPECT_EQ(got, tc.want.empty);
  }
}

TEST(CharTest, IsValid)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      const char *str;
    } in;

    struct Want
    {
      bool valid;
    } want;
  };

  // Table-Driven Testing
  const char nonAscii[] = {static_cast<char>(0x80), '\0'};
  const std::vector<Tests> tests = {
    {"null", {nullptr}, {false}},
    {"empty", {""}, {false}},
    {"ascii", {"abc123"}, {true}},
    {"non-ascii", {nonAscii}, {false}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    auto got = char_isValid(tc.in.str);

    // Assert
    EXPECT_EQ(got, tc.want.valid);
  }
}

TEST(CharTest, AreEqual)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      const char *str1;
      const char *str2;
    } in;

    struct Want
    {
      bool equal;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"both-equal", {"abc", "abc"}, {true}},
    {"different", {"abc", "abd"}, {false}},
    {"null-left", {nullptr, "abc"}, {false}},
    {"empty-left", {"", "abc"}, {false}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    auto got = char_areEqual(tc.in.str1, tc.in.str2);

    // Assert
    EXPECT_EQ(got, tc.want.equal);
  }
}

TEST(CharTest, ToInteger)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      const char *str;
    } in;

    struct Want
    {
      int value;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"null", {nullptr}, {0}},
    {"empty", {""}, {0}},
    {"positive", {"123"}, {123}},
    {"negative", {"-42"}, {-42}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    auto got = char_toInteger(tc.in.str);

    // Assert
    EXPECT_EQ(got, tc.want.value);
  }
}

TEST(CharTest, FromInteger)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      int value;
    } in;

    struct Want
    {
      const char *text;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"zero", {0}, {"0"}},
    {"negative", {-42}, {"-42"}},
    {"positive", {123}, {"123"}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    auto got = char_fromInteger(tc.in.value);

    // Assert
    ASSERT_NE(got, nullptr);
    EXPECT_STREQ(got, tc.want.text);
    char_free(got);
  }
}

TEST(CharTest, ToFloat)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      const char *str;
    } in;

    struct Want
    {
      double value;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"null", {nullptr}, {0.0}},
    {"empty", {""}, {0.0}},
    {"integer", {"2"}, {2.0}},
    {"float", {"3.14"}, {3.14}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    auto got = char_toFloat(tc.in.str);

    // Assert
    EXPECT_NEAR(got, tc.want.value, 1e-9);
  }
}

TEST(CharTest, FromFloat)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      double value;
    } in;

    struct Want
    {
      const char *text;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"zero", {0.0}, {"0.000000"}},
    {"fraction", {1.25}, {"1.250000"}},
    {"negative", {-2.5}, {"-2.500000"}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    auto got = char_fromFloat(tc.in.value);

    // Assert
    ASSERT_NE(got, nullptr);
    EXPECT_STREQ(got, tc.want.text);
    char_free(got);
  }
}

TEST(CharTest, FromNumber)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      double value;
    } in;

    struct Want
    {
      const char *text;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"integer", {2.0}, {"2"}},
    {"fraction", {1.25}, {"1.250000"}},
    {"negative-int", {-7.0}, {"-7"}},
    {"close-to-int", {1.0000001}, {"1.000000"}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    auto got = char_fromNumber(tc.in.value);

    // Assert
    ASSERT_NE(got, nullptr);
    EXPECT_STREQ(got, tc.want.text);
    char_free(got);
  }
}

TEST(CharTest, Concats)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      const char *a;
      const char *b;
      const char *c;
    } in;

    struct Want
    {
      const char *text;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"three-parts", {"a", "b", "c"}, {"abc"}},
    {"leading-empty", {"", "b", "c"}, {"bc"}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    auto got = char_concats(tc.in.a, tc.in.b, tc.in.c, nullptr);

    // Assert
    ASSERT_NE(got, nullptr);
    EXPECT_STREQ(got, tc.want.text);
    char_free(got);
  }
}

TEST(CharTest, TrimSpace)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      const char *str;
    } in;

    struct Want
    {
      const char *text;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"null", {nullptr}, {nullptr}},
    {"trim-both", {"  hello  "}, {"hello"}},
    {"trim-tabs", {"\t\nhi\t"}, {"hi"}},
    {"no-trim", {"hello"}, {"hello"}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    auto got = char_trimSpace(tc.in.str);

    // Assert
    if (tc.want.text == nullptr) {
      EXPECT_EQ(got, nullptr);
    } else {
      ASSERT_NE(got, nullptr);
      EXPECT_STREQ(got, tc.want.text);
      char_free(got);
    }
  }
}

TEST(CharTest, TrimNonAlphanum)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      const char *str;
    } in;

    struct Want
    {
      const char *text;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"null", {nullptr}, {nullptr}},
    {"trim-both", {"--abc--"}, {"abc"}},
    {"keep-middle", {"!!a1!!"}, {"a1"}},
    {"no-trim", {"abc"}, {"abc"}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    auto got = char_trimNonAlphanum(tc.in.str);

    // Assert
    if (tc.want.text == nullptr) {
      EXPECT_EQ(got, nullptr);
    } else {
      ASSERT_NE(got, nullptr);
      EXPECT_STREQ(got, tc.want.text);
      char_free(got);
    }
  }
}

TEST(CharTest, ToUppercase)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      const char *str;
    } in;

    struct Want
    {
      const char *text;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"null", {nullptr}, {nullptr}},
    {"mixed", {"Abc!"}, {"ABC!"}},
    {"already-upper", {"ABC"}, {"ABC"}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    auto got = char_toUppercase(tc.in.str);

    // Assert
    if (tc.want.text == nullptr) {
      EXPECT_EQ(got, nullptr);
    } else {
      ASSERT_NE(got, nullptr);
      EXPECT_STREQ(got, tc.want.text);
      char_free(got);
    }
  }
}

TEST(CharTest, ToLowercase)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      const char *str;
    } in;

    struct Want
    {
      const char *text;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"null", {nullptr}, {nullptr}},
    {"mixed", {"AbC!"}, {"abc!"}},
    {"already-lower", {"abc"}, {"abc"}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    auto got = char_toLowercase(tc.in.str);

    // Assert
    if (tc.want.text == nullptr) {
      EXPECT_EQ(got, nullptr);
    } else {
      ASSERT_NE(got, nullptr);
      EXPECT_STREQ(got, tc.want.text);
      char_free(got);
    }
  }
}

TEST(CharTest, Occurrences)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      const char *str;
      const char *sub;
    } in;

    struct Want
    {
      size_t count;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"null", {nullptr, "a"}, {0}},
    {"empty-sub", {"abc", ""}, {0}},
    {"no-match", {"abc", "z"}, {0}},
    {"non-overlap", {"abababa", "aba"}, {2}},
    {"single", {"abc", "b"}, {1}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    auto got = char_occurrences(tc.in.str, tc.in.sub);

    // Assert
    EXPECT_EQ(got, tc.want.count);
  }
}

TEST(CharTest, Replace)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      const char *str;
      const char *pre;
      const char *post;
      int count;
    } in;

    struct Want
    {
      const char *text;
      bool is_null;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"null-post", {"aa", "a", nullptr, 1}, {nullptr, true}},
    {"count-zero", {"abc", "a", "z", 0}, {"abc", false}},
    {"no-occurrence", {"abc", "z", "x", 3}, {"abc", false}},
    {"replace-all", {"aa", "a", "b", 2}, {"bb", false}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    auto got = char_replace(tc.in.str, tc.in.pre, tc.in.post, tc.in.count);

    // Assert
    if (tc.want.is_null) {
      EXPECT_EQ(got, nullptr);
    } else {
      ASSERT_NE(got, nullptr);
      EXPECT_STREQ(got, tc.want.text);
      char_free(got);
    }
  }
}

TEST(CharTest, Contains)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      const char *str;
      const char *sub;
    } in;

    struct Want
    {
      bool contains;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"null", {nullptr, "a"}, {false}},
    {"empty-sub", {"abc", ""}, {false}},
    {"missing", {"abc", "z"}, {false}},
    {"present", {"abc", "b"}, {true}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    auto got = char_contains(tc.in.str, tc.in.sub);

    // Assert
    EXPECT_EQ(got, tc.want.contains);
  }
}

TEST(CharTest, Free)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      char *str;
    } in;
  };

  // Table-Driven Testing
  Tests freeAllocated{"allocated", {char_fromInteger(123)}};
  Tests freeNull{"null", {nullptr}};
  const std::vector<Tests> tests = {freeAllocated, freeNull};

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    char_free(tc.in.str);

    // Assert
    // No assertion needed, just ensure no crash
  }
}
