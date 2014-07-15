//*** <<< Use Configuration Wizard in Context Menu >>> ***

 /**
 ****************************************************************************************
 *
 * @file periph_setup.h
 *
 * @brief Peripheral Setup file. GPIO assignment, SPI, SPI Flash, I2C, EEPROM configuration settings, Example selection
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
#include "i2c_eeprom.h"
#include "spi.h"


/* Test Definition - Select the Configuration that matches your H/W setup
*---------------------------------------------------------
* (0) SPI FLASH MEMORY WITH UART
* (1) I2C EEPROM WITH UART
* (2) QUADRATURE ENCODER WITH BUZZER AND UART
*---------------------------------------------------------
*/ 
// <o> Hardware Configuration <0=> SPI Flash with UART <1=> I2C EEPROM with UART <2=> Quadrature Encoder, Timers & Buzzer with UART
#define HARDWARE_CONFIGURATION_INDEX (2)

#if HARDWARE_CONFIGURATION_INDEX == 0
 #define SPI_FLASH_WITH_UART_EXAMPLE
#elif HARDWARE_CONFIGURATION_INDEX == 1
 #define I2C_EEPROM_WITH_UART_EXAMPLE
#elif HARDWARE_CONFIGURATION_INDEX == 2
 #define QUADRATURE_ENCODER_WITH_BUZZER_AND_UART_EXAMPLE
#endif


// Wakeup timer
#define WKUP_ENABLED


// Select UART settings
#define UART_BAUDRATE     UART_BAUDRATE_115K2       // Baudrate in bits/s: {9K6, 14K4, 19K2, 28K8, 38K4, 57K6, 115K2}
#define UART_DATALENGTH   UART_DATALENGTH_8         // Datalength in bits: {5, 6, 7, 8}
#define UART_PARITY       UART_PARITY_NONE          // Parity: {UART_PARITY_NONE, UART_PARITY_EVEN, UART_PARITY_ODD}
#define UART_STOPBITS     UART_STOPBITS_1           // Stop bits: {UART_STOPBITS_1, UART_STOPBITS_2} 
#define UART_FLOWCONTROL  UART_FLOWCONTROL_DISABLED // Flow control: {UART_FLOWCONTROL_DISABLED, UART_FLOWCONTROL_ENABLED}


// Select EEPROM characteristics
#define I2C_EEPROM_SIZE   0x20000         // EEPROM size in bytes
#define I2C_EEPROM_PAGE   256             // EEPROM's page size in bytes
#define I2C_SLAVE_ADDRESS 0x50            // Set slave device address
#define I2C_SPEED_MODE    I2C_FAST        // Speed mode: I2C_STANDARD (100 kbits/s), I2C_FAST (400 kbits/s)
#define I2C_ADDRESS_MODE  I2C_7BIT_ADDR   // Addressing mode: {I2C_7BIT_ADDR, I2C_10BIT_ADDR}
#define I2C_ADDRESS_SIZE  I2C_2BYTES_ADDR // Address width: {I2C_1BYTE_ADDR, I2C_2BYTES_ADDR, I2C_2BYTES_ADDR}


// SPI Flash options
#define SPI_FLASH_SIZE 131072             // SPI Flash memory size in bytes
#define SPI_FLASH_PAGE 256                // SPI Flash memory page size in bytes
//SPI initialization parameters
#define SPI_WORD_MODE SPI_8BIT_MODE       // Select SPI bit mode
#define SPI_SMN_MODE  SPI_MASTER_MODE     // {SPI_MASTER_MODE, SPI_SLAVE_MODE}
#define SPI_POL_MODE  SPI_CLK_INIT_HIGH   // {SPI_CLK_INIT_LOW, SPI_CLK_INIT_HIGH}
#define SPI_PHA_MODE  SPI_PHASE_1         // {SPI_PHA_MODE_0, SPI_PHA_MODE_1}
#define SPI_MINT_EN   SPI_NO_MINT         // {SPI_MINT_DISABLE, SPI_MINT_ENABLE}
#define SPI_CLK_DIV   SPI_XTAL_DIV_2      // Select SPI clock divider between 8, 4, 2 and 14


// Software cursor enable
#define SW_CURSOR

//GPIO settings

