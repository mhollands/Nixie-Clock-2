/*
 * time.c
 *
 * Created: 03/09/2017 17:49:17
 *  Author: Matt
 */ 
#include <avr/interrupt.h>
#include "time.h"

char seconds, minutes, hours;

//Wait for bytes to be transferred to asynchronous clock domain
static void waitForAsynchronous()
{
	while((ASSR & 0x1B) != 0) { }
}

void initialiseTimeCounter()
{
	seconds = 0;
	minutes = 41;
	hours = 21;
	
	//This must be performed in a specific order to prevent corruption due to asynchronous clock
	ASSR |= (1 << EXCLK); //enable clocking from XTAL1
	ASSR |= (1 << AS0); //enable asynchronous clocking
	TCNT0 = 0; //reset timer
	TCCR0A |= (1 << WGM01) | (0 << WGM00); //set it in Clear Timer on Compare Match (CTC) mode
	OCR0A = 31; //set overflow at 31 (1 second)
	waitForAsynchronous();
	TIFR0 &= ~((1 << OCF0A) | (1 << TOV0)); //clear interrupt flags	 
	TIMSK0 |= (1 << OCIE0A); //enable interrupt on timer compare
}

void startTimeCounter()
{
	TCNT0 = 0; //reset timer
	TCCR0B |= (1 << CS02) | (1 << CS01) | (1 << CS00); //set prescaler to /1024
	waitForAsynchronous();
}

void stopTimeCounter()
{
	TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00)); //set prescaler to 0 (stop clock)
	waitForAsynchronous();
}

void verifyTimeCounterSleepSafety()
{
	//To check it is safe to enter sleep, write to an asynchronous register and wait for non-busy
	TCCR0A |= (1 << WGM01) | (0 << WGM00); //set it in Clear Timer on Compare Match (CTC) mode
	waitForAsynchronous();
}

void incrementMinutes()
{
	minutes++;
	if(minutes > 59)
	{
		minutes = 0;
		incrementHours();
	}
}

void incrementHours()
{
	hours++;
	if(hours > 23)
	{
		hours = 0;
	}
}

void incrementSeconds()
{
	seconds++;
	if(seconds > 59)
	{
		seconds = 0;
		incrementMinutes();
	}
}

ISR(TIMER0_COMPA_vect)
{
	//increment the clock
	incrementSeconds();
}