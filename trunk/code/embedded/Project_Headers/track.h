/*
 * track.h
 *
 *  Created on: Nov 8, 2012
 *      Author: ulab
 */

#ifndef TRACK_H_
#define TRACK_H_

#include "derivative.h"

void ResetTrackMap(void);

typedef struct {
	int16_t risingEdges[10];
	uint16_t risingEdgeCount;
	int16_t fallingEdges[10];
	uint16_t fallingEdgeCount;
} TrackMap;

typedef enum {
	BLACK, WHITE
} Region;


#endif /* TRACK_H_ */
