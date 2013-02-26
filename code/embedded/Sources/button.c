/*
 * button.c
 *
 *  Created on: Nov 16, 2012
 *      Author: ulab
 */

#include "button.h"
#include "derivative.h"
#include "mode.h"

int toggleButtonState; // button state

void enable_irq(int irq); // enables irq for NVIC table based on its K40 table value

void initButton(void) {  

	  //Set PTC5 (connected to SW3) for GPIO functionality, falling IRQ,
	  //   and to use internal pull-ups. (pin defaults to input state)
	
	// allow clock to port C
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	
	// Outside button for start/stop
	PORTC_PCR13 =
			PORT_PCR_MUX(1) | 
			PORT_PCR_IRQC(10) |
			PORT_PCR_PE_MASK |
			PORT_PCR_PS_MASK;
	
	// Inside button for mode switch
	PORTC_PCR5 =
			PORT_PCR_MUX(1) |
			PORT_PCR_IRQC(10) |
			PORT_PCR_PE_MASK |
			PORT_PCR_PS_MASK;

	NVICIP89 |= 0x30; // sets button priority 
	enable_irq(89); // port c Interrupt


}

void portc_isr(void) {
	Mode mode = GetMode();
	if (PORTC_ISFR & (1 << 13)) {
		if (mode != BONUS) {
			toggleButtonState = 1;
		}
	}
	else {
		if (mode == ACCURACY) {
			SetMode(SPEED);
		}
		else if (mode == SPEED) {
			SetMode(BONUS);
		}
		else {
			SetMode(ACCURACY);
		}
	}
	
	PORTC_ISFR = 0xFFFFFFFF; // clear port c Interrupt flag
	
}

void ResetButtonState(void) {
	toggleButtonState = 0; // resets button sate to zero for finish line detection
}

int GetButtonState(void) {
	return toggleButtonState; // returns state of toggled button
}

void enable_irq(int irq) {
	int div;

	/* Make sure that the IRQ is an allowable number. Right now up to 91 is 
	 * used.
	 */
	if (irq > 91)
		printf("\nERR! Invalid IRQ value passed to enable irq function!\n");

	/* Determine which of the NVICISERs corresponds to the irq */
	div = irq / 32;

	switch (div) {
	case 0x0:
		NVICICPR0 |= 1 << (irq % 32);
		NVICISER0 |= 1 << (irq % 32);
		break;
	case 0x1:
		NVICICPR1 |= 1 << (irq % 32);
		NVICISER1 |= 1 << (irq % 32);
		break;
	case 0x2:
		NVICICPR2 |= 1 << (irq % 32);
		NVICISER2 |= 1 << (irq % 32);
		break;
	}
}