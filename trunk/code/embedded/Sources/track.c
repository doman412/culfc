/*
 * track.c
 *
 *  Created on: Nov 8, 2012
 *      Author: ulab
 */

#include "track.h"
#include "derivative.h"

TrackMap trackMap;

void ResetTrackMap(void) {
	trackMap.risingEdgeCount = 0;
	trackMap.fallingEdgeCount = 0;
}

void AddFallingEdge(uint16_t index) {
	trackMap.fallingEdges[trackMap.fallingEdgeCount] = index;
	trackMap.fallingEdgeCount++;
}

void AddRisingEdge(uint16_t index) {
	trackMap.risingEdges[trackMap.risingEdgeCount] = index;
	trackMap.risingEdgeCount++;
}

