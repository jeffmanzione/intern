#ifndef COM_GITHUB_JEFFMANZIONE_INTERN_INTERNAL_INTERN_RWLOCK_H_
#define COM_GITHUB_JEFFMANZIONE_INTERN_INTERNAL_INTERN_RWLOCK_H_

#include <stdint.h>

#include "intern/internal/platform.h"

#if defined(SYSTEM_POSIX)
#include <pthread.h>
typedef pthread_rwlock_t RWLock;
#elif defined(SYSTEM_WINDOWS)
typedef SRWLOCK RWLock;
#else
typedef void RWLock;
#endif

void rwlock_init(RWLock *rw_lock);
void rwlock_destroy(RWLock *rw_lock);
void rwlock_lock_for_read(RWLock *rw_clok);
void rwlock_lock_for_write(RWLock *rw_lock);
void rwlock_unlock_for_read(RWLock *rw_clok);
void rwlock_unlock_for_write(RWLock *rw_lock);

#endif /* COM_GITHUB_JEFFMANZIONE_INTERN_INTERNAL_INTERN_RWLOCK_H_ */
