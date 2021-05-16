#include <Servo.h>
#include "bigServo.h"

/*Pin decs
const int LMS = 22;
const int LMA = 23;
const int LMB = 24;
const int RMA = 25;
const int RMB = 26;
const int RMS = 27;

const int ServoPin = 2;

const int Lpot = 0;
const int Rpot = 1;
*/

bigServo left;
bigServo right;

Servo pen;

void setup() {
	left.attach(6,7,8,0);
	right.attach(3,5,2,1);

	pen.attach(9);
	Serial.begin(9600);
}

void loop() {

	// unsigned long time = millis();
	// int pos = (int(time / 1000) % 3);

	// if(pos == 0){
	// 	right.setTarget(random(100, 923));
	// }
	// else if(pos == 1){
	// 	pen.write(random(0, 180));
	// }
	// else if(pos == 2){

	// }

	left.upDate();  
	right.upDate();

	// Serial.print(left.upDate());
	// Serial.print(" - ");
	// Serial.print(right.upDate());
	// Serial.println("");
	// delay(250);
}