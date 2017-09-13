/*
 * time.h
 *
 * Created: 03/09/2017 17:43:28
 *  Author: Matt
 *
 * Handles all aspects of the 32.768kHz clock for accurate time keeping
 *
 */ 


#ifndef TIME_H_
#define TIME_H_

extern char seconds, minutes, hours;
void initialiseTimeCounter(); //Set up timer registers but do not start timer (only call once)
void startTimeCounter(); //Start timer
void stopTimeCounter(); //Stop timer
void verifyTimeCounterSleepSafety(); //Verify that it is safe to enter sleep mode
void incrementSeconds(); //Increment time by one second
void incrementMinutes(); //Increment time by one minute
void incrementHours(); //Increment time by one hour

#endif /* TIME_H_ */