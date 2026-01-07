// SPDX-License-Identifier: Apache-2.0

#ifndef INCLUDE_UTILITY_C_TYPES_H_
#define INCLUDE_UTILITY_C_TYPES_H_

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

#ifdef S_POSIX
  #define s_mutex_t pthread_mutex_t
#else
  #define s_mutex_t cyg_mutex_t
#endif  // S_POSIX

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_UTILITY_C_TYPES_H_
