/*
 * main implementation: use this 'C' sample to create your own application
 *
 */


#include "derivative.h" /* include peripheral declarations */
#include "bluetooth.h"

int blink;
int color_counter;

int i;	
int temp_reg;	


void switchLed(int red, int green, int blue);

int main(void){
	
	// configure clock to 48 MHz from a 8 MHz crystal
	MCG_C2 = (MCG_C2_RANGE0(1) | MCG_C2_EREFS0_MASK); 	// configure the oscillator settings
	MCG_C1 = (MCG_C1_CLKS(2) | MCG_C1_FRDIV(3));		// divider for 8 MHz clock (8M coming from oscillator)
	for (i = 0 ; i < 24000 ; i++);						// wait for OSCINIT to set
	// now in FBE mode
	MCG_C6 |= MCG_C6_CME0_MASK;		// enable the clock monitor
	MCG_C5 |= MCG_C5_PRDIV0(1); 	// set PLL ref divider to divide by 2 (PLL division factor)
	
	temp_reg = MCG_C6; 				// store present C6 value (as CME0 bit was previously set)
	temp_reg &= ~MCG_C6_VDIV0_MASK; // clear VDIV settings
	temp_reg |= MCG_C6_PLLS_MASK | MCG_C6_VDIV0(0); 	// write new VDIV and enable PLL (PLL multiplier by 24)
	MCG_C6 = temp_reg; 				// update MCG_C6		
	for (i = 0 ; i < 4000 ; i++) 	// wait for PLLST status bit to set
	// now in PBE mode
	SIM_CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(1) | SIM_CLKDIV1_OUTDIV4(1));	// core clock, bus clock div by 2	
	MCG_C1 &= ~MCG_C1_CLKS_MASK;	// switch CLKS mux to select the PLL as MCGCLKOUT = 96MHz	
	for (i = 0 ; i < 2000 ; i++)	// Wait for clock status bits to update
	// now in PEE mode, core and system clock 48 MHz, bus and flash clock 24 MHz

	// turn on all port clocks
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;
	
	// turn on all UART clocks 
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK | SIM_SCGC4_UART1_MASK | SIM_SCGC4_UART2_MASK;
	
	// turn on all TPM clocks
	SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM1_MASK | SIM_SCGC6_TPM2_MASK;

	// turn on ADC0 clock
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;

	// set ADC inputs to ADC inputs
	PORTC_PCR2  |= PORT_PCR_MUX(0);		// Camera 1 PTC2 ADC0_SE11
	
	// set GPIOs to GPIO function
	PORTC_PCR0  |= PORT_PCR_MUX(0);	// PTC0 Push Button S2, channel into ADC0_SE14
	PORTB_PCR18 |= PORT_PCR_MUX(1);	// PTB18 LED red
	PORTB_PCR19 |= PORT_PCR_MUX(1);	// PTB19 LED green
	PORTD_PCR1  |= PORT_PCR_MUX(1);	// PTD1  LED blue
	PORTB_PCR8  |= PORT_PCR_MUX(1);	// PTB8 Camera SI
	PORTB_PCR9  |= PORT_PCR_MUX(1);	// PTB9 Camera Clock
	PORTA_PCR4  = 0;				// Set PTA4 Pin Control Register to Zero
	PORTA_PCR4  |= PORT_PCR_MUX(3);	// PTA4 Motor 1 In 1 (speed) PTA4 TPM0_CH1
	PORTA_PCR5  |= PORT_PCR_MUX(1);	// PTA5 Motor 1 In 2 (direction)
	PORTC_PCR8  |= PORT_PCR_MUX(1);	// PTC8 Motor 2 In 1 (direction)
	PORTC_PCR9  |= PORT_PCR_MUX(3);	// PTC9 Motor 2 In 2 (speed) PTC8 TPM0_CH5
	
	// set GPIO outputs to outputs
	GPIOB_PDDR |= (1<<18);			// PTB18 LED red
	GPIOB_PDDR |= (1<<19);			// PTB19 LED green
	GPIOD_PDDR |= (1<<1);			// PTD1  LED blue
	GPIOB_PDDR |= (1<<8);			// PTB8 Camera SI
	GPIOB_PDDR |= (1<<9);			// PTB9 Camera Clock
	GPIOA_PDDR |= (1<<4);			// PTA4 Motor 1 In 1
	GPIOA_PDDR |= (1<<5);			// PTA5 Motor 1 In 2 (direction)
	GPIOC_PDDR |= (1<<8);			// PTC9 Motor 2 In 1 (direction)
	GPIOC_PDDR |= (1<<9);			// PTC9 Motor 2 In 2 
	
	// all LEDs off
	GPIOB_PDOR |= GPIO_PDOR_PDO(1<<18);   // red LED off
	GPIOB_PDOR |= GPIO_PDOR_PDO(1<<19);   // green LED off
	GPIOD_PDOR |= GPIO_PDOR_PDO(1<<1);    // blue LED off	
	
	// GPIOB_PDOR &= ~GPIO_PDOR_PDO(1<<18);  // red LED on
	//GPIOB_PDOR &= ~GPIO_PDOR_PDO(1<<19);  // green LED on
	// GPIOD_PDOR &= ~GPIO_PDOR_PDO(1<<1);   // blue LED on	
			
	// set GPIO input to input
	GPIOC_PDDR &= ~ (1<<0);			// PTC0 Push Button S2

	// set PWM outputs
	PORTA_PCR12 |= PORT_PCR_MUX(3);	// Servo Motor 1 Out  PTA12 TPM1_CH0
	
	// configure TPM clock source to be 48 MHz
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);		// MCGFLLCLK clock or MCGPLLCLK/2
	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;	// use MCGPLLCLK/2 clock when running from 8 MHz and PLL
		
	// set TPM prescaler before enabling the timer
	TPM0_SC |= 3;					// prescaler for TPM0 (Motor) is 8
	TPM1_SC |= 3;					// prescaler for TPM1 (Servo) is 8
	TPM2_SC |= 6; 					// Prescale Factor Selection
	
	// TPM modulo register, set frequency
	TPM0_MOD = 600;					// modulo TPM0 (Motor), periode = 0.10 ms (10000 Hz)
	TPM1_MOD = 60000;				// modulo TPM1 (Servo), periode = 10 ms (100 Hz)
	TPM2_MOD = 60000;				// modulo TPM2
	
	// set TPM clock mode to enable timer
	TPM0_SC |= TPM_SC_CMOD(1);		// enable TPM0 (Motor)
	TPM1_SC |= TPM_SC_CMOD(1);		// enable TPM1 (Servo)
	TPM2_SC |= TPM_SC_CMOD(1);		// enable TPM2
	
	
	// configure TPM channel outputs high-true pulses
	TPM0_C1SC = 0x28;				// TPM0 channel1 Motor 1 In 1 speed left
	TPM0_C5SC = 0x28;				// TPM0 channel5 Motor 2 In 2 speed right
	TPM1_C0SC = 0x28;				// TPM1 channel0 Servo 1
	
	// TPM channel value registers, sets duty cycle
	// TPM1_C0V = servo_base;				// TPM1 channel0 Servo 1 ca. 1.5 ms (middle)
	
    // initial configuration of motors
    TPM0_C1V = 150;					// TPM0 channel1 left Motor 1 In 1 slow forward
	TPM0_C5V = 150;					// TPM0 channel5 right Motor 2 In 2 slow forward
	GPIOA_PDOR &= ~(1<<5);			// Set PTA5 left Motor 1 In 2 forward
	GPIOC_PDOR &= ~(1<<8);			// Set PTC8 right Motor 2 In 1 forward
	
    // configure interrupts in TPM1
	TPM1_SC |= TPM_SC_TOIE_MASK;	// enable overflow interrupt in TPM1 (10 ms rate)
	TPM2_SC |= TPM_SC_TOIE_MASK;	//Enable interrupt when overflow
	
	// ADC0 clock configuration
	ADC0_CFG1 |= 0x01;				// clock is bus clock divided by 2 = 12 MHz
	
	// ADC0 resolution    
	ADC0_CFG1 |= 0x08;				// resolution 10 bit, max. value is 1023

	// ADC0 conversion mode
	ADC0_SC3 = 0x00;				// single conversion mode
	    
	/************************* ADD-ON BLUETOOTH CONFIGURATION ***********************************/
	//UART0_Bluetooth_Init();
	UART1_Bluetooth_Init();
	UART2_Bluetooth_Init();
	
	color_counter = 0;
	
	read_data_position = 0;
	write_data_position = 0;
	end_read_data_position = 0;
	end_write_data_position = 0;
	
	for (i = 0; i < 128; ++i) debug_data.data.camera_data_1[i] = 'a';
	for (i = 0; i < 128; ++i) debug_data.data.camera_data_2[i] = ' ';
	
	debug_data.data.A0 = 2748103;
	debug_data.data.B1 = 51.551;
	debug_data.data.C2 = 199605818;
	debug_data.data.D3 = -96527697.54512;
	debug_data.data.E4 = 25442;
	debug_data.data.F5 = 4537.75456;
	debug_data.data.G6 = 2000000000;
	debug_data.data.H7 = 887653.75445;
	debug_data.data.I8 = -451364;
	debug_data.data.J9 = 12753.44574;
	debug_data.data.K10 = 4377316;
	debug_data.data.L11 = 7567534378.7578637;
	debug_data.data.M12 = -7754646;
	debug_data.data.N13 = -7843753378412.7545;
	debug_data.data.O14 = -472577;
	debug_data.data.P15 = -0.0054813;
	debug_data.data.Q16 = -973531;
	debug_data.data.R17 = -181315.1113;
	debug_data.data.S18 = 9797635;
	debug_data.data.T19 = 0.000511;
	
	debug_data.data.CONTROL_0 = 5;
	debug_data.data.CONTROL_1 = 7;
	debug_data.data.CONTROL_2 = -9;
	debug_data.data.CONTROL_3 = 2;
	debug_data.data.CONTROL_4 = 3;
	debug_data.data.CONTROL_5 = 5.5;
	debug_data.data.CONTROL_6 = 6.256;
	debug_data.data.CONTROL_7 = -70.864;
	debug_data.data.CONTROL_8 = 59.64;
	debug_data.data.CONTROL_9 = -100.25;
	
	blink = 0;
	/************************* ADD-ON BLUETOOTH CONFIGURATION ***********************************/
	
    // enable interrupts 18 (TPM = FTM1)  in NVIC, no interrupt levels (table 3.7, page 53)
    // interrupt vector on TPM1 at IRQ18
	NVIC_ICPR |= (1 << 18);			// clear pending interrupt 18
    NVIC_ISER |= (1 << 18);			// enable interrupt 18
    // interrupt vector on TPM1 at IRQ19
    NVIC_ICPR |= (1 << 19);			// clear pending interrupt 19
    NVIC_ISER |= (1 << 19);			// enable interrupt 19
        
    // enable interrupts globally
    asm (" CPSIE i ");				// enable interrupts on core level
    
    // Main loop
    for(;;) {						// endless loop
    	/*
    	if (UART1_S1 & UART_S1_RDRF_MASK) {
    		switchLed(1,0,0);
    		buffer = UART1_D;
    		UART_Bluetooth_Send(buffer); // resend the data back via bluetooth
    	}
    	*/
	}								// end of endless loop	
}

