/*
 * buffer.c
 *
 *  Created on: Dec 3, 2012
 *      Author: ulab
 */
#include "buffer.h"

/* Circular buffer object */

CircularBuffer cb;
 
void InitBuffer(void) {
    cb.start = 0;
    cb.end   = 0;
}
 
int BufferFull() {
    return (cb.end + 1) % CB_SIZE == cb.start;
}
 
int BufferEmpty() {
    return cb.end == cb.start;
}
 
/* Write an element, overwriting oldest element if buffer is full. App can
   choose to avoid the overwrite by checking cbIsFull(). */
void BufferWrite(char data) {
    cb.elems[cb.end] = data;
    cb.end = (cb.end + 1) % CB_SIZE;
    if (cb.end == cb.start) {
        cb.start = (cb.start + 1) % CB_SIZE; /* full, overwrite */
    }
}
 
/* Read oldest element. App must ensure !cbIsEmpty() first. */
char BufferRead() {
    char data = cb.elems[cb.start];
    cb.start = (cb.start + 1) % CB_SIZE;
    return data;
}