#include <Servo.h>
#include <math.h>
#include "miniStepper.h"

byte val; // variable to receive data from the serial port
int ledpin = 13; // Arduino LED pin 13 (on-board LED)

//Settings that can be changed in code, values are in 1/10"
int stepDelay = 10;
int stepDist = 52;
int lInit = 710;
int base = 620;
int lServoPos[] = {85, 95};
int rServoPos[] = {95,85};
int imageScale = 60;
int lineDimPeriod = 520;
//Reference vals by pointer set
int *params[] = {&stepDelay, &stepDist, &lInit, &base, &lServoPos[0], &lServoPos[1], &rServoPos[0], &rServoPos[1], &imageScale, &lineDimPeriod};

float w0 = base/2;
float h0 = sqrt(pow(lInit, 2) - pow(base/2, 2));

byte command;
byte inBytes[10];

miniStepper left;
miniStepper right;

Servo lServo;
Servo rServo;


int twoByteInt(int MSB, int LSB){
   int sum = MSB*256 +LSB;
   return(sum);
}

float getDist(float x, float y){
   return(sqrt(pow(x, 2) + pow(y, 2)));
}

void getLR_fromXY(int* outLR, float X, float Y){ //Converts XY raws into LR step positions
   float w = (X/32767)*imageScale + w0; //x position in dim + init width
   float h = (-1* Y/32767)*imageScale + h0; //y pos in dim + init h
   
   // Serial.println(X);
   // Serial.println(Y);
   // Serial.println('-');
   // Serial.println(w);
   // Serial.println(h);
   
   outLR[0] = floor(getDist(w,h) - lInit*stepDist); //L position
   outLR[1] = floor(getDist(base -w,h) -lInit*stepDist); //R position

   // Serial.println('-');
   // Serial.println(outLR[0]);
   // Serial.println(outLR[1]);
}

void getXY_fromLR(int* outXY, float R, float L){
   float w = (pow(L/stepDist +lInit,2) - pow(R/stepDist +lInit,2) + pow(base,2)) / (2*base);
   float h = sqrt(pow(L/stepDist +lInit,2) - pow(w,2));

   outXY[0] = (w -w0) * (32767/imageScale);
   outXY[1] = (h -h0) * (32767/imageScale) *-1;

   // Serial.println('-');
   // Serial.println(w);
   // Serial.println(h);

   // Serial.println('-');
   // Serial.println(outXY[0]);
   // Serial.println(outXY[1]);
   // Serial.println("-----");
}


void setup() {
   Serial.begin(9600); // start serial communication at 9600bps

   // Serial.println("-----");
   int LR[] = {0,0};
   getLR_fromXY(LR, 0, 0);
   int XY[] = {0,0};
   getXY_fromLR(XY, LR[0], LR[1]);


   left.attach(2,3,4,5);
   right.attach(7,8,11,12);

   lServo.attach(9);
   lServo.write(90);
   rServo.attach(10);
   rServo.write(90);

   pinMode(ledpin, OUTPUT); // pin 13 (on-board LED) as OUTPUT
}


