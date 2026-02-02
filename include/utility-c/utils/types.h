// SPDX-License-Identifier: Apache-2.0

#ifndef INCLUDE_UTILITY_C_TYPES_H_
#define INCLUDE_UTILITY_C_TYPES_H_

/**
 * @file types.h
 * @brief Platform-independent type definitions.
 *
 * This module provides platform-independent type definitions to abstract
 * differences between operating systems and threading libraries. It enables
 * portable code that works across POSIX systems (Linux, macOS) and eCos
 * embedded systems.
 *
 * @note On POSIX systems, mutex types wrap pthread_mutex_t.
 * @note On eCos systems, mutex types wrap cyg_mutex_t.
 *
 * @code
 * #include "utility-c/utils/types.h"
 *
 * // Platform-independent mutex declaration
 * s_mutex_t my_mutex;
 *
 * // On POSIX, this is equivalent to:
 * // pthread_mutex_t my_mutex;
 * @endcode
 *
 * @see mutex.h for mutex operations
 *
 * @copyright Copyright (c) 2023-2026 sentenz
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifdef S_HAVE_CONFIG_H
  #include "utility-c/config.h"
#endif

#ifdef S_POSIX
  #include <pthread.h>
  #include <stdbool.h>
#else
  #include <cyg/kernel/kapi.h>
#endif  // S_POSIX

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Platform-independent mutex type.
 *
 * Provides a portable mutex type that abstracts the underlying threading
 * library. Use with mutex_lock() and mutex_unlock() from mutex.h.
 *
 * - On POSIX systems: Aliases to pthread_mutex_t
 * - On eCos systems: Aliases to cyg_mutex_t
 *
 * @see mutex_lock, mutex_unlock
 */
#ifdef S_POSIX
  #define s_mutex_t pthread_mutex_t
#else
  #define s_mutex_t cyg_mutex_t
#endif  // S_POSIX

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_TYPES_H_
