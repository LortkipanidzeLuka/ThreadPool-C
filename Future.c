#ifndef Future_C_INCLUDED
#define Future_C_INCLUDED

#include "Future.h"




void* FutureFunc(void* args){

    future* f = (future*) args;
    void* retVal = f->func(f->args);
    memcpy(f->retVal,retVal,f->elem_size);
    free(retVal);

    
    return NULL;

}

void* CreateFuture(future* f ,void* (*func)(void*) , void* args , int elemSize){
    f->elem_size = elemSize;
    f->retVal=malloc(elemSize);
    f->thread = malloc(sizeof (pthread_t));
    f->args=args;
    f->func=func;
    pthread_create(f->thread , NULL , FutureFunc , f);
    
    return f->retVal;
    
}

void getFuture(future* f){
    pthread_join(*(f->thread) , NULL);

}

void ClearFuture(future* f){
    free(f->retVal);
    free(f->thread);


}
#endif