// switch LED for ez debug
void switchLed(int red, int green, int blue) {
	GPIOB_PDOR |= GPIO_PDOR_PDO(1<<18);   // red LED off
	GPIOB_PDOR |= GPIO_PDOR_PDO(1<<19);   // green LED off
	GPIOD_PDOR |= GPIO_PDOR_PDO(1<<1);    // blue LED off
	if(red == 1) {
		GPIOB_PDOR &= ~GPIO_PDOR_PDO(1<<18);  // red LED on
	}
	if (green == 1) {
		GPIOB_PDOR &= ~GPIO_PDOR_PDO(1<<19);  // green LED on
	}	
	if (blue == 1) {
		GPIOD_PDOR &= ~GPIO_PDOR_PDO(1<<1);   // blue LED on
	}
}

void FTM1_IRQHandler()				// TPM1 ISR
{
	//switchLed(0,0,0);
	TPM1_SC |= 0x80;	
}

void FTM2_IRQHandler()				// TPM2 ISR
{
	//UART0_Bluetooth_Send('a');
	debug_data.data.A0 = debug_data.data.CONTROL_1 + debug_data.data.CONTROL_3;
	
	if (blink && color_counter == 0) {
		switchLed(1,0,0);
		blink = 0;
	}else if (blink && color_counter == 1) {
		switchLed(0,1,0);
		blink = 0;
	}else if (blink && color_counter == 2) {
		switchLed(0,0,1);
		blink = 0;
	}else if (blink && color_counter == 3) {
		switchLed(1,1,0);
		blink = 0;
	}else if (blink && color_counter == 4) {
		switchLed(0,1,1);
		blink = 0;
	}else if (blink && color_counter == 5) {
		switchLed(1,0,1);
		blink = 0;
	}else if (blink && color_counter == 6) {
		switchLed(1,1,1);
		blink = 0;
	}else {
		switchLed(0,0,0);
		blink = 1;
	}
	++color_counter;
	if (color_counter == 7) color_counter = 0;
	
	TPM2_SC |= 0x80;	
}

