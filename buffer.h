/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   buffer.h
 * Author: oscreader
 *
 * Created on April 27, 2018, 12:10 PM
 */

#ifndef BUFFER_H
#define BUFFER_H

#define BUFFER_SIZE 5

typedef int buffer_item; 

void initialize_buffer();
int insert_item(buffer_item item, int* message, char* errorType);
int remove_item(buffer_item* item, int* message, char* errorType);
void printCurrentBufferControlVariables(int incFull, int incEmpty);
void printCurrentBufferContents();

#endif /* BUFFER_H */
