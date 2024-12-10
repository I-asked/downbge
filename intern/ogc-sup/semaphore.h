#pragma once

#include_next <semaphore.h>

#define sem_init LWP_SemInit

#define sem_post(s) LWP_SemPost(*s)

#define sem_destroy(s) LWP_SemDestroy(*s)

#define sem_wait(s) LWP_SemWait(*s)

#define sem_trywait(s) LWP_SemTryWait(*s)

inline __attribute__((always_inline)) s32 LWP_SemTryWait(sem_t sem) {
  u32 res = 0;
  s32 err = 0;
  err = LWP_SemGetValue(sem, &res);
  if (err)  return err;
  else      return res;
}
