/*
 * cmd.c
 *
 *  Created on: Oct 31, 2012
 *      Author: Derek Arner
 */
#include "cmd.h"
#include "null.h"
#include "button.h"
#include "parameter.h"
#include "FlexTimer.h"
#include "FlexTimer1.h"
#include "mode.h"
#include "stdlib.h"
#include "pid.h"

const CommandProperties command_list[] = {
		{
			.name = "s",
			.cmd_ptr = &set_servo_command
		},
		{
			.name = "m",
			.cmd_ptr = &set_motor_command
		},
		{
			.name = "l",
			.cmd_ptr = &set_left_motor_command
		},
		{
			.name = "r",
			.cmd_ptr = &set_right_motor_command
		},
		{
			.name = "set",
			.cmd_ptr = &set_parameter_command
		},
		{
			.name = "get",
			.cmd_ptr = &get_parameter_command
		},
		{
			.name = "go",
			.cmd_ptr = &go_command
		},
		{
			.name = "stop",
			.cmd_ptr = &stop_command
		},
		{
			.name = "rp",
			.cmd_ptr = &reset_pid_command
		},
		{
			.name = "servo",
			.cmd_ptr = &set_servo_command
		},
		{
			.name = "motor",
			.cmd_ptr = &set_motor_command
		},
		{
			.name = "left_motor",
			.cmd_ptr = &set_left_motor_command
		},
		{
			.name = "right_motor",
			.cmd_ptr = &set_right_motor_command
		},
		{
			.name = "mode",
			.cmd_ptr = &mode_command
		},
		{
			.name = "reset_pid",
			.cmd_ptr = &reset_pid_command
		}
};

char msg[100];

void default_cmd(void) {
	
}

Command find_command(char* cmd_name) {
	for (unsigned int i = 0; i < NUM_CMDS; i++) {
		CommandProperties cp = command_list[i];
		if (strcmp(cp.name, cmd_name) == 0) {
			return cp.cmd_ptr;
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
	return NULL;
}

char* get_parameter_command(char** argv, unsigned int argc) {
	if (argc != 1) {
		return "Syntax: set <variable> <value>";
	}
	
	char* varname = argv[0];
	float varvalue;
	if (read_parameter(varname, &varvalue) == 0) { // Returns 0 if error
		return "No such variable";
	}
	else {
		sprintf(msg, "%s = %f", varname, (double)varvalue);
		return msg;
	}
		
}

char* set_servo_command(char** argv, unsigned int argc) {
	if (argc != 1) {
		return "Syntax: s[ervo] <value>";
	}
	
	float value = (float) atof(argv[0]);
	updateServo(value);
	return NULL ;
 }


char* set_motor_command(char** argv, unsigned int argc){
	if (argc != 1) {
			return "Syntax: m[otor] <value>";
	}
	
	int value = atoi(argv[0]);
	updateM1(value);
	updateM2(value);
	return NULL;
}

char* set_left_motor_command(char** argv, unsigned int argc){
	if (argc != 1) {
			return "Syntax: l[eft_motor] <value>";
	}
	
	int value = atoi(argv[0]);
	updateM1(value);
	return NULL;
}

char* set_right_motor_command(char** argv, unsigned int argc){
	if (argc != 1) {
			return "Syntax: r[ight_motor] <value>";
	}
	
	int value = atoi(argv[0]);
	updateM2(value);
	return NULL;
}

char* go_command(char** argv, unsigned int argc){
	SetButtonState();
	return NULL;
}

extern int moving;

char* stop_command(char** argv, unsigned int argc){
	moving = 0;
	updateServo(0);
	updateM1(MOTOR_BRAKE);
	updateM2(MOTOR_BRAKE);
	return NULL;
}

char* mode_command(char** argv, unsigned int argc){
	Mode mode = GetMode();
	
	if (mode == ACCURACY)
		SetMode(SPEED);
	else if (mode == SPEED)
		SetMode(DIAGNOSTICS);
	else
		SetMode(ACCURACY);
	
	return NULL;
}

char* reset_pid_command(char** argv, unsigned int argc){
	reset_pid();
	return NULL;
}

