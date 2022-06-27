#define Torpin A2  // for b1: N/A for b2: M   red wire   // THIS WILL BE TORQUE PIN 
#define F2pin A3  // for b1: BO  for b2: RO  white wire
#define F3pin A4  // for b1: RO  for b2: P or R  yellow wire
#define F4pin A5  // for b1: GW  for b2: BO  green wire

float Tor;
float F2;
float F3;
float F4;
float Torvolt;
float F2volt;
float F3volt;
float F4volt;


void setup()
{
  
 Serial.begin(9600);
  pinMode(Torpin, INPUT);
  pinMode(F2pin, INPUT);
  pinMode(F3pin, INPUT);
  pinMode(F4pin, INPUT);
  
}

void loop() 
{
  // Read Values
  
 Tor = analogRead(Torpin);
 F2 = analogRead(F2pin);
 F3 = analogRead(F3pin);
 F4 = analogRead(F4pin);
 Torvolt = Tor * (5.0/1023.0);
 F2volt = F2 * (5.0/1023.0);
 F3volt = F3 * (5.0/1023.0);
 F4volt = F4 * (5.0/1023.0);

   // Send values to LabVIEW
   Serial.print(Torvolt);
   Serial.print(",");
   Serial.print(F2volt);
   Serial.print(",");
   Serial.print(F3volt);
   Serial.print(",");
   Serial.println(F4volt);

   delay(1);
 
}
