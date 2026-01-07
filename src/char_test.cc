// SPDX-License-Identifier: Apache-2.0

#include "utility-c/char.h"

#include "gtest/gtest.h"

static void teardown(char *str) {
  char_free(str);
}

TEST(char, isEmpty) {
  typedef struct s_test {
    char *in;
    bool want;
    bool got;
  } test_t;

  test_t test[6] = {{.in = "Not NULL\0", .want = false},
                    {.in = "⌘{čřžŧ¶'`[łĐŧđĐ¶\0", .want = false},
                    {.in = "  \0", .want = false},
                    {.in = "\0", .want = true},
                    {.in = "", .want = true},
                    {.in = NULL, .want = true}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = char_isEmpty(test[i].in);
    EXPECT_EQ(test[i].got, test[i].want);
  }
}

TEST(char, isValid) {
  typedef struct s_test {
    char *in;
    bool want;
    bool got;
  } test_t;

  test_t test[5] = {{.in = "⌘{čřžŧ¶'`[łĐŧđĐ¶←^€~[←^ø{&}čž\0", .want = false},
                    {.in = "abcABC1123\0", .want = true},
                    {.in = "@{}[]().:;_-+~*!%$#\0", .want = true},
                    {.in = "\0", .want = false},
                    {.in = NULL, .want = false}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = char_isValid(test[i].in);
    EXPECT_EQ(test[i].got, test[i].want);
  }
}

TEST(char, areEqual) {
  typedef struct s_test {
    char *in[2];
    bool want;
    bool got;
  } test_t;

  test_t test[10] = {{.in = {"unit\0", "unit\0"}, .want = true},
                     {.in = {"unit test\0", "unit test\0"}, .want = true},
                     {.in = {"11235\0", "11235\0"}, .want = true},
                     {.in = {"@{}[]().:;_-+~*!%$#\0", "@{}[]().:;_-+~*!%$#\0"}, .want = true},
                     {.in = {"⌘{čřžŧ¶'`[łĐŧđĐ¶\0", "⌘{čřžŧ¶'`[łĐŧđĐ¶\0"}, .want = true},
                     {.in = {"unit\0", "test\0"}, .want = false},
                     {.in = {"  \0", "  \0"}, .want = true},
                     {.in = {"\0", "\0"}, .want = false},
                     {.in = {"\0", NULL}, .want = false},
                     {.in = {NULL, NULL}, .want = false}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = char_areEqual(test[i].in[0], test[i].in[1]);
    EXPECT_EQ(test[i].got, test[i].want);
  }
}

TEST(char, toInteger) {
  typedef struct s_test {
    char *in;
    int want;
    int got;
  } test_t;

  test_t test[14] = {{.in = "11235\0", .want = 11235},
                     {.in = " +11235 \0", .want = 11235},
                     {.in = "-11235\0", .want = -11235},
                     {.in = " +3.14 \0", .want = 3},
                     {.in = "-3.14\0", .want = -3},
                     {.in = "++11235\0", .want = 0},
                     {.in = "--11235\0", .want = 0},
                     {.in = "0x2BE3\0", .want = 0},
                     {.in = "0\0", .want = 0},
                     {.in = "-0\0", .want = 0},
                     {.in = "No+11235Integer\0", .want = 0},
                     {.in = "⌘{čřžŧ¶'`[łĐŧđĐ¶\0", .want = 0},
                     {.in = "\0", .want = 0},
                     {.in = NULL, .want = 0}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = char_toInteger(test[i].in);
    EXPECT_EQ(test[i].got, test[i].want);
  }
}

TEST(char, fromInteger) {
  typedef struct s_test {
    int in;
    char *want;
    char *got;
  } test_t;

  test_t test[5] = {{.in = 11235, .want = "11235\0"},
                    {.in = +11235, .want = "11235\0"},
                    {.in = -11235, .want = "-11235\0"},
                    {.in = -0, .want = "0\0"},
                    {.in = 0, .want = "0\0"}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = char_fromInteger(test[i].in);
    EXPECT_STREQ(test[i].got, test[i].want);

    teardown(test[i].got);
  }
}

