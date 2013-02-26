/*
 * File:		adc_demo.c
 * Purpose:		Demo the Analog to Digtal Converter triggered by the Programable Delay Block
 *                      Call the function Hw_Trig_Test which demonstrates and tests this function.
 *                      Runs on the Tower Kinetis Card, using the potentiometer as input.
 *                      The operator will be prompted on the serial port at 115200 baud, 8n1.
 *                      The LED's will also indicate the timing of the conversions.
 *
 */

//#include "common.h"    // Common Kinetis defines
//#include "adc16.h"     // ADC driver defines
//#include "adc_demo.h"   // For this function
#include "derivative.h";
#include "math.h"
#include "cam.h"

/*    global variable    */
//tADC_Config Master_Adc_Config; // This is the structure that contains the desired ADC/PGA configuration.

static int current_line = 0;
int stored_port_values_0[349];
uint8_t processing_stored_port_values[7][349];
int stored_port_values_1[349];
int stored_port_values_2[349];
int stored_port_values_3[349];
int stored_port_values_4[349];
int stored_port_values_top[349];
int stored_port_values_bottom[349];
		int delay_offset = 2;
		int number_to_capture = 348;
	int operation_mode;

void Block(long Value);


int end_of_frame;



//##### Vars for debugging ######//
//variables for accurate mode
int varSelect = 0;
int highest_black_pixel_threshold = 140; //turns this filter off
int line_width_threshold = 70;
int detection_threshold = 55;
int angular_denominator = 87;
int high_speed = 70;
int mid_speed = 50;
int low_speed = 40;
//float kp = .1;
int straightBand = 5;
int straightSpeed = 50;
int lower_line_width_threshold = 15;
int white_threshold_lower = 15;
int white_threshold_upper = 100;
int speed_threshold = 5;
int near_weight = 7;
int far_weight = 3; 
int difference_threshold = 3;

int lost_line_threshold = 5;
int lost_line_multiplier = 15;

int knp = 90;
int kni = 5;
int knd = 30;

int kfp = 45;

int near_far_ratio = 100;

void varSwitch(int s){
	varSelect = s;
}
void btDebug(int d){
	switch(varSelect){
	case 0:
		angular_denominator = d;
		break;
	case 1:
		line_width_threshold = d;
		break;
	case 2:
		highest_black_pixel_threshold = d;
		break;
	case 3:
		detection_threshold = d;
		break;
	case 4:
		high_speed = d;
			break;	
	case 5:
		mid_speed = d;
			break;
	case 6:
		low_speed = d;
			break;
	case 7:
		lower_line_width_threshold = d;
		break;
	case 8:
		white_threshold_lower =d;
		break;
	case 9:
		white_threshold_upper =d;
		break;
	case 10:
		speed_threshold = d;
		break;
	case 11:
		near_weight = d;
		break;
	case 12:
		far_weight = d;
		break;
	case 13:
		difference_threshold = d;
		break;
		
	case 14:
		knp = d;
		break;
	case 15:
		kni = d;
		break;
	case 16:
		knd = d;
		break;
	case 17:
		kfp = d;
		break;
	case 18:
		near_far_ratio = d;
		break;
	case 19:
		lost_line_threshold = d;
		break;
	case 20:
		lost_line_multiplier = d;
		break;
	}
}

void cmdB(int a){ 
	breakCar(); 
	operation_mode = 0;
}

void opGo(int g){
	operation_mode = 1;
}

//##### END Vars for debugging ######//

void Block(long Value)
{
    long i;

    for(i=0;i<Value;i++)
    {
    }
}




