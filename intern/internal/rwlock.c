#include "intern/internal/rwlock.h"

#include <stdio.h>
#include <stdlib.h>

#if defined(SYSTEM_WINDOWS)
#include <windows.h>
#endif

/* Consistent error handler for unsupported platforms */
#if !defined(SYSTEM_WINDOWS) && !defined(SYSTEM_POSIX)
#define RWLOCK_UNIMPLEMENTED() perror("RWLock unimplemented")
#endif

void rwlock_init(RWLock *rwlock) {
#if defined(SYSTEM_WINDOWS)
  /* SRW locks require no allocation or configuration */
  InitializeSRWLock(rwlock);
#elif defined(SYSTEM_POSIX)
  pthread_rwlock_init(rwlock, NULL);
#else
  RWLOCK_UNIMPLEMENTED();
#endif
}

void rwlock_destroy(RWLock *rwlock) {
#if defined(SYSTEM_POSIX)
  /* POSIX locks must be explicitly destroyed */
  pthread_rwlock_destroy(rwlock);
#else
  /* Windows SRW locks need no destruction */
#if !defined(SYSTEM_WINDOWS)
  RWLOCK_UNIMPLEMENTED();
#endif
#endif
}

void rwlock_read_lock(RWLock *rwlock) {
#if defined(SYSTEM_WINDOWS)
  AcquireSRWLockShared(rwlock);
#elif defined(SYSTEM_POSIX)
  pthread_rwlock_rdlock(rwlock);
#else
  RWLOCK_UNIMPLEMENTED();
#endif
}

void rwlock_read_unlock(RWLock *rwlock) {
#if defined(SYSTEM_WINDOWS)
  ReleaseSRWLockShared(rwlock);
#elif defined(SYSTEM_POSIX)
  pthread_rwlock_unlock(rwlock);
#else
  RWLOCK_UNIMPLEMENTED();
#endif
}

void rwlock_write_lock(RWLock *rwlock) {
#if defined(SYSTEM_WINDOWS)
  AcquireSRWLockExclusive(rwlock);
#elif defined(SYSTEM_POSIX)
  pthread_rwlock_wrlock(rwlock);
#else
  RWLOCK_UNIMPLEMENTED();
#endif
}

void rwlock_write_unlock(RWLock *rwlock) {
#if defined(SYSTEM_WINDOWS)
  ReleaseSRWLockExclusive(rwlock);
#elif defined(SYSTEM_POSIX)
  pthread_rwlock_unlock(rwlock);
#else
  RWLOCK_UNIMPLEMENTED();
#endif
}
