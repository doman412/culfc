/*
 * pid.c
 *
 *  Created on: Nov 13, 2012
 *      Author: ulab
 */

#include "pid.h"
#include "math.h"
#include "FlexTimer.h"

static float previousError1 = 0;
static float previousError2 = 0;
static float previousServoOutput = 0;
static float previousMotorOutput = 0;

void PIDErrorControl(int error, Mode mode, OutputMap* outputMap) {
	static float previousError1 = 0;
	static float previousError2 = 0;

	static float previousServoOutput = 0;
	static float previousMotorOutput = 0;

	float currentServoOutput;
	float currentLeftOutput;
	float currentRightOutput;

	if (mode == SPEED) {

		// Update motors
		if (error <= -35) { // We are off to the right
			currentLeftOutput = 20;
			currentRightOutput = 40;
		}
		else if (error >= 35) { // We are off to the left
			currentLeftOutput = 40;
			currentRightOutput = 20;
		}
		else {
			currentLeftOutput = 55;
			currentRightOutput = 55;
		}
		

		// Update servo
		currentServoOutput = 
			K1_SPEED * error + 
			K2_SPEED * previousError1 +
			K3_SPEED * previousError2 +
			previousServoOutput;

	} else if (mode == ACCURACY) {
		
		// Update motors
		currentLeftOutput = 30;
		currentRightOutput = 30;
		
		// Update servo
		currentServoOutput = 
			K1_ACCURACY * error + 
			K2_ACCURACY * previousError1 +
			K3_ACCURACY * previousError2 +
			previousServoOutput;
	}


	// Prepare data for next run
	
	previousError2 = previousError1;
	previousError1 = error;
	previousServoOutput = currentServoOutput;

	// Clamp the servo value
	
	if (currentServoOutput < -4.5)
		currentServoOutput = -4.5;
	else if (currentServoOutput > 4.5)
		currentServoOutput = 4.5;
	
	// Write outputs
	outputMap->servo = currentServoOutput;
	outputMap->leftMotor = currentLeftOutput;
	outputMap->rightMotor = currentRightOutput;
}

void reset_pid(){
	previousError1 = 0;
	previousError2 = 0;
	previousServoOutput = 0;
	previousMotorOutput = 0;
}