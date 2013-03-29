/*
 * uart.c
 *
 *  Created on: Dec 3, 2012
 *      Author: ulab
 */

#include "uart.h"
#include "derivative.h"
#include "queue.h"

int data;

void InitUart(void) {

	// First, turn on power to the module
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
	
	// Tell the RX line its function
	PORTA_PCR15 |= PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK;

	// Ensure UART is off before changing settings
	UART0_C2 &= ~(UART_C2_RE_MASK | UART_C2_TE_MASK);

	// Set baud rate
	// (96,000,000 Hz / 9600 Hz) / 16 = 625, or 0x271
	// BDH[4:0] = 0x2
	// BDL = 0x71
	UART0_BDH |= 0x2;
	UART0_BDL = 0x71;

	// Set FIFO watermark to 1
	UART0_RWFIFO |= UART_RWFIFO_RXWATER(1);
	UART0_PFIFO |= UART_PFIFO_RXFE_MASK;
	
	// Enable receiver interrupt (45)
	NVICIP11 = 0x40;
	NVICICPR1 |= (1 << 13);
	NVICISER1 |= (1 << 13);
	
	// Turn on receiver, and receiver interrupt enable
	UART0_C2 |= (UART_C2_RE_MASK | UART_C2_RIE_MASK);
}

void DisableUart(void) {
	SIM_SCGC4 &= ~SIM_SCGC4_UART0_MASK;
}

void Uart0Isr(void) {
	static int status;
	static int data;
	
	status = UART0_S1;
	data = UART0_D;
	BufferWrite(data);
}