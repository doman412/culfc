/*
 * buffer.h
 *
 *  Created on: Dec 3, 2012
 *      Author: ulab
 */

#ifndef BUFFER_H_

#define BUFFER_H_

#define CB_SIZE 10

/* Circular buffer object */
typedef struct {
    int         start;  /* index of oldest element              */
    int         end;    /* index at which to write new element  */
    char        elems[10];  /* vector of elements                   */
} CircularBuffer;
 
void InitBuffer(void);
 
int BufferFull() ;
 
int BufferEmpty() ;
 
/* Write an element, overwriting oldest element if buffer is full. App can
   choose to avoid the overwrite by checking BufferFull(). */
void BufferWrite(char data) ;
 
/* Read oldest element. App must ensure !BufferEmpty() first. */
char BufferRead() ;


#endif /* BUFFER_H_ */
