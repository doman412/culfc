/*
 * configs.c
 *
 *  Created on: Mar 18, 2013
 *      Author: TopCat
 */
#include "configs.h"

typedef struct {
	float KP;
	float KI;
	float KD;	
} ConfigTable;

 __declspec(configtable) ConfigTable __config_table;
 extern ConfigTable __config_table;
 
void ResetConfigTable(void) {
	
	__config_table.KP = 1.0;
	__config_table.KD = 2.0;
	__config_table.KI = 3.0;
}
