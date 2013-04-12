
#include "FlexTimer.h"
#include "derivative.h"
#include "Clock.h"

void InitFlexTimer0() {
	SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK; //Enable the clock to the FTM0 module

	//turn off FTM write protection by setting FTM0_MODE[WPDIS] = 1;
	FTM0_MODE |= FTM_MODE_WPDIS_MASK;

	FTM0_SC = 0; //Make sure its Off!
	//Edit registers when no clock is fed to timer so the MOD value, gets pushed in immediately

	FTM0_CNTIN = 0; // starting value for counter
	FTM0_CNT = 0; // loads the counter with CNTIN

	// Alignment must be set via this command for all used channels and
	// Before the pulse width command may be altered from FTM0_CnV where n is the channel

	FTM0_C5SC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK; //PTD5 // B39
	FTM0_C6SC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK; //PTD6 // B34
	//FTM0_C6SC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;  //PTA1 // B38
	//FTM0_C7SC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK; //PTA2 // B37
	FTM0_C3SC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK; // PTC4 // A37
	FTM0_C4SC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK; // PTD4 // B40 

	FTM0_MOD = FTM0_MOD_VALUE; // set the mod value

	//  Optionally, if you want FTM to operate during debug, set the BDM mode	
	FTM0_CONF = FTM_CONF_BDMMODE(3); // set FTM to operate normally in BDM mode

	FTM0_SC |= FTM_SC_PS(FTM0_CLK_PRESCALE); //divide the input clock down by 2^FTM0_CLK_PRESCALE

	FTM0_SC |= FTM_SC_CLKS(1);

}

const int LEFT_MOTOR_BIAS[] = {0,1,1,2,3,4,5,6,7,4};

// FTM0_CV_VALUE 3000   // 3000/(MOD+1) = 25% duty cycle
void updateM1(int m1Percent) {
	// Do math to convert dutyCycle to CV value
	// Set ftm0c3v register to value 
	uint16_t m1Duty;
	int m1PercentNorm;
	
	if (m1Percent == MOTOR_FREEWHEEL) {
		DisableM1();
	}
	else if (m1Percent == MOTOR_BRAKE) {
		EnableMotors();
		FTM0_C5V = 0;
		FTM0_C4V = 0;
	}
	else if (m1Percent < 0) {
		m1PercentNorm = m1Percent + LEFT_MOTOR_BIAS[(m1Percent-1)/10];
		m1Duty = (uint16_t) (m1PercentNorm * -30);
		FTM0_C5V = 0; // Motor 1 Forward
		FTM0_C4V = m1Duty; // Motor 1 Reverse
	} else if (m1Percent > 0) {
		m1PercentNorm = m1Percent - LEFT_MOTOR_BIAS[(m1Percent-1)/10];
		m1Duty = (uint16_t) (m1PercentNorm * 30);
		FTM0_C5V = m1Duty; // Motor 1 Forward
		FTM0_C4V = 0; // Motor 1 Reverse
	}
}



void DisableM1(void) {
	BITBAND_REG(GPIOA_PDOR, 6) = 1;
}

void DisableM2(void) {
	BITBAND_REG(GPIOA_PDOR,12) = 1;
}

void EnableMotors(void) {
	BITBAND_REG(GPIOA_PDOR, 6) = 0;
	BITBAND_REG(GPIOA_PDOR,12) = 0;
}

void updateM2(int m2Percent) {
	// Do math to convert dutyCycle to CV value
	// Set ftm0c3v register to value 
	uint16_t m2Duty;

	if (m2Percent == MOTOR_FREEWHEEL) {
		DisableM2();
	}
	else if (m2Percent == MOTOR_BRAKE) {
		EnableMotors();
		FTM0_C3V = 0;
		FTM0_C6V = 0;
	}
	else if (m2Percent < 0) {
		EnableMotors();
		m2Duty = (uint16_t) (m2Percent * -30);
		FTM0_C3V = 0; // Motor 1 Forward
		FTM0_C6V = m2Duty; // Motor 1 Reverse
	} else if (m2Percent > 0) {
		EnableMotors();
		m2Duty = (uint16_t) (m2Percent * 30);
		FTM0_C3V = m2Duty; // Motor 1 Forward
		FTM0_C6V = 0; // Motor 1 Reverse
	}
}