#ifndef CHANNEL_C_INCLUDED
#define CHANNEL_C_INCLUDED
#include "Channel.h"

void CreateChannel (channel* c ,int  nMessages){ 
    c->nMessages=nMessages;
    c->currentWriteIndx=0;
    c->currentReadIndx=0;
    c->messages=malloc(nMessages * sizeof(void*));
    for(int i=0;i<nMessages;i++){
       void* dest =(message*)c->messages + i;
       dest = malloc(sizeof(message));

    }

    c->readMessageLock=malloc(sizeof(sem_t));
    c->sendMessageLock=malloc(sizeof(sem_t));
    sem_init(c->readMessageLock,0,0);
    sem_init(c->sendMessageLock,0,nMessages);

    c->readIndexLock=malloc(sizeof(sem_t));
    c->writeIndexLock=malloc(sizeof(sem_t));
    sem_init(c->readIndexLock , 0 , 1);
    sem_init(c->writeIndexLock, 0 , 1);
}

void SendMessage (channel* c ,  message* sentMessage) {

    sem_wait(c->sendMessageLock);
    int index;

    sem_wait(c->writeIndexLock);

    index=c->currentWriteIndx % c->nMessages;
    c->currentWriteIndx++;

    sem_post(c->writeIndexLock);

    message* msg  = (message*)c->messages + index; 
    msg->messageSize=sentMessage->messageSize;
    msg->messageAddr = malloc(sentMessage->messageSize);
    
    memcpy(msg->messageAddr , sentMessage->messageAddr , sentMessage->messageSize);
    sem_post(c->readMessageLock);






}

message* ReadMessage(channel *c ){
    sem_wait(c->readMessageLock);
    int index;

    sem_wait(c->readIndexLock);

    index=c->currentReadIndx%c->nMessages;
    c->currentReadIndx++;

    sem_post(c->readIndexLock);

    message* msgToRead =  (message*)c->messages + index; 
    message* retMsg = malloc(sizeof(message));
    retMsg->messageAddr=malloc(msgToRead->messageSize);
    retMsg->messageSize=msgToRead->messageSize;
    memcpy(retMsg->messageAddr , msgToRead->messageAddr,msgToRead->messageSize);
    free(msgToRead->messageAddr);    
 

    sem_post(c->sendMessageLock);
    
    return retMsg;
}
void ClearChannel(channel* c){
    for(int i=0;i<c->nMessages;i++){
        message* dest =(message*)c->messages + i;
        free(dest->messageAddr);


    }
    free(c->messages);
    sem_destroy(c->writeIndexLock);
    free(c->writeIndexLock);
    sem_destroy(c->sendMessageLock);
    free(c->sendMessageLock);
    sem_destroy(c->readMessageLock);
    free(c->readMessageLock);
    sem_destroy(c->readIndexLock);
    free(c->readIndexLock);



}
#endif
