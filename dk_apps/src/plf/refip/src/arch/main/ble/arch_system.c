/**
 ****************************************************************************************
 *
 * @file arch_system.c
 *
 * @brief System setup.
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


/*
 * INCLUDES
 ****************************************************************************************
 */

#include "arch.h"
#include "arch_sleep.h"
#include <stdlib.h>
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition
#include "rwip.h"     // BLE initialization
#include "llc.h"
#include "pll_vcocal_lut.h"
#include "gpio.h"
#include "rf_580.h"
#include "gtl_env.h"
#include "gtl_task.h"

/**
 * @addtogroup DRIVERS
 * @{
 */

#define LP_CLK_OTP_OFFSET 0x7f74     //OTP IQ_Trim offset

extern uint32_t last_temp_time;         // time of last temperature count measurement
extern uint16_t last_temp_count;        /// temperature counter

extern uint32_t lld_sleep_us_2_lpcycles_func(uint32_t us);
extern uint32_t lld_sleep_lpcycles_2_us_func(uint32_t lpcycles);

uint32_t lp_clk_sel __attribute__((section("retention_mem_area0"),zero_init));   //low power clock selection  
uint32_t rcx_freq __attribute__((section("retention_mem_area0"),zero_init));
uint8_t cal_enable  __attribute__((section("retention_mem_area0"),zero_init));
uint32_t rcx_period __attribute__((section("retention_mem_area0"),zero_init));
float rcx_slot_duration __attribute__((section("retention_mem_area0"),zero_init));;


//#define RCX_MEASURE
#ifdef RCX_MEASURE
uint32_t rcx_freq_min __attribute__((section("retention_mem_area0"),zero_init));
uint32_t rcx_freq_max __attribute__((section("retention_mem_area0"),zero_init));
uint32_t rcx_period_last __attribute__((section("retention_mem_area0"),zero_init));
uint32_t rcx_period_diff __attribute__((section("retention_mem_area0"),zero_init));
#endif

/*
 * EXPORTED FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Read low power clock selection from 
 *
 * The Hclk and Pclk are set
 ****************************************************************************************
*/

void select_lp_clk()
{

#if (CFG_LP_CLK == LP_CLK_FROM_OTP)    
    int cnt=100000;

#ifndef APP_BOOT_FROM_OTP    
#define XPMC_MODE_MREAD   0x1
    uint16_t *lp_clk = (uint16_t *)(0x40000 + LP_CLK_OTP_OFFSET);   //where in OTP header is IQ_Trim
    
    SetBits16(CLK_AMBA_REG, OTP_ENABLE, 1);		// enable OTP clock	
    while ((GetWord16(ANA_STATUS_REG) & LDO_OTP_OK) != LDO_OTP_OK && cnt--)
        /* Just wait */;
        
    // set OTP in read mode 
    SetWord32 (OTPC_MODE_REG,XPMC_MODE_MREAD);
#else
    uint16_t *lp_clk = (uint16_t *)(0x20000000 + LP_CLK_OTP_OFFSET);   //where in OTP header is IQ_Trim
#endif    

    lp_clk_sel  = (*lp_clk);
    SetBits16(CLK_AMBA_REG, OTP_ENABLE, 0);     //disable OTP clock     
    
#else //CFG_LP_CLK 
    lp_clk_sel = CFG_LP_CLK; 
#endif    
    
}

/**
 ****************************************************************************************
 * @brief Initialisation of ble core, pwr and clk
 *
 * The Hclk and Pclk are set
 ****************************************************************************************
 */
