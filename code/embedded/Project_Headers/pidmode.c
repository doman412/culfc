/*
 * pidmode.c
 *
 *  Created on: Dec 3, 2012
 *      Author: ulab
 */

#include "mode.h"
#include "pid.h"
#include "FlexTimer.h"
#include "FlexTimer1.h"

int moving;
int error;
OutputMap container;

void PIDMode(void) {
	Mode mode = GetMode();
	while (mode == ACCURACY || mode == SPEED) {

		if (moving) {

			if (GetButtonState() || FinishLineDetected()) {
				ResetButtonState();
				moving = 0;
				continue;
			}

			error = GetCameraData();
			PIDErrorControl(error, mode, &container);

			updateM1(container.rightMotor);
			updateM2(container.leftMotor);
			updateServo(container.servo);

		} else {
			if (GetButtonState()) {
				moving = 1;
				ResetButtonState();
				continue;
			}

			updateServo(0);
			updateM1(MOTOR_BRAKE);
			updateM2(MOTOR_BRAKE);
		}
	}
}