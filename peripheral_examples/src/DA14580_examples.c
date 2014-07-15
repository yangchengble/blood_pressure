
 /*
 ****************************************************************************************
 *
 * @file DA14580_examples.c
 *
 * @brief DA14580 Peripheral Examples for DA14580 SDK
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
#include <stdio.h>
#include "global_io.h"

#include "DA14580_examples.h"
#include "peripherals.h"
#include "spi.h"
#include "spi_flash.h"
#include "i2c_eeprom.h"
#include "battery.h"
#include "spi_test.h"
#include "uart.h"
#include "eeprom_test.h"
#include "periph_setup.h"
#include "quad_decoder_test.h"
#include "pwm_test.h"
 
 /**
 ****************************************************************************************
 * @brief Battery Level Indication  example function 
  * 
 ****************************************************************************************
*/
void batt_test(void){
	
        printf_string("\n\r\n\r****************");
        printf_string("\n\r* BATTERY TEST *\n\r");
        printf_string("****************\n\r");
	
#if BATT_CR2032    
        printf_string("\n\rBattery type: CR2032");  
        printf_string("\n\rCurrent battery level (max.0xFF%): 0x");            
		printf_byte(battery_get_lvl(BATT_CR2032));  
        printf_string("% left");
#else
        printf_string("\n\rBattery type unknown");  
#endif

} 
 
 
 
/**
 ****************************************************************************************
 * @brief print menu function
  * 
 ****************************************************************************************
*/
void print_menu(void)
{
		printf_string("\n\r====================================\n\r");
		printf_string("= DA14580 Peripheral Examples Menu =\n\r");
		printf_string("\n\r====================================\n\r");    
		printf_string("u. UART Print String Example\n\r");
#ifdef SPI_ENABLED     
		printf_string("f. SPI Flash Memory Example\n\r");
#else
    	printf_string("f. (disabled in this build) SPI Flash Memory Example\n\r");
#endif //SPI_ENABLED    
#ifdef EEPROM_ENABLED        
		printf_string("e. I2C EEPROM Example\n\r");  
#else
		printf_string("e. (disabled in this build) I2C EEPROM Example\n\r");     
#endif //EEPROM_ENABLED     
#ifdef QUADEC_ENABLED
		printf_string("q. Quadrature Encoder Example\n\r");
#else
  		printf_string("q. (disabled in this build) Quadrature (Rotary Encoder) Example\n\r");  
#endif //QUADEC_ENABLED    
    
#ifdef BUZZER_ENABLED 
		printf_string("t. Timer0 (PWM0, PWM1) Example\n\r");
		printf_string("p. Timer2 (PWM2, PWM3, PWM4) Example\n\r");  
#else
 		printf_string("t. (disabled in this build) Timer0 (PWM0, PWM1) Example\n\r");
		printf_string("p. (disabled in this build) Timer2 (PWM2, PWM3, PWM4) Example\n\r");     
#endif //BUZZER_ENABLED    
    
        printf_string("b. Battery Example\n\r");
		printf_string("x. Exit\n\r\n\r");
		print_input();
}

/**
 ****************************************************************************************
 * @brief print input function
  * 
 ****************************************************************************************
*/
void print_input(void)
{
		printf_string("\n\rMake a choice : ");
}
/**
 ****************************************************************************************
 * @briefend of test option 
  * 
 ****************************************************************************************
*/
void endtest_bridge(short int *idx)
{
		short int index2 = 0;
		char bchoice;
		printf_string("\n\r\n\rPress m for Peripheral Examples Menu or x to exit : ");
		while(1){	
				if (index2 == 1) break;
				bchoice = uart_receive_byte();
				switch (bchoice){
						case 'm' : print_menu(); index2 = 1; break;
						case 'x' : (*idx)=1; index2 = 1; break;
						default : ; break;
				};
		};
}
 /**
 ****************************************************************************************
 * @briefend exit test
  * 
 ****************************************************************************************
*/
void exit_test(void)
{
		printf_string("\n\r  End of tests \n\r");
}
 
 /**
 ****************************************************************************************
 * @brief  Main routineof the DA14580 Peripheral Examples Functions
 * DA14580 Peripherals Udage Examples
 *        - UART
 *        - SPI Flash
 *        - Boot From SPI flash
 *        - EEPROM
 *        - Timers
 *        - Battery Level Indication - ADC
 *        - Quadrature
 *        - Buzzer
 * Due to HW Development Kit limitations, user must select one of the follwoing configuartion for UART, SPI, I2C.
 * Addicional Hardware (SPI, EEPROM boadrs) or Hardware modification may needed for some tof the tests. 
 * More information in the file periph_setup.h, Application Notes, and User Guide for DA14580
 *        - UART only  ( No HW modifications on rev C2 motherboard, No additional hardware)
 *        - SPI Flash with UART (HW modifications & additional Hardware needed , SPI_DI_PIN on the additional SPI / EEPROM daughterboard )
 *        - Boot From SPI Flash with UART  (HW modifications & additional Hardware needed, (UART TX) )
 *        - Boot From SPI Flash without UART (Additional Hardware needed)
 *        - Boot From EEPROM with UART (Additional Hardware needed)
 * 
 ****************************************************************************************
*/
int main (void)
{
	  short int index = 0;
	  char mchoice;
 
	  periph_init();
	  printf_string("\n\rDA14580 Peripheral Examples\n\r");
  	  printf_string(    "---------------------------\n\r");  
    printf_string("Before running the tests:\n\r");
    printf_string("  1) Make sure you have connected the appropriate peripheral(s).\n\r");
    printf_string("  2) Select the associated hardware configuration in 'periph_setup.h'.\n\r");
    printf_string("  3) Build.\n\r");
	printf_string("Please, refer to DA14580 Peripheral Examples User Manual\n\r");
    printf_string("for detailed instructions.\n\r");
    
	  print_menu();
	 	 
		while(1){
			if (index==1) break;
			mchoice = uart_receive_byte();
			switch (mchoice){
					case 'u': uart_test(); endtest_bridge(&index); break;
#ifdef SPI_ENABLED                 
					case 'f': spi_test(); endtest_bridge(&index); break;               
#endif //SPI_ENABLED
#ifdef EEPROM_ENABLED                
					case 'e': i2c_test(); endtest_bridge(&index); break;                
#endif //EEPROM_ENABLED                                
#ifdef QUADEC_ENABLED
					case 'q': quad_decoder_test(); endtest_bridge(&index); break;
#endif //QUADEC_ENABLED      

#ifdef BUZZER_ENABLED                
					case 't': timer0_test(); endtest_bridge(&index); break;
                    case 'p': timer2_test(); endtest_bridge(&index); break;        
#endif //BUZZER_ENABLED              
				  case 'b': batt_test(); endtest_bridge(&index); break;
				  case 'x': index=1;break;		
					default: print_input(); continue;
			};
		};
	
		printf_string("\n\r End of tests\n\r");
		while(1);
}
