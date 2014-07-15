
 /*
 ****************************************************************************************
 *
 * @file eeprom_test.c
 *
 * @brief Eeprom test functions
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
#include "periph_setup.h"
#include "peripherals.h"
#include "i2c_eeprom.h"
#include "uart.h"


  /**
 ****************************************************************************************
 * @brief EEPROM test function
  * 
 ****************************************************************************************
 */
void i2c_test(void){
	uint8_t rrb;
	uint16_t wrb;
	uint16_t i, btrd;
	uint8_t wr_data[256];
	uint8_t rd_data[256];
	uint16_t read_size = 256; // set this variable to the desired read size
	uint16_t write_size = 256; // set this variable to the desired write size
	uint8_t * rd_data_ptr = rd_data;
	uint8_t * wr_data_ptr = wr_data;
	
	// populate write vector
	for (i=0 ; i<256 ; i++){
		wr_data[i] = i;
	}
	// reset read vector
	for (i=0 ; i<256 ; i++){
		rd_data[i] = 0;
	}
	
    // Initialize I2C interface for the EEPROM
	i2c_eeprom_init(I2C_SLAVE_ADDRESS, I2C_SPEED_MODE, I2C_ADDRESS_MODE, I2C_ADDRESS_SIZE);
    
	printf_string("\n\r\n\r************");
	printf_string("\n\r* I2C TEST *\n\r");
	printf_string("************\n\r");
	
	// Page Write
	printf_string("\n\rWriting page to EEPROM (values 0x00-FF)...");
	i2c_eeprom_write_data(wr_data_ptr, 0, write_size);
	printf_string("Page written.\n\r\n\r");
	
	// Page Read
	printf_string("\n\rReading EEPROM...\n\r\n\r");
	btrd = i2c_eeprom_read_data(rd_data_ptr,0,read_size);
	// Display Results
	for (i=0 ; i<read_size ; i++){
		printf_byte(rd_data[i]);
		printf_string(" ");
	}
	printf_string("\n\r\n\rBytes Read: 0x");
	printf_byte((btrd>>8)&0xFF);
	printf_byte((btrd)&0xFF);
	printf_string("\n\r");
	
	// Setting 1 : 0x5a@22 (dec)
	// Write Byte
	printf_string("\n\r\n\rWriting byte (0x5A) @ address 22 (zero based)...");
	wrb = 90;
	i2c_eeprom_write_byte(22,wrb);
	printf_string("done.\n\r");
	// Random Read Byte
	rrb = i2c_eeprom_read_byte(22);
	printf_string("Byte Read @ address 22: 0x");
	printf_byte((rrb)&0xFF);
	printf_string("\n\r");
	
	// Setting 2 : 0x6a@00 (dec)
	// Write Byte
	printf_string("\n\r\n\rWriting byte (0x6A) @ address 0 (zero based)...");
	wrb = 106;
	i2c_eeprom_write_byte(0,wrb);
	printf_string("done.\n\r");
	// Random Read Byte
	rrb = i2c_eeprom_read_byte(0);
	printf_string("Byte Read @ address 00: 0x");
	printf_byte((rrb)&0xFF);
	printf_string("\n\r");
	
	// Setting 3 : 0x7a@255 (dec)
	// Write Byte
	printf_string("\n\r\n\rWriting byte (0x7A) @ address 255 (zero based)...");
	wrb = 122;
	i2c_eeprom_write_byte(255,wrb);
	printf_string("done.\n\r");
	// Random Read Byte
	rrb = i2c_eeprom_read_byte(255);
	printf_string("Byte Read @ address 255: 0x");
	printf_byte((rrb)&0xFF);
	printf_string("\n\r");
	
	
	// Setting 4 : 0xFF@30 (dec)
	// Write Byte
	printf_string("\n\r\n\rWriting byte (0xFF) @ address 30 (zero based)...");
	wrb = 255;
	i2c_eeprom_write_byte(30,wrb);
	printf_string("done.\n\r");
	// Random Read Byte
	rrb = i2c_eeprom_read_byte(30);
	printf_string("Byte Read @ address 30: 0x");
	printf_byte((rrb)&0xFF);
	printf_string("\n\r");

	// page read for verification
	printf_string("\n\rPage Read to verify that new bytes have been written correctly\n\r");
	printf_string("--------------------------------------------------------------\n\r");
	btrd = i2c_eeprom_read_data(rd_data_ptr,0,read_size);
	// Display Results
	for (i=0 ; i<read_size ; i++){
		printf_byte(rd_data[i]);
		printf_string(" ");
	}
	
	i2c_eeprom_release();
}
