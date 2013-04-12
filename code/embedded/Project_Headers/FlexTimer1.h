#ifndef FLEXTIMER1_H_
#define FLEXTIMER1_H_

#define FTM1_CLK_PRESCALE	5
#define FTM1_CLK_SOURCE   1
#define FTM1_MOD_VALUE   29999 //With prescale of 5, this generates a 50 Hz PWm
 
extern int SERVO_OFFSET;
void InitFlexTimer1();
//void SetFTM1DutyCycle(int DuyCycleS1);
void updateServo(float servoPos);
#endif /* FLEXTIMER1_H_ */