#include <gtest/gtest.h>

#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

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

std::string joinPath(const std::string &base, const std::string &leaf)
{
  if (base.empty())
  {
    return leaf;
  }
  if (leaf.empty())
  {
    return base;
  }
  return base + "/" + leaf;
}

std::string makeTempDir(const std::string &prefix)
{
  std::string pattern = "/tmp/utility-c-" + prefix + "-XXXXXX";
  std::vector<char> buffer(pattern.begin(), pattern.end());
  buffer.push_back('\0');
  char *result = mkdtemp(buffer.data());
  if (result == nullptr)
  {
    return {};
  }
  return std::string(result);
}

bool writeFileRaw(const std::string &path, const std::string &content)
{
  FILE *file = fopen(path.c_str(), "w");
  if (file == nullptr)
  {
    return false;
  }
  if (!content.empty())
  {
    if (fwrite(content.data(), 1, content.size(), file) != content.size())
    {
      fclose(file);
      return false;
    }
  }
  fclose(file);
  return true;
}

std::string readFileRaw(const std::string &path)
{
  FILE *file = fopen(path.c_str(), "r");
  if (file == nullptr)
  {
    return {};
  }
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);
  if (size < 0)
  {
    fclose(file);
    return {};
  }
  std::string content(static_cast<size_t>(size), '\0');
  if (size > 0)
  {
    if (fread(&content[0], 1, static_cast<size_t>(size), file) != static_cast<size_t>(size))
    {
      fclose(file);
      return {};
    }
  }
  fclose(file);
  return content;
}

bool removeDirectoryRecursive(const std::string &path)
{
  DIR *dir = opendir(path.c_str());
  if (dir == nullptr)
  {
    return false;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != nullptr)
  {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
    {
      continue;
    }

    std::string subpath = joinPath(path, entry->d_name);
    struct stat about;
    if (stat(subpath.c_str(), &about) != 0)
    {
      continue;
    }

    if (S_ISDIR(about.st_mode))
    {
      if (!removeDirectoryRecursive(subpath))
      {
        closedir(dir);
        return false;
      }
    }
    else
    {
      if (unlink(subpath.c_str()) != 0)
      {
        closedir(dir);
        return false;
      }
    }
  }

  closedir(dir);
  return rmdir(path.c_str()) == 0;
}
}  // namespace

class FsTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
    const auto *info = ::testing::UnitTest::GetInstance()->current_test_info();
    const char *name = info != nullptr ? info->name() : "fs";
    root_ = makeTempDir(name);
    ASSERT_FALSE(root_.empty());
  }

  void TearDown() override
  {
    if (!root_.empty())
    {
      removeDirectoryRecursive(root_);
    }
  }

  const std::string &root() const
  {
    return root_;
  }

private:
  std::string root_;
};

TEST_F(FsTest, CreateDirectory)
{
  std::string existing = joinPath(root(), "existing");
  ASSERT_EQ(mkdir(existing.c_str(), 0755), 0);

  std::string filePath = joinPath(root(), "file.txt");
  ASSERT_TRUE(writeFileRaw(filePath, "content"));

  std::string nested = joinPath(root(), "nested/dir/path");
  std::string tooLong(static_cast<size_t>(PATH_MAX), 'a');

  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      std::string path;
      bool useNull;
      mode_t mode;
    } in;
    struct Want
    {
      bool expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"null-path", {"", true, 0755}, {false}},
    {"empty-path", {"", false, 0755}, {false}},
    {"too-long-path", {tooLong, false, 0755}, {false}},
    {"existing-directory", {existing, false, 0755}, {true}},
    {"new-nested-directory", {nested, false, 0755}, {true}},
    {"path-is-file", {filePath, false, 0755}, {false}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    const char *path = tc.in.useNull ? nullptr : tc.in.path.c_str();

    // Act
    bool got = fs_createDirectory(path, tc.in.mode);

    // Assert
    EXPECT_EQ(got, tc.want.expected);
  }

}

TEST_F(FsTest, RemoveDirectory)
{
  std::string toRemove = joinPath(root(), "to-remove");
  ASSERT_EQ(mkdir(toRemove.c_str(), 0755), 0);
  std::string nested = joinPath(toRemove, "nested");
  ASSERT_EQ(mkdir(nested.c_str(), 0755), 0);
  ASSERT_TRUE(writeFileRaw(joinPath(nested, "file.txt"), "data"));

  std::string missing = joinPath(root(), "missing");

  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      std::string path;
      bool useNull;
    } in;
    struct Want
    {
      bool expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"null-path", {"", true}, {false}},
    {"missing-path", {missing, false}, {false}},
    {"existing-path", {toRemove, false}, {true}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);

    const char *path = tc.in.useNull ? nullptr : tc.in.path.c_str();

    // Act
    bool got = fs_removeDirectory(path);

    // Assert
    EXPECT_EQ(got, tc.want.expected);
  }

}

