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
#include "util.h"
#include "BT.h"
#include "parameter.h"

int main(void) {

	int i;
	
	InitClock();

	SetupPorts();
	
	SetMode(ACCURACY);
	
	InitializeCamera();
	initButton();

	InitFlexTimer0();
	InitFlexTimer1();
	initBT(1);
	update_table();
	
	updateServo(1.0);
	
	blocking_delay(10000000);
	updateServo(-1.0);
	blocking_delay(10000000);
	updateServo(0.0);
	
	

	for (;;) {
		if (GetMode() == DIAGNOSTICS) {
			 
			DiagnosticsMode();
			
		}
		else {
			PIDMode();
		}
	}

	return 0;
}

