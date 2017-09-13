/*
 * ui.h
 *
 * Created: 05/09/2017 01:26:33
 *  Author: Matt
 */ 


#ifndef UI_H_
#define UI_H_

#define TIME_MODE 0
#define HOUR_SET_MODE 1
#define MINUTE_SET_MODE 2

//Flags indicating if minutes/hour field should be blanked (to achieve flashing effect)
extern char blanking_minutes;
extern char blanking_hours;

extern char mode;

//LED commands
void initialiseLEDs(); 
void greenOn();
void greenOff();
void redOn();
void redOff();

void enterUI(); //start UI clock
void exitUI(); //stop UI clock

//switch commands
int leftSwitch();
int rightSwitch();

#endif /* UI_H_ */