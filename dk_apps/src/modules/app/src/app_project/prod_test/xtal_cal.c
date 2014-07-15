// auto calibration of 16MHz crystal block
// last update: hm20140619

#include "global_io.h"
#include "gpio.h"
#include <core_cm0.h>


// *** defines


#define ideal_cnt 8000055			// 500 msec  x = 1187
//#define ideal_cnt 4800026		// 300 msec  x = 1126

// *** variables ***  

#define FlagPrint		// ***

long x_input, x_input_old, y_input, x_output;
long delta_x_input;
long ccc;

long delta = -250;

//long ideal_value = 4800026; // 	300 msec
long ideal_value = 8000055; //	500 msec

volatile long limits = 2; 	// e.g. error <= 2 

long max_factor, factor;

long actual_triming_value; 
long new_triming_value;
long loop_calculation_counter=0;

int x_simulation;
char error_sign, error_sign_old, error_sign_change_counter;

// *** routines ***

long calculations(long xx_input, long yy_input);
long calculations_2(long xx_input, long yy_input);
long Clock_Read(char port_number);
void delay(void);
void print_dec_numbers(long numbers);
void Setting_Trim(int Trim_Value);
long data_simulation(long x1);
void data_testing(void);
	
// *** end of variables / routines ***

void delay(void)
{
	long j;
	
	for (j = 1; j<=100000; j++)
	{
		__nop();
		__nop();
	}
}

void Setting_Trim(int Trim_Value)	// program new Trim_Value
{
	if (Trim_Value < 256) 	Trim_Value = 256;
	if (Trim_Value > 2047)	Trim_Value = 2047;

	SetWord16(CLK_FREQ_TRIM_REG,Trim_Value);
	delay();
}	

long Clock_Read(char port_number)	// testing block wave via input e.g. P0_6 ... port can be selected
{	
		long ccc1 = 0; // !!! important to have this variable as local, otherwise speed is too low !!!
		char port_number_10, port_number_1;	// 10th and 1th e.g. 2 and 3 => port P2_3
		char shift_bit;
	
		uint32_t datareg;
		volatile uint32_t tick_counter = 0;
		int measure_pulse(int datareg, int shift_bit); // forward declaration

	
			port_number_10 	= port_number / 10;
			port_number_1 	= port_number % 10;
		
		shift_bit = (1 << port_number_1);

		switch (port_number_10)
			{
					case 0: datareg = P0_DATA_REG; break;                                                                
					case 1: datareg = P1_DATA_REG; break;
					case 2: datareg = P2_DATA_REG; break;                                                      
			}
			
			SysTick->LOAD = 0xFFFFFF;
			SysTick->VAL = 0;
			SetBits32(&SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk, 1);
			
			// during counting, no interrupts should should appear
			GLOBAL_INT_DISABLE();	// disable interrupts		

		ccc1 = 0;		// reset counter-value ccc1				

		tick_counter =  measure_pulse(datareg, shift_bit);
		
	//	tick_counter =  SysTick->VAL;
		SetBits32(&SysTick->CTRL, SysTick_CTRL_ENABLE_Msk, 0); // disable counting
		ccc1 = 0xFFFFFF - tick_counter;

		GLOBAL_INT_RESTORE();	// enable interrupts

		return (ccc1);
}

long Simulation(volatile long x)		// simulation graph: input = x (256 ... 2047)
{
	volatile float y_calc;
	volatile float aa, bb, cc, dd;
 
	// y = 1.716395904E-07x3 - 2.917958001E-04x2 + 3.814021247E-01x + 7.999748448E+06  at 500 msec
	// x = 1187; 	// test
	
	aa = x * x * 1.7163959 / 10000 * x / 1000;
	bb = x * x * 2.917958001 / 100000;
	cc = x * 3.81402124 / 10;	
	
	y_calc = aa - bb + cc + 7999357; //		ideal: x = 8000055

	return ((long)y_calc);
}

long calculations_2(long xx_input, long yy_input)
{	
	long hy, hx, hz_goal;
	char plus=0, minus=0;
	
	x_input = xx_input;				// e.g. 1347
	x_input_old = xx_input;
	y_input = yy_input;				// e.g. 4800089

	hy = Simulation(x_input);	// create start value
	hx = hy - y_input;
	hz_goal = ideal_cnt + hx;		// goal

	do
	{
		if (hy >= hz_goal)	
		{
			x_input--;
			plus = 1;
		}
		else										
		{
			x_input++;
			minus = 1;
		}
		hy = Simulation(x_input);
	}
	while(!(plus == 1 && minus == 1));	//crossing the 0-line

	return (x_input);
}


