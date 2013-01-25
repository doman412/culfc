/*
 * cam.c
 *
 *  Created on: Nov 15, 2012
 *      Author: ulab
 */

#include "cam.h"

//transmit an SCCB signal to the camera- registers must be initialized
void sendSCCBSignal(int slaveAddress, int address, int data)

{
	int index;
	//port E11 = SDA, PTE12 = SDC
	//begin with clock low, data high
	//GPIOE_PCOR |= 1 << 12;
		GPIOE_PSOR |= 1 << 11;
		Block(74);
		
	for( index =0; index < 32; index++)
	{
		if (index == 0)
		{
			//begin with high clock, high data
			GPIOE_PSOR |= 1 << 12;
			GPIOE_PSOR |= 1 << 11;
			Block(74);
		}
		if (index == 1)
				{
			//raise clock
					GPIOE_PSOR |= 1 << 12;
					
					Block(37);
				}
		if (index == 2)
				{
			//lower data
					
					GPIOE_PCOR |= 1 << 11;
					Block(74);
				}
		
		//for all address bits
		if ((index >= 3) && (index < 11))
				{
			//lower clock
					GPIOE_PCOR |= 1 << 12;
					//wait			
					Block(37);
			//raise/lower address bit
			if (((slaveAddress >> (10 - index) )% 2) == 0)
					GPIOE_PCOR |= 1 << 11;
			else
				GPIOE_PSOR |= 1 << 11;
			//wait
					Block(37);
					//raise clock
				GPIOE_PSOR |= 1 << 12;
					//wait
				Block(74);
				}
		
		//wait for acknowledge
		if ((index == 11)||(index == 20)||(index == 30))
								{
			
							//lower clock
									GPIOE_PCOR |= 1 << 12;
									
									//wait			
									Block(37);
									//switch SDA (port 11) to input port
									GPIOE_PCOR |= 1 << 11;
									GPIOE_PDDR &= 0xFFFFF7FF;
						
							//wait
									Block(37);
									//raise clock
								GPIOE_PSOR |= 1 << 12;
								
								//change sda back to output
								GPIOE_PDDR |= 1 << 11;
								Block(74);
								}
		
		//for all data bits
				if ((index >= 12) && (index < 20))
						{
					//lower clock
							GPIOE_PCOR |= 1 << 12;
							//wait			
							Block(37);
					//raise/lower address bit
					if (((address >> (19 - index) )% 2) == 0)
							GPIOE_PCOR |= 1 << 11;
					else
						GPIOE_PSOR |= 1 << 11;
					//wait
							Block(37);
							//raise clock
						GPIOE_PSOR |= 1 << 12;
							//wait
						Block(74);
						}
				if ((index >= 22) && (index < 30))
						{
					//wait for data input- set SDA as input
					if (slaveAddress == 0x61)
					{
					GPIOE_PCOR |= 1 << 11;
					GPIOE_PDDR &= 0xFFFFF7FF;
					}
					//lower clock
							GPIOE_PCOR |= 1 << 12;
							//wait			
							Block(37);
					//raise/lower address bit
					if (((data >> (30 - index) )% 2) == 0)
							GPIOE_PCOR |= 1 << 11;
					else
						GPIOE_PSOR |= 1 << 11;
					//wait
							Block(37);
							//raise clock
						GPIOE_PSOR |= 1 << 12;
							//wait
						Block(74);
						}
				
				if (index == 31)
						{
					//lower clock
					GPIOE_PDDR |= 1 << 11;
							GPIOE_PCOR |= 1 << 12;
							//wait
							Block(37);
							//lower data
							GPIOE_PCOR |= 1 << 11;
							//wait
							Block(37);
							//raise clock
							GPIOE_PSOR |= 1 << 12;
							Block(37);
							//raise data
							GPIOE_PSOR |= 1 << 11;
							Block(74);
							//lower data
							
						}
	}
	
}

