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
int directionX = 0, directionY=0; //initial movement right & up
int count = 0;

char incomingByte;
int start = 0, crashToWall = 0;
void setup()
{
  pinMode(dirPin, OUTPUT);
  pinMode(stpPin, OUTPUT);

  pinMode(dirPin2, OUTPUT);
  pinMode(stpPin2, OUTPUT);

  Serial.begin(9600);
}

void stepX(boolean dir,int steps)
{
  digitalWrite(dirPin,dir);
  delay(50);
  for(int i=0; i<steps; i++) {
    pinMode(stpPin, HIGH);
    delayMicroseconds(2000);
    pinMode(stpPin, LOW);
  }
  if(dir == 0) {
    Xpos++;
  } else {
    Xpos--;
  }
}
void stepY(boolean dir, int steps)
{
  digitalWrite(dirPin2,dir); 
  delay(50);
  for(int j=0; j<steps; j++){
    pinMode(stpPin2, HIGH);
    delayMicroseconds(5000);
    pinMode(stpPin2, LOW);
    if(directionY == 0) { 
      Ypos = j; 
    } else { 
      Ypos = steps-j-1; 
    }
    Serial.print(Xpos); Serial.print(" "); Serial.println(Ypos);
  }
}
void move(int X, int Y) {
  int dirX = (X > Xpos) ? 0 : 1;
  int dirY = (Y > Ypos) ? 0 : 1;
  while(Xpos != X) {
    stepX(dirX, 50);
    Serial.println(Xpos);
  }
  while(Ypos != Y) {
    stepY(dirY, 1000);
    Serial.println(Ypos);
  }
}
void loop() {
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
    stepY(directionY, 1000); //90mm
    delay(500);
    stepX(directionX, 50); //~2mm
    delay(500);
    directionY = (directionY==0) ? 1 : 0;
    if(count >= 100) { //90
      directionX = (directionX==0) ? 1 : 0;
      count--;
    } else {
      count++;
    }
  }
  if(crashToWall) {
    //crashToWall logic
    start = 0;
    move(0, 0);
    directionX = 0; directionY=0; Xpos=0; Ypos=0;
    crashToWall = 0;
  }
}
