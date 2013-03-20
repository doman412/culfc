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

int m1Percent;
int m2Percent;
float servo;

void DiagnosticsMode(void) {
	
	m1Percent = 0;
	m2Percent = 0;
	servo = 0.0;
	
	initBT(1);
	
	while (GetMode() == DIAGNOSTICS) {
		updateM1(m1Percent);
		updateM2(m2Percent);
		updateServo(servo);
		echoBT();
	}
	
	updateM1(0);
	updateM2(0);
	updateServo(0.0);
	
}