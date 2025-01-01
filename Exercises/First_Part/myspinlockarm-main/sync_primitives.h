// sync_primitives.h
#ifndef SYNC_PRIMITIVES_H
#define SYNC_PRIMITIVES_H

#include <stdint.h>

// Spinlock
typedef struct {
    volatile int lock;
} spinlock_t;

void spinlock_init(spinlock_t *spinlock);
void spinlock_acquire(spinlock_t *spinlock);
void spinlock_release(spinlock_t *spinlock);


#endif
