// SPDX-License-Identifier: Apache-2.0

#include "utility-c/fs.h"

#include "gtest/gtest.h"

static void setup(const char *path, const char *filename, const char *str) {
  if (path != NULL) {
    fs_createDirectory(path, ACCESSPERMS);
  }

  if (path != NULL && filename != NULL) {
    fs_createFile(path, filename);
  }

  if (path != NULL && filename != NULL && str != NULL) {
    fs_writeFile(path, filename, str);
  }
}

static void teardown(const char *path, const char *filename) {
  if (path != NULL && filename != NULL) {
    fs_removeFile(path, filename);
  }

  if (path != NULL) {
    fs_removeDirectory(path);
  }
}

TEST(fs, createDirectory) {
  typedef struct s_test {
    char *in1;
    mode_t in2;
    int want;
    int got;
  } test_t;

  test_t test[19] = {
      {.in1 = "/tmp/test/fs/a\0", .in2 = ACCESSPERMS, .want = true},
      {.in1 = "/tmp/test/fs/a\0", .in2 = ACCESSPERMS, .want = true},
      {.in1 = "/tmp/test/fs/b\0", .in2 = ALLPERMS, .want = true},
      {.in1 = "/tmp/test/fs/c/\0", .in2 = DEFFILEMODE, .want = true},
      {.in1 = "/tmp/test/fs/d/\0", .in2 = S_IRWXU | S_IRWXG | S_IRWXO | S_ISUID, .want = true},
      {.in1 = "/tmp/test/fs/e/\0", .in2 = S_IRGRP | S_IRGRP | S_IROTH, .want = true},
      {.in1 = "/tmp/test/fs/f//\0", .in2 = 0, .want = true},
      {.in1 = "/tmp/test/fs/g///\0", .in2 = 0, .want = true},
      {.in1 = "/tmp/test/fs/h///j\0", .in2 = 0, .want = true},
      {.in1 = "/tmp/test/fs/i///k/\0", .in2 = 0, .want = true},
      {.in1 = "/tmp/test/fs/l/ /m/\0", .in2 = 0, .want = true},
      {.in1 = "/tmp/test/fs/n/o p/q/\0", .in2 = 0, .want = true},
      {.in1 = "/tmp/test/fs/r/.config\0", .in2 = 0, .want = true},
      {.in1 = "/tmp/test/fs/s/.config\0", .in2 = 0, .want = true},
      {.in1 = "/tmp/test/fs/{}[]()\0", .in2 = ACCESSPERMS, .want = true},
      {.in1 = "/tmp/test/fs/⌘☹Ж\0", .in2 = ACCESSPERMS, .want = true},
      {.in1 = "/tmp/test/fs\0", .in2 = ACCESSPERMS, .want = true},
      {.in1 = "\0", .in2 = 0, .want = false},
      {.in1 = NULL, .in2 = 0, .want = false}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    test[i].got = fs_createDirectory(test[i].in1, test[i].in2);
    EXPECT_EQ(test[i].got, test[i].want);

    teardown(test[i].in1, NULL);
  }
}

TEST(fs, removeDirectory) {
  typedef struct s_test {
    char *in;
    bool want;
    bool got;
  } test_t;

  test_t test[19] = {{.in = "/tmp/test/fs/a\0", .want = true},
                     {.in = "/tmp/test/fs/a\0", .want = true},
                     {.in = "/tmp/test/fs/b\0", .want = true},
                     {.in = "/tmp/test/fs/c/\0", .want = true},
                     {.in = "/tmp/test/fs/d/\0", .want = true},
                     {.in = "/tmp/test/fs/e/\0", .want = true},
                     {.in = "/tmp/test/fs/f//\0", .want = true},
                     {.in = "/tmp/test/fs/g///\0", .want = true},
                     {.in = "/tmp/test/fs/h///j\0", .want = true},
                     {.in = "/tmp/test/fs/i///k/\0", .want = true},
                     {.in = "/tmp/test/fs/l/ /m/\0", .want = true},
                     {.in = "/tmp/test/fs/n/o p/q/\0", .want = true},
                     {.in = "/tmp/test/fs/r/.config\0", .want = true},
                     {.in = "/tmp/test/fs/s/.config\0", .want = true},
                     {.in = "/tmp/test/fs/{}[]()\0", .want = true},
                     {.in = "/tmp/test/fs/⌘☹Ж\0", .want = true},
                     {.in = "/tmp/test/fs\0", .want = true},
                     {.in = "\0", .want = false},
                     {.in = NULL, .want = false}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    setup(test[i].in, "test.txt", NULL);

    test[i].got = fs_removeDirectory(test[i].in);
    EXPECT_EQ(test[i].got, test[i].want);
  }
}

