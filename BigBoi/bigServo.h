/*
	bigServo.h
	Servo using DC motor, L298N, and potetiometer.
	Made by Garett Morrison
*/

#ifndef Morse_h
#define Morse_h

#include "Arduino.h"

class bigServo
{
public:
	attach(int _sPin, int _aPin, int _bPin, int _potPin);
	setTarget(int _target);
	int upDate();
private:
	int sPin;
	int aPin;
	int bPin;
	int potPin;
	int target;
	int currVal;
};

#endif