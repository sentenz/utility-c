// SPDX-License-Identifier: Apache-2.0

#ifndef INCLUDE_UTILITY_C_URI_H_
#define INCLUDE_UTILITY_C_URI_H_

/**
 * @file uri.h
 * @brief URI (Uniform Resource Identifier) parsing utilities.
 *
 * This module provides functions for parsing and extracting components from
 * URI strings according to RFC 3986. It supports extraction of scheme, host,
 * port, user, path, query, and fragment components.
 *
 * @note All extraction functions return NULL if the component is not present
 *       or if the input is invalid.
 *
 * URI structure:
 * @code
 * scheme://[user@]host[:port]/path[?query][#fragment]
 *
 * Example: https://user@example.com:8080/path/to/resource?key=value#section
 *   - scheme:   "https"
 *   - user:     "user"
 *   - host:     "example.com"
 *   - port:     "8080"
 *   - path:     "/path/to/resource"
 *   - query:    "key=value"
 *   - fragment: "section"
 * @endcode
 *
 * @code
 * const char *url = "https://api.example.com:443/v1/users?id=123#details";
 *
 * char *scheme = uri_scheme(url);  // "https"
 * char *host = uri_host(url);      // "api.example.com"
 * char *port = uri_port(url);      // "443"
 * char *path = uri_path(url);      // "/v1/users"
 * char *query = uri_query(url);    // "id=123"
 * char *fragment = uri_fragment(url);  // "details"
 *
 * // Free all allocated strings
 * free(scheme);
 * free(host);
 * free(port);
 * free(path);
 * free(query);
 * free(fragment);
 * @endcode
 *
 * @see https://datatracker.ietf.org/doc/html/rfc3986
 *
 * @copyright Copyright (c) 2023-2026 sentenz
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifdef S_HAVE_CONFIG_H
  #include "utility-c/config.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Extracts the scheme component from a URI.
 *
 * The scheme identifies the protocol (e.g., "http", "https", "ftp", "file").
 *
 * @param[in] str The null-terminated URI string to parse.
 *
 * @return A newly allocated string containing the scheme (without "://").
 * @return NULL if no scheme is present or memory allocation fails.
 *
 * @pre @p str must be a valid null-terminated string.
 *
 * @note The caller is responsible for freeing the returned string using free().
 *
 * @code
 * char *scheme = uri_scheme("https://example.com");
 * // scheme = "https"
 * free(scheme);
 * @endcode
 */
char *uri_scheme(const char *str);

/**
 * @brief Extracts the host component from a URI.
 *
 * The host is the domain name or IP address of the server.
 *
 * @param[in] str The null-terminated URI string to parse.
 *
 * @return A newly allocated string containing the host.
 * @return NULL if no host is present or memory allocation fails.
 *
 * @pre @p str must be a valid null-terminated string.
 *
 * @note The caller is responsible for freeing the returned string using free().
 *
 * @code
 * char *host = uri_host("https://www.example.com:8080/path");
 * // host = "www.example.com"
 * free(host);
 * @endcode
 */
char *uri_host(const char *str);

/**
 * @brief Extracts the port component from a URI.
 *
 * The port number, if explicitly specified in the URI.
 *
 * @param[in] str The null-terminated URI string to parse.
 *
 * @return A newly allocated string containing the port number.
 * @return NULL if no port is specified or memory allocation fails.
 *
 * @pre @p str must be a valid null-terminated string.
 *
 * @note The caller is responsible for freeing the returned string using free().
 * @note Default ports (80 for http, 443 for https) are typically not
 *       included in URIs and will return NULL.
 *
 * @code
 * char *port = uri_port("https://example.com:8443/api");
 * // port = "8443"
 * free(port);
 * @endcode
 */
char *uri_port(const char *str);

/**
 * @brief Extracts the user (userinfo) component from a URI.
 *
 * The user information appears before the host and is separated by '@'.
 * This may include username and optionally password (deprecated).
 *
 * @param[in] str The null-terminated URI string to parse.
 *
 * @return A newly allocated string containing the user information.
 * @return NULL if no user info is present or memory allocation fails.
 *
 * @pre @p str must be a valid null-terminated string.
 *
 * @note The caller is responsible for freeing the returned string using free().
 *
 * @warning Including passwords in URIs is deprecated for security reasons.
 *
 * @code
 * char *user = uri_user("ftp://admin@files.example.com");
 * // user = "admin"
 * free(user);
 * @endcode
 */
char *uri_user(const char *str);

/**
 * @brief Extracts the query component from a URI.
 *
 * The query string contains key-value pairs following the '?' character.
 *
 * @param[in] str The null-terminated URI string to parse.
 *
 * @return A newly allocated string containing the query (without '?').
 * @return NULL if no query is present or memory allocation fails.
 *
 * @pre @p str must be a valid null-terminated string.
 *
 * @note The caller is responsible for freeing the returned string using free().
 * @note The returned string may be URL-encoded and require decoding.
 *
 * @code
 * char *query = uri_query("https://search.example.com?q=hello&lang=en");
 * // query = "q=hello&lang=en"
 * free(query);
 * @endcode
 */
char *uri_query(const char *str);

/**
 * @brief Extracts the fragment component from a URI.
 *
 * The fragment identifier follows the '#' character and typically refers
 * to a secondary resource or anchor within the primary resource.
 *
 * @param[in] str The null-terminated URI string to parse.
 *
 * @return A newly allocated string containing the fragment (without '#').
 * @return NULL if no fragment is present or memory allocation fails.
 *
 * @pre @p str must be a valid null-terminated string.
 *
 * @note The caller is responsible for freeing the returned string using free().
 * @note Fragments are typically processed client-side and not sent to servers.
 *
 * @code
 * char *fragment = uri_fragment("https://docs.example.com/guide#installation");
 * // fragment = "installation"
 * free(fragment);
 * @endcode
 */
char *uri_fragment(const char *str);

/**
 * @brief Extracts the path component from a URI.
 *
 * The path identifies the specific resource within the host.
 *
 * @param[in] str The null-terminated URI string to parse.
 *
 * @return A newly allocated string containing the path.
 * @return NULL if no path is present or memory allocation fails.
 *
 * @pre @p str must be a valid null-terminated string.
 *
 * @note The caller is responsible for freeing the returned string using free().
 * @note The returned path includes the leading '/' if present.
 *
 * @code
 * char *path = uri_path("https://api.example.com/v2/users/123?format=json");
 * // path = "/v2/users/123"
 * free(path);
 * @endcode
 */
char *uri_path(const char *str);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_URI_H_
