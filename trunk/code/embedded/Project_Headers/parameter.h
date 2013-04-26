/*
 * parameter.h
 *
 *  Created on: Mar 26, 2013
 *      Author: ulab
 */
#ifndef PARAMETER_H_
#define PARAMETER_H_

#include <derivative.h>
#include "mode.h"

//#pragma define_section configtable ".configtable" ".configtable" ".configtable" far_abs RW

#define NUM_VARIABLES 11
#define NUM_FLASH_VARIABLES 3


typedef struct {
	char* name;
	float* ptr;
} variable_entry_t;

extern Mode mode;
extern int ECHO;
extern int line_capture;

void update_table();
int change_parameter(char* name, float value);
int read_parameter(char* name, float* value);

#endif /* PARAMETER_H_ */
