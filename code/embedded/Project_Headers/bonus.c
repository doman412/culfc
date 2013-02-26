/*
 * bonus.c
 *
 *  Created on: Dec 3, 2012
 *      Author: ulab
 */

#include "bonus.h"
#include "mode.h"
#include "derivative.h"
#include "buffer.h"
#include "FlexTimer.h"
#include "FlexTimer1.h"

void ParseCommandData(int command);

void BonusMode(void) {
	int commandData;
	
	InitUart();
	
	while (GetMode() == BONUS) {
		if (!BufferEmpty()) {
			commandData = BufferRead();
			ParseCommandData(commandData);
		}
	}
	
	DisableUart();
	
	
}

void ParseCommandData(int commandData) {
	int command;
	int data;
	int motors;
	float servo;
	
	// The high four bits of the commandData byte are the acutal "command" portion
	// 0x0: stop
	// 0x1: control motors
	// 0x2: control servo
	
	command = (commandData & 0xf0) >> 4;
	data = commandData & 0x0f;
	
	if (command == 0) {
		updateM1(0);
		updateM2(0);
		updateServo(0);
	}
	else if (command == 1) {
		// We have four bits of data that map to a motor duty cycle
		// Need to map from 0-16 to 0-100
		motors = 6 * data;
		updateM1(motors);
		updateM2(motors);
	}
	else if (command == 2) { // left turn
		// data indicates how much to turn
		servo = -data;
		updateServo(servo);
	}
	else if (command == 3) { // right turn
		// data indicates how much to turn
		servo = data;
		updateServo(servo);
	}
	else if (command == 4) { 
		updateServo(0); // Center servo
	}
	else if (command == 5) { // Reverse
		motors = -6 * data;
		updateM1(motors);
		updateM2(motors);
	}
	
}