/*
 * parameter.h
 *
 *  Created on: Mar 26, 2013
 *      Author: ulab
 */
#ifndef PARAMETER_H_
#define PARAMETER_H_

#include <derivative.h>
#include <string.h>

#pragma define_section configtable ".configtable" ".configtable" ".configtable" far_abs RW

#define NUM_VARIABLES 3

//typedef union {
//	float f;
//	int i;
//	double d;
//	long l;
//} flash_parameter_t;

typedef struct {
	char* name;
	float* ptr;
} variable_entry_t;

//#define KP (parms[0])
//#define KI (parms[1])
//#define KD (parms[2])

void update_table();
int change_parameter(char* name, float value);
int read_parameter(char* name, float* value);

#endif /* PARAMETER_H_ */