/********************************************************************/
void main(void) {
	int speed_to_run;
	int angle_to_turn;

	int line_iterator;
	int current_consecutive_zeros[7];
	int longest_consecutive_zeros[7];
	int longest_consecutive_zeros_right_edge[7];
	
	int last_10_centers[10];
	int longest_consecutive_zeros_center[7];
	int near_proportional;
	int integral;
	int derivative;
	int zero_just_seen[7];
	int previous_angle;
	int times_finish_line_gone_over;
	int number_of_0_pixels[7];
	int number_of_1_pixels[7];
	int times_longest_zero_alternates;
	int beginning_line_seen;
	int times_alternated;
	int highest_line_size[5];
	//new finish line detection
	int just_went_over_line;
	int highest_total_number_zeros[5];
	int cumulative_number_terms;
	int highest_second_zeros;
	int highest_third_zeros;
	int filter_activated;
	int most_black_pixels;
	int index;
	int longest_consecutive_white[5];
	int longest_consecutive_ones_right_edge[5];
	int highest_line_all_samples;
	int current_consecutive_ones[5];
	int highest_black_all_samples;
	int lowest_longest_white;
	int far_proportional = 0;
		
	int kn1 = knp + kni + knd;
	int kn2 = -(knp + 2*knd);
	int kn3 = knd;
	
	int kf1 = kfp;
			
	int en[3] = {0,0,0};
	int ef = 0;
		
		
		highest_line_all_samples = 0;
			highest_black_all_samples = 0;
	most_black_pixels = 0;
	just_went_over_line = 0;
	lowest_longest_white = 200;
	
	
	
	
	//reset everything
	for(index =0; index < 7; index++)
	{
		highest_total_number_zeros[index] = 0;
		longest_consecutive_zeros[index] = 0;
		longest_consecutive_zeros_right_edge[index] = 0;
		number_of_0_pixels[index] = 0;
		number_of_1_pixels[index] = 0;
		current_consecutive_zeros[index] = 0;
		highest_line_size[index] = 0;
		zero_just_seen[index] = 0;
		longest_consecutive_white[index] = 0;
	}
	cumulative_number_terms = 0;
	
	InitClock();
	highest_third_zeros = 0;
	highest_second_zeros = 0;
	end_of_frame = 0;
	current_line = 0;
	speed_to_run = 0;
	angle_to_turn = 0;
	operation_mode = 0;
	
	times_longest_zero_alternates = 0;
	times_finish_line_gone_over = 0;
	beginning_line_seen = 0;
	//from derek's stuff
	initBT(1);
	initWheels();
	steer(900);
	initBtn();
	
	initCamera();
	steer(0);

	//simple processing loop
		for(;;)
			{
				//calculate only once per frame
				if(end_of_frame==1)
				{
				times_alternated = 0;
				times_longest_zero_alternates = 0;
				
				
				for(index =0; index < 7; index++)
					{
						
						longest_consecutive_zeros[index] = 0;
						longest_consecutive_zeros_right_edge[index] = 0;
						number_of_0_pixels[index] = 0;
						number_of_1_pixels[index] = 0;
						current_consecutive_zeros[index] = 0;
						current_consecutive_ones[index] = 0;
						longest_consecutive_white[index] = 0;
						longest_consecutive_ones_right_edge[index] = 0;
					}
				filter_activated=0;
				//find the longest consecutive number of 0's in 1 line
				for (line_iterator = 0; line_iterator < number_to_capture/2; line_iterator++) {
					for(index =0; index < 7; index++)
					{	
					if (processing_stored_port_values[index][line_iterator] == 0) {
						number_of_0_pixels[index]++;
						current_consecutive_zeros[index]++;
						if (current_consecutive_zeros[index] > longest_consecutive_zeros[index]) {
							times_longest_zero_alternates++;

							longest_consecutive_zeros[index] = current_consecutive_zeros[index];
							longest_consecutive_zeros_right_edge[index] = line_iterator;
						} 
						//if ((zero_just_seen[index] == 1)) {
							current_consecutive_ones[index] = 0;
						//}
						//zero_just_seen[index] = 1;
					} else {
						number_of_1_pixels[index]++;
						current_consecutive_ones[index]++;
						if (current_consecutive_ones[index] > longest_consecutive_white[index]) {
							
							longest_consecutive_white[index] = current_consecutive_ones[index];
							longest_consecutive_ones_right_edge[index] = line_iterator;
						}
						//allow one bit of noise in data
						//				if ((zero_just_seen[index] == 0)) {
						current_consecutive_zeros[index] = 0;
											

						//				} else {
						//					times_alternated++;
						//					zero_just_seen[index] = 0;
						//				}
							
						
					}
					}
				}
				
				//edge detection on line 5 data
				for(index =0; index < 7; index++)
							{
				longest_consecutive_zeros_center[index]= longest_consecutive_zeros_right_edge[index] - longest_consecutive_zeros[index]/2;
							
							}
				GPIOC_PSOR |= LED_E1_LOC;
				GPIOC_PSOR |= LED_E2_LOC;
				GPIOC_PSOR |= LED_E3_LOC;
				GPIOB_PSOR |= LED_E4_LOC;
				//5 = top
				//6 = bottom
				//0-4 = middle
						if ((longest_consecutive_zeros_center[5] > number_to_capture/4))
						{
							
							GPIOC_PCOR |= LED_E1_LOC;
							
						}
						if ((longest_consecutive_zeros_center[0] > number_to_capture/4))
						{
							
							GPIOC_PCOR |= LED_E2_LOC;
							
						}
						if ((longest_consecutive_zeros_center[4] > number_to_capture/4))
						{
							
							GPIOC_PCOR |= LED_E3_LOC;
							
						}
						if ((longest_consecutive_zeros_center[6] > number_to_capture/4))
						{
							
							GPIOB_PCOR |= LED_E4_LOC;
							
						}
				
				//check for no line detected
				/*if (longest_consecutive_zeros < 7)
								{
									GPIOC_PCOR |= LED_E1_LOC;
									GPIOC_PCOR |= LED_E2_LOC;
									GPIOC_PCOR |= LED_E3_LOC;
									GPIOB_PCOR |= LED_E4_LOC;
								}*/
				//debugging- find line width
/*				integral = 0;
				
				for(line_iterator = 9; line_iterator > 0; line_iterator--)
				{
					
					integral += last_10_centers[line_iterator]- number_to_capture/4;
					last_10_centers[line_iterator] = last_10_centers[line_iterator - 1];
				}
				integral += longest_consecutive_zeros_center[5]- number_to_capture/4;
				last_10_centers[0] = longest_consecutive_zeros_center[5];*/
				
				
				if (operation_mode == 1)
				{
		//			sendChar(200);
		//			sendChar(number_of_0_pixels);
		//			sendChar(times_alternated);
		//			sendChar(longest_consecutive_zeros);
		//			sendChar(longest_consecutive_zeros_right_edge);
		//			sendChar(longest_consecutive_zeros_right_edge);
							
					//accurate phase 1 processing mode
				
					//turn the angle based on the arctan approximation
					 //angle_to_turn = arctan_approximation(longest_consecutive_zeros_center);
					en[2] = en[1];
					en[1] = en[0];
					en[0] = longest_consecutive_zeros_center[2] - number_to_capture/4;
					//near_proportional = longest_consecutive_zeros_center[2] - number_to_capture/4; //(longest_consecutive_zeros_center[0] + longest_consecutive_zeros_center[1] + longest_consecutive_zeros_center[2] + longest_consecutive_zeros_center[3] + longest_consecutive_zeros_center[4])/5 - number_to_capture/4;
					//far_proportional = longest_consecutive_zeros_center[5] - number_to_capture/4;
					ef = longest_consecutive_zeros_center[5] - number_to_capture/4;
					//integral = integral / 10;
					
					// angle_to_turn = (int)(((0.5*proportional + 0.5*integral) * 900)/69);
					//if (longest_consecutive_zeros[5] > 15)
					// {
					// angle_to_turn = (int)(((near_proportional*0.6 + far_proportional*0.4) * 900)/(angular_denominator));
					// }
					//else //no far point seen- on turn
					//{
						//angle_to_turn = (int)((((near_weight*near_proportional) + (far_weight*far_proportional))* 900)/(angular_denominator*10));
											 	
					//}
					
					angle_to_turn = ((near_far_ratio*(((kn1*en[0])+(kn2*en[1])+(kn3*en[2])))/(10)) + ((100-near_far_ratio)*kf1*ef))/100;
					angle_to_turn += previous_angle;
					 if(angle_to_turn > 900)
						 angle_to_turn = 900;
					 if(angle_to_turn < -900)
						 angle_to_turn = -900; 
					
					 //set the speed based on how much correction needs to be done- if angle is low, set to maximum speed
					//stepwise speed
					 /*if ((angle_to_turn <= 10) & (angle_to_turn >= -10)){
						 speed_to_run = high_speed;
					 }
					 else if (((angle_to_turn <= 20) & (angle_to_turn > 10)) | ((angle_to_turn < -10) & (angle_to_turn >= -20)))
							 speed_to_run = mid_speed;
					 else
						 speed_to_run = low_speed;*/
						//linear speed profile
						//speed_to_run = (((900 - abs(angle_to_turn))* 70)/900) + 30;
						
					//derivative based
						//if(derivative < 100)
						//speed_to_run = high_speed;
						//else
						//speed_to_run = mid_speed;
						
					//difference based
					if (abs(longest_consecutive_zeros_center[5] - longest_consecutive_zeros_center[2]) < speed_threshold)
						speed_to_run = high_speed;
					else
					speed_to_run = mid_speed;
					
					
					 
					 //if no line detected, lower the speed and increase the turn angle
					 if (longest_consecutive_zeros[2] < lost_line_threshold)
					 {
						 angle_to_turn = previous_angle * lost_line_multiplier/10;
					 if(angle_to_turn > 900)
						 angle_to_turn = 900;
					 if(angle_to_turn < -900)
						 angle_to_turn = -900;
						 speed_to_run = low_speed;
					 }
					 
					 //finish line detection
					 //(longest_consecutive_zeros_right_edge > 110)&
					 //if ((longest_consecutive_zeros > 30)&(number_of_0_pixels < 110)&(second_longest_consecutive_zeros < 50)& (third_longest_consecutive_zeros>5))
					 /*if((longest_consecutive_zeros_right_edge>130)&&(longest_consecutive_zeros< 60)&&(number_of_0_pixels<105))
					 {
						beginning_line_seen++;
						if(beginning_line_seen > (11- speed_to_run/10))
							 times_finish_line_gone_over++;
					 }			 //intersection detection- reset the finish line detection
					 if ((number_of_0_pixels < 160) && (number_of_0_pixels >= 110))
					 {
						 beginning_line_seen = 0;
						// times_finish_line_detected = 0;
					 }
					 if ((number_of_0_pixels < 40))
					 {
						 beginning_line_seen = 0;
						// times_finish_line_detected = 0;
					 }*/
					 //revised finish line detection algorithm
					 most_black_pixels = 0;
					 for(index =0; index < 5; index++)
					 {
						 if(number_of_0_pixels[index] > most_black_pixels)
							 	most_black_pixels = number_of_0_pixels[index];
					 }	
					 
					
					 //just the black line
					 if(most_black_pixels < detection_threshold)
					 {
						 
						 if(just_went_over_line >= 1)
						 {
							 					 
							 
							 				
							 if ((highest_black_all_samples < 113)) {
								GPIOC_PCOR |= LED_E1_LOC;
								GPIOC_PSOR |= LED_E2_LOC;
								GPIOC_PSOR |= LED_E3_LOC;
								GPIOB_PSOR |= LED_E4_LOC;
							}
							if ((highest_black_all_samples < 125) & (highest_black_all_samples >= 113)) {
								GPIOC_PSOR |= LED_E1_LOC;
								GPIOC_PCOR |= LED_E2_LOC;
								GPIOC_PSOR |= LED_E3_LOC;
								GPIOB_PSOR |= LED_E4_LOC;
							}
							if ((highest_black_all_samples < 137) & (highest_black_all_samples >= 125)) {
								GPIOC_PSOR |= LED_E1_LOC;
								GPIOC_PSOR |= LED_E2_LOC;
								GPIOC_PCOR |= LED_E3_LOC;
								GPIOB_PSOR |= LED_E4_LOC;
							}
							if ((highest_black_all_samples < 200) & (highest_black_all_samples >= 137)) {
								GPIOC_PSOR |= LED_E1_LOC;
								GPIOC_PSOR |= LED_E2_LOC;
								GPIOC_PSOR |= LED_E3_LOC;
								GPIOB_PCOR |= LED_E4_LOC;
							}
							  //compute the average and compare to known line values
							 if ( highest_total_number_zeros[2]>= 123)
							 {
								GPIOC_PCOR |= LED_E1_LOC;
								GPIOC_PCOR |= LED_E2_LOC;
								GPIOC_PCOR |= LED_E3_LOC;
								GPIOB_PCOR |= LED_E4_LOC;
							 }
							 diag("detected",0);
							 if ((highest_black_all_samples < highest_black_pixel_threshold))
								 diag("highblack",1);
					
							 else
								 diag("highblack",0);
							 
							 diag("val", highest_black_all_samples);
							 if (highest_line_all_samples < line_width_threshold)
								 diag("widthhigh",1);
							 else
								 diag("widthhigh",0);
							 
							 diag("val", highest_line_all_samples);
							 if (highest_line_all_samples > lower_line_width_threshold) 
								 diag("widthlow",1);
							 else
								 diag("widthlow",0);
							 
							 diag("val", highest_line_all_samples);
							if (lowest_longest_white < white_threshold_upper)
								diag("whitehigh",1);
							 else
							 diag("whitehigh",0);		
							
							diag("val", lowest_longest_white);
							if (lowest_longest_white > white_threshold_lower)
								diag("whitelow",1);
							else
								 diag("whitelow",0);	
							
							diag("val", lowest_longest_white);
							if (abs(highest_black_all_samples - highest_line_all_samples) > difference_threshold)
								diag("difference",1);
							else
								diag("difference",0);
							 //if ((highest_total_number_zeros < 105)&(highest_total_number_zeros > 50)&(highest_line_size < 50))
							 if ((highest_black_all_samples < highest_black_pixel_threshold)&(highest_line_all_samples < line_width_threshold)&(highest_line_all_samples > lower_line_width_threshold) & (lowest_longest_white < white_threshold_upper)& (lowest_longest_white > white_threshold_lower) & (abs(highest_black_all_samples - highest_line_all_samples) > difference_threshold))
							 {
								 
								 //turn on LEDS to correspond to line location
								 		
								 times_finish_line_gone_over++;	 
							 }
							 for(index =0; index < 5; index++)
							 	{
							 		highest_total_number_zeros[index] = 0;
							 		
							 		highest_line_size[index] = 0;
							 		
							 	}
							 highest_line_all_samples = 0;
							 	highest_black_all_samples = 0;
							 just_went_over_line = 0;
							 cumulative_number_terms=0;
							 lowest_longest_white = 200;
						 }
					 }
					 //either the finish line or an intersection
					 if(most_black_pixels > detection_threshold)
					 {
						 GPIOC_PSOR |= LED_E1_LOC;
						GPIOC_PSOR |= LED_E2_LOC;
						GPIOC_PSOR |= LED_E3_LOC;
						GPIOB_PSOR |= LED_E4_LOC;
						 just_went_over_line++;
						 for(index =0; index < 5; index++)
						 {
						 if(number_of_0_pixels[index] > highest_total_number_zeros[index])
							 highest_total_number_zeros[index]= number_of_0_pixels[index];
						 
						 if (longest_consecutive_zeros[index]>highest_line_size[index])
							 highest_line_size[index] = longest_consecutive_zeros[index];
						 if(longest_consecutive_zeros[index]>highest_line_all_samples)
							 highest_line_all_samples = longest_consecutive_zeros[index];
						 if(longest_consecutive_white[index] < lowest_longest_white)
							 lowest_longest_white = longest_consecutive_white[index];
						 if(number_of_0_pixels[index] > highest_black_all_samples)
							 highest_black_all_samples = number_of_0_pixels[index];
						 
						 }
						 //highest_black_all_samples = (highest_total_number_zeros[0] + highest_total_number_zeros[1] + highest_total_number_zeros[2] + highest_total_number_zeros[3] + highest_total_number_zeros[4] )/5;
						// highest_line_all_samples = (highest_line_size[0] + highest_line_size[1] + highest_line_size[2] + highest_line_size[3] + highest_line_size[4])/5;
						 
						 cumulative_number_terms++;
						 
		//				 sendChar(200);
		//			 sendChar(number_of_0_pixels);
		//			sendChar(highest_third_zeros);
		//			sendChar(highest_total_number_zeros);
		//			sendChar(longest_consecutive_zeros);
		//				sendChar(96);
						 
						//keep the car straight if a black line/finish line detected
						angle_to_turn = 0;
					 }
					 
					 //stop the car if the finish line is detected
					 if(times_finish_line_gone_over > 0)
					 {
						 angle_to_turn = 0;
						 speed_to_run = 0;
						 
						 operation_mode = 0;
					 }
					 
		//			 if(processing_stored_port_values[115]==0 && processing_stored_port_values[45]==0)
		//				 sendStr("115-45");
		//			 if(processing_stored_port_values[110]==0 && processing_stored_port_values[45]==0)
		//				 sendStr("110-45");
		//			 if(processing_stored_port_values[110]==0 && processing_stored_port_values[40]==0)
		//				 sendStr("110-40");
		//			 if(processing_stored_port_values[115]==0 && processing_stored_port_values[40]==0)
		//				 sendStr("110-40");
						 
						 
						 
					 steer(angle_to_turn);
					 go(speed_to_run);
					 previous_angle = angle_to_turn;
					 times_finish_line_gone_over = 0;
					 decodeBT();
				}
				else
				{
					//off mode
					decodeBT();
					times_finish_line_gone_over = 0;
					//angle_to_turn = 0;
					//speed_to_run = 0;
					//steer(angle_to_turn);
					//go(speed_to_run);
					previous_angle = 0;
					kn1 = knp + kni + knd;
					kn2 = -(knp + 2*knd);
					kn3 = knd;
					kf1 = kfp;
					en[0] = 0;
					en[1] = 0;
					en[2] = 0;
					ef = 0;
				}
				end_of_frame = 0;
				}
				
			}
	
}







