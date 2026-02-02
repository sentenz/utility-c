// SPDX-License-Identifier: Apache-2.0

#ifndef INCLUDE_UTILITY_C_FS_H_
#define INCLUDE_UTILITY_C_FS_H_

/**
 * @file fs.h
 * @brief File system utilities.
 *
 * This module provides functions for common file system operations including
 * directory management, file I/O, and file manipulation.
 *
 * @note All path parameters should be absolute paths or relative to the current
 *       working directory.
 *
 * @warning File operations may fail due to permission issues, disk space
 *          limitations, or other system-level constraints.
 *
 * @code
 * // Create a directory and file
 * fs_createDirectory("/tmp/mydir", 0755);
 * fs_createFile("/tmp/mydir", "config.txt");
 *
 * // Write and read file content
 * fs_writeFile("/tmp/mydir", "config.txt", "key=value");
 * char *content = fs_readFile("/tmp/mydir", "config.txt");
 * free(content);
 *
 * // Cleanup
 * fs_removeFile("/tmp/mydir", "config.txt");
 * fs_removeDirectory("/tmp/mydir");
 * @endcode
 *
 * @copyright Copyright (c) 2023-2026 sentenz
 * @license SPDX-License-Identifier: Apache-2.0
 */

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

/**
 * @brief Creates a directory at the specified path.
 *
 * Creates a new directory with the specified permissions. Parent directories
 * must already exist.
 *
 * @param[in] path The absolute or relative path of the directory to create.
 * @param[in] mode The permission bits for the new directory (e.g., 0755).
 *
 * @return @c true if the directory was created successfully.
 * @return @c false if creation failed (e.g., path exists, permission denied,
 *         parent directory doesn't exist).
 *
 * @pre @p path must be a valid, non-NULL path string.
 * @pre Parent directory must exist and be writable.
 *
 * @note On POSIX systems, @p mode is modified by the process umask.
 *
 * @see fs_removeDirectory
 */
bool fs_createDirectory(const char* path, const mode_t mode);

/**
 * @brief Removes an empty directory.
 *
 * Deletes the directory at the specified path. The directory must be empty.
 *
 * @param[in] path The absolute or relative path of the directory to remove.
 *
 * @return @c true if the directory was removed successfully.
 * @return @c false if removal failed (e.g., directory not empty, doesn't exist,
 *         permission denied).
 *
 * @pre @p path must be a valid, non-NULL path string.
 * @pre The directory must be empty.
 *
 * @see fs_createDirectory
 */
bool fs_removeDirectory(const char* path);

/**
 * @brief Checks if a file exists in a directory.
 *
 * Verifies that a file with the given name exists in the specified directory.
 *
 * @param[in] path     The directory path to search in.
 * @param[in] filename The name of the file to check for.
 *
 * @return @c true if the file exists and is accessible.
 * @return @c false if the file doesn't exist or cannot be accessed.
 *
 * @pre @p path and @p filename must be valid, non-NULL strings.
 *
 * @note This function checks file existence, not readability or writability.
 */
bool fs_existFile(const char* path, const char* filename);

/**
 * @brief Opens a file for reading, writing, or appending.
 *
 * Opens the specified file with the given mode and returns a file handle.
 *
 * @param[in] path     The directory path containing the file.
 * @param[in] filename The name of the file to open.
 * @param[in] mode     The file access mode (see fopen(3) for valid modes):
 *                     - "r"  : Open for reading (file must exist)
 *                     - "w"  : Open for writing (creates/truncates)
 *                     - "a"  : Open for appending (creates if needed)
 *                     - "r+" : Open for reading and writing
 *                     - "w+" : Open for reading and writing (creates/truncates)
 *                     - "a+" : Open for reading and appending
 *
 * @return Pointer to the opened FILE stream.
 * @return NULL if the file could not be opened.
 *
 * @pre @p path, @p filename, and @p mode must be valid, non-NULL strings.
 *
 * @note The caller is responsible for closing the file using fs_closeFile().
 *
 * @see fs_closeFile
 */
FILE* fs_openFile(const char* path, const char* filename, const char* mode);

/**
 * @brief Closes an open file.
 *
 * Flushes any buffered data and closes the file stream.
 *
 * @param[in] file Pointer to the FILE stream to close.
 *
 * @pre @p file should be a valid file pointer returned by fs_openFile().
 *
 * @note It is safe to pass NULL to this function.
 *
 * @see fs_openFile
 */
void fs_closeFile(FILE* file);

/**
 * @brief Creates an empty file.
 *
 * Creates a new empty file at the specified location. If the file already
 * exists, its contents may be truncated.
 *
 * @param[in] path     The directory path where the file should be created.
 * @param[in] filename The name of the file to create.
 *
 * @return @c true if the file was created successfully.
 * @return @c false if creation failed (e.g., permission denied, path invalid).
 *
 * @pre @p path must be a valid, existing directory.
 * @pre @p filename must be a valid filename.
 *
 * @see fs_removeFile
 */
bool fs_createFile(const char* path, const char* filename);

/**
 * @brief Removes (deletes) a file.
 *
 * Permanently deletes the specified file from the file system.
 *
 * @param[in] path     The directory path containing the file.
 * @param[in] filename The name of the file to remove.
 *
 * @return @c true if the file was removed successfully.
 * @return @c false if removal failed (e.g., file doesn't exist, permission denied).
 *
 * @pre @p path and @p filename must be valid, non-NULL strings.
 *
 * @warning This operation is irreversible.
 *
 * @see fs_createFile
 */
bool fs_removeFile(const char* path, const char* filename);

/**
 * @brief Reads the entire contents of a file into a string.
 *
 * Allocates memory and reads the complete file content as a null-terminated
 * string.
 *
 * @param[in] path     The directory path containing the file.
 * @param[in] filename The name of the file to read.
 *
 * @return A newly allocated null-terminated string containing the file contents.
 * @return NULL if the file could not be read or memory allocation failed.
 *
 * @pre @p path and @p filename must be valid, non-NULL strings.
 * @pre The file must exist and be readable.
 *
 * @note The caller is responsible for freeing the returned string using free().
 *
 * @warning For binary files, use fs_openFile() with binary mode instead.
 *
 * @see fs_writeFile, fs_updateFile
 */
char* fs_readFile(const char* path, const char* filename);

/**
 * @brief Writes a string to a file, replacing existing content.
 *
 * Writes the provided string to the file, overwriting any existing content.
 * Creates the file if it doesn't exist.
 *
 * @param[in] path     The directory path containing the file.
 * @param[in] filename The name of the file to write to.
 * @param[in] str      The null-terminated string to write.
 *
 * @return @c true if the content was written successfully.
 * @return @c false if writing failed.
 *
 * @pre @p path, @p filename, and @p str must be valid, non-NULL strings.
 *
 * @note Existing file content is completely replaced.
 *
 * @see fs_readFile, fs_updateFile
 */
bool fs_writeFile(const char* path, const char* filename, const char* str);

/**
 * @brief Appends a string to an existing file.
 *
 * Appends the provided string to the end of the file without modifying
 * existing content.
 *
 * @param[in] path     The directory path containing the file.
 * @param[in] filename The name of the file to update.
 * @param[in] str      The null-terminated string to append.
 *
 * @return @c true if the content was appended successfully.
 * @return @c false if the operation failed.
 *
 * @pre @p path, @p filename, and @p str must be valid, non-NULL strings.
 * @pre The file should exist (behavior may vary if it doesn't).
 *
 * @see fs_readFile, fs_writeFile
 */
bool fs_updateFile(const char* path, const char* filename, const char* str);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_FS_H_
