/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: oscreader
 *
 * Created on April 27, 2018, 12:09 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"
#include <pthread.h>
#include <unistd.h>

#define RAND_SLEEP_MAX 4

/*
 * 
 */

void* produce(void* index){
    buffer_item item;
    int errorMessage;
    char errorType[50];

    while(1){
        sleep(rand()%RAND_SLEEP_MAX);
        item = rand();
        if(insert_item(item, &errorMessage, errorType)){
            fprintf(stderr, "error has occurred when attempting to insert an item");
        }
        else{
            

#ifdef DEBUG
            printf("producer %d produced %d\n",(int)index, item);
#else
            printf("producer produced %d\n", item);
#endif 

            
        }
    }
}

void* consume(void* index){
    buffer_item item;
    int errorMessage;
    char errorType[50];
    
    while(1){
        sleep(rand()%RAND_SLEEP_MAX);
        item = rand();
        if(remove_item(&item, &errorMessage, errorType)){
            fprintf(stderr, "error has occurred when attempting to remove an item");
        }
        else{
#ifdef DEBUG
            printf("consumer %d consumed %d\n",(int)index, item);
#else 
            printf("consumer consumed %d\n", item);
#endif
        }
    }
}

int main(int argc, char** argv) {
    
    int producerThreads = atoi(argv[2]);
    int consumerThreads = atoi(argv[3]);
    int sleepTime = atoi(argv[1]);
    int index;
    
    pthread_t producer[producerThreads], consumer[consumerThreads];
    pthread_attr_t attr;
    
    /*
     * Initialize the buffer and pthread. The abstraction of The buffer is 
     * maintained with no implementation details present in main for buffer.
     */
    
    initialize_buffer();
    pthread_attr_init(&attr);
    
    /*
     *create producer and consumer threads. Each thread has it's own index 
     *identifier. The identifier will not be printed to standard output unless
     *main.c is compiled with DDEBUG directive
     */
    
    for(index = 0; index<producerThreads; index++)
    {
        pthread_create(&(producer[index]), &attr, produce, (void*)index);
    }
    
    for(index = 0; index<consumerThreads; index++)
    {
        pthread_create(&(consumer[index]), &attr, consume, (void*)index);
    }
    
    sleep(sleepTime);

    return (EXIT_SUCCESS);
}

