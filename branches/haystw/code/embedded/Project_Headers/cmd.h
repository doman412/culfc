/*
 * cmd.h
 *
 *  Created on: Oct 31, 2012
 *      Author: ulab
 */
#include "FTM.h"
#include "BT.h"

#ifndef CMD_H_
#define CMD_H_



typedef void (*Command) (int a);

void initCommands(void);
extern void cmdB(int);
extern void btDebug(int);
extern void varSwitch(int);
extern void opGo(int);
extern Command commandList[];
extern void setCamProperty(int);

#endif /* CMD_H_ */