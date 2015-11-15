/*************************************************************************
* File Name          : TestStepper.ino
* Author             : Mark Wong , modified from  xiaoyu/makeblock
* Version            : V1.0.0
* Date               : 06/11/2015
* Parts required     : Me Stepper Driver , Stepper motor
* Description        : XY beam scanner for AlCap

* License            : CC-BY-SA 3.0
* Modified from Copyright (C) 2013 Maker Works Technology Co., Ltd. All right reserved.
* http://www.makeblock.cc/
**************************************************************************/

//Stepper Driver connection
//connect  	1A and 1B to stepper coil 1  nornally  black and green wire
//connect 	2A and 2B to stepper coil 2  nornally red and blue wire

#include <Makeblock.h>
#include <SoftwareSerial.h>
#include <Wire.h>


int dirPin = mePort[PORT_1].s1; //the direction pin connect to Base Board PORT1 SLOT1
int stpPin = mePort[PORT_1].s2; //the Step pin connect to Base Board PORT1 SLOT2

int dirPin2 = mePort[PORT_2].s1;
int stpPin2 = mePort[PORT_2].s2;

int Xpos=0, Ypos=0;
int directionX = 0, directionY=0;
int count = 0;

char incomingByte;
int start = 0, crashToWall = 0;
void setup()
{
	pinMode(dirPin, OUTPUT);
	pinMode(stpPin, LOW);

  pinMode(dirPin2, OUTPUT);
  pinMode(stpPin2, LOW);

  Serial.begin(9600);
}

void stepX(boolean dir,int steps)
{
	digitalWrite(dirPin,dir);
	delay(50);
	for(int i=0; i<steps; i++){
    pinMode(stpPin, HIGH);
    //digitalWrite(stpPin, HIGH);
    delayMicroseconds(1000);
    //digitalWrite(stpPin, LOW);
    delayMicroseconds(1000);
    pinMode(stpPin, LOW);
	}
  Xpos++;
}
void stepY(boolean dir, int steps)
{
  digitalWrite(dirPin2,dir); 
  delay(50);
  for(int j=0; j<steps; j++){
    pinMode(stpPin2, HIGH);
    //digitalWrite(stpPin2, HIGH);
    delayMicroseconds(2750);
    //digitalWrite(stpPin2, LOW);
    delayMicroseconds(2750);
    pinMode(stpPin2, LOW);
    Ypos = j;
    Serial.print(Xpos); Serial.print(" "); Serial.println(Ypos);
    Serial.flush();
  }
}
void loop()
{
  delay(50);
  if(Serial.available() > 0) {
    incomingByte = Serial.read();
    if(incomingByte == 'p') start = 1; //p
    if(incomingByte == 'l') start = 0; //l
    if(incomingByte == 'o') crashToWall = 1; //o
    Serial.println(incomingByte);
    Serial.println(start);
    Serial.print(incomingByte); Serial.println(" command sent.");
  }
  if(start) {
  	stepY(directionY,8000); //90mm
	  delay(500);
    stepX(directionX,50); //~2mm
    delay(500);
    directionY = (directionY==0) ? 1 : 0;
    count++;
    if(count == 5) { //90
      directionX = (directionX==0) ? 1 : 0;
      count = 0;
    }
  }
  if(crashToWall) {
    
  }
}
