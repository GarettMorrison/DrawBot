#include <Servo.h>
#include <math.h>
#include "miniStepper.h"

byte val; // variable to receive data from the serial port
int ledpin = 13; // Arduino LED pin 13 (on-board LED)

//Settings that can be changed in code, values are in 1/10"
int stepDelay = 7;
int stepDist = 52;
int lInit = 510;
int base = 455;
int lServoPos[] = {35, 135};  
int rServoPos[] = {125,45};
int imageScale = 60;
int lineXYDistStep = 20;
//Reference vals by pointer set
int *params[] = {&stepDelay, &stepDist, &lInit, &base, &lServoPos[0], &lServoPos[1], &rServoPos[0], &rServoPos[1], &imageScale, &lineXYDistStep};

float w0 = base/2;
float h0 = sqrt(pow(lInit, 2) - pow(base/2, 2));

byte command;
byte inBytes[10];

miniStepper left;
miniStepper right;

Servo lServo;
Servo rServo;


int twoByteInt(int MSB, int LSB){
   long int sum = (MSB*256 +LSB) -32767;
   if(sum > 32767){sum = 32767;}
   if(sum < -32767){sum = -32767;}
   return(sum);
}

float getDist(float x, float y){
   return(sqrt(pow(x, 2) + pow(y, 2)));
}

void getLR_fromXY(long int* outLR, float X, float Y){ //Converts XY raws into LR step positions
   float w = (X/32000)*imageScale + w0; //x position in dim + init width
   float h = (-1* Y/32000)*imageScale + h0; //y pos in dim + init h
   
   // Serial.println(X);
   // Serial.println(Y);
   // Serial.println('-');
   // Serial.println(w);
   // Serial.println(h);
   
   outLR[0] = floor((getDist(w      ,h) -lInit) *stepDist); //L position
   outLR[1] = floor((getDist(base -w,h) -lInit) *stepDist); //R position

   // Serial.println('-');
   // Serial.println(outLR[0]);
   // Serial.println(outLR[1]);
}

void getXY_fromLR(long int* outXY, float L, float R){
   float w = (pow(L/stepDist +lInit,2) - pow(R/stepDist +lInit,2) + pow(base,2)) / (2*base);
   float h = sqrt(pow(L/stepDist +lInit,2) - pow(w,2));

   outXY[0] = (w -w0) * (32000/imageScale);
   outXY[1] = (h -h0) * (32000/imageScale) *-1;

   // Serial.println('-');
   // Serial.println(w);
   // Serial.println(h);

   // Serial.println('-');
   // Serial.println(outXY[0]);
   // Serial.println(outXY[1]);
   // Serial.println("-----");
}

void moveR(int lTarget, int rTarget){
   bool doLoop = true;

   while(doLoop){
      int rPos = right.getPos();
      int lPos = left.getPos();

      if(lTarget == lPos && rTarget == rPos){
         doLoop = false;
         break;
      }

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
   }
}



// void doLine(int xTarget, int yTarget){
//    //Get start of line from current pos
//    int startXY[2];
//    getXY_fromLR(startXY, left.getPos(), right.getPos());

//    //Calculated vals
//    int xDist = xTarget - startXY[0];
//    int yDist = yTarget - startXY[1];
//    float dist = getDist(xDist, yDist);
//    int pos = 0;

//    int targLR[2];

//    Serial.println("-");

//    Serial.print("Targs: ");
//    Serial.print(xTarget);
//    Serial.print(" | ");
//    Serial.println(yTarget);


//    Serial.print("StartPos: ");
//    Serial.print(startXY[0]);
//    Serial.print(" | ");
//    Serial.println(startXY[1]);



//    Serial.print("Dists: ");
//    Serial.print(dist);
//    Serial.print(" | ");
//    Serial.print(xDist);
//    Serial.print(" | ");
//    Serial.println(yDist);

//    while(true){
//       float frac = pos/dist;

//       //Break if after line
//       if(frac > 1){
//          break;
//       }

//       int xPos = frac*xDist +startXY[0];
//       int yPos = frac*yDist +startXY[1];

//       getLR_fromXY(targLR, xPos, yPos);

//       Serial.print(floor(frac*1000)); Serial.print(" | "); Serial.print(xPos); Serial.print(" | "); Serial.print(yPos);Serial.print(" | "); Serial.print(targLR[0]); Serial.print(" | "); Serial.println(targLR[1]);

//       moveR(targLR[0], targLR[1]); //Move to this pos



//       pos += lineXYDistStep; //Increment pos
//    }
// }



