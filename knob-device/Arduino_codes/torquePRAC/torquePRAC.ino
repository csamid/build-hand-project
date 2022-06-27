// Torque Sensor Code

//pin loadout

#define Tor_pin A2

//

float tor; 
float torV; 


void setup() 
{
  Serial.begin(9600);
  pinMode(Tor_pin, INPUT);
  
}

void loop() 
{
 // Read analog input 
 tor = analogRead(Tor_pin);
 torV = tor * (5.0/1023.0);

 Serial.println(torV);

 delay(1);

}
