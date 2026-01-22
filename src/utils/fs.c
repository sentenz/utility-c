// SPDX-License-Identifier: Apache-2.0

#include "utility-c/utils/fs.h"

#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#ifdef S_LINUX
  #include <linux/limits.h>
#endif  // S_LINUX

bool fs_createDirectory(const char* path, const mode_t mode) {
  if (path == NULL) {
    return false;
  }

  size_t len = strlen(path);
  if (len == 0 || len >= PATH_MAX) {
    return false;
  }

  // Copy path
  char content[PATH_MAX];
  snprintf(content, sizeof(content), "%s", path);

  // Check if path exists and is a directory
  struct stat about;
  if (stat(content, &about) == 0) {
    if (S_ISDIR(about.st_mode)) {
      return true;
    }
  }

  // Recursive mkdir
  char* ptr = NULL;
  for (ptr = content + 1; *ptr; ptr++) {
    if (*ptr == '/') {
      *ptr = 0;

      // Test path
      if (stat(content, &about) != 0) {
        // Path does not exist - create directory
        if (0 > mkdir(content, mode)) {
          return false;
        }
      } else if (!S_ISDIR(about.st_mode)) {
        // Not a directory
        return false;
      }

      *ptr = '/';
    }
  }

  // Test path
  if (stat(content, &about) != 0) {
    // Path does not exist, create directory
    if (0 > mkdir(content, mode)) {
      return false;
    }
  } else if (!S_ISDIR(about.st_mode)) {
    // Not a directory
    return false;
  }

  return true;
}

bool fs_removeDirectory(const char* path) {
  if (path == NULL) {
    return false;
  }

  DIR* dir = opendir(path);
  if (dir == NULL) {
    return false;
  }

  struct dirent* entry;
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    char subpath[PATH_MAX];
    snprintf(subpath, sizeof subpath, "%s/%s", path, entry->d_name);

    struct stat about;
    if (stat(subpath, &about) == -1) {
      continue;
    }

    if (S_ISDIR(about.st_mode)) {
      if (!fs_removeDirectory(subpath)) {
        return false;
      }
    } else {
      if (unlink(subpath) == -1) {
        return false;
      }
    }
  }

  closedir(dir);

  if (rmdir(path) == -1) {
    return false;
  }

  return true;
}

bool fs_existFile(const char* path, const char* filename) {
  if (path == NULL || filename == NULL) {
    return false;
  }

  if (chdir(path) < 0) {
    return false;
  }

  if (access(filename, F_OK) < 0) {
    return false;
  }

  return true;
}

FILE* fs_openFile(const char* path, const char* filename, const char* mode) {
  if (path == NULL || filename == NULL || mode == NULL) {
    return NULL;
  }

  if (chdir(path) < 0) {
    return NULL;
  }

  return fopen(filename, mode);
}

void fs_closeFile(FILE* file) {
  if (file == NULL) {
    return;
  }

  fclose(file);
}

bool fs_createFile(const char* path, const char* filename) {
  if (path == NULL || filename == NULL) {
    return false;
  }

  if (chdir(path) < 0) {
    return false;
  }

  FILE* file = fopen(filename, "a\0");
  if (file == NULL) {
    return false;
  }

  fs_closeFile(file);

  return true;
}

bool fs_removeFile(const char* path, const char* filename) {
  if (path == NULL || filename == NULL) {
    return false;
  }

  if (chdir(path) < 0) {
    return false;
  }

  if (remove(filename) != 0) {
    return false;
  }

  return true;
}

char* fs_readFile(const char* path, const char* filename) {
  if (path == NULL || filename == NULL) {
    return NULL;
  }

  if (chdir(path) < 0) {
    return NULL;
  }

  FILE* file = fopen(filename, "r\0");
  if (file == NULL) {
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  rewind(file);

  char* content = (char*)malloc(sizeof(char) * size + 1);
  if (content == NULL) {
    fs_closeFile(file);

    return NULL;
  }

  if (fread(content, 1, size, file) != size) {
    fs_closeFile(file);
    free(content);

    return NULL;
  }
  content[size] = '\0';

  fs_closeFile(file);

  return content;
}

bool fs_writeFile(const char* path, const char* filename, const char* str) {
  if (path == NULL || filename == NULL || str == NULL) {
    return false;
  }

  if (chdir(path) < 0) {
    return false;
  }

  FILE* file = fopen(filename, "w+\0");
  if (file == NULL) {
    return false;
  }

  fprintf(file, "%s", str);

  fs_closeFile(file);

  return true;
}

bool fs_updateFile(const char* path, const char* filename, const char* str) {
  if (path == NULL || filename == NULL || str == NULL) {
    return false;
  }

  if (chdir(path) < 0) {
    return false;
  }

  FILE* file = fopen(filename, "a+\0");
  if (file == NULL) {
    return false;
  }

  fprintf(file, "%s", str);

  fs_closeFile(file);

  return true;
}