void UART1_IRQHandler() 
{
	unsigned char register_status = UART1_S1;
	unsigned char buffer = UART1_D; // read from recv buffer of UART module
	
	// receive command from app
	if (register_status & UART_S1_RDRF_MASK && end_write_data_position == write_data_position) {
		// enable TC for continuous send operation since most of these will be followed by sum more transmission to app
		UART1_C2 |= UART_C2_TCIE_MASK;
		
		// main logic for handling data read
		/******************************** reset request **************************************/
		if (buffer == 0xFF) {   
			read_data_position = 0;
			end_read_data_position = 0;
			write_data_position = 0;
			end_write_data_position = 0;
			UART1_Bluetooth_Send((unsigned char)0xFF); 
			UART1_C2 &= ~UART_C2_TCIE_MASK;  // no next transmission pending
		}/******************************** camera 1 request **************************************/
		else if (buffer == 0xFE) {  
			read_data_position = 0;
			end_read_data_position = read_data_position + 128;
			UART1_Bluetooth_Send((unsigned char)debug_data.byte_block[read_data_position++]);
		}/******************************** camera 2 request **************************************/
		else if (buffer == 0xFD) {  
			read_data_position = 128;
			end_read_data_position = read_data_position + 128;
			UART1_Bluetooth_Send((unsigned char)debug_data.byte_block[read_data_position++]);
		}/************************* monitoring variable request **********************************/
		else if (buffer >= 0x00 && buffer <= 0x13) { 
			read_data_position = 256 + (unsigned int)buffer*4;
			end_read_data_position = read_data_position + 4;
			UART1_Bluetooth_Send((unsigned char)debug_data.byte_block[read_data_position++]);
		}/******************************** state variable READ request ***************************/
		else if ((buffer&0xC0)>>6 == 1) {
			read_data_position = 336 + (unsigned int)(buffer&0x3F)*4;
			end_read_data_position = read_data_position + 4;
			UART1_Bluetooth_Send((unsigned char)debug_data.byte_block[read_data_position++]);
		}/******************************** state variable WRITE request ***************************/ 
		else if ((buffer&0xC0)>>6 == 2) {
			write_data_position = 336 + (unsigned int)(buffer&0x3F)*4;
			end_write_data_position =  write_data_position + 4;
		}
	} 
	// receive data but in middle of receiving data from app
	else if (register_status & UART_S1_RDRF_MASK && write_data_position < end_write_data_position) {
		debug_data.byte_block[write_data_position++] = buffer;
	}
	// transmission complete and there are pending transmission to come
	else if (register_status & UART_S1_TC_MASK) {
		if (read_data_position < end_read_data_position) {
			UART1_D = ((unsigned char)debug_data.byte_block[read_data_position++]);	
		}
		if (read_data_position == end_read_data_position) UART1_C2 &= ~UART_C2_TCIE_MASK; // disable TC to receive data
	}
	// doesn't actually make any difference
	else { 
		UART1_S1 = 0;
	}
}

