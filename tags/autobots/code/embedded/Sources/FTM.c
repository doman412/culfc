/*
 * test.c
 *
 *  Created on: Oct 3, 2012
 *      Author: ulab
 */

#include "derivative.h"
#include "Clock.h"
#include "FTM.h"

static int SERVO_CENTER = 4500;
static int SERVO_MAX_RIGHT = 5400;
static int SERVO_MAX_LEFT = 3600;

static int rearMOD;

int initFTM(int module, long freqHZ){
	long mod;
	int prescale;
	FTM_MemMapPtr base;
	
	//Enable the Clock to the FTM0 Module
	switch(module){
	case 0:
		SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;
		base = FTM0_BASE_PTR;
		break;
	case 1:
		SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;
		base = FTM1_BASE_PTR;
		break;
	case 2:
		SIM_SCGC3 |= SIM_SCGC3_FTM2_MASK;
		base = FTM2_BASE_PTR;
		break;
	}
	
	
	//FTM0_MODE[WPDIS] = 1; //Disable Write Protection - enables changes to QUADEN, DECAPEN, etc.  
	FTM_MODE_REG(base) |= FTM_MODE_WPDIS_MASK;
	
	//Edit registers when no clock is fed to timer so the MOD value, gets pushed in immediately
	FTM_SC_REG(base) = 0; //Make sure its Off!
	
	FTM_CNTIN_REG(base) = 0; //Set the Counter Initial Value to 0 
	FTM_CNT_REG(base) = 0x0; //FTM Counter Value - reset counter to zero
	
//	FTM_CnSC_REG(FTM0_BASE_PTR, channel) = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;  // set edge-aligned PWM with true pulse on output
	
	FTM_CONF_REG(base) = FTM_CONF_BDMMODE(3); // allow for use in BDM mode
	
	
	mod = (48000000/freqHZ);
	
	prescale = 0;
	
	while(mod>65536){
		mod = mod/2;
		prescale++;
	}
	
	FTM_MOD_REG(base) = mod;
	//FTM_CnV_REG(FTM0_BASE_PTR, channel) = mod/13
	
	//Status and Control bits 
	FTM_SC_REG(base) =  FTM_SC_CLKS(1); // Selects Clock source to be "system clock" or (01) 

	//sets pre-scale value see details below
	FTM_SC_REG(base) |= FTM_SC_PS(prescale); 
	
	return mod;
}
/*
 * initialize all things to do with wheels: steering servo PWM, forward/reverse PWM 
 */
void initWheels(void){
	
	
	
	rearMOD = initFTM(0,4800); // init FTM0 at 4.8kHz
	initFTM(2,50); // init FTM2 at 50Hz

	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; // turn on clock to port A
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; // turn on clock to port C
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK; // turn on clock to port D
	
	PORTD_PCR4 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK; // drive PWM out: B39
	PORTD_PCR5 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK; // drive PWM out: B40
	PORTD_PCR6 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK; // drive PWM out: B34
	PORTC_PCR4 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK; // drive PWM out: A37
	
	PORTA_PCR10 = PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK; // servo PWM out: B66
	
	FTM0_C3SC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;  // set edge-aligned PWM with true pulse on output
	FTM0_C4SC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;  // set edge-aligned PWM with true pulse on output
	FTM0_C5SC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;  // set edge-aligned PWM with true pulse on output
	FTM0_C6SC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;  // set edge-aligned PWM with true pulse on output
	
	FTM2_C0SC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;  // set edge-aligned PWM with true pulse on output
}

/*
 * Servo Control on FTM2:0 [A10](B66)
 * set raw duty cycle for servo channel
 */
void servoDuty(int duty){
	FTM2_C0V = duty;
}
/*
 * steers the servo based on a range of -900:900
 */
void steer(int dir){
	if( (dir >= -900) && (dir<=900) )
		servoDuty(dir+SERVO_CENTER);
}
/*
 * Right Forward PWM on FTM0:3 [C4](A37)
 * set raw duty cycle for channel
 */
void RF_duty(int duty){
	FTM0_C3V = duty;
}
/*
 * Right Reverse PWM on FTM0:4 [D4](B39)
 * set raw duty cycle for channel
 */
void RR_duty(int duty){
	FTM0_C4V = duty;
}
/*
 * Left Forward PWM on FTM0:5 [D5](B40)
 * set raw duty cycle for channel
 */
void LF_duty(int duty){
	FTM0_C5V = duty;
}
/*
 * Left Reverse PWM on FTM0:6 [D6](B34)
 * set raw duty cycle for channel
 */
void LR_duty(int duty){
	FTM0_C6V = duty;
}
/*
 * sets the forward duty for both left and right to the same value
 */
void forwardDuty(int r, int l){
	RF_duty(r);
	LF_duty(l);
}
/*
 * sets the reverse duty for both left and right
 */
void reverseDuty(int r, int l){
	RR_duty(r);
	LR_duty(l);
}
/*
 * breaks the car, theoretically.
 */
void breakCar(void){
	forwardDuty(0,0);
	reverseDuty(1,1);
}

void go(int percent){
	int d = 0;
	if(percent>0)
		d = (percent* rearMOD)/100;
	
	forwardDuty(d,(int)(0.9077*d));

}
void reverse(int percent){
	int d = 0;
	if(percent>0)
		d = (percent*rearMOD)/100 ;
	
	reverseDuty(d,d);
}

