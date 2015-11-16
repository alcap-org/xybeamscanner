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
#include <SoftwareSerial.h>


//L L L   Full Step   2 Phase
//H L L   Half Step   1-2 Phase
//L H L   Quarter Step  W1-2 Phase
//H H L   Eighth Step   2W1-2 Phase
//H H H   Sixteenth Step  4W1-2 Phase  =>    

// steps/mm fraction DIP setting
//   5.5       0.5     L L L  //don't use!
//  11         1       H L L
//  22         2       L H L
//  44         3       H H L
//  88         4       H H H      
#define BASE_STEPS_PER_MM 11
#define FRACTION 4


//Connect port 2 to the y-xais stepper.
//Connnect port 1 to the x-axis stepper.
#define X_PORT PORT_1
#define Y_PORT PORT_2

int dirPinX = mePort[X_PORT].s1;//the direction pin connect to Base Board PORT1 SLOT1
int stpPinX = mePort[X_PORT].s2;//the Step pin connect to Base Board PORT1 SLOT2

int dirPinY = mePort[Y_PORT].s1;
int stpPinY = mePort[Y_PORT].s2;

//direction 0 is toward the stepper motors

int xx = 0; int yy =0;
int direction = 0, direction2 = 0;  
int count = 0;

int initialised = 0;

//-------1---------2---------3---------4---------5---------6---------7---------8
int mm_to_step(float mm){
  return floor(mm * BASE_STEPS_PER_MM * FRACTION + 0.5);
}

float step_to_mm(int steps){
  return (1. * steps)/(1. * BASE_STEPS_PER_MM * FRACTION);
}


void move_steps(bool rev_dir, int steps, int dirPin, int stpPin){
  digitalWrite(dirPin, rev_dir);
  delay(50);
  for(int i=0; i < steps; ++i){
    digitalWrite(stpPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stpPin, LOW);
    delayMicroseconds(1000);
  }   
}

int move_mm_imp(float mm, int dirPin, int stpPin)
{
  // positive values are toward the stepper motor
  int steps = mm_to_step(fabs(mm));
  bool rev_dir = (mm > 0) ? 0 : 1;
  move_steps(rev_dir, steps, dirPin, stpPin);
  return steps * (rev_dir ? -1 : 1); 
}


inline void report_position_steps(int x, int y)
{
  Serial.print("(");
  Serial.print(x);
  Serial.print(", ");   
  Serial.print(y);
  Serial.print(")");
}

inline void report_position_mm(int x, int y)
{
  report_position_steps(step_to_mm(x), step_to_mm(y));    
}

inline void report_completed_move(int dx, int dy)
  {
  Serial.print("Moved ");
  report_position_mm(xx-dx,yy-dy);
  Serial.print(" => ");   
  report_position_mm(xx,yy);
  Serial.println("");   
  } 
  

inline void move_mm_X(float mm){
  // positive values are toward the stepper motor
  int dx = move_mm_imp(mm, dirPinX, stpPinX );
  xx += dx;
  report_completed_move(dx,0);
}

inline void move_mm_Y(float mm){
  // positive values are toward the stepper motor
  int dy = move_mm_imp(mm, dirPinY, stpPinY);
  yy += dy;
  report_completed_move(0,dy);
}

//-------1---------2---------3---------4---------5---------6---------7---------8
void do_at_start()
{
  if (!initialised){ 
  start_point_cross();
  initialised =1;
  }
}

void crash_home()
{
  move_steps(1,mm_to_step(120), dirPinX, stpPinX);
  move_steps(1,mm_to_step(105), dirPinX, stpPinX);
}

void go_home(){
  //xx, yy must alwways be positive if we haven't crashed
  if (xx > 0) move_steps(1,xx, dirPinX, stpPinX);
  if (yy > 0) move_steps(1,yy, dirPinY, stpPinY);
  xx = 0;
  yy = 0;
  Serial.println("Returned to (0,0)");
}

//-------1---------2---------3---------4---------5---------6---------7---------8
void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  pinMode(dirPinX, OUTPUT);
  pinMode(stpPinX, OUTPUT);
  pinMode(dirPinY, OUTPUT);
  pinMode(stpPinY, OUTPUT);
  delay(5000);
}
/*
String inputString ="";
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
*/
void random_walk(){
  move_mm_X(5); //x_walk[] = {5,30,-5
move_mm_Y(5);
move_mm_X(30);
move_mm_Y(10);
move_mm_X(-5);
move_mm_Y(-10);
move_mm_X(5);
move_mm_Y(-5);
move_mm_X(5);
move_mm_Y(20);
move_mm_X(5);
move_mm_Y(-10);
move_mm_X(5);
move_mm_Y(5);
move_mm_X(15);
move_mm_Y(10);
move_mm_X(10);
move_mm_Y(-5);
move_mm_X(-10);
move_mm_Y(5);
move_mm_X(-10);
move_mm_Y(5);
move_mm_X(5);
move_mm_Y(15);
move_mm_X(10);
move_mm_Y(10);
}

#define GRID_SIZE_X 16
#define GRID_SIZE_Y 16
#define GRID_STEP_X 3
#define GRID_STEP_Y 3


inline void next_point()
{
  next_point_cross();
}

void start_point_cross(){
  move_mm_X(20);
  move_mm_Y(20);
  move_mm_Y(0.5*GRID_SIZE_Y * GRID_STEP_X);
}

void next_point_cross(){
  static int point = 1;
  if (point < GRID_SIZE_X){
    move_mm_X(1*GRID_STEP_X);
  }
  else if (point == GRID_SIZE_X){
    move_mm_X(-0.5*(GRID_SIZE_X-1)*GRID_STEP_X);
    move_mm_Y(-0.5*(GRID_SIZE_Y-1)*GRID_STEP_Y);   
  }
  else if (point < (GRID_SIZE_X + GRID_SIZE_Y)){
    move_mm_Y(1*GRID_STEP_Y);
  }
  else {
    move_mm_X(-0.5*(GRID_SIZE_X-1)*GRID_STEP_X);
    move_mm_Y(-0.5*(GRID_SIZE_Y-1)*GRID_STEP_Y);
    point = 0;       
  }
  ++point;
}

void start_point_grid(){
  move_mm_X(20);
  move_mm_Y(20);
}

void next_point_grid()
{
  static int point = 1;
  if (point == GRID_SIZE_X * GRID_SIZE_Y){
    point = 0;
    move_mm_Y((1-GRID_SIZE_Y)*GRID_STEP_Y);
    move_mm_X((1-GRID_SIZE_X)*GRID_STEP_X);
  }
  else if (point % GRID_SIZE_X == 0){ 
    move_mm_Y(1*GRID_STEP_Y);
    move_mm_X((1-GRID_SIZE_X)*GRID_STEP_X);
  }
  else {
    move_mm_X(1*GRID_STEP_X);
  }
  ++point;
}


//-------1---------2---------3---------4---------5---------6---------7---------8
void loop() {
  do_at_start();
  //serialEvent(); //call the function
  //delay(2000);  
  //Wait for instruction
  
  while (true) {
    char inChar = (char)Serial.read();
    
    switch (inChar){
      case 't': Serial.println("This is a test"); continue;
      case 'r': report_position_mm(xx,yy);    

      //Take a step.  
      case 'n': goto step; 
      //case '\n': goto step;
      default: delay(20); continue;
    }
    continue;
 
    step:
      next_point();
      break;
  } 

}
