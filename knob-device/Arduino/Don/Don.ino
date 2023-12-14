//                Pin Loadout
int               motorPin = 11;       //PWM Pin Motor
int               dirPin   = 13;       //Direction Pin Motor
int               encPin1  = 2;        //Encoder Pin Motor 1
int               encPin2  = 3;        //Encoder Pin Motor 2
int               torPin   = 0;        //Torque Sensor Analog Pin

//                Variable
char              command;
String            string = "10002:";

/*****************************************************************************************************************************************************
******************************************************************************************************************************************************
******************************************************************************************************************************************************
******************************************************************************************************************************************************
***************                                                                                                                        ***************
***************     "string" is the input from LabVIEW, and each "byte" from that is represented of the number is as follows:          ***************
***************                                                                                                                        ***************
***************     string[0] is the check if the state of the "STOP" button from LabVIEW is pressed.                                  ***************
***************       = 0 when the "STOP" button is pressed and the program is NOT running.                                            ***************
***************       = 1 when the program is running.                                                                                 ***************
***************                                                                                                                        ***************
***************     string[1] is to set the test type of the system.                                                                   ***************
***************       = 0 for static test.                                                                                             ***************
***************       = 1 for dynamic test.                                                                                            ***************
***************                                                                                                                        ***************
***************     string[2] is for the gain being applied by the motor.                                                              ***************
***************       = 0 for low motor gain.                                                                                          ***************
***************       = 1 for medium motor gain.                                                                                       ***************
***************       = 2 for high motor gain.                                                                                         ***************
***************                                                                                                                        ***************
***************     string[3] is for the direction of the test.                                                                        ***************
***************       = 0 for CW.                                                                                                      ***************
***************       = 1 for CCW.                                                                                                     ***************
***************                                                                                                                        ***************
***************     string[4] is for the maximum angle of twist.                                                                       ***************
***************       = 0 for 45 degrees.                                                                                              ***************
***************       = 1 for 90 degrees.                                                                                              ***************
***************                                                                                                                        ***************
******************************************************************************************************************************************************
******************************************************************************************************************************************************
******************************************************************************************************************************************************
*****************************************************************************************************************************************************/

//                string[n] from above are as follows
int               Test          = 1;
int               Resistance    = 2;
int               Direction     = 3;
int               Angle_Twist   = 4;

//                Variable              definition
int               motorPwr;             //Power of motor (From LabVIEW)
int               alpha;                //Maximum angle of twist (From LabVIEW)

volatile long     encCount = 0;         //Encoder count (From Encoder)
int               incomingByte = 0;
volatile int      pin1state;
volatile int      pin2state;
volatile char     REGVALS;
void              pin1Change();
void              pin2Change();
boolean           output_enable;

boolean           stat    = false;
boolean           dyna    = false;
int               theta   = 0;            //Angle of twist (Calculated from Encoder count)
boolean           dir;
boolean           CW;
boolean           CCW;
boolean           flag_r  = false;
boolean           flag_l  = false;

int               x = 20;               //Threshold to be determined later, exaggerated for analysis

float             torVol;               //Torque sensor value (From Torque Sensor)
float             torVal;               //Torque sensor voltage (Calculated from Torque Sensor Value)
float             tau;                  //Torque value (Calibrated Torque from Voltage)

void setup()
{
  Serial.begin(9600);
  Serial.flush();

  pinMode(encPin1,  INPUT);
  pinMode(encPin2,  INPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(dirPin,   OUTPUT);

  attachInterrupt(0, pin1Change, CHANGE);
  attachInterrupt(1, pin2Change, CHANGE);

  pin1state = digitalRead(encPin1);
  pin2state = digitalRead(encPin2);

  output_enable = true;
  
  pinMode(torPin, INPUT);

}

void loop()
{
//-- Communication from LabView ----------------------------------------------------------------------------------------------------------------------------//

  if (Serial.available() > 0)
  {
    string = "";
  }

  while (Serial.available() > 0)
  {
    command = ((byte)Serial.read());
    if (command == ':')
    {
      break;
    }

    else
    {
      string += command;
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
  
//-- POWER ------------------------------------------------------------------//
  if (String(string[Resistance]).toInt() == 0)
  {
    motorPwr = 85;
  }
  else if (String(string[Resistance]).toInt() == 1)
  {
    motorPwr = 170;
  }
  else if (String(string[Resistance]).toInt() == 2)
  {
    motorPwr = 255;
  }
  
//-- DIRECTION ------------------------------------------------------------------//
  if (String(string[Direction]).toInt() == 0)
  {
     CW  = true;
  }
  else if (String(string[Direction]).toInt() == 1)
  {
     CCW = true;
  }
  
//-- ANGLE LIMIT ------------------------------------------------------------------//
  if (String(string[Angle_Twist]).toInt() == 0)
  {
    alpha = 60;
  }
  else if (String(string[Angle_Twist]).toInt() == 1)
  {
    alpha = 90;
  }
  else if (String(string[Angle_Twist]).toInt() == 2)
  {
    alpha = 120;
  }


//-- Angle Definitions  ----------------------------------------------------------------------------------------------------------------------------//
  theta = (((long)encCount) % 2000) * (360.0 / 2000.0); 

//-- Torque Definitions  ----------------------------------------------------------------------------------------------------------------------------//
  torVal = analogRead(torPin);
  torVol = (torVal) * (5.0 / 1023.0);
//  tau = //VALUE FROM CALBRAION

//-- Motor Commands with Angle  ----------------------------------------------------------------------------------------------------------------------------//
 
 //----- SETTING THE DIRECTION OF THE "MOTOR" ---------------------------------
 if(theta > 0)
 {
   dir = HIGH;
 }
 else if(theta < 0)
 {
   dir = LOW;
 }
 
 //----- CHANGING FLAG STATES ---------------------------------
 if(theta > x)
 {
   flag_r = true;
 }
 if(theta < -x)
 {
   flag_l = true;
 }
 
 //----- CONDITION FOR CW ROTATION ---------------------------------
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
       analogWrite(motorPin,255);
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
 
 //----- CONDITION FOR CCW ROTATION ---------------------------------
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
       analogWrite(motorPin,255);
       digitalWrite(dirPin,dir);
     }
     else if(theta > 0) //Reset the value of flag
     {
       analogWrite(motorPin,0);
       digitalWrite(dirPin,dir);
       flag_l = false;
     }
   }

    //----- CONDITION FOR CW ROTATION (DYNA TEST)---------------------------------
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
       analogWrite(motorPin,255);
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
 
 //----- CONDITION FOR CCW ROTATION (DYNA TEST)---------------------------------
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
       analogWrite(motorPin,255);
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
 
//-- Serial Communication with LabVIEW  ----------------------------------------------------------------------------------------------------------------------------//
  Serial.print    (motorPwr);
  Serial.print    ("\t");
  Serial.print    (theta);
  Serial.print    ("\t");
  Serial.print    (alpha);
  Serial.print    ("\t");
  Serial.print    (torVal);
  Serial.print    ("\t");
  Serial.println  (encCount);

}

//-- Encoder Functions  ----------------------------------------------------------------------------------------------------------------------------//
void pin1Change()
{
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

void pin2Change()
{
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
