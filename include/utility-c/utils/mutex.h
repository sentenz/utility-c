// SPDX-License-Identifier: Apache-2.0

#ifndef INCLUDE_UTILITY_C_MUTEX_H_
#define INCLUDE_UTILITY_C_MUTEX_H_

/**
 * @file mutex.h
 * @brief Mutex (mutual exclusion) synchronization primitives.
 *
 * This module provides platform-independent mutex operations for thread
 * synchronization. It abstracts the underlying threading library (POSIX
 * pthreads or eCos) to provide a consistent API.
 *
 * @note On POSIX systems, this module wraps pthread_mutex_t.
 * @note On eCos systems, this module wraps cyg_mutex_t.
 *
 * @warning Mutexes must be properly initialized before use.
 * @warning Failing to unlock a mutex can lead to deadlocks.
 *
 * @code
 * s_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // POSIX initialization
 *
 * void thread_safe_operation(void) {
 *     if (mutex_lock(&mutex) == 0) {
 *         // Critical section - only one thread can execute this at a time
 *         perform_operation();
 *         mutex_unlock(&mutex);
 *     }
 * }
 * @endcode
 *
 * @see types.h for the s_mutex_t type definition
 *
 * @copyright Copyright (c) 2023-2026 sentenz
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifdef S_HAVE_CONFIG_H
  #include "utility-c/config.h"
#endif

#include "utility-c/utils/types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Acquires (locks) a mutex.
 *
 * Attempts to lock the mutex. If the mutex is already locked by another
 * thread, the calling thread will block until the mutex becomes available.
 *
 * @param[in,out] mutex Pointer to the mutex to lock.
 *
 * @retval 0  Success - mutex acquired.
 * @retval -1 Failure (e.g., invalid mutex, deadlock detected).
 *
 * @pre @p mutex must be a valid, initialized mutex.
 * @pre The current thread must not already hold the mutex (to avoid deadlock
 *      with non-recursive mutexes).
 *
 * @post On success, the calling thread owns the mutex.
 *
 * @warning Ensure every mutex_lock() has a corresponding mutex_unlock().
 *
 * @see mutex_unlock
 */
int mutex_lock(s_mutex_t *mutex);

/**
 * @brief Releases (unlocks) a mutex.
 *
 * Releases the mutex, allowing other threads waiting on it to acquire it.
 *
 * @param[in,out] mutex Pointer to the mutex to unlock.
 *
 * @retval 0  Success - mutex released.
 * @retval -1 Failure (e.g., mutex not locked by calling thread).
 *
 * @pre @p mutex must be a valid mutex that is currently locked by the
 *      calling thread.
 *
 * @post The mutex is released and available for other threads to acquire.
 *
 * @see mutex_lock
 */
int mutex_unlock(s_mutex_t *mutex);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_MUTEX_H_
