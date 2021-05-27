#include "steps.h"
// #include <Servo.h>

static int delayTime = 3;
static int stepsPerMove = 4;

class drawServo{
	public:
		drawServo(int pin);
		void penUp();
		void penDown();
	private:
		// bool isPenDown;
		// int thisServo;
		int pin;
};

drawServo::drawServo(int inPin){
	pin = inPin;
	pinMode(pin, OUTPUT);
	// thisServo.attach(pin);
	// thisServo.write(90);
	// isPenDown = false;
}

void drawServo::penUp(){
	analogWrite(pin, 180);
	// thisServo.write(90);
}
void drawServo::penDown(){
	analogWrite(pin, 200);
	// thisServo.write(180);
}


class stepper{
	public:
		// stepper();
		stepper(int A, int B, int C, int D);
		void stepCCW();
		void stepCW();
		void disable();
		long int getStep();

	private:
		int aPin;
		int bPin;
		int cPin;
		int dPin;
		int stepPos;
		long int stepCount;
};

stepper::stepper(int A, int B, int C, int D){
	aPin = A;
	bPin = B;
	cPin = C;
	dPin = D;

	pinMode(aPin, OUTPUT);
	pinMode(bPin, OUTPUT);
	pinMode(cPin, OUTPUT);
	pinMode(dPin, OUTPUT);

	stepPos = 0;
	stepCount = 0;

	digitalWrite(aPin, HIGH);
	digitalWrite(bPin, LOW);
	digitalWrite(cPin, LOW);
	digitalWrite(dPin, LOW);
}

// stepper::stepper(){
// 	aPin = -1;
// 	bPin = -1;
// 	cPin = -1;
// 	dPin = -1;
// 	stepPos = 0;
// 	stepCount = 0;
// }

void stepper::stepCCW(){
	if(stepPos == 0){
		digitalWrite(aPin, LOW);
		digitalWrite(bPin, HIGH);
		digitalWrite(cPin, LOW);
		digitalWrite(dPin, LOW);

		stepPos = 1;
	}
	else if(stepPos == 1){
		digitalWrite(aPin, LOW);
		digitalWrite(bPin, LOW);
		digitalWrite(cPin, HIGH);
		digitalWrite(dPin, LOW);

		stepPos = 2;
	}
	else if(stepPos == 2){
		digitalWrite(aPin, LOW);
		digitalWrite(bPin, LOW);
		digitalWrite(cPin, LOW);
		digitalWrite(dPin, HIGH);

		stepPos = 3;
	}
	else if(stepPos == 3){
		digitalWrite(aPin, HIGH);
		digitalWrite(bPin, LOW);
		digitalWrite(cPin, LOW);
		digitalWrite(dPin, LOW);

		stepPos = 0;
	}

	stepCount++;
	delay(delayTime);
}

void stepper::stepCW(){
	if(stepPos == 2){
		digitalWrite(aPin, LOW);
		digitalWrite(bPin, HIGH);
		digitalWrite(cPin, LOW);
		digitalWrite(dPin, LOW);

		stepPos = 1;
	}
	else if(stepPos == 3){
		digitalWrite(aPin, LOW);
		digitalWrite(bPin, LOW);
		digitalWrite(cPin, HIGH);
		digitalWrite(dPin, LOW);

		stepPos = 2;
	}
	else if(stepPos == 0){
		digitalWrite(aPin, LOW);
		digitalWrite(bPin, LOW);
		digitalWrite(cPin, LOW);
		digitalWrite(dPin, HIGH);

		stepPos = 3;
	}
	else if(stepPos == 1){
		digitalWrite(aPin, HIGH);
		digitalWrite(bPin, LOW);
		digitalWrite(cPin, LOW);
		digitalWrite(dPin, LOW);

		stepPos = 0;
	}

	stepCount--;
	delay(delayTime);
}

void stepper::disable(){
	digitalWrite(aPin, LOW);
	digitalWrite(bPin, LOW);
	digitalWrite(cPin, LOW);
	digitalWrite(dPin, LOW);
}

long int stepper::getStep(){
	return(stepCount);
}



//Declare our two steppers
stepper left(30,32,34,36);
stepper right(38,40,42,44);
drawServo draw(4);


/*
left 
CCW = down
CW = up

right
CCW = up
cw = down
*/

