/*
 * cam.h
 *
 *  Created on: Nov 15, 2012
 *      Author: ulab
 */

#ifndef CAM_H_
#define CAM_H_

#include "derivative.h"

#define LED_E1_LOC (1<<7)
#define LED_E2_LOC (1<<8)
#define LED_E3_LOC (1<<9)
#define LED_E4_LOC (1<<11)
#define GPIO_PIN_MASK            0x1Fu
#define GPIO_PIN(x)              (((1)<<(x & GPIO_PIN_MASK)))


void sendSCCBSignal(int slaveAddress, int address, int data);
void initCamera(void);
int arctan_approximation(int distance_from_black_line);

#endif /* CAM_H_ */