TEST(fs, existFile) {
  typedef struct s_test {
    char *in[2];
    bool want;
    bool got;
  } test_t;

  test_t test[18] = {{.in = {"/tmp/test/fs/a\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/a\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/b\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/c/\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/d/\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/e/\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/f//\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/g///\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/h///j\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/i///k/\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/l/ /m/\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/n/o p/q/\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/r/.config\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/s/.config\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/{}[]()\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/⌘☹Ж\0", "\0"}, .want = false},
                     {.in = {"\0", "\0"}, .want = false},
                     {.in = {NULL, NULL}, .want = false}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    setup(test[i].in[0], test[i].in[1], NULL);

    test[i].got = fs_existFile(test[i].in[0], test[i].in[1]);
    EXPECT_EQ(test[i].got, test[i].want);

    teardown(test[i].in[0], test[i].in[1]);
  }
}

TEST(fs, openFile) {
  typedef struct s_test {
    char *in[3];
    FILE *want;
    FILE *got;
  } test_t;

  test_t test[18] = {{.in = {"/tmp/test/fs/a\0", "example.txt\0", "r\0"}, .want = NULL},
                     {.in = {"/tmp/test/fs/a\0", "example.txt\0", "w\0"}, .want = NULL},
                     {.in = {"/tmp/test/fs/b\0", "example.txt\0", "a\0"}, .want = NULL},
                     {.in = {"/tmp/test/fs/c/\0", "example.txt\0", "r+\0"}, .want = NULL},
                     {.in = {"/tmp/test/fs/d/\0", "example.txt\0", "w+\0"}, .want = NULL},
                     {.in = {"/tmp/test/fs/e/\0", "example.txt\0", "a+\0"}, .want = NULL},
                     {.in = {"/tmp/test/fs/f//\0", "example.txt\0", "a+\0"}, .want = NULL},
                     {.in = {"/tmp/test/fs/g///\0", "example.txt\0", "r\0"}, .want = NULL},
                     {.in = {"/tmp/test/fs/h///j\0", "example.txt\0", "r\0"}, .want = NULL},
                     {.in = {"/tmp/test/fs/i///k/\0", "example.txt\0", "r\0"}, .want = NULL},
                     {.in = {"/tmp/test/fs/l/ /m/\0", "example.txt\0", "r\0"}, .want = NULL},
                     {.in = {"/tmp/test/fs/n/o p/q/\0", "example.txt\0", "r\0"}, .want = NULL},
                     {.in = {"/tmp/test/fs/r/.config\0", "example.txt\0", "r\0"}, .want = NULL},
                     {.in = {"/tmp/test/fs/s/.config\0", "example.txt\0", "r\0"}, .want = NULL},
                     {.in = {"/tmp/test/fs/{}[]()\0", "example.txt\0", NULL}, .want = (FILE *)1},
                     {.in = {"/tmp/test/fs/⌘☹Ж\0", "\0"}, .want = (FILE *)1},
                     {.in = {"\0", "\0", "\0"}, .want = (FILE *)1},
                     {.in = {NULL, NULL, NULL}, .want = (FILE *)1}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    setup(test[i].in[0], test[i].in[1], NULL);

    test[i].got = fs_openFile(test[i].in[0], test[i].in[1], test[i].in[2]);
    EXPECT_NE(test[i].got, test[i].want);

    teardown(test[i].in[0], test[i].in[1]);
  }
}

TEST(fs, closeFile) {
  typedef struct s_test {
    FILE *in;
  } test_t;

  test_t test[2] = {{.in = fs_openFile("/tmp/test/fs/a\0", "example.txt\0", "r\0")}, {.in = NULL}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    setup("/tmp/test/fs/a\0", "example.txt\0", NULL);

    fs_closeFile(test[i].in);

    teardown("/tmp/test/fs/a\0", "example.txt\0");
  }
}

