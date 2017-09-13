/*
 * shift.c
 *
 * Created: 03/09/2017 17:47:00
 *  Author: Matt
 */ 

#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include "shift.h"

//Look up table for which bits on shift register correspond to which number
int nixie4[10] = {8, 9, 0, 1, 2, 3, 4, 5, 6, 7};
int nixie3[6] = {14, 15, 10, 11, 12, 13};
int nixie2[10] = {8, 9, 0, 1, 2, 3, 4, 5, 6, 7};
int nixie1[3] = {12, 10, 11};

static void shiftByte(char out)
{
	for(int i = 0; i < 8; i++)
	{
		PORTA = (PORTA & ~0x30) | ((out & 0x80) >> 3); //update the data line and set clock to low
		_delay_ms(1);
		PORTA = (PORTA & ~0x30) | ((out & 0x80) >> 3) | 0x20; // set the clock high
		_delay_ms(1);
		out = (out << 1); //shift out data
	}
}

void initialiseShift()
{
	DDRA |= 0x33; //Set SERIAL_LATCH, SERIAL_RESET, SERIAL_DATA, SERIAL_CLOCK as outputs
	PORTA |= 0x01; //De-assert SERIAL_RESET
	PORTA |= 0x02; //Assert SERIAL_LATCH
}

void shift(char data[4])
{
	PORTA &= ~0x02; //De-assert SERIAL_LATCH
	for(int i = 3; i >= 0; i--)
	{
		shiftByte(data[i]);	
	}
	PORTA |= 0x02; //Assert SERIAL_LATCH
}

void displayTime(int hours, int minutes)
{
	unsigned long data_l = 0x00000000;
	unsigned long data_h = 0x00000000;
	
	//decode minutes if it is valid
	if(minutes < 60)
	{
		unsigned int digit4 = minutes % 10;
		unsigned int digit3 = minutes/10;	
		data_l |= (1 << nixie4[digit4]) | (1 << nixie3[digit3]);
	}
	
	//decode hours if it is valid
	if(hours < 24)
	{
		unsigned int digit2 = hours%10;
		unsigned int digit1 = hours/10;	
		data_h |= (1 << nixie2[digit2]) | (1 << nixie1[digit1]);
	}

	char data[4];
	data[0] = ((data_h >> 8) & 0x000000ff);
	data[1] = (data_h & 0x000000ff);
	data[2] = ((data_l >> 8) & 0x000000ff);
	data[3] = (data_l & 0x000000ff);
	
	shift(data);
}

void clearDisplay()
{
	PORTA &= ~0x02; //De-assert SERIAL_LATCH
	PORTA &= ~0x01; //Assert SERIAL_RESET
	_delay_ms(1);
	PORTA |= 0x01; //De-assert SERIAL_RESET
	PORTA |= 0x02; //Assert SERIAL_LATCH
}
