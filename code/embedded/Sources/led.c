/*
 * led.c
 *
 *  Created on: Oct 9, 2012
 *      Author: ulab
 */
#include "led.h"
#include "derivative.h"

void setLedData(uint8_t data) {
	if (data & 0x01)
		GPIOC_PCOR |= LED_E1_LOC;
	else
		GPIOC_PSOR |= LED_E1_LOC;
	if (data & 0x02)
		GPIOC_PCOR |= LED_E2_LOC;
	else
		GPIOC_PSOR |= LED_E2_LOC;
	if (data & 0x04)
		GPIOC_PCOR |= LED_E3_LOC;
	else
		GPIOC_PSOR |= LED_E3_LOC;
	if (data & 0x08)
		GPIOB_PCOR |= LED_E4_LOC;
	else
		GPIOB_PSOR |= LED_E4_LOC;
}

