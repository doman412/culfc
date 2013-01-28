/*
 * btn.c
 *
 *  Created on: Nov 12, 2012
 *      Author: Derek Arner
 */
#include "derivative.h";

void initBtn(void){
	
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	
	//Set PTC5 and PTC13 (connected to SW3 and SW4) for GPIO functionality, falling IRQ,
	//   and to use internal pull-ups. (pin defaults to input state)
	PORTC_PCR5=(0|PORT_PCR_MUX(1)|PORT_PCR_IRQC(0xA)|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK);
	PORTC_PCR13=(0|PORT_PCR_MUX(1)|PORT_PCR_IRQC(0xA)|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK);
	
	enable_irq(89);
}