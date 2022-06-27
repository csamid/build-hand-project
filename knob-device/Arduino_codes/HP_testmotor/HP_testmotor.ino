 //--- Pin Loadout ----//
/*
#define BOpin A3     
#define ROpin A4      
#define GWpin A5
*/
int motorPin = 11;
int dirPin = 13;
int encPin1 = 2;
int encPin2 = 3;


String string = "1003:";
char state;

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
***************************************************************************************************************************************************
*************                                                                                                                        **************
*************     "string" is the input from LabVIEW, and each "byte" from that is represented of the number is as follows:          **************
*************                                                                                                                        **************
*************     string[0] initializes the motor and demonstates the state of the LabVIEW VI (on/off)                               **************
*************       = 0 when enum is set to NO and the program is NOT running.                                                       **************
*************       = 1 when the enum is set to YES and program is running.                                                          **************
*************                                                                                                                        **************
*************     string[1] is to set the test type of the system.                                                                   **************
*************       = 0 for static test.                                                                                             **************
*************       = 1 for dynamic test.                                                                                            

*************     string[2] is for the direction of the test.                                                                        **************
                    = 0 for CW.
*************       = 1 for CCW.
*************                                                                                                                        **************                   
*************     string[3] is for the gain being applied by the motor.                                                              **************
*************       = 0 for low motor gain.                                                                                          **************
*************       = 1 for medium motor gain.                                                                                       **************
*************       = 2 for high motor gain.                                                                                         **************
*************                                                                                                                        **************    
*************      /////////////////////////////////////////////////                                                                 **************
*************     string[4] is for the maximum angle of twist.                                                                       **************
*************       = 0 for 45 degrees.                                                                                              **************
*************       = 1 for 90 degrees.                                                                                              **************
*************                                                                                                                        **************
***************************************************************************************************************************************************
***************************************************************************************************************************************************
**************************************************************************************************************************************************/

//string[n] from above
int Test         = 1;
int Direction    = 2;
int Resistance   = 3;



boolean           stat    = false;
boolean           dyna    = false;
float             beta;                 // (Calculated from Encoder count)
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


/*
float RO;      testing motor so do not need anything with FSR circuit
float BO;
float GW;
float ROvolt;
float BOvolt;
float GWvolt;
*/
int               x = 20;               //Threshold to be determined later, exaggerated for analysis 

void setup()
{
  Serial.begin(9600);
  Serial.flush();

  pinMode(encPin1,  INPUT);
  pinMode(encPin2,  INPUT);
  pinMode(dirPin,   OUTPUT);
  pinMode(motorPin, OUTPUT);
  /*
  pinMode(ROpin,    INPUT);     testing motor so do not need anything with FSR circuit
  pinMode(BOpin,    INPUT);
  pinMode(GWpin,    INPUT);
  */

  attachInterrupt(0, pin1Change, CHANGE);
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


//-- Variable Conversions  ----------------------------------------------------------------------------------------------------------------------------//
//-- Motor Command ------------------------------------------------------------------//
  if (String(string[0]).toInt() == 1)
  {
    analogWrite(motorPin, 0);
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
    motorPwr = 100;  //85 last time
  }
  else if (String(string[Resistance]).toInt() == 1)
  {
    motorPwr = 170;
  }
  else if (String(string[Resistance]).toInt() == 2)
  {
    motorPwr = 255;
  }
  
  
 //------------ Read Values from Sensors ----------------------------------------------//
  
  beta = (((long)encCount) * 360.0) / 2000.0;                 
  theta = beta / 4.1295;   
  /*               
  RO = analogRead(ROpin);   testing motor so do not need anything with FSR circuit
  BO = analogRead(BOpin);
  GW = analogRead(GWpin);
  ROvolt = RO * (5.0/1023.0);
  BOvolt = BO * (5.0/1023.0);
  GWvolt = GW * (5.0/1023.0);
 */
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
  //Serial.print(ROvolt);  
  //Serial.print(",");
  //Serial.print(GWvolt);
 // Serial.print(",");
  //Serial.print(BOvolt);
  //Serial.print(","); 
    Serial.print(2.5);  
  Serial.print(",");
  Serial.print(2.5);
  Serial.print(",");
  Serial.print(2.5);
  Serial.print(",");
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
