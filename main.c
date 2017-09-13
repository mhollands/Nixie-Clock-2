/*
 * Nixie_Clock_2.c
 *
 * Created: 02/08/2017 14:24:15
 * Author : Matt
 */ 

#define F_CPU 1000000
#define SLEEP_MODE_PWR_SAVE (0x03<<1) //For some reason POWER SAVE mode is not defined so we have to define it ourselves

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/io.h>
#include <util/delay.h>
#include "time.h"
#include "shift.h"
#include "ui.h"

static char oldMode;

int main(void)
{
	oldMode = TIME_MODE;
	mode = TIME_MODE;
	
	sei(); //enable global interrupts
		
	//Enable pull-ups on inputs
	PORTA |= 0xcc;
	PORTB |= 0x63;
	
	initialiseLEDs();
	
	initialiseShift();
	
	set_sleep_mode(SLEEP_MODE_PWR_SAVE); //This mode stops main clock

	initialiseTimeCounter();
	startTimeCounter();
	
	greenOn();

	while(1)
	{	
		char currentMode = mode; //don't want mode to change halfway through the loop so sample it at the start of the loop
		
		//if mode has changed, start relevant timers etc
		if(currentMode != oldMode)
		{
			switch(currentMode)
			{
				case(TIME_MODE):
					exitUI();
					startTimeCounter();
					_delay_ms(500); //give user time to release buttons
					break;
				case(HOUR_SET_MODE):
					stopTimeCounter();
					seconds = 0;
					enterUI();
					break;
			}
			oldMode = currentMode;
		}
		
		if(currentMode == TIME_MODE)
		{
			if(leftSwitch() == 0)
			{
				mode = HOUR_SET_MODE;
			}
			else
			{
				//blank display if right button is pressed
				if(rightSwitch() == 0)
				{
					clearDisplay();
				}
				else
				{
					displayTime(hours, minutes);
				}
				
				//go to sleep only in TIME_MODE (ui clock depends on main clock)
				verifyTimeCounterSleepSafety();
				greenOff();
				sleep_mode();
				greenOn();
			}
		}
		else
		{
			displayTime((blanking_hours ? 24 : hours), (blanking_minutes ? 60 : minutes)); //show time, but blank hours or minutes as requested by UI
		}		
	}
}

