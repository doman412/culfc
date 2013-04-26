/*
 * buffer.c
 *
 *  Created on: Dec 3, 2012
 *      Author: ulab
 */
#include "queue.h"


// Push = change data, then index
// Pop = change index, then return data
// "last_index" gets initialized to 0
// "first_index" gets initialized to QUEUE_SIZE - 1



void init_queue(Queue* queue) {
    queue->last_index = 0;
    queue->first_index = MAX_QUEUE_SIZE - 1;
    queue->size = 0;
}
 
unsigned char queue_full(Queue* queue) {
	return queue->size >= MAX_QUEUE_SIZE;
}
 
unsigned char queue_empty(Queue* queue) {
	return queue->size == 0;
}
 
void push_front(Queue* queue, char element) {
	// Change data, then index
	unsigned int index = queue->last_index;
	queue->elems[index] = element;
	queue->last_index = (index + 1) % MAX_QUEUE_SIZE;
	queue->size++;
	
}
void push_back(Queue* queue, char element) {
	// Change data, then index
	unsigned int index = queue->first_index;
	queue->elems[index] = element;
	queue->first_index = 
		(index + MAX_QUEUE_SIZE-1) % MAX_QUEUE_SIZE;
	queue->size++;
}

char pop_front(Queue* queue) {
	// Change index, then data
	unsigned int index = queue->last_index;
	index = 
			(index + MAX_QUEUE_SIZE-1) % MAX_QUEUE_SIZE;
	queue->last_index = index;
	queue->size--;
	return queue->elems[index];
}
char pop_back(Queue* queue) {
	// Change index, then data
	unsigned int index = queue->first_index;
	index = (index + 1) % MAX_QUEUE_SIZE;
	queue->first_index = index;
	queue->size--;
	return queue->elems[index];
}