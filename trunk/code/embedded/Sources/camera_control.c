/*
 * camera_control.c
 *
 *  Created on: Oct 30, 2012
 *      Author: ulab
 */

#include "camera_control.h"
#include "derivative.h"
#include "adc.h"
#include "track.h"
#include "BT.h"

void InitPortA(void);
void InitGPIO(void);
void ConfigureNVIC(void);
void InitializeFTM2(void);
void ConfigureAndStartPIT(void);
void FindBlackBoundaries(void);

uint32_t iptFlag = 0;

uint16_t fallingEdgeCount;
uint16_t prevEdgeCount;
uint16_t currentLine[128];

uint16_t dataReady = 0;
uint16_t blackLength = 0;

extern TrackMap trackMap;

static int error = 0;

void InitializeCamera(void) {
	InitPortA();
	InitGPIO();
	ConfigureNVIC();
	InitializeADC(ADC0_BASE_PTR, EIGHT_BIT);
	InitializeFTM2();
	ConfigureAndStartPIT();
}

void InitializeFTM2() {
	SIM_SCGC3 |= SIM_SCGC3_FTM2_MASK;
	FTM2_MODE |= FTM_MODE_WPDIS_MASK;
	FTM2_SC = 0;
	FTM2_CNTIN = 0;
	FTM2_CNT = 0;
	FTM2_C1SC = FTM_CnSC_CHIE_MASK | FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;

	FTM2_MOD = 959;
	FTM2_C1V = 0;
	//FTM2_C1V = 240;
	FTM2_SC |= FTM_SC_CLKS(1);
}

void InitPortA(void) {
	// Clock to Port A
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;

	// Set as FTM2_CH1
	PORTA_PCR11 |= PORT_PCR_MUX(3);

	// Set as GPIO
	PORTA_PCR9 = PORT_PCR_MUX(1);
}

void InitGPIO(void) {

	// Set as output
	CAMERA_SI_DIRECTION = 1;

	// Clear
	CAMERA_SI_DATA = 0;

}

void ConfigureNVIC(void) {
	// FTM 2
	NVICIP64 = 0x10;
	NVICICPR2 |= 1;
	NVICISER2 |= 1;

	// PIT 0
	NVICIP68 = 0x20;
	NVICICPR2 |= (1 << 4);
	NVICISER2 |= (1 << 4);

	// ADC 0
	NVICIP57 = 0x00;
	NVICICPR1 |= (1 << 25);
	NVICISER1 |= (1 << 25);
}

void ConfigureAndStartPIT(void) {
	// Turn on power to PIT
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
	PIT_MCR = 0x01; // Turn on the PIT
	PIT_LDVAL0 = 239999;
	PIT_TCTRL0 = PIT_TCTRL_TIE_MASK; // Enable interrupts
	PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK; // Turn on the timer
}

void PITExpiredISR(void) {

	PIT_TFLG0 = PIT_TFLG_TIF_MASK; // Turn off interrupt flag
	iptFlag = PIT_TFLG0; // Read the register back (fixes mask error)


	CAMERA_SI_DATA = 1;
	// Start clock
	//FTM2_SC |= FTM_SC_CLKS(1);
	FTM2_CNT = 0;
	FTM2_C1V = 480;

}

void HandleCameraData(void) {
	// Here, rising edge count varies from
	// 1 to 129

	// Therefore, index will range from 0 to 128
	// We only want 0-127 to make it in to the array
	currentLine[fallingEdgeCount - 1] = ADC0_RA;
}

void movingAverageFilter(){
	for(int i = 0; i < 127; i++){
		currentLine[i]= (currentLine[i]+currentLine[i+1])>>1;
	}
}

void FindBlackBoundaries(void) {
	uint16_t i;
	int16_t derivative;
	Region currentRegion;
	dataReady = 0;
	ResetTrackMap();
	if (currentLine[10] < 25) {
		currentRegion = BLACK;
	} else {
		currentRegion = WHITE;
	}
	for (i = 11; i < 118; i++) {
		derivative = currentLine[i] - currentLine[i - 1];
		if (currentRegion == BLACK) {
			if (derivative >= BLACK_WHITE_THRESHOLD) {
				if (blackLength >= 10) {
					AddRisingEdge(i);
				}
				else {
					RemoveLastFallingEdge();
				}
				currentRegion = WHITE;
			}
			else {
				blackLength++;
			}
		} else {
			if (derivative <= -BLACK_WHITE_THRESHOLD) {
				AddFallingEdge(i);
				currentRegion = BLACK;
				blackLength = 0;
			}
		}
	}
	error = ((trackMap.fallingEdges[0] + trackMap.risingEdges[0]) / 2) - 63;
	dataReady = 1;
}

int FinishLineDetected(void) {

	int firstFalling;
	int firstRising;

	int secondFalling;
	int secondRising;

	int index = 0;

	while (dataReady == 0) ;

	if (trackMap.fallingEdgeCount > 1 && trackMap.risingEdgeCount > 1) {

		// We have at least two falling edges, and at least two rising edges.
		firstFalling = trackMap.fallingEdges[0];
		secondFalling = trackMap.fallingEdges[1];
		firstRising = trackMap.risingEdges[0];
		secondRising = trackMap.risingEdges[1];

		// Which comes first?
		if (firstRising < firstFalling) {
			// We have "rising, falling, rising, falling"
			// which corresponds to "black, white, black, white, black"

			// The middle "white, black white" can be measured
			if (firstFalling - firstRising > 10 && secondRising - firstFalling
					> 10 && secondFalling - secondRising > 10) {
				return 1;
			}
		}
	} else {
		return 0;
	}

}

void CamFalling(void) {
	iptFlag = FTM2_C1SC;
	FTM2_C1SC &= ~FTM_CnSC_CHF_MASK;

	fallingEdgeCount++;
	CAMERA_SI_DATA = 0;
	
	if (fallingEdgeCount == 129) {
		//line_capture=capture=0;
		FTM2_C1V = 0;
		//FTM2_SC = 0;
		fallingEdgeCount = 0;
		//movingAverageFilter();
		FindBlackBoundaries();
	} else {
		StartADCConversion(ADC0_BASE_PTR, 0);
	}

}

int GetCameraData(void) {
	while (dataReady == 0) ;
	return error;
}

void sendLineData(){
	sendStr("l\n");
	for(int i=0; i<128; i++)
		sendChar(currentLine[i] >> 1);
	sendStr("e\n");
	sendChar(error);
}