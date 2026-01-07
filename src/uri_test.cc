// SPDX-License-Identifier: Apache-2.0

#include "utility-c/uri.h"

#include "gtest/gtest.h"

TEST(uri, scheme) {
  typedef struct s_test {
    char *in;
    char *want;
    char *got;
  } test_t;

  test_t test[15] = {
      {.in = "http://example.de\0", .want = "http\0"},
      {.in = "https://example.com/\0", .want = "https\0"},
      {.in = "http://example.de/path/to/object/a\0", .want = "http\0"},
      {.in = "https://example.com/path/to/object/b/\0", .want = "https\0"},
      {.in = "ftp:///home/user/config.txt\0", .want = "ftp\0"},
      {.in = "opc.tcp://localhost:4840\0", .want = "opc.tcp\0"},
      {.in = "opc.mqtt://127.0.0.1:1883/\0", .want = "opc.mqtt\0"},
      {.in = "http://foo:bar@host:123\0", .want = "http\0"},
      {.in = "http://foo:bar@host:123/\0", .want = "http\0"},
      {.in = "http://user:pass@host:123/path\0", .want = "http\0"},
      {.in = "http://foo:bar@host:123/path?query\0", .want = "http\0"},
      {.in = "http://foo:bar@host:123/path?query#fragment\0", .want = "http\0"},
      {.in = "scheme://user:pass@[v7.X]:5555/path/?query#fragment\0", .want = "scheme\0"},
      {.in = "\0", .want = NULL},
      {.in = NULL, .want = NULL}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = uri_scheme(test[i].in);
    EXPECT_STREQ(test[i].got, test[i].want);
    free(test[i].got);
  }
}

TEST(uri, host) {
  typedef struct s_test {
    char *in;
    char *want;
    char *got;
  } test_t;

  test_t test[15] = {
      {.in = "http://example.de\0", .want = "example.de\0"},
      {.in = "https://example.com/\0", .want = "example.com\0"},
      {.in = "http://example.de/path/to/object/a\0", .want = "example.de\0"},
      {.in = "https://example.com/path/to/object/b/\0", .want = "example.com\0"},
      {.in = "ftp:///home/user/config.txt\0", .want = "\0"},
      {.in = "opc.tcp://localhost:4840\0", .want = "localhost\0"},
      {.in = "opc.mqtt://127.0.0.1:1883/\0", .want = "127.0.0.1\0"},
      {.in = "http://foo:bar@host:123\0", .want = "host\0"},
      {.in = "http://foo:bar@host:123/\0", .want = "host\0"},
      {.in = "http://user:pass@host:123/path\0", .want = "host\0"},
      {.in = "http://foo:bar@host:123/path?query\0", .want = "host\0"},
      {.in = "http://foo:bar@host:123/path?query#fragment\0", .want = "host\0"},
      {.in = "scheme://user:pass@[v7.X]:5555/path/?query#fragment\0", .want = "v7.X\0"},
      {.in = "\0", .want = NULL},
      {.in = NULL, .want = NULL}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = uri_host(test[i].in);
    EXPECT_STREQ(test[i].got, test[i].want);
    free(test[i].got);
  }
}

TEST(uri, port) {
  typedef struct s_test {
    char *in;
    char *want;
    char *got;
  } test_t;

  test_t test[15] = {
      {.in = "http://example.de\0", .want = NULL},
      {.in = "https://example.com/\0", .want = NULL},
      {.in = "http://example.de/path/to/object/a\0", .want = NULL},
      {.in = "https://example.com/path/to/object/b/\0", .want = NULL},
      {.in = "ftp:///home/user/config.txt\0", .want = NULL},
      {.in = "opc.tcp://localhost:4840\0", .want = "4840\0"},
      {.in = "opc.mqtt://127.0.0.1:1883/\0", .want = "1883\0"},
      {.in = "http://foo:bar@host:123\0", .want = "123\0"},
      {.in = "http://foo:bar@host:123/\0", .want = "123\0"},
      {.in = "http://user:pass@host:123/path\0", .want = "123\0"},
      {.in = "http://foo:bar@host:123/path?query\0", .want = "123\0"},
      {.in = "http://foo:bar@host:123/path?query#fragment\0", .want = "123\0"},
      {.in = "scheme://user:pass@[v7.X]:5555/path/?query#fragment\0", .want = "5555\0"},
      {.in = "\0", .want = NULL},
      {.in = NULL, .want = NULL}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = uri_port(test[i].in);
    EXPECT_STREQ(test[i].got, test[i].want);
    free(test[i].got);
  }
}

