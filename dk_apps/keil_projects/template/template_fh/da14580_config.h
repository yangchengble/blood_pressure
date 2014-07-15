/**
 ****************************************************************************************
 *
 * @file da14580_config.h
 *
 * @brief Compile configuration file.
 *
 * Copyright (C) 2014. Dialog Semiconductor Ltd, unpublished work. This computer 
 * program includes Confidential, Proprietary Information and is a Trade Secret of 
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited 
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

#ifndef DA14580_CONFIG_H_
#define DA14580_CONFIG_H_

#include "da14580_stack_config.h"

/////////////////////////////////////////////////////////////
/*FullEmbedded - FullHosted*/
#define CFG_APP  
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
/*Profiles*/
/* Battery Service Client */
#undef CFG_PRF_BASC
/* Battery Service Server */
#undef CFG_PRF_BASS
/* Data Information Service Client */
#undef CFG_PRF_DISC
/* Data Information Service Server */
#undef CFG_PRF_DISS
/* Proximity Profile Monitor */
#undef CFG_PRF_PXPM
/* Proximity Profile Reporter */
#undef CFG_PRF_PXPR
/* Findme Profile Locator */
#undef CFG_PRF_FMPL
/* Findme Profile Target */
#undef CFG_PRF_FMPT
/* Alert Notification Profile Client */
#undef CFG_PRF_ANPC
/* Alert Notification Profile Server */
#undef CFG_PRF_ANPS
/* Blood Pressure Profile Collector */
#undef CFG_PRF_BLPC
/* Blood Pressure Profile Sensor */
#undef CFG_PRF_BLPS
/* Cycling Speed and Cadence Profile Collector */
#undef CFG_PRF_CSCPC
/* Cycling Speed and Cadence Profile Server */
#undef CFG_PRF_CSCPS
/* Glucose Profile Collector */
#undef CFG_PRF_GLPC
/* Glucose Profile Sensor */
#undef CFG_PRF_GLPS
/* HID Device  */
#undef CFG_PRF_HOGPD
/* HID Boot Host */
#undef CFG_PRF_HOGPBH
/* HID Boot Host */
#undef CFG_PRF_HOGPRH
/* Heart Rate Profile Collector */
#undef CFG_PRF_HRPC
/* Heart Rate Profile Sensor */
#undef CFG_PRF_HRPS
/* Health Thermometer Profile Thermometer */
#undef CFG_PRF_HTPT
/* Health Thermometer Profile Collector */
#undef CFG_PRF_HTPC
/* Phone Alert Status Profile Client */
#undef CFG_PRF_PASPC
/* Phone Alert Status Profile Server */
#undef CFG_PRF_PASPS
/* Running Speed and Cadence Profile Collector */
#undef CFG_PRF_RSCPC
/* Running Speed and Cadence Profile Server */
#undef CFG_PRF_RSCPS
/* Scan Parameter Profile Client */
#undef CFG_PRF_SCPPC
/* Scan Parameter Profile Server */
#undef CFG_PRF_SCPPS
/* Time Profile Client */
#undef CFG_PRF_TIPC
/* Time Profile Server */
#undef CFG_PRF_TIPS
/* Accelerometer Profile */
#undef CFG_PRF_ACCEL
/* Dialog's Software Patch Over The Air Profile */
#undef CFG_PRF_SPOTAR 
/////////////////////////////////////////////////////////////

/*Misc*/

/* Application Definition. Project's configuration */
#define CFG_APP_TEMPLATE 

/* NVDS structure */
#define CFG_NVDS

/* BLE Security  */
#define CFG_APP_SEC

/* Coarse calibration */
#define CFG_LUT_PATCH

/*Watchdog*/
#undef CFG_WDOG 

/*Sleep modes*/
#undef CFG_EXT_SLEEP  
#undef CFG_DEEP_SLEEP  

/*Maximum user connections*/
#define BLE_CONNECTION_MAX_USER 1

/*Build for OTP or JTAG*/
#define DEVELOPMENT_DEBUG     1       //0: code at OTP, 1: code via JTAG
/*Application boot from OTP memory - Bootloader copies OTP Header to sysRAM */
#undef APP_BOOT_FROM_OTP
/*NVDS struture is padded with 0 - NVDS struture data must be written in OTP in production procedure*/
#undef READ_NVDS_STRUCT_FROM_OTP

/*Low power clock selection*/
#define CFG_LP_CLK              0x00    //0x00: XTAL32, 0xAA: RCX20, 0xFF: Select from OTP Header

/*Fab Calibration - Must be defined for calibrated devices*/
#undef CFG_CALIBRATED_AT_FAB  

/*
 * Scatterfile: Memory maps
 */
#if defined(CFG_EXT_SLEEP) || !defined(CFG_DEEP_SLEEP)
#define REINIT_DESCRIPT_BUF     0       //0: keep in RetRAM, 1: re-init is required (set to 0 when Extended Sleep is used)
#define USE_MEMORY_MAP          EXT_SLEEP_SETUP

#else
/*The following are valid for BLE_CONNECTION_MAX_USER == 1. Please use the results of the excel tool! */
#define REINIT_DESCRIPT_BUF     0       //0: keep in RetRAM, 1: re-init is required (set to 0 when Extended Sleep is used)
#define USE_MEMORY_MAP          DEEP_SLEEP_SETUP
#define DB_HEAP_SZ              1024
#define ENV_HEAP_SZ             328
#define MSG_HEAP_SZ             1312
#define NON_RET_HEAP_SZ         1024
#endif

#endif // DA14580_CONFIG_H_