void init_pwr_and_clk_ble(void) 
{

    
    SetBits16(CLK_RADIO_REG, BLE_DIV, 0);
    SetBits16(CLK_RADIO_REG, BLE_ENABLE, 1); 
    SetBits16(CLK_RADIO_REG, RFCU_DIV, 1);
    SetBits16(CLK_RADIO_REG, RFCU_ENABLE, 1);

    /* 
     * Power up BLE core & reset BLE Timers
    */
    SetBits16(CLK_32K_REG,  RC32K_ENABLE, 1);  
    SetBits16(SYS_CTRL_REG, CLK32_SOURCE, 0);  
    SetBits16(CLK_RADIO_REG, BLE_LP_RESET, 1);  
    SetBits16(PMU_CTRL_REG, RADIO_SLEEP, 0);
    while (!(GetWord16(SYS_STAT_REG) & RAD_IS_UP)); // Just wait for radio to truely wake up
                                
    select_lp_clk();   
    if (lp_clk_sel == LP_CLK_XTAL32)
    {
        SetBits16(CLK_32K_REG, XTAL32K_ENABLE, 1);  // Enable XTAL32KHz 

        // Disable XTAL32 amplitude regulation in BOOST mode
        if (GetBits16(ANA_STATUS_REG, BOOST_SELECTED) == 0x1) 
            SetBits16(CLK_32K_REG,  XTAL32K_DISABLE_AMPREG, 1);  
        else
            SetBits16(CLK_32K_REG,  XTAL32K_DISABLE_AMPREG, 0);  
        SetBits16(CLK_32K_REG,  XTAL32K_CUR, 5);
        SetBits16(CLK_32K_REG,  XTAL32K_RBIAS, 3);
        SetBits16(SYS_CTRL_REG, CLK32_SOURCE, 1);  // Select XTAL32K as LP clock
        
     }
    else if (lp_clk_sel == LP_CLK_RCX20)
    {
        SetBits16(CLK_RCX20K_REG, RCX20K_NTC, 0xB);      
        SetBits16(CLK_RCX20K_REG, RCX20K_BIAS, 1);
        SetBits16(CLK_RCX20K_REG, RCX20K_TRIM, 0);
        SetBits16(CLK_RCX20K_REG, RCX20K_LOWF, 1);
                           
        SetBits16(CLK_RCX20K_REG, RCX20K_ENABLE, 1);
    
        SetBits16(CLK_RCX20K_REG, RCX20K_SELECT, 1);
    
        SetBits16(SYS_CTRL_REG, CLK32_SOURCE, 0);                                               
    }
    SetBits16(CLK_32K_REG,  RC32K_ENABLE, 0);   // Disable RC32KHz
                                
    SetBits16(CLK_RADIO_REG, BLE_LP_RESET, 0);
    
    if (GetBits16(ANA_STATUS_REG, BOOST_SELECTED) == 0x1) 
        SetWord16(DCDC_CTRL3_REG, 0x5);
    
    /* 
     * Just make sure that BLE core is stopped (if already running)
     */
    SetBits32(BLE_RWBTLECNTL_REG, RWBLE_EN, 0); 
 
    /* 
     * Since BLE is stopped (and powered), set CLK_SEL
     */    
    SetBits32(BLE_CNTL2_REG, BLE_CLK_SEL, 16);
    SetBits32(BLE_CNTL2_REG, BLE_RSSI_SEL, 1);    

    /* 
     * Set spi interface to software
     */   
#ifdef FPGA_USED     
    // the following 2 lines are for FPGA implementation
    SetBits32(BLE_CNTL2_REG, SW_RPL_SPI, 0);
    SetBits32(BLE_CNTL2_REG, BB_ONLY, 1);     
#endif    
}

/**
 ****************************************************************************************
 * @brief Creates sw cursor in power profiler tool. Used for Development/ Debugging 
 *
 * @return void 
 ****************************************************************************************
 */

void set_pxact_gpio(void)
{
#if DEVELOPMENT_DEBUG    
    uint32_t i;

    SetWord16(P13_MODE_REG, PID_GPIO|OUTPUT);
    SetWord16(P1_SET_DATA_REG, 0x8);
    for ( i=0;i<150;i++); //20 is almost 7.6usec of time.
    SetWord16(P1_RESET_DATA_REG, 0x8);
#endif
    return;
}

/**
 ****************************************************************************************
 * @brief Starts RCX20 calibration. 
 *
 * @param[in]   cal_time. Calibration time in RCX20 cycles. 
 *
 * @return void 
 ****************************************************************************************
 */
void calibrate_rcx20(uint16_t cal_time)
{
    
    SetWord16(CLK_REF_CNT_REG, cal_time);
    SetBits16(CLK_REF_SEL_REG, REF_CLK_SEL, 0x3); //RCX select 
    SetBits16(CLK_REF_SEL_REG, REF_CAL_START, 0x1); //Start Calibration
    cal_enable = 1;
}



/**
 ****************************************************************************************
 * @brief Calculates RCX20 frequency. 
 *
 * @param[in]   cal_time. Calibration time in RCX20 cycles. 
 *
 * @return void 
 ****************************************************************************************
 */

