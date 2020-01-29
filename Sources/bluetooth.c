/*
 * uart.c
 *
 *  Created on: Aug 4, 2019
 *      Author: Utilisateur
 */

#include "bluetooth.h"

void UART1_Bluetooth_Init() {
	// turn on all UART clocks 
	//SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1); // select MGCPLLCLK/2 as the source clock for UART0 
	SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;
			
	// MUX pin E0 and E1 to be used at UART1_TX/UART1_RX respectively 
	PORTE_PCR0 |= PORT_PCR_MUX(3);
	PORTE_PCR1 |= PORT_PCR_MUX(3);
		
	// disable Tx and Rx on UART1 before making changes
	UART1_C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
		
	// configure the UART for 8-bit mode, no parity, clear to default 
	UART1_C1 = 0;
		
	// configure the UART to have baud rate 115385 (0.16% error from nominal 115200) < 3.75% max error allowed for 8-bit UART
	UART1_BDH = 0;    // clear LIN, RxD interrupt, stop bit = 1
	UART1_BDL = 13;
	
	// enable Tx and Rx on UART1 after changes
	UART1_C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);
	
	// enable interrupt via RDRF(Receive Data Register Full)
	UART1_C2 |= UART_C2_RIE_MASK;
	
	// NVIC for UART1 is IRQ 13 (table 3.7, page 53)
	NVIC_ICPR |= (1 << 13);			// clear pending interrupt 13
	NVIC_ISER |= (1 << 13);			// enable interrupt 13
}

void UART1_Bluetooth_Send(unsigned char data) {
	while(!(UART1_S1 & UART_S1_TDRE_MASK));
	UART1_D = (unsigned char)data;
}

void UART0_Bluetooth_Init() {
	// select clock source for UART0 and enable clock source for UART0
	SIM_SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK;
	SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
	
	// Mux pin E20 and E21 for use as UART0_TX and RX respectively
	PORTA_PCR1 |= PORT_PCR_MUX(2);
	PORTA_PCR2 |= PORT_PCR_MUX(2);
	
	// clear interrupt mask before doing configuration on UART0
	UART0_C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
	
	// configure the UART for 8-bit mode, no parity, clear to default 
	UART0_C1 = 0;
	
	// configure the BR rate: this yields 1411765 baud rate, 2.124% deviation from nominal
	UART0_BDH = 0;    // clear LIN, RxD interrupt, stop bit = 1
	//UART0_BDL = 2;
	UART0_BDL = 26;
		
	UART0_C5|= UART0_C5_BOTHEDGE_MASK;
	
	// enable Tx and Rx on UART1 after changes
	UART0_C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);
	
	// enable interrupt via RDRF(Receive Data Register Full)
	UART0_C2 |= UART_C2_RIE_MASK;	
	
	// NVIC for UART1 is IRQ 12 (table 3.7, page 53)
	//NVIC_ICPR |= (1 << 12);			// clear pending interrupt 12
	//NVIC_ISER |= (1 << 12);			// enable interrupt 12
}

void UART0_Bluetooth_Send(unsigned char data) {
	while(!(UART0_S1 & UART_S1_TDRE_MASK));
	UART0_D = (unsigned char)data;
}

void UART2_Bluetooth_Init() {
	// turn on all UART clocks 
	//SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1); // select MGCPLLCLK/2 as the source clock for UART0 
	SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;
			
	// MUX pin E22 and E23 to be used at UART2_TX/UART2_RX respectively 
	PORTE_PCR22 |= PORT_PCR_MUX(4);
	PORTE_PCR23 |= PORT_PCR_MUX(4);
		
	// disable Tx and Rx on UART2 before making changes
	UART2_C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
		
	// configure the UART for 8-bit mode, no parity, clear to default 
	UART2_C1 = 0;
		
	// configure the UART to have baud rate 115385 (0.16% error from nominal 115200) < 3.75% max error allowed for 8-bit UART
	UART2_BDH = 0;    // clear LIN, RxD interrupt, stop bit = 1
	UART2_BDL = 13;
	
	// enable Tx and Rx on UART2 after changes
	UART2_C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);
	
	// enable interrupt via RDRF(Receive Data Register Full)
	UART2_C2 |= UART_C2_RIE_MASK;
	
	// NVIC for UART2 is IRQ 14 (table 3.7, page 53)
	NVIC_ICPR |= (1 << 14);			// clear pending interrupt 14
	NVIC_ISER |= (1 << 14);			// enable interrupt 14
}

void UART2_Bluetooth_Send(unsigned char data) {
	while(!(UART2_S1 & UART_S1_TDRE_MASK));
	UART2_D = (unsigned char)data;
}
