/*
 * parameter.c
 *
 *  Created on: Mar 26, 2013
 *      Author: ulab
 */
#include "parameter.h"

__declspec(configtable) float __parms[NUM_VARIABLES];

variable_entry_t variables[] = {
		{
			.name = "kp",
			.ptr = &__parms[0]
		},
		{
			.name = "ki",
			.ptr = &__parms[1]
		},
		{
			.name = "kd",
			.ptr = &__parms[2]
		}
};

float KP;
float KI;
float KD;

void update_table(){
	KP = __parms[0];
	KI = __parms[1];
	KD = __parms[2];
}

int change_parameter(char* name, float value) {
	for (int i = 0; i < NUM_VARIABLES; i++) {
		if (strcmp(name, variables[i].name) == 0) {
		//if (name == variables[i].name) {
			*(variables[i].ptr) = value;
			update_table();
			return 1;
		}
	}
	return 0;
}

int read_parameter(char* name, float* value) {
	for (int i = 0; i < NUM_VARIABLES; i++) {
		if (strcmp(name, variables[i].name) == 0){
			*value = *variables[i].ptr;
			return 1;
		}
	}
	return 0;
}