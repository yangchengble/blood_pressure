
 /*
 ****************************************************************************************
 *
 * @file pwm_test.c
 *
 * @brief PWM test functions
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
//#include <stdio.h>
#include "global_io.h"
#include "periph_setup.h"
#include "peripherals.h"
#include "uart.h"
#include "pwm_test.h"
#include "sw_cursor.h"

#define TIMER0_TEST_EXPIRATION_COUNTER_RELOAD_VALUE 50

const uint16_t notes[] = {1046,987,767,932,328,880,830,609,783,991,739,989,698,456,659,255,622,254,587,554,365,523,251,493,466,440};
uint16_t timer0_test_expiration_counter;
uint8_t timer2_has_started = false;


  /**
 ****************************************************************************************
 * @brief  PWM user callback function
  * 
 ****************************************************************************************
 */ 
void pwm0_user_callback_function(void)
{ 
    static uint8_t n=0;
    static uint8_t i = 0;  
    
    if (n==10)
    {
        n=0;
        timer0_set_pwm_on_counter(0xFFFF);

        timer0_set_pwm_high_counter(notes[i]/3*2);

        timer0_set_pwm_low_counter(notes[i]/3);

        printf_string("*");    
        
				set_sw_cursor();
			
        i = ++i%25;        
        
        if (timer0_test_expiration_counter)
            timer0_test_expiration_counter--;          
    }
    n++;
}

 
  /**
 ****************************************************************************************
 * @brief  PWM test function
  * 
 ****************************************************************************************
 */
void timer0_test(void)
{
    printf_string("\n\r\n\r***************");
	printf_string("\n\r* TIMER0 TEST *\n\r");
	printf_string("***************\n\r");
     
    //Enables TIMER0,TIMER2 clock
    set_tmr_enable(CLK_PER_REG_TMR_ENABLED);
    
    //Sets TIMER0,TIMER2 clock division factor to 8
    set_tmr_div(CLK_PER_REG_TMR_DIV_8);
    
    // initilalize PWM with the desired settings
    timer0_init(TIM0_CLK_FAST, PWM_MODE_ONE, TIM0_CLK_NO_DIV);
  
    // set pwm Timer0 On, Timer0 'high' and Timer0 'low' reload values
    timer0_set(1000, 500, 200);
    
    // register callback function for SWTIM_IRQn irq
    timer0_register_callback(pwm0_user_callback_function);
    
    // enable SWTIM_IRQn irq
    timer0_enable_irq();
    
    timer0_test_expiration_counter = TIMER0_TEST_EXPIRATION_COUNTER_RELOAD_VALUE;
       
    printf_string("\n\rTIMER0 starts!");
    printf_string("\n\rYou can hear the sound produced by PWM0 or PWM1");
    printf_string("\n\rif you attach a buzzer on pins P0_2 or P0_3 respectively.");
    printf_string("\n\rPlaying melody. Please wait...\n\r");
    
    // start pwm0
    timer0_start();

    while (timer0_test_expiration_counter);
    
    timer0_stop();
    
		set_sw_cursor();
		
    set_tmr_enable(CLK_PER_REG_TMR_DISABLED);   // Disable TIMER0, TIMER2 interrupts
    
    printf_string("\n\rTIMER0 stopped!\n\r");
    
} 

void timer2_test(void)
{
    printf_string("\n\r\n\r***************");
	printf_string("\n\r* TIMER2 TEST *\n\r");
	printf_string("***************\n\r");
    
    if (timer2_has_started == false)
    {       
        //Enables TIMER0,TIMER2 clock
        set_tmr_enable(CLK_PER_REG_TMR_ENABLED);
    
        //Sets TIMER0,TIMER2 clock division factor to 8
        set_tmr_div(CLK_PER_REG_TMR_DIV_8);
        
        // initilalize PWM with the desired settings (sw paused)
        timer2_init(HW_CAN_NOT_PAUSE_PWM_2_3_4, PWM_2_3_4_SW_PAUSE_ENABLED, 500);

        // note: timer2_set_pwm_frequency() is called from timer2_init()
        //       the function timer2_set_pwm_frequency() can be used to set the timer2 frequency individually
        
        // set pwm2 duty cycle
        timer2_set_pwm2_duty_cycle(100);
        
        // set pwm3 duty cycle
        timer2_set_pwm3_duty_cycle(250);
        
        // set pwm4 duty cycle
        timer2_set_pwm4_duty_cycle(400);
        
        // release sw pause to let pwm2,pwm3,pwm4 run
        timer2_set_sw_pause(PWM_2_3_4_SW_PAUSE_DISABLED);
        
        timer2_has_started = true;
        
        printf_string("\n\rTIMER2 started!");
        printf_string("\n\rYou can hear the sound produced by PWM2,3,4 if you attach a buzzer");
        printf_string(" on pins P0_7, P1_0 and P1_2 respectively.");
        printf_string("To stop the timer, call this test again.\n\r");
    }
    else
    {
        timer2_stop();
        set_tmr_enable(CLK_PER_REG_TMR_DISABLED);   // Disable TIMER0, TIMER2 interrupts
        timer2_has_started = false;
        printf_string("\n\rTIMER2 stopped!\n\r");
    }
} 
