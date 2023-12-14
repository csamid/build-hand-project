//libraries:

//Futek load cell:
#define FutekLoadCellPin A1
int FutekBinary = 0;  // binary value from sensor
float FutekVoltage = 0; // mapped voltage
float t;

#define COMPARE_MATCH 0x271 // 3125, 100Hz = 10ms
static double dt = 0.01; // 0.01s = 10ms
volatile uint8_t flag = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(FutekLoadCellPin, INPUT);

  timerInterruptSetup();
  
}

void loop() {

 // if (flag) {
    
  // Read Futek load cell pin & map to a voltage value between 0V-5V, 1000 = 1V
  FutekBinary = analogRead(FutekLoadCellPin);
  FutekVoltage = FutekBinary * (5000.0/1023.0);

  // get current time
  t = ((float) millis())/1000.0;
  //
  //if (t < 1.01) {
  //Write to serial port
  writeSerialPort();
 // }
   //Update flag
  // flag = 0;
    
  //}
}

ISR(TIMER1_COMPA_vect) {
  flag = 1;
}

void writeSerialPort() {
  // comp filter data and time
  //Serial.print(t);
  //Serial.print(" ");
  //Serial.print(1,3); 
  //Serial.print(",");
  Serial.print(t,3);
  Serial.print(","); 
  Serial.print(1,3); 
  Serial.print(","); 
  Serial.println(1,3); 


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
