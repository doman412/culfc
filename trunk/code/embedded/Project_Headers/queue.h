/*
 * buffer.h
 *
 *  Created on: Dec 3, 2012
 *      Author: ulab
 */

#ifndef QUEUE_H_

#define QUEUE_H_

#define MAX_QUEUE_SIZE 128

/* Queue object */
typedef struct {
    unsigned int         first_index;          
    unsigned int         last_index;
    unsigned int		size;
    char        elems[MAX_QUEUE_SIZE];  /* vector of elements                   */
} Queue;
 
void init_queue(Queue* queue);
 
unsigned char queue_full() ;
 
unsigned char queue_empty() ;
 
void push_front(Queue* queue, char element);
void push_back(Queue* queue, char element);

char pop_front(Queue* queue);
char pop_back(Queue* queue);

#endif /* BUFFER_H_ */
