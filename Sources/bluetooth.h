/*
 * uart.h
 *
 *  Created on: Aug 4, 2019
 *      Author: Utilisateur
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#define UART0

#include "derivative.h"
#include "data.h"

#define MONITORING_REQUEST 0xFF

/*
 * data.h is essential for control data
 * All camera data, monitoring variables and control variables must be declared in data.h
 * The format of the file must be respected for the Java interface to read
 * It is necessary to use UART0 to maximise the transfer baud to 1382400 on HC-05
 * This is not possible with other UART1/2 due to several limitations
 * It is still possible but it is necessary to reconfigure the entire micro's sys clock to 44MHz to attain this
 */

unsigned int read_data_position;
unsigned int end_read_data_position;
unsigned int write_data_position;
unsigned int end_write_data_position;

/*
 * This function initialize UART0 module used to drive the external HC-05 module
 * Baud rate 9600 (default baud of HC-05)
 * RIE interrupt is enabled
 * 
 * Param : void
 * Return : void
 */
void UART1_Bluetooth_Init();

/*
 * This function perform a byte transmit to the UART transmitter
 * 
 * Param: void
 * Return : void
 */
void UART1_Bluetooth_Send(unsigned char data);

void UART0_Bluetooth_Init();
void UART0_Bluetooth_Send(unsigned char data);

void UART2_Bluetooth_Init();
void UART2_Bluetooth_Send(unsigned char data);

#endif /* UART_H_ */
