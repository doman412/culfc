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
#include "queue.h"
#include "parameter.h"
#include "stdio.h"
#include "cmd.h"

int m1Percent;
int m2Percent;
float servo;

//static Queue queue;

void DiagnosticsMode(void) {
	
//	m1Percent = 30;
//	m2Percent = 30;
//	servo = 0.0;
//	
//	init_queue(&queue);
//	push_front(&queue, 'a');
//	push_front(&queue, 'b');
//	push_front(&queue, 'c');
//	
//	char read;
//	read = pop_back(&queue);
//	if (read != 'a') {
//		printf("failed queue test 1\n the char was %c\n", read);
//	}
//	read = pop_back(&queue);
//	if (read != 'b') {
//		printf("failed queue test 2\n the char was %c\n", read);
//	}
//	read = pop_back(&queue);
//	if (read != 'c') {
//		printf("failed queue test 3\n the char was %c\n", read);
//	}
	
	
	decodeBT();

	
	
	
//	change_parameter("kp", 1.0);
//	change_parameter("ki", 2.0);
//	change_parameter("kd", 3.0);
//	
//	float f;
//	read_parameter("kp", &f);
//	read_parameter("ki", &f);
//	read_parameter("kd", &f);
//	
//	
//	
//	while (GetMode() == DIAGNOSTICS) {
//		//updateM1(m1Percent);
//		//updateM2(m2Percent);
//		updateServo(servo);
//		if (!queue_empty(&queue))
//			sendChar(pop_back(&queue));
//	}
//	
//	
//	
//	updateM1(0);
//	updateM2(0);
//	updateServo(0.0);
	
}