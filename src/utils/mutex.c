// SPDX-License-Identifier: Apache-2.0

#include "utility-c/mutex.h"

#ifdef S_POSIX

static int internal_posix_mutex_lock(pthread_mutex_t *mutex) {
  int err = pthread_mutex_init(mutex, NULL);
  if (0 != err) {
    return err;
  }
  return pthread_mutex_lock(mutex);
}

static int internal_posix_mutex_unlock(pthread_mutex_t *mutex) {
  int err = pthread_mutex_unlock(mutex);
  if (0 != err) {
    return err;
  }
  return pthread_mutex_destroy(mutex);
}

#else

static int internal_ecos_mutex_lock(cyg_mutex_t *mutex) {
  cyg_mutex_init(mutex);
  bool err = cyg_mutex_lock(mutex);
  if (!err) {
    return -1;
  }
  return 0;
}

static int internal_ecos_mutex_unlock(cyg_mutex_t *mutex) {
  cyg_mutex_unlock(mutex);
  cyg_mutex_destroy(mutex);
  return 0;
}

#endif  // S_POSIX

static int internal_mutex_lock(s_mutex_t *mutex) {
#ifdef S_POSIX
  return internal_posix_mutex_lock(mutex);
#else
  return internal_ecos_mutex_lock(mutex);
#endif  // S_POSIX
}

static int internal_mutex_unlock(s_mutex_t *mutex) {
#ifdef S_POSIX
  return internal_posix_mutex_unlock(mutex);
#else
  return internal_ecos_mutex_unlock(mutex);
#endif  // S_POSIX
}

int mutex_lock(s_mutex_t *mutex) {
  return internal_mutex_lock(mutex);
}

int mutex_unlock(s_mutex_t *mutex) {
  return internal_mutex_unlock(mutex);
}
