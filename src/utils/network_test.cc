#include <gtest/gtest.h>

#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "utility-c/utils/network.h"

#ifdef S_POSIX
TEST(NetworkTest, SetIpv4Address)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      const char *ip;
      const char *interface;
    } in;

    struct Want
    {
      int result;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"invalid-interface", {"127.0.0.2", "invalid0"}, {-1}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    auto got = network_setIpv4Address(tc.in.ip, tc.in.interface);

    // Assert
    EXPECT_EQ(got, tc.want.result);
  }
}
#endif

TEST(NetworkTest, GetIpv4Address)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      const char *interface;
    } in;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"no-interface", {nullptr}},
    {"empty-string", {""}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    char *got = network_getIpv4Address(tc.in.interface);

    // Assert
    if (got != nullptr) {
      EXPECT_NE(std::strlen(got), 0u);
      std::free(got);
    }
  }
}

TEST(NetworkTest, GetIpv6Address)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      const char *interface;
    } in;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"no-interface", {nullptr}},
    {"empty-string", {""}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    char *got = network_getIpv6Address(tc.in.interface);

    // Assert
    if (got != nullptr) {
      EXPECT_NE(std::strlen(got), 0u);
      std::free(got);
    }
  }
}

#ifdef S_POSIX
TEST(NetworkTest, SetIpv4Netmask)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      const char *netmask;
      const char *interface;
    } in;

    struct Want
    {
      int result;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"invalid-interface", {"255.255.255.0", "invalid0"}, {-1}},
    {"empty-interface", {"255.255.255.0", ""}, {-1}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    auto got = network_setIpv4Netmask(tc.in.netmask, tc.in.interface);

    // Assert
    EXPECT_EQ(got, tc.want.result);
  }
}
#endif

TEST(NetworkTest, GetIpv4Netmask)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      const char *interface;
    } in;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"no-interface", {nullptr}},
    {"empty-string", {""}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    char *got = network_getIpv4Netmask(tc.in.interface);

    // Assert
    if (got != nullptr) {
      EXPECT_NE(std::strlen(got), 0u);
      std::free(got);
    }
  }
}

TEST(NetworkTest, GetIpv4Gateway)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"default"},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    char *got = network_getIpv4Gateway();

    // Assert
    if (got != nullptr) {
      EXPECT_NE(std::strlen(got), 0u);
      std::free(got);
    }
  }
}


TEST(NetworkTest, GetHostname)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"default"},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    char *got = network_getHostname();

    // Assert
    if (got != nullptr) {
      EXPECT_NE(std::strlen(got), 0u);
      std::free(got);
    }
  }
}

TEST(NetworkTest, SetUsername)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;

    struct In
    {
      const char *username;
    } in;

    struct Want
    {
      int result;
    } want;
  };

  // Table-Driven Testing
#ifdef S_POSIX
  const std::vector<Tests> tests = {
    {"unsupported", {"user"}, {-1}},
  };
#else
  const std::vector<Tests> tests = {
    {"not-supported", {"user"}, {0}},
  };
#endif

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    auto got = network_setUsername(tc.in.username);

    // Assert
    EXPECT_EQ(got, tc.want.result);
  }
}

TEST(NetworkTest, GetUsername)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"default"},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    // no setup needed

    // Act
    char *got = network_getUsername();

    // Assert
#ifdef S_POSIX
    if (got != nullptr) {
      EXPECT_NE(std::strlen(got), 0u);
      std::free(got);
    }
#else
    EXPECT_EQ(got, nullptr);
#endif
  }
}
