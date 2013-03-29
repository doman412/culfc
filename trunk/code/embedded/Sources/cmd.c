/*
 * cmd.c
 *
 *  Created on: Oct 31, 2012
 *      Author: Derek Arner
 */
#include "cmd.h"
#include "null.h"
#include "parameter.h"
#include "FlexTimer1.h"
#include "stdlib.h"

const CommandProperties command_list[] = {
		{
			.name = "set",
//			.num_args = 2,
//			.arg_types = (ArgumentType[]) {STRING, FLOAT},
			.cmd_ptr = &set_parameter_command
		},
		
		{
			.name = "servo",
//			.num_args = 1,
//			.arg_types = (ArgumentType[]) {FLOAT},
			.cmd_ptr = &set_servo_command
		}
};

void default_cmd(void) {
	
}

Command find_command(char* cmd_name) {
	for (unsigned int i = 0; i < NUM_CMDS; i++) {
		CommandProperties cp = command_list[i];
		if (strcmp(cp.name, cmd_name) == 0) {
			cp.cmd_ptr;
		}
	}
	return NULL;
}

char* set_parameter_command(char** argv, unsigned int argc) {
	if (argc != 2) {
		return "Syntax: set <variable> <value>";
	}
	
	char* varname = argv[0];
	float varvalue = (float) atof(argv[1]);
	if (change_parameter(varname, varvalue) == 0) { // Returns 0 if error
		return "No such variable";
	}
}

 char* set_servo_command(char** argv, unsigned int argc) {
	if (argc != 1) {
		return "Syntax: servo <value>";
	}
	
	float value = (float) atof(argv[0]);
	updateServo(value);
	return NULL;
 }