TEST(fs, createFile) {
  typedef struct s_test {
    char *in[2];
    int want;
    int got;
  } test_t;

  test_t test[19] = {{.in = {"/tmp/test/fs/a\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/a\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/b\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/c/\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/d/\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/e/\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/f//\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/g///\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/h///j\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/i///k/\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/l/ /m/\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/n/o p/q/\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/r/.config\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/s/.config\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/{}[]()\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/⌘☹Ж\0", "\0"}, .want = false},
                     {.in = {"/etc/test\0", "\0"}, .want = false},
                     {.in = {"\0", "\0"}, .want = false},
                     {.in = {NULL, NULL}, .want = false}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    setup(test[i].in[0], NULL, NULL);

    test[i].got = fs_createFile(test[i].in[0], test[i].in[1]);
    EXPECT_EQ(test[i].got, test[i].want);

    teardown(test[i].in[0], test[i].in[1]);
  }
}

TEST(fs, removeFile) {
  typedef struct s_test {
    char *in[2];
    int want;
    int got;
  } test_t;

  test_t test[19] = {{.in = {"/tmp/test/fs/a\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/a\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/b\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/c/\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/d/\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/e/\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/f//\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/g///\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/h///j\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/i///k/\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/l/ /m/\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/n/o p/q/\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/r/.config\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/s/.config\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/{}[]()\0", "example.txt\0"}, .want = true},
                     {.in = {"/tmp/test/fs/⌘☹Ж\0", "\0"}, .want = false},
                     {.in = {"/tmp/test/fs\0", "\0"}, .want = false},
                     {.in = {"\0", "\0"}, .want = false},
                     {.in = {NULL, NULL}, .want = false}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    setup(test[i].in[0], test[i].in[1], NULL);

    test[i].got = fs_removeFile(test[i].in[0], test[i].in[1]);
    EXPECT_EQ(test[i].got, test[i].want);

    teardown(test[i].in[0], NULL);
  }
}

TEST(fs, readFile) {
  typedef struct s_test {
    char *in[2];
    char *want;
    char *got;
  } test_t;

  test_t test[19] = {
      {.in = {"/tmp/test/fs/a\0", "example.txt\0"}, .want = "hello world a\nhello world b\n\0"},
      {.in = {"/tmp/test/fs/a\0", "example.txt\0"}, .want = "hello world a\nhello world b\n\0"},
      {.in = {"/tmp/test/fs/b\0", "example.txt\0"}, .want = "hello world c\n\0"},
      {.in = {"/tmp/test/fs/c/\0", "example.txt\0"}, .want = "hello world d\n\0"},
      {.in = {"/tmp/test/fs/d/\0", "example.txt\0"}, .want = "hello world e\n\0"},
      {.in = {"/tmp/test/fs/e/\0", "example.txt\0"}, .want = "hello world f\n\0"},
      {.in = {"/tmp/test/fs/f//\0", "example.txt\0"}, .want = "hello world g\n\0"},
      {.in = {"/tmp/test/fs/g///\0", "example.txt\0"}, .want = "hello world h\n\0"},
      {.in = {"/tmp/test/fs/h///j\0", "example.txt\0"}, .want = "hello world j\n\0"},
      {.in = {"/tmp/test/fs/i///k/\0", "example.txt\0"}, .want = "hello world i\n\0"},
      {.in = {"/tmp/test/fs/l/ /m/\0", "example.txt\0"}, .want = "hello world k\n\0"},
      {.in = {"/tmp/test/fs/n/o p/q/\0", "example.txt\0"}, .want = "hello world l\n\0"},
      {.in = {"/tmp/test/fs/r/.config\0", "example.txt\0"}, .want = "hello world l\n\0"},
      {.in = {"/tmp/test/fs/s/.config\0", "example.txt\0"}, .want = "hello world {}[]()\n\0"},
      {.in = {"/tmp/test/fs/{}[]()\0", "example.txt\0"}, .want = "hello world ⌘☹Ж\n\0"},
      {.in = {"/tmp/test/fs/⌘☹Ж\0", "\0"}, .want = NULL},
      {.in = {"/tmp/test/fs\0", "\0"}, .want = NULL},
      {.in = {"\0", "\0"}, .want = NULL},
      {.in = {NULL, NULL}, .want = NULL}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    setup(test[i].in[0], test[i].in[1], test[i].want);

    test[i].got = fs_readFile(test[i].in[0], test[i].in[1]);
    EXPECT_STREQ(test[i].got, test[i].want);

    free(test[i].got);

    teardown(test[i].in[0], test[i].in[1]);
  }
}

