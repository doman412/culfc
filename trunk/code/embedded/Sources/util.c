/*
 * util.c
 *
 *  Created on: Oct 15, 2012
 *      Author: Derek Arner
 */
#include "util.h"



int findChar(char ch, char str[]){
	int i;
	for(i=0; str[i]!=NULL; i++){
		if(str[i]==ch)
			return i;
	}
	return -1;
}
int strLen(char str[]){
	int i;
	int cnt;
	for(i=0; str[i]!=NULL; i++){
		cnt = i;
	}
	return cnt+1;
}

int parseInt(char str[]){
	int number =0;
	int sign = 1;
	int mult = 1;
	int ch;
	int i;
	int limit = 0;
	int len = strLen(str)-1;
	
	if(str[0]=='-'){
		sign = -1;
		limit = 1;
	}
	for(i=len; i>=limit; i--){
		ch = str[i]-48;
		ch = ch * mult;
		mult = mult * 10;
		number = number+ch;
		
	}
	return number*sign;
}
void enable_irq (int irq)
{
    int div;
    
    /* Make sure that the IRQ is an allowable number. Right now up to 91 is 
     * used.
     */
    if (irq > 91)
        printf("\nERR! Invalid IRQ value passed to enable irq function!\n");
    
    /* Determine which of the NVICISERs corresponds to the irq */
    div = irq/32;
    
    switch (div)
    {
    	case 0x0:
              NVICICPR0 |= 1 << (irq%32);
              NVICISER0 |= 1 << (irq%32);
              break;
    	case 0x1:
              NVICICPR1 |= 1 << (irq%32);
              NVICISER1 |= 1 << (irq%32);
              break;
    	case 0x2:
              NVICICPR2 |= 1 << (irq%32);
              NVICISER2 |= 1 << (irq%32);
              break;
    }              
}