TEST_F(FsTest, ExistFile)
{
  std::string filename = "exists.txt";
  ASSERT_TRUE(writeFileRaw(joinPath(root(), filename), "data"));

  std::string missingFile = "missing.txt";
  std::string missingPath = joinPath(root(), "missing-dir");

  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      std::string path;
      std::string file;
      bool useNullPath;
      bool useNullFile;
    } in;
    struct Want
    {
      bool expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"null-path", {"", filename, true, false}, {false}},
    {"null-file", {root(), "", false, true}, {false}},
    {"missing-path", {missingPath, filename, false, false}, {false}},
    {"missing-file", {root(), missingFile, false, false}, {false}},
    {"existing-file", {root(), filename, false, false}, {true}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);
    CwdGuard guard;

    const char *path = tc.in.useNullPath ? nullptr : tc.in.path.c_str();
    const char *file = tc.in.useNullFile ? nullptr : tc.in.file.c_str();

    // Act
    bool got = fs_existFile(path, file);

    // Assert
    EXPECT_EQ(got, tc.want.expected);
  }

}

TEST_F(FsTest, OpenCloseFile)
{
  std::string filename = "open.txt";
  ASSERT_TRUE(writeFileRaw(joinPath(root(), filename), "data"));
  std::string missingPath = joinPath(root(), "missing-dir");

  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      std::string path;
      std::string file;
      std::string mode;
      bool useNullPath;
      bool useNullFile;
      bool useNullMode;
    } in;
    struct Want
    {
      bool expectNull;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"null-path", {"", filename, "r", true, false, false}, {true}},
    {"null-file", {root(), "", "r", false, true, false}, {true}},
    {"null-mode", {root(), filename, "", false, false, true}, {true}},
    {"missing-path", {missingPath, filename, "r", false, false, false}, {true}},
    {"valid-open", {root(), filename, "r", false, false, false}, {false}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);
    CwdGuard guard;

    const char *path = tc.in.useNullPath ? nullptr : tc.in.path.c_str();
    const char *file = tc.in.useNullFile ? nullptr : tc.in.file.c_str();
    const char *mode = tc.in.useNullMode ? nullptr : tc.in.mode.c_str();

    // Act
    FILE *got = fs_openFile(path, file, mode);

    // Assert
    if (tc.want.expectNull)
    {
      EXPECT_EQ(got, nullptr);
    }
    else
    {
      EXPECT_NE(got, nullptr);
      fs_closeFile(got);
    }
  }

}

TEST_F(FsTest, CreateFile)
{
  std::string filename = "new.txt";
  std::string existing = "existing.txt";
  ASSERT_TRUE(writeFileRaw(joinPath(root(), existing), "data"));
  std::string missingPath = joinPath(root(), "missing-dir");

  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      std::string path;
      std::string file;
      bool useNullPath;
      bool useNullFile;
    } in;
    struct Want
    {
      bool expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"null-path", {"", filename, true, false}, {false}},
    {"null-file", {root(), "", false, true}, {false}},
    {"missing-path", {missingPath, filename, false, false}, {false}},
    {"new-file", {root(), filename, false, false}, {true}},
    {"existing-file", {root(), existing, false, false}, {true}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);
    CwdGuard guard;

    const char *path = tc.in.useNullPath ? nullptr : tc.in.path.c_str();
    const char *file = tc.in.useNullFile ? nullptr : tc.in.file.c_str();

    // Act
    bool got = fs_createFile(path, file);

    // Assert
    EXPECT_EQ(got, tc.want.expected);
  }

  EXPECT_EQ(access(joinPath(root(), filename).c_str(), F_OK), 0);
  EXPECT_EQ(access(joinPath(root(), existing).c_str(), F_OK), 0);
}

TEST_F(FsTest, RemoveFile)
{
  std::string filename = "remove.txt";
  ASSERT_TRUE(writeFileRaw(joinPath(root(), filename), "data"));
  std::string missingPath = joinPath(root(), "missing-dir");

  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      std::string path;
      std::string file;
      bool useNullPath;
      bool useNullFile;
    } in;
    struct Want
    {
      bool expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"null-path", {"", filename, true, false}, {false}},
    {"null-file", {root(), "", false, true}, {false}},
    {"missing-path", {missingPath, filename, false, false}, {false}},
    {"missing-file", {root(), "missing.txt", false, false}, {false}},
    {"existing-file", {root(), filename, false, false}, {true}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);
    CwdGuard guard;

    const char *path = tc.in.useNullPath ? nullptr : tc.in.path.c_str();
    const char *file = tc.in.useNullFile ? nullptr : tc.in.file.c_str();

    // Act
    bool got = fs_removeFile(path, file);

    // Assert
    EXPECT_EQ(got, tc.want.expected);
  }

  EXPECT_EQ(access(joinPath(root(), filename).c_str(), F_OK), -1);
}

