#include "Wire.h"

// I2Cdev and MPU9250 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU9250.h"
#include "math.h"

// class default I2C address is 0x68
MPU9250 imu;
I2Cdev   I2C_M;

uint8_t buffer_m[6];


int16_t ax, ay, az;
float Axyz[3];
float pitch;
float roll;

#define COMPARE_MATCH 0x271 // 625 <--> 10ms
//static double dt = 0.01; // 10ms = 0.01s
volatile uint8_t flag = 0;
float t;
float dt = 0;

void setup() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize serial communication
    Serial.begin(115200);

    // initialize device
    //Serial.println("Initializing I2C devices...");
    imu.initialize();

    // Timer1 setup
    timerInterruptSetup();
    

    // verify connection
    //Serial.println("Testing device connections...");
    //Serial.println(imu.testConnection() ? "MPU9250 connection successful" : "MPU9250 connection failed");

    //delay(1000);
    //Serial.println("     ");

  
}

void loop() {
  if (flag) {
    
  getAccel_Data();
  getRollPitch();
  t = ((float) millis())/1000.0;
  writeSerialPort();

  flag = 0;
  }
}

ISR(TIMER1_COMPA_vect) {
  flag = 1;
}

void getAccel_Data(void)
{
    imu.getAcceleration(&ax, &ay, &az);
    Axyz[0] = (double) ax / 16384;
    Axyz[1] = (double) ay / 16384;
    Axyz[2] = (double) az / 16384;
}

void writeSerialPort() {
  Serial.print(roll);
  Serial.print(",");
  Serial.print(roll);
  Serial.print(",");
  Serial.println(roll);
  //Serial.print(",");
  //Serial.println(roll);
}

void getRollPitch() {
  pitch = atan2(Axyz[0],Axyz[2]) * 180/PI;
  roll = atan2(Axyz[1],Axyz[2]) * 180/PI;
}

void timerInterruptSetup() {
  // Timer/Counter 1
  // Reset Timer1 Control Reg A
  TCCR1A = 0x00;
  
  // Set prescalar to 256
  TCCR1B |= (1 << CS12);
  TCCR1B &= ~(1 << CS11);
  TCCR1B &= ~(1 << CS10);
  
  // Set CTC mode
  TCCR1B &= ~(1 << WGM13);
  TCCR1B |= (1 << WGM12);
  
  //Initialize Timer1 and Set compare match
  TCNT1 = 0x00;
  OCR1A = COMPARE_MATCH;

  // Enable Timer1 compare interrupt
  TIMSK1 = (1 << OCIE1A);

  // enable global interrupts
  sei();
}
