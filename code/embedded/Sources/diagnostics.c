/*
 * diagnostics.c
 *
 *  Created on: Mar 13, 2013
 *      Author: ulab
 */

#include "diagnostics.h"
#include "mode.h"
#include "FlexTimer.h"
#include "FlexTimer1.h"
#include "BT.h"
#include "buffer.h"
#include "parameter.h"
#include "stdio.h"

int m1Percent;
int m2Percent;
float servo;

void DiagnosticsMode(void) {
	
	m1Percent = 30;
	m2Percent = 30;
	servo = 0.0;
	
	//change_parameter("kp", 1.0);
	//change_parameter("ki", 2.0);
	//change_parameter("kd", 3.0);
	
	float f;
	read_parameter("kp", &f);
	printf("kp is %d\n", f);
	read_parameter("ki", &f);
	printf("ki is %d\n", f);
	read_parameter("kd", &f);
	printf("kd is %d\n", f);
	
	
	
	while (GetMode() == DIAGNOSTICS) {
		//updateM1(m1Percent);
		//updateM2(m2Percent);
		updateServo(servo);
		if (!BufferEmpty())
			sendChar(BufferRead());
	}
	
	
	
	updateM1(0);
	updateM2(0);
	updateServo(0.0);
	
}