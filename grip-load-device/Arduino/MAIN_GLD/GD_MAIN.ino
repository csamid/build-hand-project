// csamid (6/23/22)

// Optional LPF 
template <int order> // order is 1 or 2
class LowPass
{
  private:
    float a[order];
    float b[order+1];
    float omega0;
    float dt;
    bool adapt;
    float tn1 = 0;
    float x[order+1]; // Raw values
    float y[order+1]; // Filtered values

  public:  
    LowPass(float f0, float fs, bool adaptive){
      // f0: cutoff frequency (Hz)
      // fs: sample frequency (Hz)
      // adaptive: boolean flag, if set to 1, the code will automatically set
      // the sample frequency based on the time history.
      
      omega0 = 6.28318530718*f0;
      dt = 1.0/fs;
      adapt = adaptive;
      tn1 = -dt;
      for(int k = 0; k < order+1; k++){
        x[k] = 0;
        y[k] = 0;        
      }
      setCoef();
    }

    void setCoef(){
      if(adapt){
        float t = micros()/1.0e6;
        dt = t - tn1;
        tn1 = t;
      }
      
      float alpha = omega0*dt;
      if(order==1){
        a[0] = -(alpha - 2.0)/(alpha+2.0);
        b[0] = alpha/(alpha+2.0);
        b[1] = alpha/(alpha+2.0);        
      }
      if(order==2){
        float c1 = 2*sqrt(2)/alpha;
        float c2 = 4/(alpha*alpha);
        float denom = 1.0+c1+c2;
        b[0] = 1.0/denom;
        b[1] = 2.0/denom;
        b[2] = b[0];
        a[0] = -(2.0-2.0*c2)/denom;
        a[1] = -(1.0-c1+c2)/(1.0+c1+c2);      
      }
    }

    float filt(float xn){
      // Provide me with the current raw value: x
      // I will give you the current filtered value: y
      if(adapt){
        setCoef(); // Update coefficients if necessary      
      }
      y[0] = 0;
      x[0] = xn;
      // Compute the filtered values
      for(int k = 0; k < order; k++){
        y[0] += a[k]*y[k+1] + b[k]*x[k];
      }
      y[0] += b[order]*x[order];

      // Save the historical values
      for(int k = order; k > 0; k--){
        y[k] = y[k-1];
        x[k] = x[k-1];
      }
  
      // Return the filtered value    
      return y[0];
    }
};

// LIBRARIES
#include "SoftwareSerial.h"
#include "I2Cdev.h"
#include "MPU6050.h" 

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  #include "Wire.h"
#endif

#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
  #include <EEPROM.h>
#endif

//CONSTANTS

// GF load cell amplifier
const int HX711_dout = A2; // mcu > HX711 dout pin
const int HX711_sck = A3; //mcu > HX711 sck pin
const int calVal_eepromAdress = 0;

#define FutekLoadCellPin A1
#define COMPARE_MATCH 0x271 // 625 <--> 10ms (Timer), sends values every 10ms

// VARIABLES
float minA;   // min value (GF load cell)
float grams;  //grams value (GF load cell)
int FutekBinary = 0;  // binary value from sensor mapped to (0 - 1023) by internal ADC (Futek load cell)
float FutekVoltage = 0; // mapped voltage (Futek load cell)
float FutekV_filtered;
int16_t ax, ay, az; // to store raw accel value on z axis
long accl_x, accl_y, accl_z;
long resultant; // accel resultant
volatile uint8_t flag = 0; //Flag for Timer
float t; //time

// OBJECT(S) AND CONSTRUCTOR(S)
 MPU6050 mpu;  // Object of MPU
 HX711_ADC LoadCell(HX711_dout, HX711_sck); // HX711 constructor

// Filter instance
LowPass<1> lp(7,100,true);


void setup() {
  
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        //Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    Serial.begin(115200); 

    // initialize device
    mpu.initialize();

    // Timer1 setup
    timerInterruptSetup();

    // initialize GF Load cell 
    LoadCell.begin();
    float calibrationValue; // calibration value (see example file "Calibration.ino")
    EEPROM.get(calVal_eepromAdress, calibrationValue);
    unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
    boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
    LoadCell.start(stabilizingtime, _tare);
    if (LoadCell.getTareTimeoutFlag()) {
    //Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
    }
    else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    //Serial.println("Startup is complete");
    }


    // offsets from IMU_Zero and manually adjusted   
    mpu.setYAccelOffset(-170);
    mpu.setXAccelOffset(-275);
    mpu.setZAccelOffset(1465);  
    mpu.setFullScaleAccelRange(1);  // +-4g range, 8192 LSB/g

}

void loop() {
  if (flag) {
    // check for new data/start next conversion:
    boolean newDataReady = 0;
    if (LoadCell.update()) newDataReady = true;
    // get data
    if(newDataReady) {
      //if (millis() > t + serialPrintInterval) {
        float i = LoadCell.getData();
        if (i < 0) {
          minA = -i;
        }
        grams = i + minA;
        newDataReady = 0;      
      //}
    }
    // Optional tare operation:
    /*
    // receive command from serial terminal, send 't' to initiate tare operation:
    if (Serial.available() > 0) {
        char inByte = Serial.read();
     if (inByte == 't') LoadCell.tareNoDelay();
     }

      // check if last tare operation is complete:
     if (LoadCell.getTareStatus() == true) {
      Serial.println("Tare complete");
     }
     */
     
     //read raw accel values
     mpu.getAcceleration(&ax, &ay, &az);
     accl_x = ax;
     accl_y = ay;
     accl_z = az;
     resultant = sqrt((accl_x*accl_x) + (accl_y*accl_y) + (accl_z*accl_z)); // Obtaining resultant acceleration

     // Read Futek load cell pin & map to a voltage value between 0V-5V, 1000  = 1V
     FutekBinary = analogRead(FutekLoadCellPin);
     FutekVoltage = map(FutekBinary,0,1023,0.0,5000);

     FutekV_filtered = lp.filt(FutekVoltage); // filtered futek voltage
     
     //get current time
     t = ((float) millis())/1000.0;
     
     // Send sensor values to LabVIEW VI
     writeSerialPort();
    
    //update flag
    flag = 0;
  }    
}

ISR(TIMER1_COMPA_vect) {
  flag = 1;
}

void writeSerialPort() {
  Serial.print(resultant);
  Serial.print(",");
  Serial.print(grams);// grams from LCFD-50 output
  Serial.print(",");
  Serial.println(FutekV_filtered);
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


  
