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
//connect       1A and 1B to stepper coil 1  nornally  black and green wire
//connect       2A and 2B to stepper coil 2  nornally red and blue wire

#include <Makeblock.h>
#include <SoftwareSerial.h>
#include <Wire.h>


int dirPin = mePort[PORT_1].s1;//the direction pin connect to Base Board PORT1 SLOT1
int stpPin = mePort[PORT_1].s2;//the Step pin connect to Base Board PORT1 SLOT2

int dirPin2 = mePort[PORT_2].s1;
int stpPin2 = mePort[PORT_2].s2;

int direction = 0, direction2 = 0;
int count = 0;

void setup()
{
  pinMode(dirPin, OUTPUT);
  pinMode(stpPin, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(stpPin2, OUTPUT);

}

void step(boolean dir,int steps)
{
  digitalWrite(dirPin,dir);
  delay(50);
  for(int i=0; i<steps; i++){
    digitalWrite(stpPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stpPin, LOW);
    delayMicroseconds(1000);
  }
}
void step2(boolean dir, int steps)
{
  digitalWrite(dirPin2,dir); 
  delay(50);
  for(int j=0; j<steps; j++){
    digitalWrite(stpPin2, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stpPin2, LOW);
    delayMicroseconds(1000);
  }
}
void loop()
{
	step2(direction2,8000); //90mm
	delay(1000);
  step(direction,20); //1mm
  delay(1000);
  direction2 = (direction2==0) ? 1 : 0;
  count++;
  if(count == 90) {
    direction = (direction==0) ? 1 : 0;
    count = 0; //reset counter when reversing X-direction
  }
}
