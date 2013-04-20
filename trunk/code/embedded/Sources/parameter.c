/*
 * parameter.c
 *
 *  Created on: Mar 26, 2013
 *      Author: ulab
 */
#include "parameter.h"

__declspec(configtable) float __parms[NUM_VARIABLES];

float KP;
float KI;
float KD;
float servo_offset_float = -27;
int SERVO_OFFSET = -27;
float SERVO_OVERSHOOT = 0.022;

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
		},
		{
			.name = "mode",
			.ptr = (float*)&mode
		},
		{
			.name = "servo_offset",
			.ptr = &servo_offset_float
		},
		{
			.name = "servo_overshoot",
			.ptr = &SERVO_OVERSHOOT
		},
		{
			.name = "echo",
			.ptr = (float*)&ECHO
		},
		{
			.name = "line_capture",
		    .ptr = (float*)&line_capture
		}
};

void update_table(){
	KP = __parms[0];
	KI = __parms[1];
	KD = __parms[2];
	setLedData(1 << mode);
	SERVO_OFFSET = servo_offset_float;
	updateServo(0);
}

int change_parameter(char* name, float value) {
	for (int i = 0; i < NUM_VARIABLES; i++) {
		if (strcmp(name, variables[i].name) == 0) {
			*(variables[i].ptr) = value;
			if(i <= NUM_FLASH_VARIABLES)
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