// File:	my_pthread_t.h
// Author:	Yujie REN
// Date:	09/23/2017

// name:
// username of iLab:
// iLab Server:
#ifndef MY_PTHREAD_T_H
#define MY_PTHREAD_T_H

#define _GNU_SOURCE

#define USE_MY_PTHREAD 1

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#define STACKSIZE 8 * 1024
#define MAXTHREADS 20

typedef uint my_pthread_t;

typedef struct threadControlBlock {
  my_pthread_t thread_id;
  ucontext_t thread_context;
  int isActive;
  int isExecuted;
  int isBlocked;
  int isMain;
  int priority;
  clock_t start_time;
  struct threadControlBlock *next;
  struct blockedThreadList *blockedThreads;
} tcb, *tcb_ptr;

/* mutex struct definition */
typedef struct my_pthread_mutex_t {
  int lock;
  int count;
  volatile my_pthread_t owner;
} my_pthread_mutex_t;

typedef struct threadQueue {
    int size;
	int count;
	tcb_ptr heaparr[MAXTHREADS];
}*thread_Queue;

typedef struct blockedThreadList {
  tcb_ptr thread;
  struct blockedThreadList *next;
}*blockedThreadList_ptr;

typedef struct finishedThread {
  my_pthread_t thread_id;
  void **returnValue;
  struct finishedThread *next;
}*finishedThread_ptr;

typedef struct finishedControlBlockQueue {
  struct finishedThread *thread;
  long count;
}*finished_Queue;

tcb_ptr getControlBlock_Main();
tcb_ptr getControlBlock();
tcb_ptr getCurrentBlockByThread(thread_Queue,my_pthread_t);
tcb_ptr getCurrentBlock(thread_Queue queue);
int getQueueSize(thread_Queue queue);
thread_Queue getQueue();
void freeControlBlock(tcb_ptr);
int next(thread_Queue);
int enqueueToCompletedList(finished_Queue,finishedThread_ptr);
finishedThread_ptr getFinishedThread(finished_Queue,my_pthread_t,int);
blockedThreadList_ptr getBlockedThreadList();
int addToBlockedThreadList(tcb_ptr,tcb_ptr);
finishedThread_ptr getCompletedThread();
finished_Queue getFinishedQueue();

/* Function Declarations: */

// init process
void my_pthread_init(long period);

/* create a new thread */
int my_pthread_create(my_pthread_t * thread, pthread_attr_t * attr, void *(*function)(void*), void * arg);

/* give CPU pocession to other user level threads voluntarily */
int my_pthread_yield();

/* terminate a thread */
void my_pthread_exit(void *value_ptr);

/* wait for thread termination */
int my_pthread_join(my_pthread_t thread, void **value_ptr);

/* initial the mutex lock */
int my_pthread_mutex_init(my_pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr);

/* aquire the mutex lock */
int my_pthread_mutex_lock(my_pthread_mutex_t *mutex);

/* release the mutex lock */
int my_pthread_mutex_unlock(my_pthread_mutex_t *mutex);

/* destroy the mutex */
int my_pthread_mutex_destroy(my_pthread_mutex_t *mutex);

#ifdef USE_MY_PTHREAD
#define pthread_t my_pthread_t
#define pthread_mutex_t my_pthread_mutex_t
#define pthread_create my_pthread_create
#define pthread_exit my_pthread_exit
#define pthread_join my_pthread_join
#define pthread_mutex_init my_pthread_mutex_init
#define pthread_mutex_lock my_pthread_mutex_lock
#define pthread_mutex_unlock my_pthread_mutex_unlock
#define pthread_mutex_destroy my_pthread_mutex_destroy
#endif

#endif