void read_rcx_freq(uint16_t cal_time)
{
    if (cal_enable)
    {
        while(GetBits16(CLK_REF_SEL_REG, REF_CAL_START) == 1);
        volatile uint32_t high = GetWord16(CLK_REF_VAL_H_REG);
        volatile uint32_t low = GetWord16(CLK_REF_VAL_L_REG);
        volatile uint32_t value = ( high << 16 ) + low;
        volatile uint32_t f = (16000000 * cal_time) / value;

        cal_enable = 0;

        rcx_freq = f;
        rcx_period = ((float) 1000000/f) * 1024;
        rcx_slot_duration = 0.000625 * (float)rcx_freq;
        
#ifdef RCX_MEASURE
        if (rcx_period_last)
        {
            volatile int diff = rcx_period_last - rcx_period;
            if (abs(diff) > rcx_period_diff)
                rcx_period_diff = abs(diff);
        }
        rcx_period_last = rcx_period;
        
        if (rcx_freq_min == 0)
        {
            rcx_freq_min = rcx_freq;
            rcx_freq_max = rcx_freq;
        }
        if (rcx_freq < rcx_freq_min)
            rcx_freq_min = rcx_freq;
        else if (rcx_freq > rcx_freq_max)
            rcx_freq_max = rcx_freq;
#endif    
    }
}



/**
 ****************************************************************************************
 * @brief Sets the BLE wake-up delay.  
 *
 * @return void 
 ****************************************************************************************
 */

void set_sleep_delay(void)
{
    uint16_t delay;
    uint32_t sleep_delay;
    
    if (lp_clk_sel == LP_CLK_RCX20)
    {
        sleep_delay = /*SLP_PROC_TIME + SLEEP_PROC_TIME + */(rcx_period >> 8);    // 490
    }
    else // if (lp_clk_sel == LP_CLK_XTAL32)
    {
        sleep_delay = /*SLP_PROC_TIME + SLEEP_PROC_TIME + */(4 * XTAL32_PERIOD_MAX); // ~200
    }
    
    // Actual "delay" is application specific and is the execution time of the BLE_WAKEUP_LP_Handler(), which depends on XTAL trimming delay.
    // In case of OTP copy, this is done while the XTAL is settling. Time unit of delay is usec.
    delay = XTAL_TRIMMING_TIME_USEC;
    // Icrease time taking into account the time from the setting of DEEP_SLEEP_ON until the assertion of DEEP_SLEEP_STAT.   
    delay += sleep_delay;
    // Add any application specific delay
    delay += APP_SLEEP_DELAY_OFFSET;
    // Round up for safety because the stack counts in slots and not in usec!
    delay += delay % 625;
    
    rwip_wakeup_delay_set(delay);
}


/**
 ****************************************************************************************
 * @brief conditionally_run_radio_cals(). Runs conditionally (time + temperature) RF and coarse calibration.                                                                                                                                                                                                                                                                                 ltime and temp changes) RF and coarse calibration  
 *
 * @return void 
 ****************************************************************************************
 */

void conditionally_run_radio_cals(void) {
    
    
    uint16_t count, count_diff;
    uint8_t force_rf_cal = 0;
    bool rf_cal_stat;
    
    uint32_t current_time = lld_evt_time_get();    
    
    if (current_time < last_temp_time)
    { 
        last_temp_time = 0;
    }

    if (force_rf_cal)
    {
        
        force_rf_cal = 0;
        
        last_temp_time = current_time;
        last_temp_count = get_rc16m_count();
#if LUT_PATCH_ENABLED
        pll_vcocal_LUT_InitUpdate(LUT_UPDATE);    //Update pll look up table
#endif          
        rf_cal_stat = rf_calibration();
        if ( rf_cal_stat== false)
            force_rf_cal = 1;        

        return;
    }
    
    if ( (current_time - last_temp_time) >= 3200) //2 sec
    {    
        
        
        last_temp_time = current_time;
        count = get_rc16m_count();                  // Estimate the RC16M frequency
        
        if (count > last_temp_count)
            count_diff = count - last_temp_count;
        else
            count_diff = last_temp_count - count ;
        
        if (count_diff >= 24)// If corresponds to 5 C degrees difference
        { 

            // Update the value of last_count
            last_temp_count = count;
#if LUT_PATCH_ENABLED
             pll_vcocal_LUT_InitUpdate(LUT_UPDATE);    //Update pll look up table
#endif            
            rf_cal_stat = rf_calibration();
            
            if ( rf_cal_stat== false)
                force_rf_cal = 1;         // Perform the readio calibrations

        }
        
    }
    
}

/**
 ****************************************************************************************
 * @brief       Converts us to low power cycles for RCX20 clock.  
 *
 * @param[in]   us. microseconds
 *
 * @return      uint32. Low power cycles       
 ****************************************************************************************
 */

