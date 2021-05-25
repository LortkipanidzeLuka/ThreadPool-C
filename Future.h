#ifndef Future_H_INCLUDED
#define Future_H_INCLUDED

#include "queue.h"
#include  <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{
    void* (*func) (void*) ;
    void* args;
    int elem_size;
    void* retVal;
    pthread_t* thread;

}future;

void* CreateFuture(future* f ,void* (*func)(void*) , void* args , int elemSize);
void getFuture(future* f );
void ClearFuture(future* f);

#endif