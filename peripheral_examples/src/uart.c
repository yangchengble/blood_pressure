/**
 ****************************************************************************************
 *
 * @file uart.c
 *
 * @brief (Do not use for your designs) - (legacy) uart initialization & print functions
 *        Please, refer to the Peripheral Drivers documentation for the current uart.c driver
 *
 * Copyright (C) 2012. Dialog Semiconductor Ltd, unpublished work. This computer 
 * program includes Confidential, Proprietary Information and is a Trade Secret of 
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited 
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */
 
#include "global_io.h"
#include "gpio.h"
#include <core_cm0.h>
#include "peripherals.h"
#include "uart.h"
#include "periph_setup.h" 
 

void uart_initialization(void)
{
	SetBits16(CLK_PER_REG, UART1_ENABLE, 1);  	// enable  clock for UART 1

	SetWord16(UART_LCR_REG, 0x80); // set bit to access DLH and DLL register
	SetWord16(UART_IER_DLH_REG,(UART_BAUDRATE_115K2&0xFF>>8));//set high byte
	SetWord16(UART_RBR_THR_DLL_REG,UART_BAUDRATE_115K2&0xFF);//set low byte
	SetWord16(UART_LCR_REG,UART_DATALENGTH|UART_PARITY|UART_STOPBITS);
	SetBits16(UART_MCR_REG, UART_SIRE, 0);  // mode 0 for normal , 1 for IRDA
	SetWord16(UART_IIR_FCR_REG,1);  // enable fifo  
	SetBits16(UART_IER_DLH_REG,ERBFI_dlh0,0);  // IER access, disable interrupt for available data
}
void uart_test(void){
		printf_string("\n\r\n\r*************");
		printf_string("\n\r* UART TEST *\n\r");
		printf_string("*************\n\r");
		printf_string("\n\rHello World! == UART printf_string() ==.\n\r");
}
char uart_receive_byte(void){
	#ifndef UART_ENABLED
		return 0;
#else
	do{
	}while((GetWord16(UART_LSR_REG)&0x01)==0);	// wait until serial data is ready 
	return 0xFF&GetWord16(UART_RBR_THR_DLL_REG);	// read from receive data buffer
#endif
}

void uart_send_byte(char ch){
	#ifndef UART_ENABLED
		return ;
#else
	while((GetWord16(UART_LSR_REG)&0x20)==0);	// read status reg to check if THR is empty
	SetWord16(UART_RBR_THR_DLL_REG,(0xFF&ch)); // write to THR register
	return;
	#endif
}

void printf_string(char * str){
	#ifndef UART_ENABLED
		return ;
#else

	while(*str!=0){	      // while not reach the last string character
		uart_send_byte(*str); // send next string character
		str++;
	}
	#endif
}

void printf_byte(int a){		  // print a Byte in hex format
	#ifndef UART_ENABLED
		return ;
#else

	char b;
	b=((0xF0&a)>>4);
	b+= (b<10)?48:55;
	uart_send_byte(b);
	b=(0xF&a);
	b+= (b<10)?48:55;
	uart_send_byte(b);
		#endif
}


 /**
 ****************************************************************************************
 * @brief prints a (16-bit) half-word in hex format using printf_byte
 * @param aHalfWord The 16-bit half-word to print
  * 
 ****************************************************************************************
 */

void print_hword(uint16_t aHalfWord)
{
    printf_byte((aHalfWord >> 8)& 0xFF);
    printf_byte((aHalfWord)& 0xFF);      
}


 /**
 ****************************************************************************************
 * @brief prints a (32-bit) word in hex format using printf_byte
 * @param aHalfWord The 32-bit word to print
  * 
 ****************************************************************************************
 */

void print_word(uint32_t aWord)
{
    printf_byte((aWord >> 24)& 0xFF);
    printf_byte((aWord >> 16)& 0xFF);
    printf_byte((aWord >> 8)& 0xFF);
    printf_byte((aWord)& 0xFF);      
}

