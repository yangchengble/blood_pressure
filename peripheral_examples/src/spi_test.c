
 /*
 ****************************************************************************************
 *
 * @file spi_test.c
 *
 * @brief SPI test functions
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
#include "uart.h"
#include "spi.h"
#include "spi_flash.h"
  
uint8_t rd_data[512];
uint8_t wr_data[512];
SPI_Pad_t spi_FLASH_CS_Pad;
 
 
  /**
 ****************************************************************************************
 * @brief SPI and SPI flash Initialization fucntion
  * 
 ****************************************************************************************
 */
static void spi_flash_peripheral_init()
{
 
		spi_FLASH_CS_Pad.pin = SPI_CS_PIN;
		spi_FLASH_CS_Pad.port = SPI_GPIO_PORT;
		// Enable SPI & SPI FLASH
		spi_flash_init(SPI_FLASH_SIZE, SPI_FLASH_PAGE);
		spi_init(&spi_FLASH_CS_Pad, SPI_MODE_8BIT,	 SPI_ROLE_MASTER,  SPI_CLK_IDLE_POL_LOW,SPI_PHA_MODE_0,SPI_MINT_DISABLE,SPI_XTAL_DIV_8);
}	

  /**
 ****************************************************************************************
 * @brief SPI and SPI flash test function
  * 
 ****************************************************************************************
 */
void spi_test(void)
{
  uint16_t man_dev_id;
	uint64_t unique_id;
	uint32_t jedec_id;
	
	int16_t btrd, i;

  uint16_t read_size = 256; // set this variable to the desired read size
  uint16_t write_size = 512; // set this variable to the desired read size
  uint8_t * rd_data_ptr = rd_data;
	uint8_t * wr_data_ptr = wr_data;
	wr_data[0] = 0;
	for (i=1 ; i<512 ; i++){
		wr_data[i] = wr_data[i-1]+1;
	}
    
  printf_string("\n\r\n\r************");
	printf_string("\n\r* SPI TEST *\n\r");
	printf_string("************\n\r");
  
	// Enable FLASH and SPI
  spi_flash_peripheral_init();   
  // spi_flash_chip_erase();
  // Read SPI Flash Manufacturer/Device ID
	man_dev_id = spi_read_flash_memory_man_and_dev_id();
	
  spi_cs_low();
    
  spi_cs_high();

    
  // Erase flash
  spi_flash_chip_erase();
   
	// Read SPI Flash first 256 bytes
	printf_string("\n\r\n\rReading SPI Flash first 256 bytes...");
	btrd = spi_flash_read_data(rd_data_ptr,0,read_size);
	// Display Results
	for (i=0 ; i<read_size ; i++){
		printf_byte(rd_data[i]);
		printf_string(" ");
	}
	printf_string("\n\r\n\rBytes Read: 0x");
	printf_byte((btrd>>8)&0xFF);
	printf_byte((btrd)&0xFF);
	printf_string("\n\r");
	 
	// Read SPI Flash JEDEC ID
	jedec_id = spi_read_flash_jedec_id();
	printf_string("\n\rSPI flash JEDEC ID is ");
	printf_byte((jedec_id>>16)&0xFF);
	printf_byte((jedec_id>>8)&0xFF);
	printf_byte((jedec_id)&0xFF);

	// Read SPI Flash Manufacturer/Device ID
	man_dev_id = spi_read_flash_memory_man_and_dev_id();
	printf_string("\n\r\n\rSPI flash Manufacturer/Device ID is ");
	printf_byte((man_dev_id>>8)&0xFF);
	printf_byte((man_dev_id)&0xFF);
	
	unique_id = spi_read_flash_unique_id();
	printf_string("\n\r\n\rSPI flash Unique ID Number is ");
	printf_byte(((unique_id>>32)>>24)&0xFF);
	printf_byte(((unique_id>>32)>>16)&0xFF);
	printf_byte(((unique_id>>32)>>8)&0xFF);
	printf_byte((unique_id>>32)&0xFF);
	printf_byte((unique_id>>24)&0xFF);
	printf_byte((unique_id>>16)&0xFF);
	printf_byte((unique_id>>8)&0xFF);
	printf_byte((unique_id)&0xFF);

	// Program Page example (256 bytes)
	printf_string("\n\r\n\rPerforming Program Page...");	
	spi_flash_page_program(wr_data_ptr, 0, 256);
	printf_string("Page programmed. (");
	printf_byte(spi_flash_read_status_reg());
	printf_string(")\n\r");
	
	// Read SPI Flash first 256 bytes
	printf_string("\n\r\n\rReading SPI Flash first 256 bytes...");
	btrd = spi_flash_read_data(rd_data_ptr,0,read_size);
	// Display Results
	for (i=0 ; i<read_size ; i++){
		printf_byte(rd_data[i]);
		printf_string(" ");
	}
	printf_string("\n\r\n\rBytes Read: 0x");
	printf_byte((btrd>>8)&0xFF);
	printf_byte((btrd)&0xFF);
	printf_string("\n\r");

	printf_string("\n\rPerforming Sector Erase...");	
	spi_flash_block_erase(0,SECTOR_ERASE);
	printf_string("Sector erased. (");
	printf_byte(spi_flash_read_status_reg());
	printf_string(")\n\r");

	// Write data example (512 bytes)
	printf_string("\n\r\n\rPerforming 512 byte write...");	
	spi_flash_write_data(wr_data_ptr, 0, 512);
	printf_string("Data written. (");
	printf_byte(spi_flash_read_status_reg());
	printf_string(")\n\r");
	
	// Read SPI Flash first 512 bytes
	printf_string("\n\r\n\rReading SPI Flash first 512 bytes...");
	btrd = spi_flash_read_data(rd_data_ptr,0,write_size);
	// Display Results
	for (i=0 ; i<write_size ; i++){
		printf_byte(rd_data[i]);
		printf_string(" ");
	}
	printf_string("\n\r\n\rBytes Read: 0x");
	printf_byte((btrd>>8)&0xFF);
	printf_byte((btrd)&0xFF);
	printf_string("\n\r");
 	
}
