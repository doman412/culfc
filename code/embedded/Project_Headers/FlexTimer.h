/*
 * FlexTimer.h
 *
 *  Created on: Oct 10, 2012
 *      Author: ulab
 */

#ifndef FLEXTIMER_H_
#define FLEXTIMER_H_

#include "derivative.h"

#define MOTOR_BRAKE 255
#define MOTOR_FREEWHEEL 0

#define FTM0_CLK_PRESCALE	2
#define FTM0_CLK_SOURCE   1
#define FTM0_MOD_VALUE   3000 //With prescale of 2, this generates a 2kHz PWm
 
void EnableMotors(void);
void DisableM1(void);
void DisableM2(void);

void InitFlexTimer0();
//void SetFTM0DutyCycle(int N1Aone, int N2Atwo, int N1Bone, int N2Btwo);
void updateM1(int m1Percent);
void updateM2(int m2Percent);
#endif /* FLEXTIMER_H_ */