void runStep(bool pen, bool S0, bool S1, bool S2){
	switch(pen){
		case true:
			draw.penDown();
			//Make servo go down
			break;
		case false:
			draw.penUp();
			//make servo go up
			break;
	}

	/*
	cases
	^^ 7
	vv 6
	^v 5
	v^ 4
	^- 3
	v- 2
	-^ 1
	-v 0
	*/

	int stepperMove = S0*4 + S1*2 + S2;
	// Serial.println(stepperMove);
	for(int i = 0; i < stepsPerMove; i++){
		switch(stepperMove){
		case 0:
			left.stepCW();
			right.stepCCW();
			break;
		case 1:
			left.stepCCW();
			right.stepCW();
			break;
		case 2:
			left.stepCW();
			right.stepCW();
			break;
		case 3:
			left.stepCCW();
			right.stepCCW();
			break;
		case 4:
			left.stepCW();
			break;
		case 5:
			left.stepCCW();
			break;
		case 6:
			right.stepCCW();
			break;
		case 7:
			right.stepCW();
			break;

		}
	}
}

void setup(){
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(3, INPUT_PULLUP);
	pinMode(2, INPUT_PULLUP);

	Serial.begin(9600);
	Serial.println("BOOTED");

	right.disable();
	left.disable();

	bool startTests[3] = {false, false, false};
	while(1 == 1){
		draw.penDown();

		startTests[0] = digitalRead(3);
		startTests[1] = digitalRead(3);
		startTests[2] = digitalRead(3);

		// Serial.println(startTests[0]);

		if(startTests[0] && startTests[1] && startTests[2]){
			break;
		}
	}

	while(1 == 1){
		draw.penUp();

		startTests[0] = digitalRead(2);
		startTests[1] = digitalRead(2);
		startTests[2] = digitalRead(2);

		// Serial.println(startTests[0]);

		if(startTests[0] && startTests[1] && startTests[2]){
			break;
		}
	}


	Serial.println("STARTING");

	for(int i = 0; i < stepCount; i++){
		byte spliceByte = pgm_read_byte_near(steps + i);
		// Serial.println(spliceByte);
		//Split byte into stepBoolss
		bool stepBoolsA[4] = {false,false,false,false};
		bool stepBoolsB[4] = {false,false,false,false};


		if(spliceByte > 127){ //Pen = 1
			spliceByte -= 128;
			stepBoolsA[0] = true;
		}
		if(spliceByte > 63){ //S0 = 1
			spliceByte -= 64;
			stepBoolsA[1] = true;
		}
		if(spliceByte > 31){ //S1 = 1
			spliceByte -= 32;
			stepBoolsA[2] = true;
		}
		if(spliceByte > 15){ //S2 = 1
			spliceByte -= 16;
			stepBoolsA[3] = true;
		}

		if(spliceByte > 7){ //Pen = 1
			spliceByte -= 8;
			stepBoolsB[0] = true;
		}
		if(spliceByte > 3){ //S0 = 1
			spliceByte -= 4;
			stepBoolsB[1] = true;
		}
		if(spliceByte > 1){ //S1 = 1
			spliceByte -= 2;
			stepBoolsB[2] = true;
		}
		if(spliceByte > 0){ //S2 = 1
			stepBoolsB[3] = true;
		}

		runStep(stepBoolsA[0], stepBoolsA[1], stepBoolsA[2], stepBoolsA[3]);
		runStep(stepBoolsB[0], stepBoolsB[1], stepBoolsB[2], stepBoolsB[3]);
	}

	// for(int i = 0; i < 200; i++){
	// 	right.stepCCW();
	// }

	// for(int i = 0; i < 200; i++){
	// 	right.stepCW();
	// }

	Serial.println("Returning");

	int lStep = left.getStep();
	int rStep = right.getStep();
	do{
		lStep = left.getStep();
		rStep = right.getStep();
		if(left.getStep() < 0){left.stepCCW();}
		else if(left.getStep() > 0){left.stepCW();}

		if(right.getStep() < 0){right.stepCCW();}
		else if(right.getStep() > 0){right.stepCW();}
		Serial.println(lStep);
	}while(lStep != 0 && rStep != 0);

	draw.penUp();
	right.disable();
	left.disable();
	Serial.println("Finished");
}

void loop() {
	Serial.println("DONE!");
	digitalWrite(LED_BUILTIN, HIGH);
	// draw.penDown();
	delay(5000);
	digitalWrite(LED_BUILTIN, LOW);
	// draw.penUp();
	delay(5000);
}
