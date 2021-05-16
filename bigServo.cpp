/*
	bigServo.h
	Servo using DC motor, L298N, and potetiometer.
	Made by Garett Morrison
*/

#include "Arduino.h"
#include "bigServo.h"

const int linRange = 250;
const int okRange = 75;

bigServo::attach(int _sPin, int _aPin, int _bPin, int _potPin){
	//Save pin vals
	sPin = _sPin;
	aPin = _aPin;
	bPin = _bPin;
	potPin = _potPin;

	//Init outputs
	pinMode(aPin, OUTPUT);
	pinMode(bPin, OUTPUT);

	//Set initial val to not move
	target = analogRead(potPin);
}

bigServo::setTarget(int _target){
	target = _target;
}


int bigServo::upDate(){
	//Calculate difference
	currVal = analogRead(potPin);
	bool extend = (target > currVal);
	int diff = abs(target - currVal);

	bool doMove = true;

	int speed = 0;
	if(diff < okRange){
		doMove = false;
		speed = 0;
	}
	// else if(diff < linRange +okRange){
	// 	speed = floor(255 * (diff -okRange) / linRange);
	// }
	else{
		speed = 255;
	}

	analogWrite(sPin, speed);

	// if(doMove){
		if(extend){
			digitalWrite(aPin, HIGH);
			digitalWrite(bPin, LOW );
		}
		else{
			digitalWrite(aPin, LOW );
			digitalWrite(bPin, HIGH);

		}
	// }


	return(currVal);
}