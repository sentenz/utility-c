// SPDX-License-Identifier: Apache-2.0

#include "utility-c/network.h"

#include "gtest/gtest.h"

TEST(network, setIpv4Address) {
  // TODO(Sentenz) Check how to test netork interface in containers for unit tests for network functions
  GTEST_SKIP();

  typedef struct s_test {
    char *in[3];
    int want;
    int got;
  } test_t;

  test_t test[3] = {{.in = {"192.168.0.200", "eth0"}, .want = 0},
                    {.in = {"192.168.200.200", "eth0"}, .want = 0},
                    {.in = {"192.168.0.256", "eth0"}, .want = -1}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = network_setIpv4Address(test[i].in[0], test[i].in[1]);
    EXPECT_EQ(test[i].got, test[i].want);
  }
}

TEST(network, setIpv4Netmask) {
  // TODO(Sentenz) Check how to test netork interface in containers for unit tests for network functions
  GTEST_SKIP();

  typedef struct s_test {
    char *in[3];
    int want;
    int got;
  } test_t;

  test_t test[3] = {{.in = {"255.255.255.0", "eth0"}, .want = 0},
                    {.in = {"255.256.0.0", "eth0"}, .want = -1},
                    {.in = {"255.255.0.0", "eth0"}, .want = 0}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = network_setIpv4Netmask(test[i].in[0], test[i].in[1]);
    EXPECT_EQ(test[i].got, test[i].want);
  }
}

TEST(network, setIpv4Network) {
  // TODO(Sentenz) Check how to test netork interface in containers for unit tests for network functions
  GTEST_SKIP();
  typedef struct s_test {
    char *in[3];
    int want;
    int got;
  } test_t;

  test_t test[3] = {{.in = {"192.168.0.200", "255.255.255.0", "192.168.0.1"}, .want = 0},
                    {.in = {"192.200.200.200", "255.255.0.0", "192.168.200.1"}, .want = -1},
                    {.in = {"192.168.200.200", "255.255.0.0", "192.168.200.1"}, .want = 0}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = network_setIpv4Network(test[i].in[0], test[i].in[1], test[i].in[2]);
    EXPECT_EQ(test[i].got, test[i].want);
  }
}

TEST(network, setHostname) {
  // TODO(Sentenz) Check how to test netork interface in containers for unit tests for network functions
  GTEST_SKIP();

  typedef struct s_test {
    char *in;
    int want;
    int got;
  } test_t;

  // NOTE Valid characters for hostname (see string_pattern_hostname)
  // https://man7.org/linux/man-pages/man7/hostname.7.html#:~:text=Valid%20characters%20for%20hostnames%20are,to%20an%20address%20for%20use.
  test_t test[3] = {{.in = "dev-container-001", .want = 0},
                    {.in = "dev.container", .want = -1},
                    {.in = "-dev-container", .want = -1}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = network_setHostname(test[i].in);
    EXPECT_EQ(test[i].got, test[i].want);
  }
}
