#include <gtest/gtest.h>

#include <cstring>
#include <string>
#include <vector>

#include "utility-c/crypto/argon.h"

TEST(Argon2Test, GenerateSalt)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      bool use_null;
    } in;
    struct Want
    {
      int expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"valid-salt", {false}, {0}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    unsigned char salt[ARGON2_SALT_SIZE] = {0};
    unsigned char *target = tc.in.use_null ? nullptr : salt;

    // Act
    int got = argon2_generateSalt(target);

    // Assert
    EXPECT_EQ(got, tc.want.expected);
    if (!tc.in.use_null)
    {
      bool any_nonzero = false;
      for (size_t i = 0; i < ARGON2_SALT_SIZE; ++i)
      {
        if (salt[i] != 0)
        {
          any_nonzero = true;
          break;
        }
      }
      EXPECT_TRUE(any_nonzero);
    }
  }
}

TEST(Argon2Test, HashAndVerify)
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
      int expected_hash;
      int expected_verify;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"valid-password", {"test-password"}, {0, 0}},
    {"null-password", {NULL}, {-1, -1}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    unsigned char salt[ARGON2_SALT_SIZE] = {0};
    unsigned char encoded[ARGON2_ENCODED_SIZE] = {0};
    if (tc.in.password != NULL)
    {
      ASSERT_EQ(argon2_generateSalt(salt), 0);
    }

    // Act
    int got_hash = argon2_hashPassword(tc.in.password, salt, encoded);
    int got_verify = argon2_compareHashAndPassword(tc.in.password, (char *)encoded);

    // Assert
    EXPECT_EQ(got_hash, tc.want.expected_hash);
    EXPECT_EQ(got_verify, tc.want.expected_verify);
  }
}

TEST(Argon2Test, ExtractSalt)
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
      int expected;
      bool compare;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"extract-salt", {"secret"}, {-1, true}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    unsigned char salt[ARGON2_SALT_SIZE] = {0};
    unsigned char encoded[ARGON2_ENCODED_SIZE] = {0};
    ASSERT_EQ(argon2_generateSalt(salt), 0);
    ASSERT_EQ(argon2_hashPassword(tc.in.password, salt, encoded), 0);

    unsigned char extracted[ARGON2_SALT_SIZE] = {0};

    // Act
    int got = argon2_extractSalt(extracted, (char *)encoded);

    // Assert
    EXPECT_EQ(got, tc.want.expected);
    if (tc.want.compare)
    {
      EXPECT_EQ(std::memcmp(extracted, encoded + sizeof(uint32_t), ARGON2_SALT_SIZE), 0);
    }
  }
}