TEST(char, toFloat) {
  typedef struct s_test {
    char *in;
    double want;
    double got;
  } test_t;

  test_t test[14] = {{.in = "3.14159\0", .want = 3.14159},
                     {.in = " +3.14159 \0", .want = 3.14159},
                     {.in = "-3.14159\0", .want = -3.14159},
                     {.in = "++3.14159\0", .want = 0},
                     {.in = "--3.14159\0", .want = 0},
                     {.in = "0x2BE3\0", .want = 11235.0},
                     {.in = "11235\0", .want = 11235.0},
                     {.in = "+0.0\0", .want = 0},
                     {.in = "0\0", .want = 0},
                     {.in = "-0\0", .want = 0},
                     {.in = "No Float\0", .want = 0},
                     {.in = "⌘{čřžŧ¶'`[łĐŧđĐ¶\0", .want = 0},
                     {.in = "\0", .want = 0},
                     {.in = NULL, .want = 0}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = char_toFloat(test[i].in);
    EXPECT_EQ(test[i].got, test[i].want);
  }
}

TEST(char, fromFloat) {
  typedef struct s_test {
    double in;
    char *want;
    char *got;
  } test_t;

  test_t test[7] = {{.in = 3.14159, .want = "3.14159\0"},
                    {.in = +3.14159, .want = "3.14159\0"},
                    {.in = -3.14159, .want = "-3.14159\0"},
                    {.in = 333, .want = NULL},
                    {.in = -333, .want = NULL},
                    {.in = -0, .want = NULL},
                    {.in = 0, .want = NULL}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = char_fromFloat(test[i].in);
    EXPECT_STREQ(test[i].got, test[i].want);

    teardown(test[i].got);
  }
}

TEST(char, fromNumber) {
  typedef struct s_test {
    double in;
    char *want;
    char *got;
  } test_t;

  test_t test[7] = {{.in = 3.14159, .want = "3.14159\0"},
                    {.in = +3.14159, .want = "3.14159\0"},
                    {.in = -3.14159, .want = "-3.14159\0"},
                    {.in = 333, .want = "333\0"},
                    {.in = -333, .want = "-333\0"},
                    {.in = -0, .want = "0\0"},
                    {.in = 0, .want = "0\0"}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = char_fromNumber(test[i].in);
    EXPECT_STREQ(test[i].got, test[i].want);

    teardown(test[i].got);
  }
}

TEST(char, concats) {
  typedef struct s_test {
    char *in[3];
    char *want;
    char *got;
  } test_t;

  test_t test[5] = {{.in = {"simple\0", "unit\0", "test\0"}, .want = "simpleunittest\0"},
                    {.in = {"⌘{čřž\0", "ŧ¶'`\0", "[łĐŧđĐ¶\0"}, .want = "⌘{čřžŧ¶'`[łĐŧđĐ¶\0"},
                    {.in = {"150.3\0", "*\0", "15.5\0"}, .want = "150.3*15.5\0"},
                    {.in = {"1123\0", "5813\0", "2134\0"}, .want = "112358132134\0"},
                    {.in = {"util\0", "_function_\0", "test\0"}, .want = "util_function_test\0"}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = char_concats(test[i].in[0], test[i].in[1], test[i].in[2], NULL);
    EXPECT_STREQ(test[i].got, test[i].want);

    teardown(test[i].got);
  }
}

TEST(char, trimSpace) {
  typedef struct s_test {
    char *in;
    char *want;
    char *got;
  } test_t;

  test_t test[9] = {{.in = "  leading space\0", .want = "leading space\0"},
                    {.in = "trailing space  \0", .want = "trailing space\0"},
                    {.in = "  both space  \0", .want = "both space\0"},
                    {.in = "no space", .want = "no space"},
                    {.in = " ⌘{čřžŧ¶'`[łĐŧđĐ¶ ", .want = "⌘{čřžŧ¶'`[łĐŧđĐ¶"},
                    {.in = "!@#hello world123$%^", .want = "!@#hello world123$%^"},
                    {.in = "  \0", .want = NULL},
                    {.in = "\0", .want = NULL},
                    {.in = NULL, .want = NULL}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = char_trimSpace(test[i].in);
    EXPECT_STREQ(test[i].got, test[i].want);

    teardown(test[i].got);
  }
}