int auto_trim(uint8_t port_number)
{
	unsigned long trim_osc_16Mhz_1126 = 1126;	// centre of calibration
	unsigned long trim_osc_16Mhz_1187 = 1187;	// centre of calibration
	
	volatile long min_ccc, mean_ccc, max_ccc;
		
	
	volatile long nn_trimming, unit, unit_used;
	volatile char ii = 122, jj = 0;
	volatile char loop_counter;
	
	// system init
	SetWord16(CLK_AMBA_REG, 0x00); 				// set clocks (hclk and pclk ) 16MHz
	SetWord16(SET_FREEZE_REG,FRZ_WDOG);		// stop watch dog	

	delay();
	
	// Set port_number as an input with pull up resistor.
	switch (port_number)		
	{
		case 00:	SetBits16(P00_MODE_REG, PUPD, 1);		break;
		case 01:	SetBits16(P01_MODE_REG, PUPD, 1);		break;
		case 02:	SetBits16(P02_MODE_REG, PUPD, 1);		break;
		case 03:	SetBits16(P03_MODE_REG, PUPD, 1);		break;
		case 04:	SetBits16(P04_MODE_REG, PUPD, 1);		break;
		case 05:	SetBits16(P05_MODE_REG, PUPD, 1);		break;
		case 06:	SetBits16(P06_MODE_REG, PUPD, 1);		break;
		case 07:	SetBits16(P07_MODE_REG, PUPD, 1);		break;
		case 10:	SetBits16(P10_MODE_REG, PUPD, 1);		break;
		case 11:	SetBits16(P11_MODE_REG, PUPD, 1);		break;
		case 12:	SetBits16(P12_MODE_REG, PUPD, 1);		break;
		case 13:	SetBits16(P13_MODE_REG, PUPD, 1);		break;
		case 14:	SetBits16(P14_MODE_REG, PUPD, 1);		break;
		case 15:	SetBits16(P15_MODE_REG, PUPD, 1);		break;
		case 20:	SetBits16(P20_MODE_REG, PUPD, 1);		break;
		case 21:	SetBits16(P21_MODE_REG, PUPD, 1);		break;
		case 22:	SetBits16(P22_MODE_REG, PUPD, 1);		break;
		case 23:	SetBits16(P23_MODE_REG, PUPD, 1);		break;
		case 24:	SetBits16(P24_MODE_REG, PUPD, 1);		break;
		case 25:	SetBits16(P25_MODE_REG, PUPD, 1);		break;
		case 26:	SetBits16(P26_MODE_REG, PUPD, 1);		break;
		case 27:	SetBits16(P27_MODE_REG, PUPD, 1);		break;
		case 28:	SetBits16(P28_MODE_REG, PUPD, 1);		break;
		case 29:	SetBits16(P29_MODE_REG, PUPD, 1);		break;		
	}	

	// option !!!
	// Display on P05 the exact frequency coming from the external crystal 16 MHz 
	// Goal: 5 ppm, because of aging & temperature effect

#if 0	
	// option !!!
	SetWord32(P05_MODE_REG, 0x300); 						// show 16 MHz clock on output 
	SetBits16(TEST_CTRL_REG, SHOW_CLOCKS, 1); 	// show clock on P0_5
	SetWord16(TRIM_CTRL_REG, 0x0044);
#endif
		
#if 1	
			// step 1.
			// SetWord16(CLK_FREQ_TRIM_REG,256);	// 0 ... 255: not used (only important for startup)
			Setting_Trim(256);	
			//while (!(CLK_16M_REG & 0x0001));		// Xtal is enabled
			min_ccc = Clock_Read(port_number);		
						
			// step 3.
			//SetWord16(CLK_FREQ_TRIM_REG,2047);
			Setting_Trim(2047);
			max_ccc = Clock_Read(port_number);

			// step 2.
			//SetWord16(CLK_FREQ_TRIM_REG,1126);
			Setting_Trim(1187);
			mean_ccc = Clock_Read(port_number);		

		  max_factor = max_ccc - min_ccc;	
			ccc = mean_ccc;
												
			if (min_ccc < 100 || mean_ccc < 100 || max_ccc < 100)
			{	
				///error: No frequency generator connected, the customer has to run the test again
				return (-2);
			}		

			if ((min_ccc > ideal_value) || (max_ccc < ideal_value))	// check limits: ideal_value = 8000055 counts ~= 16.000,000 MHz
			{
					///error: out of limits
					return (-1);
			}				
#endif
			
		Setting_Trim(1187);
		delay();
		ccc = Clock_Read(port_number);
			
		__nop();
			
		// char error_sign, error_sign_old, error_sign_change_counter;
		error_sign_change_counter = 0;		
		loop_counter = 0;
			
		do
		{
			loop_counter++;
				
			if (loop_counter == 1)	
			{	
				Setting_Trim(1187);		// set best Trim first
				actual_triming_value = GetWord16(CLK_FREQ_TRIM_REG);					// read TRIM register
				new_triming_value = calculations_2(actual_triming_value, ccc);	// calculate new calibration value 
				delta_x_input = new_triming_value - actual_triming_value;
											
				Setting_Trim(new_triming_value);
				__nop();
				__nop();
				actual_triming_value = GetWord16(CLK_FREQ_TRIM_REG);					// read TRIM register
				ccc = Clock_Read(port_number);

				unit = 128;

				__nop();
			} // end of loopcounter = 1
			
			if (loop_counter >= 2) 	
			{
					unit = unit / 2;
					actual_triming_value = GetWord16(CLK_FREQ_TRIM_REG);					// read TRIM register
					if (ccc < ideal_cnt)
					{
						actual_triming_value = actual_triming_value + unit;
					}
					if (ccc > ideal_cnt) 
					{	
						actual_triming_value = actual_triming_value - unit;						
					}
					// if (ccc = ideal_cnt) => actual_triming_value not changed
			
					Setting_Trim(actual_triming_value);	// loading new calibration value
					ccc = Clock_Read(port_number);
					
					__nop();		
			} // end of loop_counter >= 2				
			__nop();
		}
		while ((unit >= 2 || unit <= -2) && loop_counter <= 10 && ccc != ideal_cnt); //
	
		
		actual_triming_value = GetWord16(CLK_FREQ_TRIM_REG);							// read TRIM register
		
		
		return (actual_triming_value);	
}

