#ifndef CMD_H
#define CMD_H

typedef char* (*Command) (char** argv, unsigned int argc);

typedef struct CommandProperties {
	char* name;
	Command cmd_ptr;
} CommandProperties;

#define NUM_CMDS 3
#define MAX_ARGC 2
extern const CommandProperties command_list[NUM_CMDS];

Command find_command(char* cmd_name);

char* set_parameter_command(char** argv, unsigned int argc);
char* set_servo_command(char** argv, unsigned int argc);

#endif 