#ifdef SPI_FLASH_WITH_UART_EXAMPLE // hardware modification needed due to confict between UART_RX_PIN and SPI_DI_PIN. SPI_DI_PIN to change to  GPIO_PIN_7
        #define UART_GPIO_PORT  GPIO_PORT_0
        #define UART_TX_PIN     GPIO_PIN_4
        #define UART_RX_PIN     GPIO_PIN_7
				
		#define SPI_GPIO_PORT  GPIO_PORT_0 
        #define SPI_CLK_PIN    GPIO_PIN_0
        #define SPI_CS_PIN     GPIO_PIN_3
        #define SPI_DI_PIN     GPIO_PIN_5
        #define SPI_DO_PIN     GPIO_PIN_6                 

 
		#define UART_ENABLED	
		#define SPI_ENABLED	
		#undef  EEPROM_ENABLED	
#endif 


#ifdef I2C_EEPROM_WITH_UART_EXAMPLE			 
		#define UART_GPIO_PORT  GPIO_PORT_0
        #define UART_TX_PIN     GPIO_PIN_4
        #define UART_RX_PIN     GPIO_PIN_5

		#define I2C_GPIO_PORT   GPIO_PORT_0
		#define I2C_SCL_PIN     GPIO_PIN_2
        #define I2C_SDA_PIN     GPIO_PIN_3

		#define UART_ENABLED	
		#undef  SPI_ENABLED	
		#define  EEPROM_ENABLED	
#endif   


#ifdef QUADRATURE_ENCODER_WITH_BUZZER_AND_UART_EXAMPLE
        #define UART_GPIO_PORT  GPIO_PORT_0
        #define UART_TX_PIN     GPIO_PIN_4
        #define UART_RX_PIN     GPIO_PIN_5
		#define UART_ENABLED	
      
         // Quadrature Decoder options
        #define QUADRATURE_ENCODER_CHX_A_PORT GPIO_PORT_0
        #define QUADRATURE_ENCODER_CHX_A_PIN  GPIO_PIN_0
        #define QUADRATURE_ENCODER_CHX_B_PORT GPIO_PORT_0        
        #define QUADRATURE_ENCODER_CHX_B_PIN  GPIO_PIN_1 
        #define QUADRATURE_ENCODER_CHX_CONFIGURATION QUAD_DEC_CHXA_P00_AND_CHXB_P01
        #define QUADRATURE_ENCODER_CHY_CONFIGURATION QUAD_DEC_CHYA_NONE_AND_CHYB_NONE
        #define QUADRATURE_ENCODER_CHZ_CONFIGURATION QUAD_DEC_CHZA_NONE_AND_CHZB_NONE
        #define WKUP_TEST_BUTTON_1_PORT GPIO_PORT_0
        #define WKUP_TEST_BUTTON_1_PIN GPIO_PIN_6
        #define WKUP_TEST_BUTTON_2_PORT GPIO_PORT_1
        #define WKUP_TEST_BUTTON_2_PIN GPIO_PIN_1  
        #define QDEC_CLOCK_DIVIDER (1)
        #define QDEC_EVENTS_COUNT_TO_INT (1)
        #define QUADEC_ENABLED   
        
        #define BUZZER_ENABLED
#endif 


#ifndef SPI_CS_PIN
        #define SPI_GPIO_PORT  GPIO_PORT_0
        #define SPI_CS_PIN     GPIO_PIN_0                           
#endif //SPI_CS_PIN

#ifdef BUZZER_ENABLED
        // Buzzer options
        #define PWM0_PORT GPIO_PORT_0
        #define PWM0_PIN GPIO_PIN_2
        #define PWM1_PORT GPIO_PORT_0
        #define PWM1_PIN GPIO_PIN_3
        #define PWM2_PORT GPIO_PORT_0
        #define PWM2_PIN GPIO_PIN_7
        #define PWM3_PORT GPIO_PORT_1
        #define PWM3_PIN GPIO_PIN_0
        #define PWM4_PORT GPIO_PORT_1
        #define PWM4_PIN GPIO_PIN_2              
#endif //BUZZER_ENABLED      

#ifdef SW_CURSOR
        // Software Cursor options
        #define SW_CURSOR_PORT GPIO_PORT_1
        #define SW_CURSOR_PIN GPIO_PIN_3
#endif
//*** <<< end of configuration section >>>    ***
