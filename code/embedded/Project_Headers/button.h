/*
 * button.h
 *
 *  Created on: Nov 7, 2012
 *      Author: ulab
 */

#ifndef BUTTON_H_

#define BUTTONLENGTH 5  

#define GPIO_PIN_MASK            0x1Fu
#define GPIO_PIN(x)              (((1)<<(x & GPIO_PIN_MASK))) // sets gpio with button shift based on port


void initButton(void); // button initilization for PTC5 with pin default to input
int GetButtonState(void); // Getter function for the state of the toggle button
void SetButtonState(void); // used to manually set button state
void ResetButtonState(void); // sets button state to zero used for line detection stop
void portc_isr(void); // interrupt service for port C

#define BUTTON_H_


#endif /* BUTTON_H_ */
