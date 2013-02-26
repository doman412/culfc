/*
 * adc.h
 *
 *  Created on: Oct 17, 2012
 *      Author: tom
 */

#include "derivative.h"

#ifndef ADC_H_
#define ADC_H_

typedef enum {
	EIGHT_BIT,
	TWELVE_BIT,
	TEN_BIT,
	SIXTEEN_BIT
} ADC_Mode;

void InitializeADC(ADC_MemMapPtr adc, ADC_Mode mode);
void StartADCConversion(ADC_MemMapPtr adc, uint8_t channel);

#endif /* ADC_H_ */
