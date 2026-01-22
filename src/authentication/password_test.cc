#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <vector>

#include "utility-c/authentication/password.h"

TEST(PasswordTest, Hash)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      const char *password;
    } in;
    struct Want
    {
      bool should_hash;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"valid-password", {"secret"}, {true}},
    {"null-password", {NULL}, {false}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // No setup needed

    // Act
    std::unique_ptr<char, decltype(&free)> got(password_hash(tc.in.password), free);

    // Assert
    if (tc.want.should_hash)
    {
      ASSERT_NE(got.get(), nullptr);
      EXPECT_NE(got.get()[0], '\0');
    }
    else
    {
      EXPECT_EQ(got.get(), nullptr);
    }
  }
}

TEST(PasswordTest, Verify)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      const char *password;
      const char *verify_password;
    } in;
    struct Want
    {
      bool expect_ok;
    } want;
  };

  std::unique_ptr<char, decltype(&free)> hash(password_hash("secret"), free);
  ASSERT_NE(hash.get(), nullptr);

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"correct-password", {"secret", "secret"}, {true}},
    {"wrong-password", {"secret", "wrong"}, {false}},
    {"null-password", {NULL, "secret"}, {false}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    const char *verify = (tc.in.password == NULL) ? NULL : tc.in.verify_password;

    // Act
    int got = password_verify(verify, hash.get());

    // Assert
    if (tc.want.expect_ok)
    {
      EXPECT_EQ(got, 0);
    }
    else
    {
      EXPECT_NE(got, 0);
    }
  }
}
