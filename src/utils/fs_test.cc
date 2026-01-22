#include <gtest/gtest.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>

#include "utility-c/utils/fs.h"

namespace {

class CwdGuard
{
public:
  CwdGuard()
  {
    if (getcwd(cwd_, sizeof(cwd_)) == nullptr)
    {
      cwd_[0] = '\0';
    }
  }

  ~CwdGuard()
  {
    if (cwd_[0] != '\0')
    {
      (void)chdir(cwd_);
    }
  }

private:
  char cwd_[PATH_MAX];
};

std::string make_temp_dir()
{
  char templ[] = "/tmp/utility-c-fs-XXXXXX";
  char *path = mkdtemp(templ);
  if (path == nullptr)
  {
    return {};
  }
  return std::string(path);
}

bool path_exists(const std::string &path)
{
  struct stat about;
  return stat(path.c_str(), &about) == 0;
}

}  // namespace

TEST(FsTest, CreateDirectory)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      const char *path;
      mode_t mode;
    } in;
    struct Want
    {
      bool expected;
    } want;
  };

  std::string temp = make_temp_dir();
  ASSERT_FALSE(temp.empty());
  std::string nested = temp + "/a/b/c";

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"valid-nested", {nested.c_str(), 0755}, {true}},
    {"null-path", {NULL, 0755}, {false}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    CwdGuard guard;

    // Act
    bool got = fs_createDirectory(tc.in.path, tc.in.mode);

    // Assert
    EXPECT_EQ(got, tc.want.expected);
  }

  EXPECT_TRUE(path_exists(nested));
  EXPECT_TRUE(fs_removeDirectory(temp.c_str()));
}

TEST(FsTest, RemoveDirectory)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      const char *path;
    } in;
    struct Want
    {
      bool expected;
    } want;
  };

  std::string temp = make_temp_dir();
  ASSERT_FALSE(temp.empty());
  std::string nested = temp + "/child";
  ASSERT_TRUE(fs_createDirectory(nested.c_str(), 0755));
  ASSERT_TRUE(fs_createFile(nested.c_str(), "file.txt"));

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"valid-remove", {temp.c_str()}, {true}},
    {"null-path", {NULL}, {false}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    CwdGuard guard;

    // Act
    bool got = fs_removeDirectory(tc.in.path);

    // Assert
    EXPECT_EQ(got, tc.want.expected);
  }

  EXPECT_FALSE(path_exists(temp));
}

TEST(FsTest, ExistFile)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      const char *path;
      const char *filename;
    } in;
    struct Want
    {
      bool expected;
    } want;
  };

  std::string temp = make_temp_dir();
  ASSERT_FALSE(temp.empty());
  ASSERT_TRUE(fs_createFile(temp.c_str(), "exists.txt"));

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"exists", {temp.c_str(), "exists.txt"}, {true}},
    {"missing", {temp.c_str(), "missing.txt"}, {false}},
    {"null-path", {NULL, "exists.txt"}, {false}},
    {"null-filename", {temp.c_str(), NULL}, {false}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    CwdGuard guard;

    // Act
    bool got = fs_existFile(tc.in.path, tc.in.filename);

    // Assert
    EXPECT_EQ(got, tc.want.expected);
  }

  EXPECT_TRUE(fs_removeDirectory(temp.c_str()));
}

TEST(FsTest, OpenCloseFile)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      const char *path;
      const char *filename;
      const char *mode;
    } in;
    struct Want
    {
      bool should_open;
    } want;
  };

  std::string temp = make_temp_dir();
  ASSERT_FALSE(temp.empty());

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"open-write", {temp.c_str(), "open.txt", "w+"}, {true}},
    {"null-path", {NULL, "open.txt", "w+"}, {false}},
    {"null-filename", {temp.c_str(), NULL, "w+"}, {false}},
    {"null-mode", {temp.c_str(), "open.txt", NULL}, {false}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    CwdGuard guard;

    // Act
    FILE *got = fs_openFile(tc.in.path, tc.in.filename, tc.in.mode);

    // Assert
    if (tc.want.should_open)
    {
      ASSERT_NE(got, nullptr);
      fs_closeFile(got);
    }
    else
    {
      EXPECT_EQ(got, nullptr);
    }
  }

  EXPECT_TRUE(fs_removeDirectory(temp.c_str()));
}

TEST(FsTest, CreateReadWriteUpdateRemoveFile)
{
  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      const char *path;
      const char *filename;
      const char *content;
      const char *append;
    } in;
    struct Want
    {
      const char *expected;
    } want;
  };

  std::string temp = make_temp_dir();
  ASSERT_FALSE(temp.empty());

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"write-and-append", {temp.c_str(), "data.txt", "hello", " world"}, {"hello world"}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    // Arrange
    CwdGuard guard;

    // Act
    bool created = fs_createFile(tc.in.path, tc.in.filename);
    bool wrote = fs_writeFile(tc.in.path, tc.in.filename, tc.in.content);
    bool updated = fs_updateFile(tc.in.path, tc.in.filename, tc.in.append);
    std::unique_ptr<char, decltype(&free)> got(fs_readFile(tc.in.path, tc.in.filename), free);
    bool removed = fs_removeFile(tc.in.path, tc.in.filename);

    // Assert
    EXPECT_TRUE(created);
    EXPECT_TRUE(wrote);
    EXPECT_TRUE(updated);
    ASSERT_NE(got.get(), nullptr);
    EXPECT_STREQ(got.get(), tc.want.expected);
    EXPECT_TRUE(removed);
  }

  EXPECT_TRUE(fs_removeDirectory(temp.c_str()));
}