TEST_F(FsTest, ReadFile)
{
  std::string filename = "read.txt";
  std::string emptyFile = "empty.txt";
  ASSERT_TRUE(writeFileRaw(joinPath(root(), filename), "hello"));
  ASSERT_TRUE(writeFileRaw(joinPath(root(), emptyFile), ""));
  std::string missingPath = joinPath(root(), "missing-dir");

  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      std::string path;
      std::string file;
      bool useNullPath;
      bool useNullFile;
    } in;
    struct Want
    {
      const char *expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"null-path", {"", filename, true, false}, {nullptr}},
    {"null-file", {root(), "", false, true}, {nullptr}},
    {"missing-path", {missingPath, filename, false, false}, {nullptr}},
    {"missing-file", {root(), "missing.txt", false, false}, {nullptr}},
    {"empty-file", {root(), emptyFile, false, false}, {""}},
    {"existing-file", {root(), filename, false, false}, {"hello"}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);
    CwdGuard guard;

    const char *path = tc.in.useNullPath ? nullptr : tc.in.path.c_str();
    const char *file = tc.in.useNullFile ? nullptr : tc.in.file.c_str();

    // Act
    std::unique_ptr<char, decltype(&free)> got(fs_readFile(path, file), free);

    // Assert
    if (tc.want.expected == nullptr)
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

TEST_F(FsTest, WriteFile)
{
  std::string filename = "write.txt";
  std::string missingPath = joinPath(root(), "missing-dir");

  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      std::string path;
      std::string file;
      std::string content;
      bool useNullPath;
      bool useNullFile;
      bool useNullContent;
    } in;
    struct Want
    {
      bool expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"null-path", {"", filename, "data", true, false, false}, {false}},
    {"null-file", {root(), "", "data", false, true, false}, {false}},
    {"null-content", {root(), filename, "", false, false, true}, {false}},
    {"missing-path", {missingPath, filename, "data", false, false, false}, {false}},
    {"write-content", {root(), filename, "hello", false, false, false}, {true}},
    {"write-empty", {root(), filename, "", false, false, false}, {true}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);
    CwdGuard guard;

    const char *path = tc.in.useNullPath ? nullptr : tc.in.path.c_str();
    const char *file = tc.in.useNullFile ? nullptr : tc.in.file.c_str();
    const char *content = tc.in.useNullContent ? nullptr : tc.in.content.c_str();

    // Act
    bool got = fs_writeFile(path, file, content);

    // Assert
    EXPECT_EQ(got, tc.want.expected);
  }

  EXPECT_EQ(readFileRaw(joinPath(root(), filename)), "");
}

TEST_F(FsTest, UpdateFile)
{
  std::string filename = "update.txt";
  ASSERT_TRUE(writeFileRaw(joinPath(root(), filename), "start"));
  std::string missingPath = joinPath(root(), "missing-dir");

  // In-Got-Want
  struct Tests
  {
    std::string label;
    struct In
    {
      std::string path;
      std::string file;
      std::string content;
      bool useNullPath;
      bool useNullFile;
      bool useNullContent;
    } in;
    struct Want
    {
      bool expected;
    } want;
  };

  // Table-Driven Testing
  const std::vector<Tests> tests = {
    {"null-path", {"", filename, "data", true, false, false}, {false}},
    {"null-file", {root(), "", "data", false, true, false}, {false}},
    {"null-content", {root(), filename, "", false, false, true}, {false}},
    {"missing-path", {missingPath, filename, "data", false, false, false}, {false}},
    {"append-content", {root(), filename, "-end", false, false, false}, {true}},
  };

  for (const auto &tc : tests)
  {
    SCOPED_TRACE(tc.label);
    CwdGuard guard;

    const char *path = tc.in.useNullPath ? nullptr : tc.in.path.c_str();
    const char *file = tc.in.useNullFile ? nullptr : tc.in.file.c_str();
    const char *content = tc.in.useNullContent ? nullptr : tc.in.content.c_str();

    // Act
    bool got = fs_updateFile(path, file, content);

    // Assert
    EXPECT_EQ(got, tc.want.expected);
  }

  EXPECT_EQ(readFileRaw(joinPath(root(), filename)), "start-end");
}
