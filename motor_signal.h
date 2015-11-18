#include <Makeblock.h>


MePort sig_X(M1);
MePort sig_Y(M2);


int mPinX1 = mePort[M1].s1;
int mPinX2 = mePort[M1].s2;
int mPinY1 = mePort[M2].s1;
int mPinY2 = mePort[M2].s2;

enum {bit_time = 25};


void signal_start(MePort& motorport)
{
  motorport.dWrite1(HIGH);
  delay(bit_time * 3);
  motorport.dWrite1(LOW);
  delay(bit_time);
    
}


void signal_bit(MePort& motorport, bool bit)
{
  if (bit) {
     
     motorport.dWrite1(HIGH); 
     delay(bit_time);
     motorport.dWrite1(LOW);
     delay(bit_time);
  }
  else {
     motorport.dWrite2(HIGH);
     delay(bit_time);
     motorport.dWrite2(LOW);
     delay(bit_time);
  }
}

void signal_byte(MePort& motorport, uint8_t byte)
{
  for (int i = 7;  i >= 0; --i) {
    // Mask each bit in the byte and store it
    signal_bit(motorport, (byte & (1 << i)));
  }  
}

#undef MOTOR_SIGNAL_AS_MAIN
#ifdef MOTOR_SIGNAL_AS_MAIN
void setup() {
  // put your setup code here, to run once:
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  pinMode(mPinX1, OUTPUT);
  pinMode(mPinX2, OUTPUT);
  pinMode(mPinY1, OUTPUT);
  pinMode(mPinY2, OUTPUT);
  delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  Serial.println("11101110 X");
  signal_start(sig_X);
  signal_byte(sig_X,0xFE);
  delay(600);
  Serial.println("00110011 Y");
  signal_start(sig_Y);
  signal_byte(sig_Y,0xF3);
  Serial.println("Done");
  */
  Serial.println("Hello World");
  signal_start(sig_X);
  signal_bit(sig_Y,1);
  signal_byte(sig_X,'H');
  signal_bit(sig_Y,1);
  signal_byte(sig_X,'e');
  signal_bit(sig_Y,1);
  signal_byte(sig_X,'l');
  signal_bit(sig_Y,1);
  signal_byte(sig_X,'l');
  signal_bit(sig_Y,1);
  signal_byte(sig_X,'o');
  signal_bit(sig_Y,1);
  signal_byte(sig_X,' ');
  signal_bit(sig_Y,1);
  signal_byte(sig_X,'W');
  signal_bit(sig_Y,1);
  signal_byte(sig_X,'o');
  signal_bit(sig_Y,1);
  signal_byte(sig_X,'r');
  signal_bit(sig_Y,1);
  signal_byte(sig_X,'l');
  signal_bit(sig_Y,1);
  signal_byte(sig_X,'d');
  delay(600);
}
#endif
