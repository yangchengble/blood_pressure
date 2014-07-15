/**
 ****************************************************************************************
 *
 * @file uart.h
 *
 * @brief uart initialization and print functions header file.
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
 
#include <stdint.h>

#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

void uart_initialization(void);
void uart_test(void);

char uart_receive_byte(void);
void uart_send_byte(char ch);
void printf_byte(int a);
void printf_string(char * str);


/*
 * ENUMERATION DEFINITIONS
 *****************************************************************************************
 */

 /// Baudrates
enum{
  /// Divider for 115200 bits/s
  UART_BAUDRATE_115K2 =  9,
	/// Divider for 57600 bits/s
  UART_BAUDRATE_57K6  = 17,
	/// Divider for 38400 bits/s
  UART_BAUDRATE_38K4  = 26,
	/// Divider for 28800 bits/s
  UART_BAUDRATE_28K8  = 35,
	/// Divider for 19200 bits/s
  UART_BAUDRATE_19K2  = 52,
	/// Divider for 14400 bits/s
  UART_BAUDRATE_14K4  = 69,
	/// Divider for 9600 bits/s
  UART_BAUDRATE_9K6   = 104,		
};

/// Character format
enum
{
    /// data length 5 bits
    UART_DATALENGTH_5 = 0,
    /// data length 6 bits
    UART_DATALENGTH_6 = 1,
    /// data length 7 bits
    UART_DATALENGTH_7 = 2,
    /// data length 8 bits
    UART_DATALENGTH_8 = 3
};


/// Parity
enum
{
    /// no parity
    UART_PARITY_NONE     = 0,
    /// even parity
    UART_PARITYBIT_EVEN  = 1,
    /// odd parity
    UART_PARITYBIT_ODD   = 3,
};


/// Stop bit
enum
{
    /// stop bit 1
    UART_STOPBITS_1 = 0,
    /* Note: The number of stop bits is 1.5 if a
     * character format with 5 bit is chosen */
    /// stop bit 2
    UART_STOPBITS_2 = 1
};


/// Flow control
enum
{
    /// disabled auto flow control
    UART_FLOWCONTROL_DISABLED,
    /// enabled auto flow control
    UART_FLOWCONTROL_ENABLED,
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
 
  /**
 ****************************************************************************************
 * @brief prints a (16-bit) half-word in hex format using printf_byte
 * @param aHalfWord The 16-bit half-word to print
  * 
 ****************************************************************************************
 */
void print_hword(uint16_t aHalfWord);

 /**
 ****************************************************************************************
 * @brief prints a (32-bit) word in hex format using printf_byte
 * @param aWord The 32-bit word to print
  * 
 ****************************************************************************************
 */
void print_word(uint32_t aWord);

 
  
#endif
