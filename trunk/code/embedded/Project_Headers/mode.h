/*
 * mode.h
 *
 *  Created on: Dec 3, 2012
 *      Author: ulab
 */

#ifndef MODE_H_
#define MODE_H_

typedef enum {
	ACCURACY = 0,
	SPEED = 1,
	DIAGNOSTICS = 2} Mode;


Mode GetMode(void);
void SetMode(Mode newMode);

#endif /* MODE_H_ */
