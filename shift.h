/*
 * shift.h
 *
 * Created: 03/09/2017 17:46:12
 *  Author: Matt
 * 
 * Handles shift register stuff i.e. SPI port
 *
 */ 


#ifndef SHIFT_H_
#define SHIFT_H_

void initialiseShift();
void shift(char[]); //shift out one byte
void displayTime(int, int); //display hours and minutes on display
void clearDisplay(); //blank display

#endif /* SHIFT_H_ */