#include <gtest/gtest.h>

#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include "utility-c/utils/uri.h"

TEST(UriTest, Scheme)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      const char *uri;
    } in;
    struct Want
    {
      const char *expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"http-scheme", {"http://example.com"}, {"http"}},
    {"https-scheme", {"https://example.com"}, {"https"}},
    {"ftp-scheme", {"ftp://example.com"}, {"ftp"}},
    {"no-scheme", {"example.com"}, {NULL}},
    {"empty-string", {""}, {NULL}},
    {"invalid-scheme", {"://example.com"}, {NULL}},
    {"scheme-only", {"http://"}, {"http"}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // No setup needed

    // Act
    std::unique_ptr<char, decltype(&free)> got(uri_scheme(tc.in.uri), free);

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

TEST(UriTest, Host)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      const char *uri;
    } in;
    struct Want
    {
      const char *expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"http-host", {"http://example.com"}, {"example.com"}},
    {"https-host", {"https://example.com"}, {"example.com"}},
    {"host-with-port", {"http://example.com:8080"}, {"example.com"}},
    {"host-with-user", {"http://user@example.com"}, {"example.com"}},
    {"no-host", {"http://"}, {""}},
    {"empty-string", {""}, {NULL}},
    {"invalid-uri", {"invalid"}, {NULL}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // No setup needed

    // Act
    std::unique_ptr<char, decltype(&free)> got(uri_host(tc.in.uri), free);

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

TEST(UriTest, Port)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      const char *uri;
    } in;
    struct Want
    {
      const char *expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"default-http-port", {"http://example.com"}, {NULL}},
    {"explicit-port", {"http://example.com:8080"}, {"8080"}},
    {"https-default", {"https://example.com"}, {NULL}},
    {"ftp-default", {"ftp://example.com"}, {NULL}},
    {"no-port", {"http://example.com"}, {NULL}},
    {"empty-string", {""}, {NULL}},
    {"invalid-uri", {"invalid"}, {NULL}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // No setup needed

    // Act
    std::unique_ptr<char, decltype(&free)> got(uri_port(tc.in.uri), free);

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

TEST(UriTest, User)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      const char *uri;
    } in;
    struct Want
    {
      const char *expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"with-user", {"http://user@example.com"}, {"user"}},
    {"user-password", {"http://user:pass@example.com"}, {"user:pass"}},
    {"no-user", {"http://example.com"}, {NULL}},
    {"empty-string", {""}, {NULL}},
    {"invalid-uri", {"invalid"}, {NULL}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // No setup needed

    // Act
    std::unique_ptr<char, decltype(&free)> got(uri_user(tc.in.uri), free);

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

TEST(UriTest, Query)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      const char *uri;
    } in;
    struct Want
    {
      const char *expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"with-query", {"http://example.com?key=value"}, {"key=value"}},
    {"multiple-params", {"http://example.com?key1=value1&key2=value2"}, {"key1=value1&key2=value2"}},
    {"no-query", {"http://example.com"}, {NULL}},
    {"empty-query", {"http://example.com?"}, {""}},
    {"empty-string", {""}, {NULL}},
    {"invalid-uri", {"invalid"}, {NULL}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // No setup needed

    // Act
    std::unique_ptr<char, decltype(&free)> got(uri_query(tc.in.uri), free);

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

TEST(UriTest, Fragment)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      const char *uri;
    } in;
    struct Want
    {
      const char *expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"with-fragment", {"http://example.com#section"}, {"section"}},
    {"no-fragment", {"http://example.com"}, {NULL}},
    {"empty-fragment", {"http://example.com#"}, {""}},
    {"empty-string", {""}, {NULL}},
    {"invalid-uri", {"invalid"}, {NULL}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // No setup needed

    // Act
    std::unique_ptr<char, decltype(&free)> got(uri_fragment(tc.in.uri), free);

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

TEST(UriTest, Path)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      const char *uri;
    } in;
    struct Want
    {
      const char *expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"with-path", {"http://example.com/path"}, {"path"}},
    {"nested-path", {"http://example.com/path/to/resource"}, {"path/to/resource"}},
    {"root-path", {"http://example.com/"}, {"X"}},
    {"no-path", {"http://example.com"}, {NULL}},
    {"empty-string", {""}, {NULL}},
    {"invalid-uri", {"invalid"}, {"invalid"}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // No setup needed

    // Act
    std::unique_ptr<char, decltype(&free)> got(uri_path(tc.in.uri), free);

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