void setup() {
   Serial.begin(9600); // start serial communication at 9600bps

   left.attach(2,3,4,5);
   right.attach(7,8,11,12);

   lServo.attach(9);
   lServo.write(90);
   rServo.attach(10);
   rServo.write(90);

   pinMode(ledpin, OUTPUT); // pin 13 (on-board LED) as OUTPUT



   //---------------------------------------------------- Test functions

   // Serial.println("------");

   //    stepDelay = 1;
   //    int startXY[2];
   //    int targLR[2];
   //    getXY_fromLR(startXY, left.getPos(), right.getPos());

   //    int xTarget = -8767;
   //    int yTarget = -32767;

   //    //Calculated vals
   //    int xDiff = xTarget - startXY[0];
   //    int yDiff = yTarget - startXY[1];
   //    float dist = getDist(xDiff, yDiff);
   //    long int pos = lineXYDistStep;


   //    while(true){
   //       float frac = pos/dist;

   //       //Break if after line
   //       if(frac > 1){
   //          break;
   //       }

   //       int xPos = floor(frac*xDiff +startXY[0]);
   //       int yPos = floor(frac*yDiff +startXY[1]);

   //       getLR_fromXY(targLR, xPos, yPos);

   //       Serial.print(floor(frac*1000)); Serial.print(" |x: "); Serial.print(xPos); Serial.print(" |y: "); Serial.print(yPos); Serial.print(" |tx "); Serial.print(targLR[0]); Serial.print(" |ty "); Serial.println(targLR[1]);
   //       moveR(targLR[0], targLR[1]); //Move to this pos


   //       pos += lineXYDistStep; //Increment pos
   //    }


   // ---------------------------------------------------- End Test functions


}


void loop() {
   Serial.readBytes(& command, 1);

   //MoveRad
   if(command == 1){ 
      Serial.readBytes(inBytes, 4);
      long int lTarget = twoByteInt(inBytes[0], inBytes[1]); //Get int value, adjust to +/-
      long int rTarget = twoByteInt(inBytes[2], inBytes[3]); //Get int value, adjust to +/-

      // moveR(lTarget, rTarget); //Move to this pos
   }

   //AdjustRad
   else if(command == 2){ 
      Serial.readBytes(inBytes, 4);
      long int lTarget = left.getPos() + twoByteInt(inBytes[0], inBytes[1]); //Get int value, adjust to +/-
      long int rTarget = right.getPos() + twoByteInt(inBytes[2], inBytes[3]); //Get int value, adjust to +/-

      moveR(lTarget, rTarget); //Move to this pos
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
      int xTarget = twoByteInt(inBytes[0], inBytes[1]); //Get int value, adjust to +/-
      int yTarget = twoByteInt(inBytes[2], inBytes[3]); //Get int value, adjust to +/-

      long int targsLR[2];//Convert to LR
      getLR_fromXY(targsLR, xTarget, yTarget);

      moveR(targsLR[0], targsLR[1]); //Move to this pos
   }

   //Adjust
   else if(command == 8){ 
      Serial.readBytes(inBytes, 4);
      int xTarget = twoByteInt(inBytes[0], inBytes[1]); //Get int value, adjust to +/-
      int yTarget = twoByteInt(inBytes[2], inBytes[3]); //Get int value, adjust to +/-

      long int targsLR[2]; //Convert to LR
      getLR_fromXY(targsLR, xTarget, yTarget);
      targsLR[0] += left.getPos(); //Add inits, to make adjust
      targsLR[1] += right.getPos();

      moveR(targsLR[0], targsLR[1]); //Move to this pos
   }

   //Line
   else if(command == 10){
      Serial.readBytes(inBytes, 4);
      int xTarget = twoByteInt(inBytes[0], inBytes[1]); //Get int value, adjust to +/-
      int yTarget = twoByteInt(inBytes[2], inBytes[3]); //Get int value, adjust to +/-


      //Get start of line from current pos
      long int startXY[2];
      long int targLR[2];

      getXY_fromLR(startXY, left.getPos(), right.getPos());

      //Calculated vals
      long int xDiff = xTarget - startXY[0];
      long int yDiff = yTarget - startXY[1];
      float dist = getDist(xDiff, yDiff);
      long int pos = lineXYDistStep;


      while(true){
         float frac = pos/dist;

         //Break if after line
         if(frac > 1){
            break;
         }

         int xPos = floor(frac*xDiff +startXY[0]);
         int yPos = floor(frac*yDiff +startXY[1]);

         getLR_fromXY(targLR, xPos, yPos);

         moveR(targLR[0], targLR[1]); //Move to this pos

         pos += lineXYDistStep; //Increment pos
      }
   }


   //setVal
   else if(command == 9){ 
      Serial.readBytes(inBytes, 3);
      int var = inBytes[0]; //Get selected variable
      int val = twoByteInt(inBytes[1], inBytes[2]); //Get int value to set to

      *params[var] = val;

      w0 = base/2;
      h0 = sqrt(pow(lInit, 2) - pow(base/2, 2));
   }


   if(command != 0){
      Serial.write(command);
   }
   command = 0;

   delay(10);
}