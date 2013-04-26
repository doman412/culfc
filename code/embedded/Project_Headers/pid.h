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
extern float KP;
extern float KI;
extern float KD;

#define KP_ACCURACY KP // 0.03125
#define KI_ACCURACY KI
#define KD_ACCURACY KD

#define K1_ACCURACY (KP_ACCURACY + KI_ACCURACY + KD_ACCURACY)
#define K2_ACCURACY (-KP_ACCURACY - 2*KD_ACCURACY)
#define K3_ACCURACY KD_ACCURACY

/*
 * 
 * Speed mode constants
 * 
 */

#define KP_SPEED KP//0.03125
#define KI_SPEED KI
#define KD_SPEED KD

#define K1_SPEED (KP_SPEED + KI_SPEED + KD_SPEED)
#define K2_SPEED (-KP_SPEED - 2*KD_SPEED)
#define K3_SPEED KD_SPEED

//#define DX_HI 80
//#define DX_LOW 40
//#define MOTOR_SPEED 70

void reset_pid();

#endif /* PID_H_ */
