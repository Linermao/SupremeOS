#ifndef SPINLOCK_H
#define SPINLOCK_H

#include "types.h"

// Mutual exclusion lock.
struct spinlock { // 24B
  uint locked;       // Is the lock held?

  // For debugging:
  char isNameInitialized;
  char *name;        // Name of lock.
  struct cpu *cpu;   // The cpu holding the lock.
};

#endif