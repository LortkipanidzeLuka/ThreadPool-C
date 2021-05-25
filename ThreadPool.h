#ifndef THREADPOOL_H_INCLUDED
#define THREADPOOL_H_INCLUDED

#include "queue.h"
#include  <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct 
{
    void* args;
    void (*func) (void*);
} job ;



typedef struct 
{
    int nThreads;
    sem_t* queueLock;
    sem_t* isElemLock;
    sem_t* exitLock;
    pthread_t** threads;
    int exit ;
    Queue* works;

}ThreadPool;


void ThreadPoolInit(ThreadPool*  pool, int nThreads);
void ThreadPoolSchedule(ThreadPool* pool, void (*func)(void*), void* args);
void ThreadPoolShutdown(ThreadPool* pool);
void ThreadPoolClear(ThreadPool* pool);



#endif
