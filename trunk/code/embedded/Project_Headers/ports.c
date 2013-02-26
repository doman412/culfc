/*
 * ports.c
 *
 *  Created on: Dec 3, 2012
 *      Author: ulab
 */

#include "derivative.h"
#include "led.h"

void SetupPorts(void) {
	
	SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK
				| SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK);
	
	//Servo Register
	PORTA_PCR8 = PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK; // CH0 // A34 - tested

	//Motor Registers
	PORTD_PCR4 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK; // CH4 B40 -tested
	PORTC_PCR4 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK; // CH3 //A37 - tested
	PORTD_PCR5 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK;//PTD5 // B39 -tested -- ch5
	PORTD_PCR6 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK;//PTD6 // B34 -tested -- ch6

	//GPIO Registers
	PORTA_PCR7 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //  // B69 -- GPIO -- high
	PORTA_PCR6 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //  // A64 -- GPIO -- d1a
	PORTA_PCR12 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //  // A25 -- GPIO -- d1b

	//Set GPIO Direction
	BITBAND_REG(GPIOA_PDDR,6) = 1;
	BITBAND_REG(GPIOA_PDDR,7) = 1;
	BITBAND_REG(GPIOA_PDDR,12) = 1;

	//Set GPIO Value
	BITBAND_REG(GPIOA_PSOR, 7) = 1;
	
	// LEDs
	
	//Important!  Each IO pin has a dedicated 32-bit Register to set it up (Selection GPIO vs peripheral, IRQ, Etc.)
	//Setup port C7 as GPIO and enable High Drive Strength
	PORTC_PCR7 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //Enable GPIO on on the pin
	PORTC_PCR8 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //Enable GPIO on on the pin
	PORTC_PCR9 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //Enable GPIO on on the pin
	PORTB_PCR11 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //Enable GPIO on on the pin

	//Make Sure the GPIO is setup to be an output
	GPIOC_PDDR |= (LED_E1_LOC | LED_E2_LOC | LED_E3_LOC);
	GPIOB_PDDR |= LED_E4_LOC;

}