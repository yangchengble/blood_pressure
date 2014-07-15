/**
 ****************************************************************************************
 *
 * @file peripherals.c
 *
 * @brief Peripherals initialization fucntions
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
#include "uart.h"
#include "gpio.h"
#include <core_cm0.h>
#include "peripherals.h"
#include "periph_setup.h"
#include "i2c_eeprom.h"


  
 /**
 ****************************************************************************************
 * @brief Enable pad's and peripheral clocks assuming that peripherals' power domain is down.
 *        The Uart and SPi clocks are set. 
 * 
 ****************************************************************************************
 */
void periph_init(void)  // set i2c, spi, uart, uart2 serial clks
{
    // system init
	SetWord16(CLK_AMBA_REG, 0x00); 				// set clocks (hclk and pclk ) 16MHz
	SetWord16(SET_FREEZE_REG,FRZ_WDOG);			// stop watch dog	
	SetBits16(SYS_CTRL_REG,PAD_LATCH_EN,1);  	// open pads
	SetBits16(SYS_CTRL_REG,DEBUGGER_ENABLE,1);  // open debugger
	SetBits16(PMU_CTRL_REG, PERIPH_SLEEP,0);  	// exit peripheral power down
	// Power up peripherals' power domain
    SetBits16(PMU_CTRL_REG, PERIPH_SLEEP, 0);
    while (!(GetWord16(SYS_STAT_REG) & PER_IS_UP));  
	// Initialize UART component
#ifdef UART_ENABLED
      uart_initialization();
#endif
  
	//Init pads
	set_pad_functions();
     
} 
 
 /**
 ****************************************************************************************
 * @brief set gpio port function mode
  * 
 ****************************************************************************************
 */
void set_pad_functions(void)       
{
#ifdef UART_ENABLED
	 GPIO_ConfigurePin( UART_GPIO_PORT, UART_TX_PIN, OUTPUT, PID_UART1_TX, false );
     GPIO_ConfigurePin( UART_GPIO_PORT, UART_RX_PIN, INPUT, PID_UART1_RX, false );    
	 
#endif
	
#ifdef SPI_ENABLED  
   GPIO_ConfigurePin( SPI_GPIO_PORT, SPI_CS_PIN, OUTPUT, PID_SPI_EN, true );
   GPIO_ConfigurePin( SPI_GPIO_PORT, SPI_CLK_PIN, OUTPUT, PID_SPI_CLK, false );
   GPIO_ConfigurePin( SPI_GPIO_PORT, SPI_DO_PIN, OUTPUT, PID_SPI_DO, false );	
   GPIO_ConfigurePin( SPI_GPIO_PORT, SPI_DI_PIN, INPUT, PID_SPI_DI, false );
#endif
	
#ifdef EEPROM_ENABLED    
   GPIO_ConfigurePin(I2C_GPIO_PORT, I2C_SCL_PIN, INPUT, PID_I2C_SCL, false);
   GPIO_ConfigurePin(I2C_GPIO_PORT, I2C_SDA_PIN, INPUT, PID_I2C_SDA,false);
#endif
    
#ifdef QUADEC_ENABLED
    GPIO_ConfigurePin( QUADRATURE_ENCODER_CHX_A_PORT, QUADRATURE_ENCODER_CHX_A_PIN, INPUT_PULLUP, PID_GPIO, true);   
    GPIO_ConfigurePin( QUADRATURE_ENCODER_CHX_B_PORT, QUADRATURE_ENCODER_CHX_B_PIN, INPUT_PULLUP, PID_GPIO, true);    
    GPIO_ConfigurePin( WKUP_TEST_BUTTON_1_PORT, WKUP_TEST_BUTTON_1_PIN, INPUT_PULLUP, PID_GPIO, true);   
    GPIO_ConfigurePin( WKUP_TEST_BUTTON_2_PORT, WKUP_TEST_BUTTON_2_PIN, INPUT_PULLUP, PID_GPIO, true);   
#endif   

#ifdef BUZZER_ENABLED
    GPIO_ConfigurePin(PWM0_PORT, PWM0_PIN, OUTPUT, PID_PWM0, true);   
    GPIO_ConfigurePin(PWM1_PORT, PWM1_PIN, OUTPUT, PID_PWM1, true); 
    GPIO_ConfigurePin(PWM2_PORT, PWM2_PIN, OUTPUT, PID_PWM2, true);   
    GPIO_ConfigurePin(PWM3_PORT, PWM3_PIN, OUTPUT, PID_PWM3, true);  
    GPIO_ConfigurePin(PWM4_PORT, PWM4_PIN, OUTPUT, PID_PWM4, true);   
#endif   

#ifdef SW_CURSOR
    GPIO_ConfigurePin(SW_CURSOR_PORT, SW_CURSOR_PIN, OUTPUT, PID_GPIO, true);    
#endif
} 

