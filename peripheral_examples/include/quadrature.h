//------------------------------------------------------------------------------
// (c) Copyright 2012, Dialog Semiconductor BV
// All Rights Reserved
//------------------------------------------------------------------------------
// Dialog SEMICONDUCTOR CONFIDENTIAL
//------------------------------------------------------------------------------
// This code includes Confidential, Proprietary Information and is a Trade 
// Secret of Dialog Semiconductor BV. All use, disclosure, and/or reproduction 
// is prohibited unless authorized in writing.
//------------------------------------------------------------------------------
// Description: Peripheral include file

#include <stdint.h>

#ifndef PERIPHERAL_H_INCLUDED
#define PERIPHERAL_H_INCLUDED

 
void set_pad_functions(void);        // set gpio port function mode
void periph_init(void) ;

void print_menu(void);
void print_input(void);
void swt_test(void);
 
 
void quad_test(void);
void buzz_test(void);
void exit_test(void);

#endif
