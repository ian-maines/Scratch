// Copyright 2016 Ian Maines
// Main code for solar voltage reader.

//**************************//
// HIGH TECH C TYPE SIZES	//
// char 8b			(1B)	//
// int/short 16b	(2B)	//
// short long 24b	(3B)	//
// long	32b			(4B)	//
// float/double 24b	(3B)	//
//**************************//

// <defines>
#include <pic.h>
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 32000000		//Configures clock frequency (32MHz)
#endif
#include "..\..\Libraries\PIC16F1783_adc.h"
#include "..\..\Libraries\PIC16F1783_serial.h"

unsigned int read_adc ()
	{
	//Maximum value this will return for a valid read is 2^12, 
	//ie 4096. Anything larger is an error
	unsigned int low = 0, high = 0, retval = 0;
	//Channel selection (Set to channel 0)
	CHS4 = 0;
	CHS3 = 0;
	CHS2 = 0;
	CHS1 = 0;
	CHS0 = 0;
	
	//Start conversion
	ADCON0 |= 0x02;		//GO/!DONE bit
	//Wait for the conversion to complete
	while(ADCON0 & 0x02);
	//Get value
	low = ADRESL;
	high = ADRESH;
	//Format value
	retval =  ((high << 4) & 0xFF0) | ((low >> 4) & 0x000F);
	return retval;
	}

int main ()
	{
	// Configure global bits.
	OSCCON = 0b11111000;		
	ADC_config ();
	uart_config ();
	
	__delay_ms (550);	// Let the LCD init.
	
	TRISC4 = 0;	// LED Output.
	// Turn on LCD
	serial_tx (0x0c);
	
	// Brightnes to max
	serial_tx (0x7c);
	serial_tx (157);
	
	while (1)
		{		
		RC4 = 1;
		__delay_ms (500);
		RC4 = 0;
		__delay_ms (500);
		serial_tx (0xFE);
		serial_tx (0x01);
		send_string ("Voltage = 24.8v");
		}
	
	while (1)
		{
		RC4 = 1;
		__delay_ms (500);
		serial_tx (0xFE);
		serial_tx (0x08);
		RC4 = 0;
		__delay_ms (500);
		serial_tx (0xFE);
		serial_tx (0x0c);
		}
	}