void initCamera(void) {
	//To use a Port, its Clock must be enabled!!
	//Lets just enable the clocks for ALL of the ports
	SIM_SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK
			| SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK
			| SIM_SCGC5_PORTE_MASK;
	SIM_SCGC3 |= SIM_SCGC3_FTM2_MASK;
	SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;
	//setup flexbus clock to 24 MHz
	SIM_CLKDIV1 &= 0xFF0FFFFF;
	SIM_CLKDIV1 |= 0x00700000;
	PORTA_PCR6 = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;

	PORTA_PCR10 = PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK;
	GPIOA_PDDR |= 0x01 << 10;
	//GPIOA_PCOR = 0x01 << 10;
	//Important!  Each IO pin has a dedicated 32-bit Register to set it up (Selection GPIO vs peripheral, IRQ, Etc.)
	//Setup port C7 as GPIO and enable High Drive Strength
	//PORTC_PCR4 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	PORTC_PCR7 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //Enable GPIO on on the pin
	PORTE_PCR9 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	PORTC_PCR8 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	PORTC_PCR9 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	PORTB_PCR11 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;

	//set up all the input pins necessary
	//PORTA_PCR10 = PORT_PCR_MUX(1) | PORT_PCR_ODE_MASK;
	PORTA_PCR7 = PORT_PCR_MUX(1) | PORT_PCR_ODE_MASK;
	PORTA_PCR29 = PORT_PCR_MUX(1) | PORT_PCR_ODE_MASK;
	PORTD_PCR15 = PORT_PCR_MUX(1) | PORT_PCR_ODE_MASK;
	PORTA_PCR9 = PORT_PCR_MUX(1) | PORT_PCR_ODE_MASK;
	PORTA_PCR11 = PORT_PCR_MUX(1) | PORT_PCR_ODE_MASK;
	PORTD_PCR12 = PORT_PCR_MUX(1) | PORT_PCR_ODE_MASK;
	PORTD_PCR13 = PORT_PCR_MUX(1) | PORT_PCR_ODE_MASK;
	PORTD_PCR14 = PORT_PCR_MUX(1) | PORT_PCR_ODE_MASK;
	PORTE_PCR8 = PORT_PCR_MUX(1) | PORT_PCR_ODE_MASK;
	PORTE_PCR9 = PORT_PCR_MUX(1) | PORT_PCR_ODE_MASK;
	PORTE_PCR10 = PORT_PCR_MUX(1) | PORT_PCR_ODE_MASK;

	//Make Sure the GPIO is setup to be an output

	GPIOC_PDDR |= LED_E1_LOC;
	GPIOC_PDDR |= LED_E2_LOC;
	GPIOC_PDDR |= LED_E3_LOC;
	GPIOB_PDDR |= LED_E4_LOC;
	//turn on LED 1
	GPIOC_PDDR |= LED_E1_LOC;
	GPIOC_PDDR |= LED_E2_LOC;
	GPIOC_PDDR |= LED_E3_LOC;
	GPIOB_PDDR |= LED_E4_LOC;

	//create a debugging port- pin B7 on tower, E2 ports
	PORTE_PCR2 = PORT_PCR_MUX(1);
	GPIOE_PDDR |= 1 << 2;

	//Init_Gpio();
	//set up initialization sequence for OV9650 camera
	PORTE_PCR12 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	PORTE_PCR7 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	PORTE_PCR11 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	//400 KHz max
	GPIOE_PDDR |= 1 << 11;
	GPIOE_PDDR |= 1 << 12;
	GPIOE_PDDR |= 1 << 7;
	//reset the camera
	GPIOE_PCOR |= 1 << 7;
	Block(1000);
	GPIOE_PSOR |= 1 << 7;
	Block(1000);
	GPIOE_PCOR |= 1 << 7;
	Block(1000);
	//initialize registers
	GPIOE_PSOR |= 1 << 12;

	sendSCCBSignal(0x60, 0x12, 0x90); // Reset all registers to default value. Output format QVGA
	Block(20000);
	//attempt to read register value
	sendSCCBSignal(0x61, 0x1C, 0xFF); // Manufacturer ID read only register
	//begin: clock low, data high
	//QQVGA initialization v1
	Block(3000);
	//set QQVGA YUV mode
	sendSCCBSignal(0x60, 0x12, 0x10); // Output format QVGA
	Block(3000);
	//set rgb:555 mode
	//sendSCCBSignal(0x60, 0x40, 0xF0);
	Block(1000);
	//enable Qvga
	sendSCCBSignal(0x60, 0x04, 0x04); // YUV/RGB skip every other row
	//Block(1000);
	sendSCCBSignal(0x60, 0x0C, 0x04); // VarioPixel for VGA and CIF
	Block(1000);
	sendSCCBSignal(0x60, 0x0D, 0x80); // VarioPixel for QVGA
	Block(1000);
	sendSCCBSignal(0x60, 0x11, 0x01); // Divide input clock by 2
	Block(1000);
	//sendSCCBSignal(0x60, 0x37, 0x91); // ADC (Specsheet says reserved?)
	//Block(1000);
	sendSCCBSignal(0x60, 0x38, 0x12); // ACOM (Specsheet says reserved?)
	Block(1000);
	//automatic black level calibration
	sendSCCBSignal(0x60, 0x3A, 0x01); // Digital BLC
	Block(1000);
	//lens edge correction
	sendSCCBSignal(0x60, 0x66, 0x01); // Lens correction control
	Block(1000);
	
	sendSCCBSignal(0x60, 0x34, 0x7F); // ARBLM
	Block(1000);
	sendSCCBSignal(0x60, 0x37, 0xFF); // ADC
	Block(1000);
	sendSCCBSignal(0x60, 0x41, 0x00); // COM16
	Block(1000);

	//set up FTM1 for input capture

	PORTA_PCR8 = PORT_PCR_MUX(3);
	PORTA_PCR9 = PORT_PCR_MUX(3);
	FTM1_MODE = FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK;
	FTM1_SC = 0; //Make sure its Off!


	//Edit registers when no clock is fed to timer so the MOD value, gets pushed in immediately

	FTM1_CNTIN = 0; // starting value for counter
	FTM1_CNT = 0; // loads the counter with CNTIN

	// -------IMPORTANT --- this must be written before the C3V register, otherwise
	// ------- C3V will not be updated with the value written to it	
	FTM1_COMBINE = 0x00;
	FTM1_C0SC = FTM_CnSC_CHIE_MASK | FTM_CnSC_ELSA_MASK;//FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK; 
	FTM1_C1SC = FTM_CnSC_CHIE_MASK | FTM_CnSC_ELSA_MASK;//FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK; 		
	FTM1_MOD = 0xC;
	FTM1_SC = 0x08;
	enable_irq(63);
}

//return a degree value from -900 to 900 (-45 to 45 degrees) based on the pixel distance to the black line center
int arctan_approximation(int black_line_center)
{
	float adjacent;
	float opposite;
	float theta;
	float divided;
	//convert pixels to cm units- from measurements of track components, will change with design
	opposite = (black_line_center - 71.5) * (12.5/145);
	//pre-measured distance to measuring line in centimeters
	adjacent = 18.1;
	//calculate arctangent approx based on maclaurin series
	divided = opposite/adjacent;
	//theta = divided - (divided * divided * divided)/3 + (divided * divided * divided * divided * divided )/5 - (divided * divided * divided * divided * divided * divided * divided )/7;
	theta = atan(divided);
	return (int) (theta*(180.0/3.1415926)*10);
	//
}

extern int varSelect;
void setCamProperty(int d){
	
	sendSCCBSignal(0x60, varSelect, d);
	Block(1000);
	
}