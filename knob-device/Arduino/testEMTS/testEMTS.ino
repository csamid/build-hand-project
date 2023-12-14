
// this code is for testing with the encoder motor and torque+amplifier comps w/o LabVIEW

   //--- Pin Loadout ----//
int motorPin = 11;
int dirPin = 13;
int encPin1 = 2;
int encPin2 = 3;




boolean           stat    = false;
boolean           dyna    = false;
float             beta; 
float tor; 
float torV;                            // 
int               motorPwr;            //Power of motor (100, 170 or 255)
int               theta   = 0;         //Angle of twist (Calculated from Encoder count)
boolean           dir;
boolean           CW = true;        // set CW or CCW true and the other false
boolean           CCW = false;
boolean           flag_r  = false;
boolean           flag_l  = false;


//--- Encoder Init ---//

volatile long encCount = 0;
int           incomingByte = 0;
volatile int  pin1state;
volatile int  pin2state;
volatile char REGVALS;
void          pin1Change();
void          pin2Change();

boolean       output_enable;



int               x = 20;               //Threshold to be determined later, exaggerated for analysis 

void setup()
{
  Serial.begin(9600);
  Serial.flush();

  pinMode(encPin1,  INPUT);
  pinMode(encPin2,  INPUT);
  pinMode(dirPin,   OUTPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(Tor_pin, INPUT);


  attachInterrupt(0, pin1Change, CHANGE);
  attachInterrupt(1, pin2Change, CHANGE);

  pin1state = digitalRead(encPin1);
  pin2state = digitalRead(encPin2);

  output_enable = true;

  analogWrite(motorPin, 0);
}

void loop()
{
 
 //------------ Read Values from Sensors ----------------------------------------------//
  
  beta = (((long)encCount) * 360.0) / 2000.0;                 
  theta = beta / 4.1295;   
  tor = analogRead(Tor_pin);
  torV = tor * (5.0/1023.0);

 //----- SETTING THE DIRECTION OF THE "MOTOR" ---------------------------------
 if(theta > 0)
 {
   dir = LOW;   
 }
 else if(theta < 0)
 {
   dir = HIGH;
 }
 
 //----- CHANGING FLAG STATES ---------------------------------
 if(theta > x)
 {
   flag_r = true;
 }
 if(theta < -1*x)
 {
   flag_l = true;
 }

 
 //----- CONDITION FOR CW ROTATION (STATIC)---------------------------------
 if(stat == true && CW == true)
 {
   if(theta > x)
   {
     flag_r = true;
   }
   
   if(flag_r == true)
   {
     if(theta > 0)
     {
       analogWrite(motorPin,motorPwr);
       digitalWrite(dirPin,dir);
     }
     else if(theta < 0) //Reset the value of flag
     {
       analogWrite(motorPin,0);
       digitalWrite(dirPin,dir);
       flag_r = false;
     }
   }
 }
 
 //----- CONDITION FOR CCW ROTATION (STATIC)---------------------------------
 if(stat == true && CCW == true)
 {
   if(theta < -1*x)
   {
     flag_l = true;
   }
   if(flag_l == true)
   {
     if(theta < 0)
     {
       analogWrite(motorPin,motorPwr);
       digitalWrite(dirPin,dir);
     }
     else if(theta > 0) //Reset the value of flag
     {
       analogWrite(motorPin,0);
       digitalWrite(dirPin,dir);
       flag_l = false;
     }
   }
 }
 
    //----- CONDITION FOR CW ROTATION (DYNAMIC) ---------------------------------
 if(dyna == true && CW == true)
 {
   if(theta > x)
   {
     flag_r = true;
   }
   
   if(flag_r == true)
   {
     if(theta > 0)
     {
       analogWrite(motorPin,motorPwr);
       digitalWrite(dirPin,dir);
     }
     else if(theta < 0) //Reset the value of flag
     {
       analogWrite(motorPin,0);
       digitalWrite(dirPin,dir);
       flag_r = false;
     }
   }
 }
 
 //----- CONDITION FOR CCW ROTATION (DYNAMIC)---------------------------------
 if(dyna == true && CCW == true)
 {
   if(theta < -1*x)
   {
     flag_l = true;
   }
   if(flag_l == true)
   {
     if(theta < 0)
     {
       analogWrite(motorPin,motorPwr);
       digitalWrite(dirPin,dir);
     }
     else if(theta > 0) //Reset the value of flag
     {
       analogWrite(motorPin,0);
       digitalWrite(dirPin,dir);
       flag_l = false;
     }
   }
 }
    

//----------- Send Values to LabVIEW ----------------//
  Serial.println(torV);
  Serial.println(",")
  Serial.println(theta);
  
}

// Encoder count code //
void pin1Change() {
  REGVALS = PIND;
  pin1state = REGVALS & B00000100;
  pin2state = REGVALS & B00001000;

  if (pin1state == B00000100) {
    if (pin2state == B00000000) {
      encCount++;
    }
    else {
      encCount--;
    }
  }
  else {
    if (pin2state == B00001000) {
      encCount++;
    }
    else {
      encCount--;
    }
  }
}


void pin2Change() {
  REGVALS = PIND;
  pin1state = REGVALS & B00000100;
  pin2state = REGVALS & B00001000;

  if (pin1state == B00000100) {
    if (pin2state != B00000000) {
      encCount++;
    }
    else {
      encCount--;
    }
  }
  else {
    if (pin2state != B00001000) {
      encCount++;
    }
    else {
      encCount--;
    }
  }
}
