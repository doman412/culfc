/*
 * UART.h
 *
 *  Created on: Oct 15, 2012
 *      Author: Derek Arner
 */
#include <stdio.h>
#include "util.h"
#include "derivative.h"

#ifndef UART_H_
#define UART_H_


void initBT(int);
int gotChar(void);
char getChar(void);
void sendChar(char ch);
void diag(char[], int);
void sendStr(char[]);
void echoBT(void);

void BT_REC(void);

void pollBT(void);

void decode(char[]);

#endif /* UART_H_ */
