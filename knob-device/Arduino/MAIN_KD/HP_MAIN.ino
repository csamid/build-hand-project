 //--- Pin Loadout ----//

#define TORpin A2 // Vout from torque sensor circuit   
#define F1pin A3  //  for b2: M   red wire
#define F2pin A4  //   for b2: RO  white wire
#define F3pin A5  //   for b2: BO  green wire


#define motorPin 11
#define dirPin 13
#define encPin1 2
#define encPin2 3


String string = "1030:";
char state;

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
***************************************************************************************************************************************************
*************                                                                                                                        **************
*************     "string" is the input from LabVIEW, and each "byte" from that is represented of the number is as follows:          **************
*************                                                                                                                        **************
*************                                                                                                                        **************
*************     string[0] is to set the test type of the system.                                                                   **************
*************       = 0 for static test.                                                                                             **************
*************       = 1 for dynamic test.                                                                                            

*************     string[1] is for the direction of the test.                                                                        **************
                    = 0 for CW.
*************       = 1 for CCW.
*************                                                                                                                        **************                   
*************     string[2] is for the gain being applied by the motor.                                                              **************
*************       = 0 for low motor gain.                                                                                          **************
*************       = 1 for medium motor gain.                                                                                       **************
*************       = 2 for high motor gain.                                                                                         **************
*************                                                                                                                        **************    
*************      /////////////////////////////////////////////////                                                                 **************
*************     string[3] is to start motor.                                                                                       **************
*************       = 0 for 0 motor gain degrees.                                                                                    **************
*************       = 1 for the motor gain set in string[2].                                                                         **************
*************                                                                                                                        **************
***************************************************************************************************************************************************
***************************************************************************************************************************************************
**************************************************************************************************************************************************/

//string[n] from above
int Test         = 0;
int Direction    = 1;
int Resistance   = 2;
int Motor        = 3;



boolean           stat    = false;
boolean           dyna    = false;
float             beta = 0;                 // (Calculated from Encoder count)
int               motorPwr;             //Power of motor (From LabVIEW)
int               theta   = 0;          //Angle of twist (Calculated from Encoder count)
boolean           dir;
boolean           CW;
boolean           CCW;
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


float F1;
float F2;
float F3;
float F1volt;
float F2volt;
float F3volt;
float tor;    //torque value from analog pin
float torV;   // conversion to voltage torque    

int               x = 20;               //Threshold to be determined later, exaggerated for analysis 

void setup()
{
  Serial.begin(9600);
  Serial.flush();

  pinMode(encPin1,  INPUT);
  pinMode(encPin2,  INPUT);
  pinMode(dirPin,   OUTPUT);
  pinMode(motorPin, OUTPUT);
  
  pinMode(F1pin, INPUT);
  pinMode(F2pin, INPUT);
  pinMode(F3pin, INPUT);
  pinMode(TORpin,INPUT);


  attachInterrupt(0, pin1Change, CHANGE); //or use attachInterrupt(digitalPinToInterrupt(encPin1), pin1Change, CHANGE)
  attachInterrupt(1, pin2Change, CHANGE);

  pin1state = digitalRead(encPin1);
  pin2state = digitalRead(encPin2);

  output_enable = true;
}

void loop()
{

//----------- Read Values from LabVIEW --------------//
  if (Serial.available() > 0)
  {
    string = "";
  }
  while (Serial.available() > 0)
  {
    state = ((byte)Serial.read());
    if (state == ':')
    {
      break;
    }
    else
    {
      string += state;
    }
    delay(1);
  }


  
//-- TEST CASE ------------------------------------------------------------------//
  if (String(string[Test]).toInt() == 0)
  {
    stat = true;
  }
  else if (String(string[Test]).toInt() == 1)
  {
    dyna = true;
  }
  else
  {
    stat = false;
    dyna = false;
  }

 //-- DIRECTION ------------------------------------------------------------------//
  if (String(string[Direction]).toInt() == 0)
  {
     CW  = true;
     CCW  = false;   // made change here
  } 
  else if (String(string[Direction]).toInt() == 1)
  {
     CCW = true;
     CW = false;   // added this line
  }
  
//-- POWER ------------------------------------------------------------------//
  if (String(string[Resistance]).toInt() == 0)
  {
    motorPwr = 85;
  }
  else if (String(string[Resistance]).toInt() == 1)
  {
    motorPwr = 135;
  }
  else if (String(string[Resistance]).toInt() == 2)
  {
    motorPwr = 185;
  }
  
  
 //------------ Read Values from Sensors ----------------------------------------------//
  
  //beta = (((long)encCount) * 360.0) / 2000.0; 
  //theta = beta / 4.1295; this was for the paddle conversion 
  beta = (long)encCount * (360.0 / 2000.0);
  theta = beta / 26.875;
  theta = theta % 360;
  tor = analogRead(TORpin);
  torV = tor * (5.0/1023.0);    
  
  F1 = analogRead(F1pin);
  F2 = analogRead(F2pin);
  F3 = analogRead(F3pin);
  F1volt = F1 * (5.0/1023.0);
  F2volt = F2 * (5.0/1023.0);
  F3volt = F3 * (5.0/1023.0);


  
  
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
 
// ------ CONDITION FOR TESTING (MOTOR) -----//   // Added this to see if motor will be "off" when in init state
if (String(string[Motor]).toInt() == 0)
{
  analogWrite(motorPin, 0);
  encCount = 0;
}
else if (String(string[Motor]).toInt() == 1)
{
  testing();
  }
      
//----------- Send Values to LabVIEW ----------------//
   Serial.print(2.5);  //F1volt 
   Serial.print(",");
   Serial.print(2.5);//F2volt
   Serial.print(",");
   Serial.print(2.5);//F3volt
   Serial.print(",");
   Serial.print(2.5); //theta
   Serial.print(",");
   Serial.println(2.5);//torV
  
}

void testing() {
  
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
     else if(theta < 0) //Reset the value of flag      // put else if(theta <= 0) to fix little issue
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
  pin1state = REGVALS & B00000100;  //B00000100
  pin2state = REGVALS & B00001000;   //B00001000

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
