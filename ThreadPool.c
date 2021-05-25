#ifndef THREADPOOL_C_INCLUDED
#define THREADPOOL_C_INCLUDED
#include "ThreadPool.h"

void* worker (void* args){
    ThreadPool* pool = (ThreadPool*) args;
    while (1)
    {

            
       sem_wait(pool->isElemLock);
        
        sem_wait(pool->exitLock);
        if(pool->exit == 1){
            printf("exit thread\n");
            sem_post(pool->exitLock);
            break;
        }
        sem_post(pool->exitLock); 
        sem_wait(pool->queueLock);

        job ** currentJob = malloc(sizeof(job*));
        if(getQueueSize(pool->works) != 0) {

            
            dequeue(pool->works , currentJob);
            sem_post(pool->queueLock);

        }else{
            sem_post(pool->queueLock);
            continue;

        }
        (*currentJob)->func((*currentJob)->args);
        free(currentJob);

        }

    return NULL;
    


}

void ThreadPoolInit(ThreadPool*  pool, int nThreads) {
    pool->nThreads = nThreads ;
    pool->exit=0;

    pool->threads= malloc (sizeof(pthread_t*)* nThreads);
    pool->isElemLock = malloc(sizeof(sem_t));
    pool->queueLock = malloc(sizeof(sem_t));
    pool->exitLock= malloc(sizeof(sem_t));
    pool->works = malloc (sizeof (Queue));

    sem_init(pool->queueLock , 0 , 1 );
    sem_init(pool->isElemLock , 0, 0);
    sem_init(pool->exitLock,0,1);
    queueInit(pool->works ,sizeof(job*) , NULL );
    

    for(int i=0 ; i < nThreads ; i++){
        pool->threads[i] = malloc(sizeof(pthread_t));
    }
 
    for (int i=0;i<nThreads;i++){
        printf("thread created\n");
        pthread_create(pool->threads[i],NULL,worker,pool);
    }

}
void ThreadPoolClear(ThreadPool* pool){
    clearQueue(pool->works);
    free(pool->works);
    sem_destroy(pool->isElemLock);
    sem_destroy(pool->queueLock);
    sem_destroy(pool->exitLock);
    free(pool->exitLock);
    free(pool->isElemLock);
    free(pool->queueLock);
     for(int i=0 ; i < pool-> nThreads ; i++){
        free(pool->threads[i]);
    }
    free(pool->threads);
    


    
}
void ThreadPoolSchedule(ThreadPool* pool, void (*func)(void*), void* args){
    job** newJobAddr = malloc(sizeof (job*));
    job* newJob = malloc(sizeof (job));
    *newJobAddr = newJob;
    newJob->args=args;
    newJob->func=func;

    enqueue(pool->works,newJobAddr);
    
    free(newJob);
    free(newJobAddr);
 
    sem_post(pool->isElemLock);
    
}

void ThreadPoolShutdown(ThreadPool* pool) {
    printf("pool shutdown started \n");

    sem_wait(pool->exitLock);
    pool->exit=1;
    sem_post(pool->exitLock);

    for(int i=0;i<pool->nThreads;i++)
        sem_post(pool->isElemLock);

    for(int i=0;i<pool->nThreads;i++){
        pthread_join(*(pool->threads[i]), NULL);

    }
    
    ThreadPoolClear(pool);
    printf("pool shutdown finished\n\n\n");

    
}













#endif



