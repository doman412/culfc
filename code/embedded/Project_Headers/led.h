/*
 * led.h
 *
 *  Created on: Oct 9, 2012
 *      Author: ulab
 */

#include "derivative.h"

#ifndef LED_H_
#define LED_H_


//The E1 LED is on Port C, Bit 7
#define LED_E1_LOC 0x80
#define LED_E2_LOC 0x100
#define LED_E3_LOC 0x200
#define LED_E4_LOC 0x800


void setLedData(uint8_t data);


#endif /* LED_H_ */
