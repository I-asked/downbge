/* Copyright  (C) 2010-2016 The RetroArch team
 *
 * ---------------------------------------------------------------------------------------
 * The following license statement only applies to this file (gx_pthread.h).
 * ---------------------------------------------------------------------------------------
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <ogcsys.h>
#include <gccore.h>
#include <cond.h>
#include <lwp.h>
#include <lwp_threads.h>

#ifndef OSThread
typedef lwp_t OSThread;
#endif

#ifndef OSCond
typedef lwpq_t OSCond;
#endif

#ifndef OSThreadQueue
typedef lwpq_t OSThreadQueue;
#endif

#ifndef OSInitMutex
#define OSInitMutex(mutex) LWP_MutexInit(mutex, 0)
#endif

#ifndef OSLockMutex
#define OSLockMutex(mutex) LWP_MutexLock(mutex)
#endif

#ifndef OSUnlockMutex
#define OSUnlockMutex(mutex) LWP_MutexUnlock(mutex)
#endif

#ifndef OSTryLockMutex
#define OSTryLockMutex(mutex) LWP_MutexTryLock(mutex)
#endif

#ifndef OSInitCond
#define OSInitCond(cond) LWP_CondInit(cond)
#endif

#ifndef OSWaitCond
#define OSWaitCond(cond, mutex) LWP_CondWait(cond, mutex)
#endif

#ifndef OSInitThreadQueue
#define OSInitThreadQueue(queue) LWP_InitQueue(queue)
#endif

#ifndef OSSleepThread
#define OSSleepThread(queue) LWP_ThreadSleep(queue)
#endif

#ifndef OSJoinThread
#define OSJoinThread(thread, val) LWP_JoinThread(thread, val)
#endif

#ifndef OSCreateThread
#define OSCreateThread(thread, func, intarg, ptrarg, stackbase, stacksize, priority, attrs) LWP_CreateThread(thread, func, ptrarg, stackbase, stacksize, priority)
#endif

#define STACKSIZE (8 * 1024)

typedef OSThread pthread_t;
typedef mutex_t pthread_mutex_t;
typedef void* pthread_mutexattr_t;
typedef int pthread_attr_t;
typedef OSCond pthread_cond_t;
typedef OSCond pthread_condattr_t;

static __attribute__((always_inline)) inline int pthread_create(pthread_t *thread,
      const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg)
{
   *thread = 0;
   return OSCreateThread(thread, start_routine, 0 /* unused */, arg,
         0, STACKSIZE, 64, 0 /* unused */);
}

static __attribute__((always_inline)) inline pthread_t pthread_self(void)
{
   /* zero 20-mar-2016: untested */
   return LWP_GetSelf();
}

static __attribute__((always_inline)) inline int pthread_mutex_init(pthread_mutex_t *mutex,
      const pthread_mutexattr_t *attr)
{
   return OSInitMutex(mutex);
}

static __attribute__((always_inline)) inline int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
   return LWP_MutexDestroy(*mutex);
}

static __attribute__((always_inline)) inline int pthread_mutex_lock(pthread_mutex_t *mutex)
{
   return OSLockMutex(*mutex);
}

static __attribute__((always_inline)) inline int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
   return OSUnlockMutex(*mutex);
}

static __attribute__((always_inline)) inline void pthread_exit(void *retval)
{
   /* FIXME: No LWP equivalent for this? */
   (void)retval;
}

static __attribute__((always_inline)) inline int pthread_detach(pthread_t thread)
{
   /* FIXME: pthread_detach equivalent missing? */
   (void)thread;
   return 0;
}

static __attribute__((always_inline)) inline int pthread_join(pthread_t thread, void **retval)
{
   return OSJoinThread(thread, retval);
}

static __attribute__((always_inline)) inline int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
   return OSTryLockMutex(*mutex);
}

static __attribute__((always_inline)) inline int pthread_cond_wait(pthread_cond_t *cond,
      pthread_mutex_t *mutex)
{
   return OSWaitCond(*cond, *mutex);
}

static __attribute__((always_inline)) inline int pthread_cond_timedwait(pthread_cond_t *cond,
      pthread_mutex_t *mutex, const struct timespec *abstime)
{
   return LWP_CondTimedWait(*cond, *mutex, abstime);
}

static __attribute__((always_inline)) inline int pthread_cond_init(pthread_cond_t *cond,
      const pthread_condattr_t *attr)
{
   return OSInitCond(cond);
}

static __attribute__((always_inline)) inline int pthread_cond_signal(pthread_cond_t *cond)
{
   return LWP_CondSignal(*cond);
}

static __attribute__((always_inline)) inline int pthread_cond_broadcast(pthread_cond_t *cond)
{
   return LWP_CondBroadcast(*cond);
}

static __attribute__((always_inline)) inline int pthread_cond_destroy(pthread_cond_t *cond)
{
   return LWP_CondDestroy(*cond);
}

extern int pthread_equal(pthread_t t1, pthread_t t2);
