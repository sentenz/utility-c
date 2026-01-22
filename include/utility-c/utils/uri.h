// SPDX-License-Identifier: Apache-2.0

#ifndef INCLUDE_UTILITY_C_URI_H_
#define INCLUDE_UTILITY_C_URI_H_

#ifdef S_HAVE_CONFIG_H
  #include "utility-c/config.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Extracts the scheme component of a URI.
/// @param str The URI string.
/// @return The scheme component of the URI.
/// @note The caller is responsible for freeing the memory using free().
char *uri_scheme(const char *str);

/// @brief Extracts the host component of a URI.
/// @param str The URI string.
/// @return The host component of the URI.
/// @note The caller is responsible for freeing the memory using free().
char *uri_host(const char *str);

/// @brief Extracts the port component of a URI.
/// @param str The URI string.
/// @return The port component of the URI.
/// @note The caller is responsible for freeing the memory using free().
char *uri_port(const char *str);

/// @brief Extracts the user component of a URI.
/// @param str The URI string.
/// @return The user component of the URI.
/// @note The caller is responsible for freeing the memory using free().
char *uri_user(const char *str);

/// @brief Extracts the query component of a URI.
/// @param str The URI string.
/// @return The query component of the URI.
/// @note The caller is responsible for freeing the memory using free().
char *uri_query(const char *str);

/// @brief Extracts the fragment component of a URI.
/// @param str The URI string.
/// @return The fragment component of the URI.
/// @note The caller is responsible for freeing the memory using free().
char *uri_fragment(const char *str);

/// @brief Extracts the path component of a URI.
/// @param str The URI string.
/// @return The path component of the URI.
/// @note The caller is responsible for freeing the memory using free().
char *uri_path(const char *str);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_URI_H_
