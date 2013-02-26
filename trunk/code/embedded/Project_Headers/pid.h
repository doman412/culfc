/*
 * pid.h
 *
 *  Created on: Nov 13, 2012
 *      Author: ulab
 */

#ifndef PID_H_
#define PID_H_

#include "derivative.h"
#include "camera_control.h"
#include "mode.h"

typedef struct {
	float servo;
	uint16_t leftMotor;
	uint16_t rightMotor;
} OutputMap;




void PIDErrorControl(int error, Mode mode, OutputMap* container);

/*
 * 
 * Accuracy mode constants.
 * 
 */

#define KP_ACCURACY 0.25
#define KI_ACCURACY 0
#define KD_ACCURACY 0

#define K1_ACCURACY (KP_ACCURACY + KI_ACCURACY + KD_ACCURACY)
#define K2_ACCURACY (-KP_ACCURACY - 2*KD_ACCURACY)
#define K3_ACCURACY KD_ACCURACY

/*
 * 
 * Speed mode constants
 * 
 */

#define KP_SPEED 0.15
#define KI_SPEED 0
#define KD_SPEED 0

#define K1_SPEED (KP_SPEED + KI_SPEED + KD_SPEED)
#define K2_SPEED (-KP_SPEED - 2*KD_SPEED)
#define K3_SPEED KD_SPEED

#endif /* PID_H_ */
