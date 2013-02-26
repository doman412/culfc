/*
 * adc.c
 *
 *  Created on: Oct 17, 2012
 *      Author: tom
 */

#include "adc.h"
#include "derivative.h"

void ConfigureADC(ADC_MemMapPtr adc, ADC_Mode mode);
void CalibrateADC(ADC_MemMapPtr adc);

void InitializeADC(ADC_MemMapPtr adc, ADC_Mode mode) {
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
	ConfigureADC(adc, mode);
	CalibrateADC(adc);
	ConfigureADC(adc, mode);
}

void StartADCConversion(ADC_MemMapPtr adc, uint8_t channel) {
	adc->SC1[0] = 
			ADC_SC1_AIEN_MASK | 
			ADC_SC1_ADCH(channel);
}

void ConfigureADC(ADC_MemMapPtr adc, ADC_Mode mode) {
	adc->CFG1 |= ADC_CFG1_MODE(mode);
}

void CalibrateADC(ADC_MemMapPtr adc) {
	
	uint16_t calibration;
	
	adc->SC3 |= ADC_SC3_CAL_MASK;
	while (adc->SC3 & ADC_SC3_CAL_MASK) {
		// Wait for calibration to complete
	}
	// Finish off the calibration
	// Initialize a 16-bit variable in RAM
	calibration = 0x0;
	// Add the plus-side calibration results to the variable
	calibration += adc->CLP0;
	calibration += adc->CLP1;
	calibration += adc->CLP2;
	calibration += adc->CLP3;
	calibration += adc->CLP4;
	calibration += adc->CLPS;
	// Divide by two
	calibration /= 2;
	// Set the MSB of the variable
	calibration |= 0x8000;
	// Store the value in the plus-side gain calibration register
	adc->PG = calibration;
	// Repeat the procedure for the minus-side calibration value
	calibration = 0x0000;
	calibration += adc->CLM0;
	calibration += adc->CLM1;
	calibration += adc->CLM2;
	calibration += adc->CLM3;
	calibration += adc->CLM4;
	calibration += adc->CLMS;
	calibration /= 2;
	calibration |= 0x8000;
	adc->MG = calibration;
}


