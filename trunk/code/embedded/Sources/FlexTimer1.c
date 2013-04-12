
#include "FlexTimer1.h"
#include "derivative.h"
#include "Clock.h"
#include "math.h"

void InitFlexTimer1() {
	SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK; //Enable the clock to the FTM1 module

	//turn off FTM write protection by setting FTM0_MODE[WPDIS] = 1;
	FTM1_MODE |= FTM_MODE_WPDIS_MASK;
	//	FTM1_MODE &= ~1;
	//    FTM1_QDCTRL &=~FTM_QDCTRL_QUADEN_MASK;  
	FTM1_SC = 0; //Make sure its Off!
	//Edit registers when no clock is fed to timer so the MOD value, gets pushed in immediately

	FTM1_CNTIN = 0; // starting value for counter
	FTM1_CNT = 0; // loads the counter with CNTIN

	// Alignment must be set via this command for all used channels and
	// Before the pulse width command may be altered from FTM0_CnV where n is the channel

	FTM1_C0SC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK; // set edge-aligned PWM with true pulse on output

	FTM1_MOD = FTM1_MOD_VALUE; // set the mod value

	//  Optionally, if you want FTM to operate during debug, set the BDM mode	
	FTM1_CONF = FTM_CONF_BDMMODE(3); // set FTM to operate normally in BDM mode

	FTM1_SC |= FTM_SC_PS(FTM1_CLK_PRESCALE); //divide the input clock down by 2^FTM0_CLK_PRESCALE

	FTM1_SC |= FTM_SC_CLKS(1);

}
// FTM0_CV_VALUE 3000   // 3000/(MOD+1) = 25% duty cycle
/*void SetFTM1DutyCycle(int DuyCycleS1) {
 // Do math to convert dutyCycle to CV value
 // Set ftm0c3v register to value 
 FTM1_C0V = DuyCycleS1;      // set the count value and determines pulse width

 }*/
float prev_pos = 0;
extern float SERVO_OVERSHOOT;
void updateServo(float servoPos) {
	float servoCv;
	uint32_t servoCvInt;
	
	if(prev_pos < servoPos){
		prev_pos = servoPos;
		servoPos+=SERVO_OVERSHOOT;
	}
	else if(prev_pos > servoPos){
		prev_pos = servoPos;
		servoPos-=SERVO_OVERSHOOT;
	}
	
	if(servoPos > 1){
		servoPos = 1;
	}
	else if (servoPos < -1){
		servoPos = -1;
	}
	
	servoCv = (servoPos * 450) + 2250 + SERVO_OFFSET;
	servoCvInt = (uint32_t) round(servoCv);
	FTM1_C0V = servoCvInt;
}