TEST(char, trimNonAlphanum) {
  typedef struct s_test {
    char *in;
    char *want;
    char *got;
  } test_t;

  test_t test[9] = {{.in = "  leading space\0", .want = "leading space\0"},
                    {.in = "trailing space  \0", .want = "trailing space\0"},
                    {.in = "  both space  \0", .want = "both space\0"},
                    {.in = "no space", .want = "no space"},
                    {.in = " ⌘{čřžŧ¶'`[łĐŧđĐ¶ ", .want = NULL},
                    {.in = "!@#hello world123$%^", .want = "hello world123"},
                    {.in = "  \0", .want = NULL},
                    {.in = "\0", .want = NULL},
                    {.in = NULL, .want = NULL}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = char_trimNonAlphanum(test[i].in);
    EXPECT_STREQ(test[i].got, test[i].want);

    teardown(test[i].got);
  }
}

TEST(char, toUppercase) {
  typedef struct s_test {
    char *in;
    char *want;
    char *got;
  } test_t;

  test_t test[8] = {{.in = "@{}[]().:;_-+~*!%$#\0", .want = "@{}[]().:;_-+~*!%$#\0"},
                    {.in = "⌘{čřžŧ¶'`[łĐŧđĐ¶\0", .want = "⌘{čřžŧ¶'`[łĐŧđĐ¶\0"},
                    {.in = "0123456789\0", .want = "0123456789\0"},
                    {.in = "  whitespaces  \0", .want = "  WHITESPACES  \0"},
                    {.in = "unit@test.com\0", .want = "UNIT@TEST.COM\0"},
                    {.in = "  \0", .want = "  \0"},
                    {.in = "\0", .want = NULL},
                    {.in = NULL, .want = NULL}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = char_toUppercase(test[i].in);
    EXPECT_STREQ(test[i].got, test[i].want);

    teardown(test[i].got);
  }
}

TEST(char, toLowercase) {
  typedef struct s_test {
    char *in;
    char *want;
    char *got;
  } test_t;

  test_t test[8] = {{.in = "@{}[]().:;_-+~*!%$#\0", .want = "@{}[]().:;_-+~*!%$#\0"},
                    {.in = "⌘{čřžŧ¶'`[łĐŧđĐ¶\0", .want = "⌘{čřžŧ¶'`[łĐŧđĐ¶\0"},
                    {.in = "0123456789\0", .want = "0123456789\0"},
                    {.in = "  WHITESPACES  \0", .want = "  whitespaces  \0"},
                    {.in = "UNIT@TEST.COM\0", .want = "unit@test.com\0"},
                    {.in = "  \0", .want = "  \0"},
                    {.in = "\0", .want = NULL},
                    {.in = NULL, .want = NULL}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = char_toLowercase(test[i].in);
    EXPECT_STREQ(test[i].got, test[i].want);

    teardown(test[i].got);
  }
}

TEST(char, occurrences) {
  typedef struct s_test {
    char *in[2];
    size_t want;
    size_t got;
  } test_t;

  test_t test[13] = {{.in = {"0/1/2/3\0", "/\0"}, .want = 3},
                     {.in = {"⌘{čřžŧ¶'`[łĐŧđĐ¶\0", "⌘\0"}, .want = 1},
                     {.in = {"unit test\0", "t\0"}, .want = 3},
                     {.in = {"11235\0", "1\0"}, .want = 2},
                     {.in = {"@{}[]().:;_-+~*!%$#\0", "#\0"}, .want = 1},
                     {.in = {"go golang\0", "go\0"}, .want = 2},
                     {.in = {"  spaces  \0", " \0"}, .want = 4},
                     {.in = {"  \0", "  \0"}, .want = 1},
                     {.in = {"str\0", "\0"}, .want = 0},
                     {.in = {"\0", "occur\0"}, .want = 0},
                     {.in = {"\0", "\0"}, .want = 0},
                     {.in = {"\0", NULL}, .want = 0},
                     {.in = {NULL, NULL}, .want = 0}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = char_occurrences(test[i].in[0], test[i].in[1]);
    EXPECT_EQ(test[i].got, test[i].want);
  }
}