uint32_t lld_sleep_us_2_lpcycles_rcx_func(uint32_t us)
{
    volatile uint32_t lpcycles;

    //RCX
    //Compute the low power clock cycles - case of a 16.342kHz clock
    lpcycles = (us * rcx_freq) / 1000000;
    
    return(lpcycles);
}

/**
 ****************************************************************************************
 * @brief       Converts low power cycles to us for RCX20 clock.  
 *
 * @param[in]   lpcycles. Low power cycles 
 *
 * @return      uint32. microseconds
 ****************************************************************************************
 */

uint32_t lld_sleep_lpcycles_2_us_rcx_func(uint32_t lpcycles)
{
    volatile uint32_t us;

    // Sanity check: The number of lp cycles should not be too high to avoid overflow
    ASSERT_ERR(lpcycles < 1000000);

    // Compute the sleep duration in us - case of a 16.342kHz clock (61.19202)
    us = (lpcycles * rcx_period) >> 10;
    
    return(us);
}    

/**
 ****************************************************************************************
 * @brief       Selects convertion function (XTAL32 or RCX20) for us to low power cycles.  
 *
 * @param[in]   us. microseconds
 *
 * @return      uint32. Low power cycles       
 ****************************************************************************************
 */

uint32_t lld_sleep_us_2_lpcycles_sel_func(uint32_t us)
{
    
    volatile uint32_t lpcycles;

    if (lp_clk_sel == LP_CLK_XTAL32)
        lpcycles = lld_sleep_us_2_lpcycles_func(us);
    else //LP_CLK_RCX20
        lpcycles = lld_sleep_us_2_lpcycles_rcx_func(us);

    return(lpcycles);        

}

/**
 ****************************************************************************************
 * @brief       Selects convertion function (XTAL32 or RCX20) for low power cycles to us.  
 *
 * @param[in]   lpcycles. Low power cycles 
 *
 * @return      uint32. microseconds
 ****************************************************************************************
 */

uint32_t lld_sleep_lpcycles_2_us_sel_func(uint32_t lpcycles)
{
        volatile uint32_t us;
        if (lp_clk_sel == LP_CLK_XTAL32)
            us = lld_sleep_lpcycles_2_us_func(lpcycles);
        else //LP_CLK_RCX20
            us = lld_sleep_lpcycles_2_us_rcx_func(lpcycles);

    return(us);        

}

#if (!BLE_APP_PRESENT)
/**
 ****************************************************************************************
 * @brief Check gtl state.
 *
 * @return bool  If gtl is idle returns true. Otherwise return false.
 ****************************************************************************************
 */

bool check_gtl_state(void)
{
    uint8_t ret = true;

    if ((ke_state_get(TASK_GTL) != GTL_TX_IDLE) ||
            ((gtl_env.rx_state != GTL_STATE_RX_START) &&
            (gtl_env.rx_state != GTL_STATE_RX_OUT_OF_SYNC)) )
            ret = false;

    return ret;
}
#endif

/*
 ****************************************************************************************
 * Replacement of ROM function uart_flow_off_func
 ****************************************************************************************
 */

#include "uart.h"
#include "reg_uart.h"   // uart register

static bool uart_is_rx_fifo_empty(void)
{
    return (uart_data_rdy_getf() == 0);
}

bool uart_flow_off_func(void)
{
    bool flow_off = true;
    volatile unsigned int i;

    do
    {
        // First check if no transmission is ongoing
        if ((uart_temt_getf() == 0) || (uart_thre_getf() == 0))
        {
            flow_off = false;
            break;
        }

        // Configure modem (HW flow control disable, 'RTS flow off')
#if 0
        uart_mcr_pack(UART_ENABLE,     // extfunc
                      UART_DISABLE,    // autorts
                      UART_ENABLE,     // autocts
                      UART_DISABLE);   // rts
#endif
        SetWord32(UART_MCR_REG, 0);


        // Wait for 1 character duration to ensure host has not started a transmission at the
        // same time
        for (i=0;i<350;i++);

        #ifndef __GNUC__
//        timer_wait(UART_CHAR_DURATION);
        #endif //__GNUC__
        // Check if data has been received during wait time
        if(!uart_is_rx_fifo_empty())
        {
            // Re-enable UART flow
            uart_flow_on();

            // We failed stopping the flow
            flow_off = false;
        }
    } while(false);

    return (flow_off);
}

/// @} 
