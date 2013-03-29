/*
 * UART.c
 *
 *  Created on: Oct 15, 2012
 *      Author: Derek Arner
 */

#include "BT.h"
#include "cmd.h"
#include "queue.h"
#include "null.h"

char sentence[MAX_QUEUE_SIZE];
volatile int buffCount;
unsigned int readyToDecode = 0;
static Queue queue;

// This looks like a buffer of command buffers, but it's never used.
//char* decodeBuf[5] = {commandBuf,0,0,0,0};

//void (*fo)(char);
//Command f;
//Command list[1];



/*
 * sends a single character.
 */
void sendChar(char ch){
	while(!(UART0_S1 & UART_S1_TDRE_MASK));   
	/* Send the character */
	UART0_D = ch;
}
/*
 * sends a c-style string of characters.
 */
void sendStr(char str[]){
	int i =0;
	for(i=0; str[i]!=NULL ; i++){
		sendChar(str[i]);
	}
}
void diag(char str[], int data){
	int i = 0;
	for(i=0; str[i]!=NULL ; i++){
		if(i>=10)
			break;
		sendChar(str[i]);
	}
	for(i; i<10; i++){
		sendChar(' ');
	}
	sendChar(data);
}
/*
 * returns a char if one is in the buffer.
 * does check first but would be best if user 
 * checked before calling function using 'gotChar()'.
 */
char getChar(void){
	if(gotChar()){
		return UART0_D; // there is char in buffer
	}
}
/*
 * returns a boolean integer value if there
 * is a character waiting to be read
 */
int gotChar(void){
	return (UART0_S1 & UART_S1_RDRF_MASK);
}
/*
 * simple echo of input, run in main loop.
 */
void echoBT(void){
	if(gotChar()){
		sendChar(getChar());
	}
}
/*
 * initializes the bluetooth setup.
 * this includes, UART0, pins PTA14,PTA15
 * connections:
 * 			BT(RX)-->K40(TX)[A22]
 * 			BT(TX)-->K40(RX)[A24] 
 * param: mode:
 * 			0 == polling method (*add 'pollBT()' to loop)
 * 			1 == interrupt method (*add 'decodeBT()' to loop, 'BT_REC' to vector table:61)
 */


void initBT(int mode){
	
	init_queue(&queue);
	// First, turn on power to the module
			SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
			
			// Tell the RX line its function
			PORTA_PCR15 |= PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK;
			// Tell the TX line its function
			PORTA_PCR14 |= PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK;

			// Ensure UART is off before changing settings
			UART0_C2 &= ~(UART_C2_RE_MASK | UART_C2_TE_MASK);

			// Set baud rate
			// (96,000,000 Hz / 9600 Hz) / 16 = 625, or 0x271
			// BDH[4:0] = 0x2
			// BDL = 0x71
//			UART0_BDH |= 0x2;
//			UART0_BDL = 0x71;
			UART0_BDH = 0;
			UART0_BDL = 52;
			UART0_C4 |= UART_C4_BRFA(0); 
				
			// Set FIFO watermark to 1
			UART0_RWFIFO |= UART_RWFIFO_RXWATER(1);
			UART0_PFIFO |= UART_PFIFO_RXFE_MASK | UART_PFIFO_TXFE_MASK;
			
			// Enable receiver interrupt (45)
			NVICIP11 = 0x40;
			NVICICPR1 |= (1 << 13);
			NVICISER1 |= (1 << 13);
			
			// Turn on receiver, and receiver interrupt enable
			UART0_C2 |= (UART_C2_RE_MASK | UART_C2_RIE_MASK | UART_C2_TE_MASK);
	
	
}
/*
 * ISR method. be sure to include in 'sys_init' 
 */
void BT_REC(void){
	char ch;
	UART0_S1 &= ~(UART_S1_RDRF_MASK);
	ch = UART0_D;
	
	sendChar(ch);
	
	if(ch==10){ // backspace was pressed.
		pop_front(&queue);
	}
	else{
		push_front(&queue, ch);
	}
	
	if(ch==13){ // enter was pressed.
		readyToDecode = readyToDecode++;
	}
	
}
/*
 * a function that is run somewhere in the main loop
 * that allows for the decoding of a bluetooth command to happen
 * outside the ISR
 */
void decodeBT(){
	if(readyToDecode) {
		unsigned int i = 0;
		while (1) {
			if (queue_empty(&queue)) {
				break;
			}
			char c = pop_back(&queue);
			if (c == '\n') {
				sentence[i] = '\0';
				break;
			}
			else {
				sentence[i++] = c;
			}
		}
		decode(sentence);
	}
	readyToDecode--;
}
/*
 * decodes the command that was sent, then executes the command
 */
void decode(char* sentence){
	char* argument;
	char* command = (char*)strtok(sentence, " ");
	argument = (char*)strtok(NULL, " ");
	char* argv[2];
	unsigned int argc = 0;
	// While we have more words
	while (argument != NULL && argc < 2) {
		argv[argc] = argument;
		argc++;
		// Get the next word
		argument = (char*)strtok(NULL, " ");
	}
	
	Command cmd = find_command(command);
	char* message = cmd(argv, argc);
	
}
/*
 * poll BT UART instead of interrupt based;
 */
void pollBT(void){
//	char ch;
//	if(gotChar()){
//		ch = UART0_D;
//		
//		if(ch==13){ // enter was pressed.
//			commandBuf[buffCount] = NULL;
//			buffCount = 0;
////			readyToDecode = 1;
//			decode(commandBuf);
//		} else {
//			commandBuf[buffCount++] = ch;
////			sendChar(ch);
//		}
//		
//		
//	}
}


