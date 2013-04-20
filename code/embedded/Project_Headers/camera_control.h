/*
 * camera_control.h
 *
 *  Created on: Oct 30, 2012
 *      Author: ulab
 */

#include "derivative.h"

#ifndef CAMERA_CONTROL_H_
#define CAMERA_CONTROL_H_

typedef enum {
	NO_LINE,
	LINE,
	FINISH_LINE
} Status;

typedef struct {
	Status status;
	int8_t error;
} CameraData;

/*
 * Connect camera as follows:
 * 
 * GND B2
 * VDD B1
 * CLK B72 (PTA11)
 * SI  B67 (PTA9)
 * AO A27 (ADC0_DP0)
 */

#define CAMERA_CLK_FREQUENCY_KHZ (20)

#define CAMERA_SI_PIN (9)

#define CAMERA_SI_DIRECTION (BITBAND_REG(GPIOA_PDDR, CAMERA_SI_PIN))


#define CAMERA_SI_DATA (BITBAND_REG(GPIOA_PDOR, CAMERA_SI_PIN))

#define BLACK_WHITE_THRESHOLD (50)


void InitializeCamera(void);

void SampleISR(void);

void CamFalling(void);

void HandleCameraData(void);

int GetCameraData(void);

uint8_t DataReady(void);

int FinishLineDetected(void);

void PITExpiredISR(void);



#endif /* CAMERA_CONTROL_H_ */
