// SPDX-License-Identifier: Apache-2.0

#ifndef INCLUDE_UTILITY_C_MUTEX_H_
#define INCLUDE_UTILITY_C_MUTEX_H_

#ifdef S_HAVE_CONFIG_H
  #include "utility-c/config.h"
#endif

#include "utility-c/types.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Locks a mutex.
/// @param mutex The mutex to lock.
/// @return 0 on success, -1 on failure.
int mutex_lock(s_mutex_t *mutex);

/// @brief Unlocks a mutex.
/// @param mutex The mutex to unlock.
/// @return 0 on success, -1 on failure.
int mutex_unlock(s_mutex_t *mutex);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_MUTEX_H_
