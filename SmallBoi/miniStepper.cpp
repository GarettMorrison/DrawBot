/*
	miniStepper.h
	Simple Stepper Library
	Made by Garett Morrison
*/

#include "Arduino.h"
#include "miniStepper.h"

miniStepper::attach(int _a, int _b, int _c, int _d){
	pos = 0;
	step = 0;

	a = _a;
	b = _b;
	c = _c;
	d = _d;

	pinMode(a, OUTPUT);
	pinMode(b, OUTPUT);
	pinMode(c, OUTPUT);
	pinMode(d, OUTPUT);

	digitalWrite(a, HIGH);
	digitalWrite(b, LOW);
	digitalWrite(c, LOW);
	digitalWrite(d, LOW);
}

miniStepper::stepUp(){
	if(step == 0){
		step = 1;
		digitalWrite(b, HIGH);
		digitalWrite(a, LOW);
	}
	else if(step == 1){
		step = 2;
		digitalWrite(c, HIGH);
		digitalWrite(b, LOW);
	}
	else if(step == 2){
		step = 3;
		digitalWrite(d, HIGH);
		digitalWrite(c, LOW);
	}
	else if(step == 3){
		step = 0;
		digitalWrite(a, HIGH);
		digitalWrite(d, LOW);
	}

	pos += 1;
}

miniStepper::stepDown(){
	if(step == 0){
		step = 3;
		digitalWrite(d, HIGH);
		digitalWrite(a, LOW);
	}
	else if(step == 1){
		step = 0;
		digitalWrite(a, HIGH);
		digitalWrite(b, LOW);
	}
	else if(step == 2){
		step = 1;
		digitalWrite(b, HIGH);
		digitalWrite(c, LOW);
	}
	else if(step == 3){
		step = 2;
		digitalWrite(c, HIGH);
		digitalWrite(d, LOW);
	}

	pos -= 1;
}

int miniStepper::getPos(){
	return(pos);
}