void loop() {
   Serial.readBytes(& command, 1);

   //MoveRad
   if(command == 1){ 
      Serial.readBytes(inBytes, 4);
      int lTarget = twoByteInt(inBytes[0], inBytes[1]) -32767; //Get int value, adjust to +/-
      int rTarget = twoByteInt(inBytes[2], inBytes[3]) -32767; //Get int value, adjust to +/-

      bool doLoop = true;

      while(doLoop){
         int rPos = right.getPos();
         int lPos = left.getPos();

         if(lTarget < lPos){
            left.stepDown();
         }
         else if(lTarget > lPos){
            left.stepUp();
         }

         if(rTarget < rPos){
            right.stepDown();
         }
         else if(rTarget > rPos){
            right.stepUp();
         }

         
         delay(stepDelay);

         if(lTarget == lPos && rTarget == rPos){
            doLoop = false;
         }
      }
   }

   //AdjustRad
   else if(command == 2){ 
      Serial.readBytes(inBytes, 4);
      int lTarget = left.getPos() + twoByteInt(inBytes[0], inBytes[1]) -32767; //Get int value, adjust to +/-
      int rTarget = right.getPos() + twoByteInt(inBytes[2], inBytes[3]) -32767; //Get int value, adjust to +/-

      bool doLoop = true;

      while(doLoop){
         int rPos = right.getPos();
         int lPos = left.getPos();

         if(lTarget < lPos){
            left.stepDown();
         }
         else if(lTarget > lPos){
            left.stepUp();
         }

         if(rTarget < rPos){
            right.stepDown();
         }
         else if(rTarget > rPos){
            right.stepUp();
         }

         
         delay(stepDelay);

         if(lTarget == lPos && rTarget == rPos){
            doLoop = false;
         }
      }
   }

   //Flash
   else if(command == 3){ 
      Serial.readBytes(inBytes, 1);
      for(int i = 0; i < int(inBytes[0]); i++){
         digitalWrite(ledpin, HIGH);
         delay(250);
         digitalWrite(ledpin, LOW);
         delay(250);
      }
   }

   //Pen
   else if(command == 5){ 
      Serial.readBytes(inBytes, 1);
      switch (inBytes[0]) {
          case 0: //All pens up
            lServo.write(90);
            rServo.write(90);
            break;
          case 1: //Pen 1 down
            lServo.write(lServoPos[0]);
            rServo.write(90);
            break;
          case 2: //Pen 2 down
            lServo.write(90);
            rServo.write(rServoPos[0]);
            break;
          case 3: //Pen 3 down
            lServo.write(lServoPos[1]);
            rServo.write(90);
            break;
          case 4: //Pen 4 down
            lServo.write(90);
            rServo.write(rServoPos[1]);
            break;
      }
   }

   //Move
   if(command == 7){ 
      Serial.readBytes(inBytes, 4);
      int xTarget = twoByteInt(inBytes[0], inBytes[1]) -32767; //Get int value, adjust to +/-
      int yTarget = twoByteInt(inBytes[2], inBytes[3]) -32767; //Get int value, adjust to +/-

      int targsLR[2];//Convert to LR
      getLR_fromXY(targsLR, xTarget, yTarget);

      bool doLoop = true;

      while(doLoop){
         int rPos = right.getPos();
         int lPos = left.getPos();

         if(targsLR[0] < lPos){
            left.stepDown();
         }
         else if(targsLR[0] > lPos){
            left.stepUp();
         }

         if(targsLR[1] < rPos){
            right.stepDown();
         }
         else if(targsLR[1] > rPos){
            right.stepUp();
         }

         
         delay(stepDelay);

         if(targsLR[0] == lPos && targsLR[1] == rPos){
            doLoop = false;
         }
      }
   }

   //Adjust
   else if(command == 8){ 
      Serial.readBytes(inBytes, 4);
      int xTarget = twoByteInt(inBytes[0], inBytes[1]) -32767; //Get int value, adjust to +/-
      int yTarget = twoByteInt(inBytes[2], inBytes[3]) -32767; //Get int value, adjust to +/-

      int targsLR[2]; //Convert to LR
      getLR_fromXY(targsLR, xTarget, yTarget);
      targsLR[0] += left.getPos(); //Add inits, to make adjust
      targsLR[1] += right.getPos();

      bool doLoop = true;

      while(doLoop){
         int rPos = right.getPos();
         int lPos = left.getPos();

         if(targsLR[0] < lPos){
            left.stepDown();
         }
         else if(targsLR[0] > lPos){
            left.stepUp();
         }

         if(targsLR[1] < rPos){
            right.stepDown();
         }
         else if(targsLR[1] > rPos){
            right.stepUp();
         }

         
         delay(stepDelay);

         if(targsLR[0] == lPos && targsLR[1] == rPos){
            doLoop = false;
         }
      }
   }

   //Line
   else if(command == 10){
      Serial.readBytes(inBytes, 4);
      int xTarget = twoByteInt(inBytes[0], inBytes[1]) -32767; //Get int value, adjust to +/-
      int yTarget = twoByteInt(inBytes[2], inBytes[3]) -32767; //Get int value, adjust to +/-

      //Get start of line from current pos
      int startXY[2];
      getXY_fromLR(startXY, left.getPos(), right.getPos());

      //Calculated vals
      float dist = getDist(startXY[0] -xTarget, startXY[1] -yTarget);
      int xDiff = xTarget - startXY[0];
      int yDiff = yTarget - startXY[1];
      int pos = 0;

      bool doPause = false;

      int targLR[2];

      while(1 == 1){
         float frac = pos/dist;

         int xPos = frac*xDiff +startXY[0];
         int yPos = frac*yDiff +startXY[1];

         getLR_fromXY(targLR, xPos, yPos);

         int lGap = targLR[0] - left.getPos();
         int rGap = targLR[1] - right.getPos();

         //Move if out of bounds
         if(lGap < 0){
            left.stepDown();
            doPause = true;
         }
         else if(lGap > 0){
            left.stepUp();
            doPause = true;
         }

         if(rGap < 0){
            right.stepDown();
            doPause = true;
         }
         else if(rGap > 0){
            right.stepUp();
            doPause = true;
         }

         //If move do pause
         if(doPause){
            delay(stepDelay);
         }

         //Break if after line
         if(frac > 1){
            break;
         }

         pos += 1; //Increment pos
      }
   }


   //setVal
   else if(command == 9){ 
      Serial.readBytes(inBytes, 3);
      int var = inBytes[0]; //Get selected variable
      int val = twoByteInt(inBytes[1], inBytes[2]) -32767; //Get int value to set to

      *params[var] = val;
   }


   if(command != 0){
      Serial.write(command);
   }
   command = 0;

   delay(10);
}