/**
 ****************************************************************************************
 *
 * @file sw_cursor.c
 *
 * @brief Software Cursor for SmartSnippets Power Profiler.
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
#include "gpio.h"
#include "periph_setup.h"

void set_sw_cursor(void)
{
//#if SW_CURSOR
    uint32_t i;

    GPIO_SetActive(SW_CURSOR_PORT,SW_CURSOR_PIN);
    for ( i=0;i<150;i++); //20 is almost 7.6usec of time.
    GPIO_SetInactive(SW_CURSOR_PORT,SW_CURSOR_PIN);
//#endif
    return;
}
