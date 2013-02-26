/*
 * mode.h
 *
 *  Created on: Dec 3, 2012
 *      Author: ulab
 */

#ifndef MODE_H_
#define MODE_H_

typedef enum {
	ACCURACY,
	SPEED,
	BONUS
} Mode;

Mode GetMode(void);
void SetMode(Mode newMode);

#endif /* MODE_H_ */