//channel interrupt- because FTM1 doesn't work as intended
void ftm1_isr(void)
{
	int pixel_iterator;
	long i;

	    
	

	
	if(FTM1_STATUS == 1)
	{
		current_line++;
		
		FTM1_STATUS = 0x00;
		if(current_line == 50)
						{
							for(i=0;i<delay_offset;i++)
											    {
											    }
							for(pixel_iterator = 0; pixel_iterator < number_to_capture; pixel_iterator+=1)
										{
								
								
								asm("nop");
								asm("nop");
								asm("nop");
								asm("nop");
								asm("nop");
								stored_port_values_top[pixel_iterator] =  GPIOD_PDIR;
								GPIOE_PTOR = 0x04;
										}
							
						
						}
		if(current_line == 78)
				{
					for(i=0;i<delay_offset;i++)
									    {
									    }
					for(pixel_iterator = 0; pixel_iterator < number_to_capture; pixel_iterator+=1)
								{
						
						
						asm("nop");
						asm("nop");
						asm("nop");
						asm("nop");
						asm("nop");
						stored_port_values_0[pixel_iterator] =  GPIOD_PDIR;
						GPIOE_PTOR = 0x04;
								}
					
				
				}
		if(current_line == 79)
				{
					for(i=0;i<delay_offset;i++)
									    {
									    }
					for(pixel_iterator = 0; pixel_iterator < number_to_capture; pixel_iterator+=1)
								{
						
						
						asm("nop");
						asm("nop");
						asm("nop");
						asm("nop");
						asm("nop");
						stored_port_values_1[pixel_iterator] =  GPIOD_PDIR;
						GPIOE_PTOR = 0x04;
								}
					
				
				}
		if(current_line == 80)
				{
					for(i=0;i<delay_offset;i++)
									    {
									    }
					for(pixel_iterator = 0; pixel_iterator < number_to_capture; pixel_iterator+=1)
								{
						
						
						asm("nop");
						asm("nop");
						asm("nop");
						asm("nop");
						asm("nop");
						stored_port_values_2[pixel_iterator] =  GPIOD_PDIR;
						GPIOE_PTOR = 0x04;
								}
					
				
				}
		if(current_line == 81)
				{
					for(i=0;i<delay_offset;i++)
									    {
									    }
					for(pixel_iterator = 0; pixel_iterator < number_to_capture; pixel_iterator+=1)
								{
						
						
						asm("nop");
						asm("nop");
						asm("nop");
						asm("nop");
						asm("nop");
						stored_port_values_3[pixel_iterator] =  GPIOD_PDIR;
						GPIOE_PTOR = 0x04;
								}
					
				
				}
		if(current_line == 82)
		{
			for(i=0;i<delay_offset;i++)
							    {
							    }
			for(pixel_iterator = 0; pixel_iterator < number_to_capture; pixel_iterator+=1)
						{
				
				
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				stored_port_values_4[pixel_iterator] =  GPIOD_PDIR;
				GPIOE_PTOR = 0x04;
						}
			
		
		}
		if(current_line == 120)
				{
					for(i=0;i<delay_offset;i++)
									    {
									    }
					for(pixel_iterator = 0; pixel_iterator < number_to_capture; pixel_iterator+=1)
								{
						
						
						asm("nop");
						asm("nop");
						asm("nop");
						asm("nop");
						asm("nop");
						stored_port_values_bottom[pixel_iterator] =  GPIOD_PDIR;
						GPIOE_PTOR = 0x04;
								}
					
				
				}
		GPIOE_PTOR = 0x04;
		
		
		

	
	
	
	}
	
	if (FTM1_STATUS >= 1)
	{
		stored_port_values_0[number_to_capture] = 0xBEEF;
		//store the previous port values in the processing port values array
		//pair averaging attempt
		for(pixel_iterator = 0; pixel_iterator < number_to_capture; pixel_iterator+=2)
		{
			//0
			processing_stored_port_values[0][pixel_iterator/2] = ((stored_port_values_0[pixel_iterator] & 0x1000) >> 12) + ((stored_port_values_0[pixel_iterator+1] & 0x1000) >> 12);
			
			//1
			processing_stored_port_values[1][pixel_iterator/2] = ((stored_port_values_1[pixel_iterator] & 0x1000) >> 12) + ((stored_port_values_1[pixel_iterator+1] & 0x1000) >> 12);
			//2
			processing_stored_port_values[2][pixel_iterator/2] = ((stored_port_values_2[pixel_iterator] & 0x1000) >> 12) + ((stored_port_values_2[pixel_iterator+1] & 0x1000) >> 12);
			//3
			processing_stored_port_values[3][pixel_iterator/2] = ((stored_port_values_3[pixel_iterator] & 0x1000) >> 12) + ((stored_port_values_3[pixel_iterator+1] & 0x1000) >> 12);
			//4
			processing_stored_port_values[4][pixel_iterator/2] = ((stored_port_values_4[pixel_iterator] & 0x1000) >> 12) + ((stored_port_values_4[pixel_iterator+1] & 0x1000) >> 12);
			//top for 2-point PID
			processing_stored_port_values[5][pixel_iterator/2] = ((stored_port_values_top[pixel_iterator] & 0x1000) >> 12) + ((stored_port_values_top[pixel_iterator+1] & 0x1000) >> 12);
			//bottom of FOV
			processing_stored_port_values[6][pixel_iterator/2] = ((stored_port_values_bottom[pixel_iterator] & 0x1000) >> 12) + ((stored_port_values_bottom[pixel_iterator+1] & 0x1000) >> 12);
						
		}
		GPIOE_PCOR = 0x04;

		current_line = 0;
		FTM1_STATUS = 0x00;
		end_of_frame = 1;
		
	}
	
	
	
}

void button_mode_toggle(void)
{
	if(PORTC_ISFR & PORT_ISFR_ISF(GPIO_PIN(5)))
		{
		 //printf("SW3 Pressed\n");
			operation_mode = 1;
		  
		}
		if(PORTC_ISFR & PORT_ISFR_ISF(GPIO_PIN(13)))
		{
			operation_mode = 0;
			go(0);
			steer(0);
			
		}
		
		//GPIOC_PTOR = LED2;
		PORTC_ISFR=0xFFFFFFFF;  //Clear Port C ISR flags
}

