/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   buffer.c
 * Author: oscreader
 * 
 * Created on April 27, 2018, 12:10 PM
 */

#include "buffer.h"
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>

buffer_item buffer[BUFFER_SIZE];
int front;
int rear;

pthread_mutex_t mutex;

sem_t empty;
sem_t full;

void initialize_buffer(){
    int i;
    for(i = 0; i<BUFFER_SIZE; i++){
        buffer[i] = 0;
    }
    front = 0;
    rear = 0;
    pthread_mutex_init(&mutex,NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
}

int insert_item(buffer_item item, int* message, char* errorType){
        
    int checkIfThereAreFreeSpots;
    int tryToLockCriticalSection;
    int tryToUnlockCriticalSection;
    int attemptToReleaseAnyLockedConsumerThread;
    
    checkIfThereAreFreeSpots = sem_wait(&empty);
    if(checkIfThereAreFreeSpots != 0){
        *message = checkIfThereAreFreeSpots;
        strcpy(errorType, "producer sem wait empty");
        return -1;
    }
    
    tryToLockCriticalSection = pthread_mutex_lock(&mutex);
    if(tryToLockCriticalSection != 0){
        *message = tryToLockCriticalSection;
        strcpy(errorType, "producer lock");
        return -1;
    }
    
    buffer[rear] = item;
    rear = (rear + 1) % BUFFER_SIZE;
    
    /*
     * Post mortem
     * 
     * I have to put these two debug functions here to guarantee that the buffer
     * and control variables are in a stable state before printing.
     * The problem with this is that the semaphore that control the producers
     * and consumer are not properly updated until after the critical section.
     * 
     * The solution? just add 1 to the relevant semaphore knowing that they will 
     * increment in the future. This will make validating results easier. 
     */

    
#ifdef DEBUG
    printCurrentBufferContents();
    printCurrentBufferControlVariables(1,0);
#endif
    
    tryToUnlockCriticalSection = pthread_mutex_unlock(&mutex);
    if(tryToUnlockCriticalSection != 0){
        *message = tryToUnlockCriticalSection;
        strcpy(errorType, "producer unlock");
        return -1;
    }
    
    attemptToReleaseAnyLockedConsumerThread = sem_post(&full);
    if(attemptToReleaseAnyLockedConsumerThread != 0){
        *message = attemptToReleaseAnyLockedConsumerThread;
        strcpy(errorType, "producer sem post full");
        return -1;
    }
    
    return 0;
    
}

int remove_item(buffer_item* item, int* message, char* errorType){
    
    int checkIfThereAreItems;
    int tryToLockCriticalSection;
    int tryToUnlockCriticalSection;
    int attemptToReleaseAnyLockedProducerThread;
    
    checkIfThereAreItems = sem_wait(&full);
    if(checkIfThereAreItems != 0){
        *message = checkIfThereAreItems;
        strcpy(errorType, "consumer sem wait full");
        return -1;
    }
    
    tryToLockCriticalSection = pthread_mutex_lock(&mutex);
    if(tryToLockCriticalSection != 0){
        *message = tryToLockCriticalSection;
        strcpy(errorType, "consumer lock");
        return -1;
    }
    
    *item = buffer[front];
    front = (front + 1) % BUFFER_SIZE;
    
    /*
     * Post mortem
     * 
     * I have to put these two debug functions here to guarantee that the buffer
     * and control variables are in a stable state before printing.
     * The problem with this is that the semaphore that control the producers
     * and consumer are not properly updated until after the critical section.
     * 
     * The solution? just add 1 to the relevant semaphore knowing that they will 
     * increment in the future. This will make validating results easier. 
     */
    
#ifdef DEBUG
    printCurrentBufferContents();
    printCurrentBufferControlVariables(0,1);
#endif
    
    tryToUnlockCriticalSection = pthread_mutex_unlock(&mutex);
    if(tryToUnlockCriticalSection != 0){
        *message = tryToUnlockCriticalSection;
        strcpy(errorType, "consumer unlock");
        return -1;
    }
    
    attemptToReleaseAnyLockedProducerThread = sem_post(&empty);
    if(attemptToReleaseAnyLockedProducerThread != 0){
        *message = attemptToReleaseAnyLockedProducerThread;
        strcpy(errorType, "consumer sem post empty");
        return -1;
    }
        
    return 0;
}

void printCurrentBufferContents()
{
    int i;
    printf("currentBufferContents: \n");
    for(i = 0; i<BUFFER_SIZE; i++){
        printf("buffer[%d]: %d\n",i,buffer[i]);
    }
}

void printCurrentBufferControlVariables(int incFull, int incEmpty){
    
    int currentFullCount;
    int currentEmptyCount;
    int currentFrontPointer;
    int currentRearPointer;
    
    sem_getvalue(&empty, &currentEmptyCount);
    sem_getvalue(&full, &currentFullCount);
    currentFrontPointer = front;
    currentRearPointer = rear;
    
    printf("empty_sem: %d full_sem: %d frontptr: %d rearptr: %d\n",\
            currentEmptyCount+incEmpty,currentFullCount+incFull,\
            currentFrontPointer,currentRearPointer);

}