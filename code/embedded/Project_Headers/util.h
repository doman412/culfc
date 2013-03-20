/*
 * util.h
 *
 *  Created on: Oct 15, 2012
 *      Author: Derek Arner
 */
#include <stdio.h>
#include "derivative.h"


#ifndef UTIL_H_
#define UTIL_H_

//void enable_irq(int);

int findChar(char, char[]);
int strLen(char []);
int parseInt(char str[]);
void enable_irq(int);
void blocking_delay(int time_approximation);
#endif /* UTIL_H_ */
