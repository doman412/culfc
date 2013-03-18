/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

/* include peripheral declarations */
#include "Clock.h"
#include "derivative.h"
#include "camera_control.h"
#include "FlexTimer1.h"
#include "pid.h"
#include "button.h"
#include "mode.h"
#include "pidmode.h"

int main(void) {

	InitClock();

	SetupPorts();
	
	SetMode(ACCURACY);
	
	InitializeCamera();
	initButton();

	InitFlexTimer0();
	InitFlexTimer1();
	
	

	for (;;) {
		PIDMode();
	}

	return 0;
}
