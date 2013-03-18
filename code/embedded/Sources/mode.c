/*
 * mode.c
 *
 *  Created on: Dec 3, 2012
 *      Author: ulab
 */

#include "mode.h"
#include "led.h"

Mode mode;

Mode GetMode(void) {
	return mode;
}

// Cycle: accuracy, speed, bonus, diagnostics
void SetMode(Mode newMode) {
	mode = newMode;
	setLedData(1 << mode);

}