// SPDX-License-Identifier: Apache-2.0

#ifndef INCLUDE_UTILITY_C_FS_H_
#define INCLUDE_UTILITY_C_FS_H_

#ifdef S_HAVE_CONFIG_H
  #include "utility-c/config.h"
#endif

#include <stdio.h>
#include <sys/types.h>
#if defined(S_POSIX)
  #include <stdbool.h>
#endif  // S_POSIX

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Creates a directory.
/// @param path The path of the directory to create.
/// @param mode The mode of the directory to create.
/// @return true if the directory was created successfully, false otherwise.
bool fs_createDirectory(const char* path, const mode_t mode);

/// @brief Removes a directory.
/// @param path The path of the directory to remove.
/// @return true if the directory was removed successfully, false otherwise.
bool fs_removeDirectory(const char* path);

/// @brief Checks if a file exists in a directory.
/// @param path The path of the directory to check.
/// @param filename The name of the file to check for.
/// @return true if the file exists, false otherwise.
bool fs_existFile(const char* path, const char* filename);

/// @brief Opens a file.
/// @param path The path of the directory containing the file.
/// @param filename The name of the file to open.
/// @param mode The mode to open the file in.
/// @return A pointer to the opened file, or NULL if an error occurred.
/// @note The caller is responsible for freeing the memory using fs_closeFile().
FILE* fs_openFile(const char* path, const char* filename, const char* mode);

/// @brief Closes a file.
/// @param file A pointer to the file to close.
void fs_closeFile(FILE* file);

/// @brief Creates a file.
/// @param path The path of the directory to create the file in.
/// @param filename The name of the file to create.
/// @return true if the file was created successfully, false otherwise.
bool fs_createFile(const char* path, const char* filename);

/// @brief Removes a file.
/// @param path The path of the directory containing the file.
/// @param filename The name of the file to remove.
/// @return true if the file was removed successfully, false otherwise.
bool fs_removeFile(const char* path, const char* filename);

/// @brief Reads the contents of a file.
/// @param path The path of the directory containing the file.
/// @param filename The name of the file to read.
/// @return A pointer to the contents of the file, or NULL if an error occurred.
/// @note The caller is responsible for freeing the memory using free().
char* fs_readFile(const char* path, const char* filename);

/// @brief Writes a string to a file.
/// @param path The path of the directory containing the file.
/// @param filename The name of the file to write to.
/// @param str The string to write to the file.
/// @return true if the string was written successfully, false otherwise.
bool fs_writeFile(const char* path, const char* filename, const char* str);

/// @brief Updates the contents of a file.
/// @param path The path of the directory containing the file.
/// @param filename The name of the file to update.
/// @param str The string to update the file with.
/// @return true if the file was updated successfully, false otherwise.
bool fs_updateFile(const char* path, const char* filename, const char* str);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_FS_H_