TEST(uri, user) {
  typedef struct s_test {
    char *in;
    char *want;
    char *got;
  } test_t;

  test_t test[15] = {
      {.in = "http://example.de\0", .want = NULL},
      {.in = "https://example.com/\0", .want = NULL},
      {.in = "http://example.de/path/to/object/a\0", .want = NULL},
      {.in = "https://example.com/path/to/object/b/\0", .want = NULL},
      {.in = "ftp:///home/user/config.txt\0", .want = NULL},
      {.in = "opc.tcp://localhost:4840\0", .want = NULL},
      {.in = "opc.mqtt://127.0.0.1:1883/\0", .want = NULL},
      {.in = "http://foo:bar@host:123\0", .want = "foo:bar\0"},
      {.in = "http://foo:bar@host:123/\0", .want = "foo:bar\0"},
      {.in = "http://user:pass@host:123/path\0", .want = "user:pass\0"},
      {.in = "http://foo:bar@host:123/path?query\0", .want = "foo:bar\0"},
      {.in = "http://foo:bar@host:123/path?query#fragment\0", .want = "foo:bar\0"},
      {.in = "scheme://user:pass@[v7.X]:5555/path/?query#fragment\0", .want = "user:pass\0"},
      {.in = "\0", .want = NULL},
      {.in = NULL, .want = NULL}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = uri_user(test[i].in);
    EXPECT_STREQ(test[i].got, test[i].want);
    free(test[i].got);
  }
}

TEST(uri, query) {
  typedef struct s_test {
    char *in;
    char *want;
    char *got;
  } test_t;

  test_t test[15] = {
      {.in = "http://example.de\0", .want = NULL},
      {.in = "https://example.com/\0", .want = NULL},
      {.in = "http://example.de/path/to/object/a\0", .want = NULL},
      {.in = "https://example.com/path/to/object/b/\0", .want = NULL},
      {.in = "ftp:///home/user/config.txt\0", .want = NULL},
      {.in = "opc.tcp://localhost:4840\0", .want = NULL},
      {.in = "opc.mqtt://127.0.0.1:1883/\0", .want = NULL},
      {.in = "http://foo:bar@host:123\0", .want = NULL},
      {.in = "http://foo:bar@host:123/\0", .want = NULL},
      {.in = "http://user:pass@host:123/path\0", .want = NULL},
      {.in = "http://foo:bar@host:123/path?query\0", .want = "query\0"},
      {.in = "http://foo:bar@host:123/path?query#fragment\0", .want = "query\0"},
      {.in = "scheme://user:pass@[v7.X]:5555/path/?query#fragment\0", .want = "query\0"},
      {.in = "\0", .want = NULL},
      {.in = NULL, .want = NULL}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = uri_query(test[i].in);
    EXPECT_STREQ(test[i].got, test[i].want);
    free(test[i].got);
  }
}

TEST(uri, fragment) {
  typedef struct s_test {
    char *in;
    char *want;
    char *got;
  } test_t;

  test_t test[15] = {
      {.in = "http://example.de\0", .want = NULL},
      {.in = "https://example.com/\0", .want = NULL},
      {.in = "http://example.de/path/to/object/a\0", .want = NULL},
      {.in = "https://example.com/path/to/object/b/\0", .want = NULL},
      {.in = "ftp:///home/user/config.txt\0", .want = NULL},
      {.in = "opc.tcp://localhost:4840\0", .want = NULL},
      {.in = "opc.mqtt://127.0.0.1:1883/\0", .want = NULL},
      {.in = "http://foo:bar@host:123\0", .want = NULL},
      {.in = "http://foo:bar@host:123/\0", .want = NULL},
      {.in = "http://user:pass@host:123/path\0", .want = NULL},
      {.in = "http://foo:bar@host:123/path?query\0", .want = NULL},
      {.in = "http://foo:bar@host:123/path?query#fragment\0", .want = "fragment\0"},
      {.in = "scheme://user:pass@[v7.X]:5555/path/?query#fragment\0", .want = "fragment\0"},
      {.in = "\0", .want = NULL},
      {.in = NULL, .want = NULL}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = uri_fragment(test[i].in);
    EXPECT_STREQ(test[i].got, test[i].want);
    free(test[i].got);
  }
}

TEST(uri, path) {
  typedef struct s_test {
    char *in;
    char *want;
    char *got;
  } test_t;

  test_t test[15] = {
      {.in = "http://example.de\0", .want = NULL},
      {.in = "https://example.com/\0", .want = "X\0"},
      {.in = "http://example.de/path/to/object/a\0", .want = "path/to/object/a\0"},
      {.in = "https://example.com/path/to/object/b/\0", .want = "path/to/object/b/\0"},
      {.in = "ftp:///home/user/config.txt\0", .want = "home/user/config.txt\0"},
      {.in = "opc.tcp://localhost:4840\0", .want = NULL},
      {.in = "opc.mqtt://127.0.0.1:1883/\0", .want = "X\0"},
      {.in = "http://foo:bar@host:123\0", .want = NULL},
      {.in = "https://foo:bar@host:123/\0", .want = "X\0"},
      {.in = "http://user:pass@host:123/path\0", .want = "path\0"},
      {.in = "http://foo:bar@host:123/path?query\0", .want = "path?query\0"},
      {.in = "http://foo:bar@host:123/path?query#fragment\0", .want = "path?query#fragment\0"},
      {.in   = "scheme://user:pass@[v7.X]:5555/path/?query#fragment\0",
       .want = "path/?query#fragment\0"},
      {.in = "\0", .want = NULL},
      {.in = NULL, .want = NULL}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = uri_path(test[i].in);
    EXPECT_STREQ(test[i].got, test[i].want);
    free(test[i].got);
  }
}