TEST(fs, writeFile) {
  typedef struct s_test {
    char *in[3];
    int want;
    int got;
  } test_t;

  test_t test[19] = {
      {.in = {"/tmp/test/fs/a\0", "example.txt\0", "hello world a\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/a\0", "example.txt\0", "hello world b\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/b\0", "example.txt\0", "hello world c\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/c/\0", "example.txt\0", "hello world d\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/d/\0", "example.txt\0", "hello world e\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/e/\0", "example.txt\0", "hello world f\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/f//\0", "example.txt\0", "hello world g\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/g///\0", "example.txt\0", "hello world h\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/h///j\0", "example.txt\0", "hello world j\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/i///k/\0", "example.txt\0", "hello world i\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/l/ /m/\0", "example.txt\0", "hello world k\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/n/o p/q/\0", "example.txt\0", "hello world l\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/r/.config\0", "example.txt\0", "hello world l\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/s/.config\0", "example.txt\0", "hello world {}[]()\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/{}[]()\0", "example.txt\0", "hello world ⌘☹Ж\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/⌘☹Ж\0", "\0", "hello world o\n\0"}, .want = false},
      {.in = {"/tmp/test/fs\0", "\0", "\0"}, .want = false},
      {.in = {"\0", "\0", "hello world p\n\0"}, .want = false},
      {.in = {NULL, NULL, "hello world q\n\0"}, .want = false}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    setup(test[i].in[0], test[i].in[1], NULL);

    test[i].got = fs_writeFile(test[i].in[0], test[i].in[1], test[i].in[2]);
    EXPECT_EQ(test[i].got, test[i].want);

    teardown(test[i].in[0], test[i].in[1]);
  }
}

TEST(fs, updateFile) {
  typedef struct s_test {
    char *in[3];
    int want;
    int got;
  } test_t;

  test_t test[19] = {
      {.in = {"/tmp/test/fs/a\0", "example.txt\0", "hello world a\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/a\0", "example.txt\0", "hello world b\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/b\0", "example.txt\0", "hello world c\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/c/\0", "example.txt\0", "hello world d\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/d/\0", "example.txt\0", "hello world e\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/e/\0", "example.txt\0", "hello world f\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/f//\0", "example.txt\0", "hello world g\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/g///\0", "example.txt\0", "hello world h\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/h///j\0", "example.txt\0", "hello world j\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/i///k/\0", "example.txt\0", "hello world i\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/l/ /m/\0", "example.txt\0", "hello world k\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/n/o p/q/\0", "example.txt\0", "hello world l\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/r/.config\0", "example.txt\0", "hello world l\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/s/.config\0", "example.txt\0", "hello world {}[]()\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/{}[]()\0", "example.txt\0", "hello world ⌘☹Ж\n\0"}, .want = true},
      {.in = {"/tmp/test/fs/⌘☹Ж\0", "\0", "hello world o\n\0"}, .want = false},
      {.in = {"/tmp/test/fs\0", "\0", "\0"}, .want = false},
      {.in = {"\0", "\0", "hello world p\n\0"}, .want = false},
      {.in = {NULL, NULL, "hello world q\n\0"}, .want = false}};

  for (size_t i = 0; i < sizeof(test) / sizeof(test[0]); ++i) {
    setup(test[i].in[0], test[i].in[1], NULL);

    test[i].got = fs_updateFile(test[i].in[0], test[i].in[1], test[i].in[2]);
    EXPECT_EQ(test[i].got, test[i].want);

    teardown(test[i].in[0], test[i].in[1]);
  }
}