void UART0_IRQHandler() 
{
	unsigned char register_status = UART0_S1;
	unsigned char buffer = UART0_D; // read from recv buffer of UART module
	
	// receive command from app
	if (register_status & UART_S1_RDRF_MASK && end_write_data_position == write_data_position) {
		// enable TC for continuous send operation since most of these will be followed by sum more transmission to app
		UART0_C2 |= UART_C2_TCIE_MASK;
		
		// main logic for handling data read
		/******************************** reset request **************************************/
		if (buffer == 0xFF) {   
			read_data_position = 0;
			end_read_data_position = 0;
			write_data_position = 0;
			end_write_data_position = 0;
			UART0_Bluetooth_Send((unsigned char)0xFF); 
			UART0_C2 &= ~UART_C2_TCIE_MASK;  // no next transmission pending
		}/******************************** camera 1 request **************************************/
		else if (buffer == 0xFE) {  
			read_data_position = 0;
			end_read_data_position = read_data_position + 128;
			UART0_Bluetooth_Send((unsigned char)debug_data.byte_block[read_data_position++]);
		}/******************************** camera 2 request **************************************/
		else if (buffer == 0xFD) {  
			read_data_position = 128;
			end_read_data_position = read_data_position + 128;
			UART0_Bluetooth_Send((unsigned char)debug_data.byte_block[read_data_position++]);
		}/************************* monitoring variable request **********************************/
		else if (buffer >= 0x00 && buffer <= 0x13) { 
			read_data_position = 256 + (unsigned int)buffer*4;
			end_read_data_position = read_data_position + 4;
			UART0_Bluetooth_Send((unsigned char)debug_data.byte_block[read_data_position++]);
		}/******************************** state variable READ request ***************************/
		else if ((buffer&0xC0)>>6 == 1) {
			read_data_position = 336 + (unsigned int)(buffer&0x3F)*4;
			end_read_data_position = read_data_position + 4;
			UART0_Bluetooth_Send((unsigned char)debug_data.byte_block[read_data_position++]);
		}/******************************** state variable WRITE request ***************************/ 
		else if ((buffer&0xC0)>>6 == 2) {
			write_data_position = 336 + (unsigned int)(buffer&0x3F)*4;
			end_write_data_position =  write_data_position + 4;
		}
	} 
	// receive data but in middle of receiving data from app
	else if (register_status & UART_S1_RDRF_MASK && write_data_position < end_write_data_position) {
		debug_data.byte_block[write_data_position++] = buffer;
	}
	// transmission complete and there are pending transmission to come
	else if (register_status & UART_S1_TC_MASK) {
		if (read_data_position < end_read_data_position) {
			UART0_D = ((unsigned char)debug_data.byte_block[read_data_position++]);	
		}
		if (read_data_position == end_read_data_position) UART0_C2 &= ~UART_C2_TCIE_MASK; // disable TC to receive data
	}
	// doesn't actually make any difference
	else { 
		UART0_S1 = 0;
	}
}

