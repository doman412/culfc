#include "derivative.h"

#pragma define_section relocate_code ".relocate_code" ".relocate_code" ".relocate_code" far_abs RX
#define __relocate_code__   __declspec(relocate_code)

#ifndef CLOCK_H_
#define CLOCK_H_

#define CORE_CLOCK			 (96000000)
#define PERIPHERAL_BUS_CLOCK (CORE_CLOCK/2)

__relocate_code__ void InitClock();

#endif
