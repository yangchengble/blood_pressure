/**`
 ****************************************************************************************
 *
 * @file wucpt_quadec.h
 *
 * @brief Wakeup IRQ & Quadrature Decoder driver header file.
 *
 * Copyright (C) 2013. Dialog Semiconductor Ltd, unpublished work. This computer 
 * program includes Confidential, Proprietary Information and is a Trade Secret of 
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited 
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */
 
 
 /* Important note: WKUP_ENABLED and QUADEC_ENABLED control the inclusion of the parts of the
 *               code that add support for to wakeup timer and quadrature decoder respectively.
 */ 
 
 /* Important note: If, upon reception of interrupt from the wakeup timer or the quadrature
 *                 decoder, the system resumes from sleep mode and you wish to resume peripherals
 *                 functionality , it is necessary to include in your interrupt handler function(s)
 *                 - the ones you register using wkupct_register_callback() and/or 
 *                                                quad_decoder_register_callback() -
 *                  the following lines:
 *
 *                    // Init System Power Domain blocks: GPIO, WD Timer, Sys Timer, etc.
 *                    // Power up and init Peripheral Power Domain blocks,
 *                    // and finally release the pad latches.
 *                    if(GetBits16(SYS_STAT_REG, PER_IS_DOWN))
 *                        periph_init();
 *                        
*/   

#include <stdint.h>
#include "periph_setup.h"

#ifndef _WKUPCT_QUADEC_H_
#define _WKUPCT_QUADEC_H_


enum
{
    SRC_WKUP_IRQ = 0x01,
    SRC_QUAD_IRQ,
};

#ifdef WKUP_ENABLED
typedef void (*wakeup_handler_function_t)(void);

/**
 ****************************************************************************************
 * @brief Enable Wakeup IRQ.
 ****************************************************************************************
 */
void wkupct_enable_irq(uint32_t sel_pins, uint32_t pol_pins, uint16_t events_num, uint16_t deb_time);

/**
 ****************************************************************************************
 * @brief Register Callback function for Wakeup IRQ.
 ****************************************************************************************
 */
void wkupct_register_callback(wakeup_handler_function_t callback);

#endif //WKUP_ENABLED

#ifdef QUADEC_ENABLED

typedef void (*quad_encoder_handler_function_t)(int16_t qdec_xcnt_reg, int16_t qdec_ycnt_reg, int16_t qdec_zcnt_reg);

typedef enum 
{
    QUAD_DEC_CHXA_NONE_AND_CHXB_NONE = 0,
    QUAD_DEC_CHXA_P00_AND_CHXB_P01 = 1,
    QUAD_DEC_CHXA_P02_AND_CHXB_P03 = 2,
    QUAD_DEC_CHXA_P04_AND_CHXB_P05 = 3,
    QUAD_DEC_CHXA_P06_AND_CHXB_P07 = 4,
    QUAD_DEC_CHXA_P10_AND_CHXB_P11 = 5,
    QUAD_DEC_CHXA_P12_AND_CHXB_P13 = 6,
    QUAD_DEC_CHXA_P23_AND_CHXB_P24 = 7,
    QUAD_DEC_CHXA_P25_AND_CHXB_P26 = 8,
    QUAD_DEC_CHXA_P27_AND_CHXB_P28 = 9,
    QUAD_DEC_CHXA_P29_AND_CHXB_P20 = 10      
}CHX_PORT_SEL_t;

typedef enum 
{
    QUAD_DEC_CHYA_NONE_AND_CHYB_NONE = 0<<4,
    QUAD_DEC_CHYA_P00_AND_CHYB_P01 = 1<<4,
    QUAD_DEC_CHYA_P02_AND_CHYB_P03 = 2<<4,
    QUAD_DEC_CHYA_P04_AND_CHYB_P05 = 3<<4,
    QUAD_DEC_CHYA_P06_AND_CHYB_P07 = 4<<4,
    QUAD_DEC_CHYA_P10_AND_CHYB_P11 = 5<<4,
    QUAD_DEC_CHYA_P12_AND_CHYB_P13 = 6<<4,
    QUAD_DEC_CHYA_P23_AND_CHYB_P24 = 7<<4,
    QUAD_DEC_CHYA_P25_AND_CHYB_P26 = 8<<4,
    QUAD_DEC_CHYA_P27_AND_CHYB_P28 = 9<<4,
    QUAD_DEC_CHYA_P29_AND_CHYB_P20 = 10<<4      
}CHY_PORT_SEL_t;

typedef enum 
{
    QUAD_DEC_CHZA_NONE_AND_CHZB_NONE = 0<<8,
    QUAD_DEC_CHZA_P00_AND_CHZB_P01 = 1<<8,
    QUAD_DEC_CHZA_P02_AND_CHZB_P03 = 2<<8,
    QUAD_DEC_CHZA_P04_AND_CHZB_P05 = 3<<8,
    QUAD_DEC_CHZA_P06_AND_CHZB_P07 = 4<<8,
    QUAD_DEC_CHZA_P10_AND_CHZB_P11 = 5<<8,
    QUAD_DEC_CHZA_P12_AND_CHZB_P13 = 6<<8,
    QUAD_DEC_CHZA_P23_AND_CHZB_P24 = 7<<8,
    QUAD_DEC_CHZA_P25_AND_CHZB_P26 = 8<<8,
    QUAD_DEC_CHZA_P27_AND_CHZB_P28 = 9<<8,
    QUAD_DEC_CHZA_P29_AND_CHZB_P20 = 10<<8      
}CHZ_PORT_SEL_t;


typedef struct 
{
    CHX_PORT_SEL_t chx_port_sel;
    CHY_PORT_SEL_t chy_port_sel;    
    CHZ_PORT_SEL_t chz_port_sel;
    uint16_t qdec_clockdiv;
    uint8_t qdec_events_count_to_trigger_interrupt;
}QUAD_DEC_INIT_PARAMS_t;


/**
 ****************************************************************************************
 * @brief Register Callback function for Quadrature Decoder IRQ.
 *
 * @param[in] callback      Callback function's reference.
 *
 * @return void
 ****************************************************************************************
 */
inline void quad_decoder_register_callback(uint32_t* callback);


/**
 ****************************************************************************************
 * @brief 
 *
 * @param[in]
 *
 * @return 
 ****************************************************************************************
 */
 
void quad_decoder_init(QUAD_DEC_INIT_PARAMS_t *quad_dec_init_params);

/**
 ****************************************************************************************
 * @brief 
 *
 * @param[in]
 *
 * @return 
 ****************************************************************************************
 */
void quad_decoder_release(void);

/**
 ****************************************************************************************
 * @brief 
 *
 * @param[in]
 *
 * @return 
 ****************************************************************************************
 */
int16_t quad_decoder_get_x_counter(void);

/**
 ****************************************************************************************
 * @brief 
 *
 * @param[in]
 *
 * @return 
 ****************************************************************************************
 */
int16_t quad_decoder_get_y_counter(void);


/**
 ****************************************************************************************
 * @brief 
 *
 * @param[in]
 *
 * @return 
 ****************************************************************************************
 */
int16_t quad_decoder_get_z_counter(void);

/**
 ****************************************************************************************
 * @brief 
 *
 * @param[in]
 *
 * @return 
 ****************************************************************************************
 */
inline void quad_decoder_enable_irq(uint8_t event_count);


/**
 ****************************************************************************************
 * @brief 
 *
 * @param[in]
 *
 * @return 
 ****************************************************************************************
 */
inline void quad_decoder_disable_irq(void);

#endif //QUADEC_ENABLED




#endif //_WKUPCT_QUADEC_H_


