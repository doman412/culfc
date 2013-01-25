/*
 * FTM.h
 *
 *  Created on: Oct 8, 2012
 *      Author: ulab
 */

#ifndef FTM_H_
#define FTM_H_


int initFTM(int, long);

void initWheels(void);

void servoDuty(int);

void RF_duty(int);
void RR_duty(int);
void LF_duty(int);
void LR_duty(int);

void forwardDuty(int, int);
void reverseDuty(int, int);

void breakCar(void);

void steer(int);

void go(int);
void reverse(int);


#endif /* FTM_H_ */
