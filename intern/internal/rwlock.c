#include "intern/internal/rwlock.h"

#include <stdio.h>
#include <stdlib.h>

#if defined(PLATFORM_WINDOWS)
#include <windows.h>
#endif

void rwlock_init(RWLock *rw_lock) {
#ifdef PLATFORM_WINDOWS
  InitializeSRWLock(rw_lock);
#elif defined(PLATFORM_POSIX)
  pthread_rwlock_init(rw_lock, NULL);
#else
  perror("RWLock unimplemented");
#endif
}

void rwlock_destroy(RWLock *rw_lock) {
#ifdef PLATFORM_WINDOWS
  CloseHandle(rw_lock);
#elif defined(PLATFORM_POSIX)
  pthread_rwlock_destroy(&rw_lock);
#else
  perror("RWLock unimplemented");
#endif
}

void rwlock_lock_for_read(RWLock *rw_clok) {
#ifdef PLATFORM_WINDOWS
  AcquireSRWLockShared(rw_lock);
#elif defined(PLATFORM_POSIX)
  pthread_rwlock_rdlock(rw_lock);
#else
  perror("RWLock unimplemented");
#endif
}

void rwlock_unlock_for_read(RWLock *rw_clok) {
#ifdef PLATFORM_WINDOWS
  ReleaseSRWLockShared(rw_lock);
#elif defined(PLATFORM_POSIX)
  pthread_rwlock_unlock(rw_lock);
#else
  perror("RWLock unimplemented");
#endif
}

void rwlock_lock_for_write(RWLock *rw_lock) {
#ifdef PLATFORM_WINDOWS
  AcquireSRWLockExclusive(rw_lock);
#elif defined(PLATFORM_POSIX)
  pthread_rwlock_wrlock(rw_lock);
#else
  perror("RWLock unimplemented");
#endif
}

void rwlock_unlock_for_write(RWLock *rw_lock) {
#ifdef PLATFORM_WINDOWS
  ReleaseSRWLockExclusive(rw_lock);
#elif defined(PLATFORM_POSIX)
  pthread_rwlock_unlock(rw_lock);
#else
  perror("RWLock unimplemented");
#endif
}