#ifndef CHANNEL_H_INCLUDED
#define CHANNEL_H_INCLUDED

#include "queue.h"
#include  <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct 
{
    int nMessages;
    void* messages;
    sem_t* readMessageLock;
    sem_t* sendMessageLock;
    sem_t* readIndexLock;
    sem_t* writeIndexLock;
    int currentReadIndx;
    int currentWriteIndx;
    

}channel;

typedef struct 
{   
   int messageSize; 
   void* messageAddr;
   
}message;

void CreateChannel (channel* c ,int  nMessages);
void SendMessage (channel* c ,  message* sentMessage);
message* ReadMessage(channel* c );
void ClearChannel(channel* c);

#endif