TEST(char, replace) {
  typedef struct s_test {
    char *in[4];
    char *want;
    char *got;
  } test_t;

  test_t test[18] = {
      {.in = {"0#1#2\0", "#\0", "$\0", "-1\0"}, .want = "0$1$2\0"},
      {.in = {"0#1#2\0", "#\0", "$\0", "0\0"}, .want = "0#1#2\0"},
      {.in = {"0#1#2\0", "#\0", "$\0", "1\0"}, .want = "0$1#2\0"},
      {.in = {"0#1#2\0", "#\0", "$\0", "2\0"}, .want = "0$1$2\0"},
      {.in = {"0#1#2\0", "#\0", "$\0", "3\0"}, .want = "0$1$2\0"},
      {.in = {"⌘{čřžŧ¶'`[łĐŧđĐ¶\0", "¶\0", "⌘\0", "-1\0"}, .want = "⌘{čřžŧ⌘'`[łĐŧđĐ⌘\0"},
      {.in = {"Hello World\0", "l\0", "00\0", "-1\0"}, .want = "He0000o Wor00d\0"},
      {.in = {"Hello World\0", "Hello\0", "Programmer\0", "-1\0"}, .want = "Programmer World\0"},
      {.in = {"0#1#2\0", "#\0", "\0", "-1\0"}, .want = "012\0"},
      {.in = {"0#1#2\0", "\0", "$\0", "-1\0"}, .want = NULL},
      {.in = {"\0", "#\0", "$\0", "-1\0"}, .want = NULL},
      {.in = {"\0", "\0", "$\0", "-1\0"}, .want = NULL},
      {.in = {"\0", "\0", "\0", "-1\0"}, .want = NULL},
      {.in = {"0#1#2\0", "#\0", NULL, "-1\0"}, .want = NULL},
      {.in = {"0#1#2\0", NULL, "$\0", "-1\0"}, .want = NULL},
      {.in = {NULL, "#\0", "$\0", "-1\0"}, .want = NULL},
      {.in = {"0#1#2\0", NULL, NULL, "-1\0"}, .want = NULL},
      {.in = {NULL, NULL, NULL, "-1\0"}, .want = NULL}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got =
        char_replace(test[i].in[0], test[i].in[1], test[i].in[2], char_toInteger(test[i].in[3]));
    EXPECT_STREQ(test[i].got, test[i].want);

    teardown(test[i].got);
  }
}

TEST(char, contains) {
  typedef struct s_test {
    char *in[2];
    bool want;
    bool got;
  } test_t;

  test_t test[13] = {{.in = {"This is a sample sentence.\0", "sentence\0"}, .want = true},
                     {.in = {"This is a sample sentence.\0", "This\0"}, .want = true},
                     {.in = {"This is a sample sentence.\0", "Sentence\0"}, .want = false},
                     {.in = {"This-is-a-sample-sentence.\0", "sentence\0"}, .want = true},
                     {.in = {"This is a sample sentence.\0", "\0"}, .want = false},
                     {.in = {"\0", "\0"}, .want = false},
                     {.in = {"\0", NULL}, .want = false},
                     {.in = {NULL, NULL}, .want = false}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = char_contains(test[i].in[0], test[i].in[1]);
    EXPECT_EQ(test[i].got, test[i].want);
  }
}

TEST(char, free) {
  typedef struct s_test {
    char *in;
  } test_t;

  test_t test[2] = {{.in = char_trimSpace(" whitespace ")}, {.in = NULL}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    char_free(test[i].in);
  }
}