void UART2_IRQHandler() 
{
	unsigned char register_status = UART2_S1;
	unsigned char buffer = UART2_D; // read from recv buffer of UART module
	
	// receive command from app
	if (register_status & UART_S1_RDRF_MASK && end_write_data_position == write_data_position) {
		// enable TC for continuous send operation since most of these will be followed by sum more transmission to app
		UART2_C2 |= UART_C2_TCIE_MASK;
		
		// main logic for handling data read
		/******************************** reset request **************************************/
		if (buffer == 0xFF) {   
			read_data_position = 0;
			end_read_data_position = 0;
			write_data_position = 0;
			end_write_data_position = 0;
			UART2_Bluetooth_Send((unsigned char)0xFF); 
			UART2_C2 &= ~UART_C2_TCIE_MASK;  // no next transmission pending
		}/******************************** camera 1 request **************************************/
		else if (buffer == 0xFE) {  
			read_data_position = 0;
			end_read_data_position = read_data_position + 128;
			UART2_Bluetooth_Send((unsigned char)debug_data.byte_block[read_data_position++]);
		}/******************************** camera 2 request **************************************/
		else if (buffer == 0xFD) {  
			read_data_position = 128;
			end_read_data_position = read_data_position + 128;
			UART2_Bluetooth_Send((unsigned char)debug_data.byte_block[read_data_position++]);
		}/************************* monitoring variable request **********************************/
		else if (buffer >= 0x00 && buffer <= 0x13) { 
			read_data_position = 256 + (unsigned int)buffer*4;
			end_read_data_position = read_data_position + 4;
			UART2_Bluetooth_Send((unsigned char)debug_data.byte_block[read_data_position++]);
		}/******************************** state variable READ request ***************************/
		else if ((buffer&0xC0)>>6 == 1) {
			read_data_position = 336 + (unsigned int)(buffer&0x3F)*4;
			end_read_data_position = read_data_position + 4;
			UART2_Bluetooth_Send((unsigned char)debug_data.byte_block[read_data_position++]);
		}/******************************** state variable WRITE request ***************************/ 
		else if ((buffer&0xC0)>>6 == 2) {
			write_data_position = 336 + (unsigned int)(buffer&0x3F)*4;
			end_write_data_position =  write_data_position + 4;
		}
	} 
	// receive data but in middle of receiving data from app
	else if (register_status & UART_S1_RDRF_MASK && write_data_position < end_write_data_position) {
		debug_data.byte_block[write_data_position++] = buffer;
	}
	// transmission complete and there are pending transmission to come
	else if (register_status & UART_S1_TC_MASK) {
		if (read_data_position < end_read_data_position) {
			UART2_D = ((unsigned char)debug_data.byte_block[read_data_position++]);	
		}
		if (read_data_position == end_read_data_position) UART2_C2 &= ~UART_C2_TCIE_MASK; // disable TC to receive data
	}
	// doesn't actually make any difference
	else { 
		UART2_S1 = 0;
	}
}
