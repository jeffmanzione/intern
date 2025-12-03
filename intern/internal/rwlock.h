#ifndef COM_GITHUB_JEFFMANZIONE_INTERN_INTERNAL_RWLOCK_H_
#define COM_GITHUB_JEFFMANZIONE_INTERN_INTERNAL_RWLOCK_H_

/**
 * @file rwlock.h
 * @brief Cross-platform abstraction for reader–writer locks (POSIX + Windows).
 *
 * Provides a unified RWLock type and basic operations. On unsupported
 * platforms, all operations call perror("RWLock unimplemented").
 *
 * Usage assumptions:
 *   - rwlock_init() must be called before any lock/unlock operation
 *     unless using RWLOCK_INIT when available.
 *   - Read locks allow multiple concurrent readers.
 *   - Write locks are exclusive.
 */

#include <stdint.h>

#include "intern/internal/platform.h"

#if defined(SYSTEM_POSIX)
#include <pthread.h>
typedef pthread_rwlock_t RWLock;
#define RWLOCK_INIT PTHREAD_RWLOCK_INITIALIZER

#elif defined(SYSTEM_WINDOWS)
typedef SRWLOCK RWLock;
#define RWLOCK_INIT SRWLOCK_INIT

#else
typedef void RWLock;
#endif

/* Initialization and teardown */
void rwlock_init(RWLock *rwlock);
void rwlock_destroy(RWLock *rwlock); /* No-op on Windows */

/* Reader-side operations */
void rwlock_read_lock(RWLock *rwlock);
void rwlock_read_unlock(RWLock *rwlock);

/* Writer-side operations */
void rwlock_write_lock(RWLock *rwlock);
void rwlock_write_unlock(RWLock *rwlock);

#endif /* COM_GITHUB_JEFFMANZIONE_INTERN_INTERNAL_RWLOCK_H_ */
