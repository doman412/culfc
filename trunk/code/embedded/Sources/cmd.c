/*
 * cmd.c
 *
 *  Created on: Oct 31, 2012
 *      Author: Derek Arner
 */
#include "cmd.h"
#include "BT.h"

Command commandList[26];

void default_cmd(float a){
	// defualt command function
}


//void cmdB(int a){ breakCar(); opGo(0); }





void initCommands(void){
	commandList[0] = &default_cmd;				// a
	commandList[1] = &default_cmd;						// b
	commandList[2] = &default_cmd;				// c
	commandList[3] = &default_cmd;				// d
	commandList[4] = &default_cmd;				// e
	commandList[5] = &default_cmd;				// f
	commandList[6] = &default_cmd;				// g
	commandList[7] = &default_cmd;				// h
	commandList[8] = &default_cmd;				// i
	commandList[9] = &default_cmd;				// j
	commandList[10] = &default_cmd;				// k
	commandList[11] = &default_cmd;				// l
	commandList[12] = &default_cmd;				// m
	commandList[13] = &default_cmd;				// n
	commandList[14] = &default_cmd;				// o
	commandList[15] = &default_cmd;				// p
	commandList[16] = &default_cmd;				// q
	commandList[17] = &default_cmd;				// r
	commandList[18] = &default_cmd;					// s
	commandList[19] = &default_cmd;				// t
	commandList[20] = &default_cmd;				// u
	commandList[21] = &default_cmd;				// v
	commandList[22] = &default_cmd;				// w
	commandList[23] = &default_cmd;				// x
	commandList[24] = &default_cmd;				// y
	commandList[25] = &default_cmd;				// z
}

