
int stepTime = 3;
int expandCount = 30;

class stepper{
	public:
		void stepCCW();
		void stepCW();
		void powerOff();
		long int getStep();
		stepper(int A, int B, int C, int D);

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
}

void stepper::powerOff(){
	digitalWrite(aPin, LOW);
	digitalWrite(bPin, LOW);
	digitalWrite(cPin, LOW);
	digitalWrite(dPin, LOW);
}

long int stepper::getStep(){
	return(stepCount);
}

/*
left 
CCW = down
CW = up

right
CCW = up
cw = down


*/
void setup(){
	stepper right(38,40,42,44);
	stepper left(30,32,34,36);

	int i = 0;
	while(1 == 1){

		for(int j = 0; j < i; j++){
			left.stepCW();
			delay(stepTime);
		}
		i += expandCount;
		left.powerOff();

		for(int j = 0; j < i; j++){
			right.stepCW();
			delay(stepTime);
		}
		i += expandCount;
		right.powerOff();


		for(int j = 0; j < i; j++){
			left.stepCCW();
			delay(stepTime);
		}
		i += expandCount;
		left.powerOff();
		
		for(int j = 0; j < i; j++){
			right.stepCCW();
			delay(stepTime);
		}
		i += expandCount;
		right.powerOff();
	}
}



void loop(){
	delay(100000);
}