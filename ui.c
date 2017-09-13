/*
 * ui.c
 *
 * Created: 05/09/2017 01:28:33
 *  Author: Matt
 */ 

#include "time.h"
#include "ui.h"
#include <avr/io.h>
#include <avr/interrupt.h>

char blanking_hours;
char blanking_minutes;
char mode;

void enterUI()
{
	blanking_hours = 0;
	blanking_minutes = 0;
	TCNT1 = 0; //reset timer
	TCCR1B |= (1 << WGM12); //set it in Clear Timer on Compare Match (CTC) mode
	OCR1A =  244; //set overflow at 488 (about half a second)
	TIMSK1 |= (1 << OCIE1A); //enable interrupt on timer compare
	TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10); //set prescaler to /1024
}

void exitUI()
{
	TCCR1B &= ~((1 << CS12) | (0 << CS11) | (1 << CS10)); //stop timer
}

int rightSwitch()
{
	return (PINA & 0x40);
}

int leftSwitch()
{
	return (PINA & 0x80);
}

void initialiseLEDs()
{
	DDRB |= 0x0c; //set pins as outputs
}

void greenOn()
{
	PORTB |= 0x04;
}

void greenOff()
{
	PORTB &= ~0x04;
}

void redOn()
{
	PORTB |= 0x08;
}

void redOff()
{
	PORTB &= ~0x08;
}

//occurs approximately every 250ms when timer 1 compare happens
ISR(TIMER1_COMPA_vect)
{
	switch(mode)
	{
		case(HOUR_SET_MODE):
			blanking_hours = ~blanking_hours; //flash display
			blanking_minutes = 0;
			if(blanking_hours == 0) //sample buttons at half the flash frequency
			{
				if(leftSwitch() == 0)
				{
					mode = MINUTE_SET_MODE;
				}
				else
				{
					if(rightSwitch() == 0)
					{
						incrementHours();
					}
				}
			}
			break;
		case(MINUTE_SET_MODE):
			blanking_minutes = ~blanking_minutes;
			blanking_hours = 0;
			if(blanking_minutes == 0)
			{
				if(leftSwitch() == 0)
				{
					mode = TIME_MODE;
				}
				else
				{
					if(rightSwitch() == 0)
					{
						incrementMinutes();
					}
				}
			}
			break;
	}
}