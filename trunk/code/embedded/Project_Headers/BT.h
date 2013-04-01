/*
 * BT.h
 *
 *  Created on: Oct 15, 2012
 *      Author: Derek Arner
 */
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "derivative.h"

#ifndef BT_H_
#define BT_H_


void initBT(int);
int gotChar(void);
char getChar(void);
void sendChar(char ch);
void sendStr(char[]);
void echoBT(void);

void bt_isr(void);

void decodeBT(void);
void decode(char[]);

#endif /* BT_H_ */
