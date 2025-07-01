/*/         Android Based Robot Car                   //

//         Interface  : HC-05 Bluetooth              //

//         Powered by e-logic Embedded   //

*/ 
//****************************************************************

//#include "SoftwareSerial.h"
#include <NewPing.h>
#include <dht.h>
//SoftwareSerial blue(2,3);

#define BAUDRATE 9600

#define maxSpeed 230
const int PumpPin = 3;

#define Buz A2

#define dataPin A3
dht DHT;

#define MT1_A    4
#define MT1_B    5 
#define MT2_A    6
#define MT2_B    7

#define TRIG_PIN 2 
#define ECHO_PIN 8 
#define MAX_DISTANCE 300 // sets maximum useable sensor measuring distance to 300cm
#define COLL_DIST 30 

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 
int curDist = 0;

int cm = 0;
int ivalue;
int temp1 = 0; 
#define Mspeed 100

char command;
bool Status = false;

boolean flag = 0;
boolean flag1 = 0;
unsigned int US1;
unsigned long t = 0;

//***************************************************************
void setup()
 {
  Serial.begin(BAUDRATE);
  //blue.begin(BAUDRATE);

  pinMode(PumpPin,OUTPUT);
  pinMode(MT1_A,OUTPUT);
  pinMode(MT1_B,OUTPUT);
  pinMode(MT2_A,OUTPUT);
  pinMode(MT2_B,OUTPUT);
  pinMode(Buz,OUTPUT);  digitalWrite(Buz,LOW);
  Serial.println("Wel-Come");
 }

//***************************************************************
void loop()
 {
  if(Serial.available())
   {
    command = (Serial.read());
    switch(command)
      {
        case 'F': Robot_Forword();  break;
        case 'B': Robot_Reverse();  break;
        case 'L': Robot_Left();     break;
        case 'R': Robot_Right();    break;
        case 'S': Robot_Stop();     break;      
        case 'A': Robot_Auto();     break;
        case 'P': Pump_ON();       break;
        case 'p': Pump_OFF();      break;
      }
    int readData = DHT.read11(dataPin);
    int t = DHT.temperature;
    int h = DHT.humidity; delay(500);
    Serial.println(String("Temp:") + t + "'C");
    Serial.println(String("Humid:") + h + "%\n");
   }      
//.....................................................
  if(flag == 0)
  {
    US1 = sonar.ping_in();  delay(50);
    if(US1 >= 1 && US1 <= 18)  { Robot_Stop(); Beep(); } 
  } 
//.....................................................
 }  // Main Loop
 
//*******************************************************************************
//*******************************************************************************
 void Robot_Forword()
  {   
   if(US1 >= 1 && US1 <= 18)  { Robot_Stop();  Beep();  Serial.println(String("Diast:") + US1 + "inch"); }

   else
      {
       digitalWrite(MT1_A, LOW);   digitalWrite(MT1_B,HIGH);
       digitalWrite(MT2_A, LOW);   digitalWrite(MT2_B, HIGH);
       flag = 0;
      }
  }
//___________________________________________________________
 void Robot_Reverse()
  { 
   digitalWrite(MT1_A, HIGH);   digitalWrite(MT1_B,LOW);
   digitalWrite(MT2_A, HIGH);   digitalWrite(MT2_B, LOW);
   flag = 1;
  }
//___________________________________________________________
 void Robot_Right()
  { 
   digitalWrite(MT1_A, LOW);   digitalWrite(MT1_B,HIGH);
   digitalWrite(MT2_A, HIGH);  analogWrite(MT2_B, LOW);
   flag = 1;
  }
//___________________________________________________________
 void Robot_Left()
  { 
   digitalWrite(MT1_A, HIGH);  digitalWrite(MT1_B,LOW);
   digitalWrite(MT2_A, LOW);   digitalWrite(MT2_B, HIGH);
   flag = 1;
  }
     
//___________________________________________________________
 void Robot_Stop()
  { 
   digitalWrite(MT1_A, LOW);   digitalWrite(MT1_B, LOW);
   digitalWrite(MT2_A, LOW);   digitalWrite(MT2_B, LOW);
   flag = 0;
  }   
//...........................................................
 void Auto_Stop()
  { 
    
   digitalWrite(MT1_A, LOW);   digitalWrite(MT1_B, LOW);
   digitalWrite(MT2_A, LOW);   digitalWrite(MT2_B, LOW);
   flag = 0;
  }
//...........................................................
 void Auto_Forword()
  { 
   if(US1 >= 1 && US1 <= 12)  { Auto_Stop();  Beep(); }

   else
      {
       digitalWrite(MT1_A, LOW);   digitalWrite(MT1_B, HIGH);
       digitalWrite(MT2_A, LOW);   digitalWrite(MT2_B, HIGH);
       flag = 0;
      }
  }
//___________________________________________________________
void Beep()
 {
   digitalWrite(Buz,HIGH); delay(50);
   digitalWrite(Buz,LOW); delay(50);
   digitalWrite(Buz,HIGH); delay(50);
   digitalWrite(Buz,LOW); delay(50);
   digitalWrite(Buz,HIGH); delay(50);
   digitalWrite(Buz,LOW); delay(50);
   digitalWrite(Buz,HIGH); delay(50);
   digitalWrite(Buz,LOW); delay(50);
 }
 
//****************************************************************
 void Pump_ON()
  { 
   for(int motorSpeed = 0; motorSpeed < maxSpeed; motorSpeed++) 
     { analogWrite(PumpPin, motorSpeed); delay(10); }
   flag = 0;
  } 
//***************************************************************
 void Pump_OFF()
  { 
   analogWrite(PumpPin, 0);
   flag = 0;
  }
//****************************************************************************************
void Robot_Auto()
  { 
   Pump_ON();
   while(!Serial.available())
    {
     t = millis();
     while(millis() < (t + 4000)) { US1 = sonar.ping_in(); Auto_Forword(); }
     Auto_Stop(); delay(1000);  Robot_Right();  delay(1000);
     Robot_Forword();  delay(1000); Robot_Right();  delay(1000);
//........................................................................................
     t = millis();
     while(millis() < (t + 4000)) { US1 = sonar.ping_in(); Auto_Forword(); }
     Auto_Stop(); delay(1000);  Robot_Left();  delay(1000);
     Robot_Forword();  delay(1000); Robot_Left();  delay(1000);
     Robot_Stop();  Pump_OFF();
     break;
//........................................................................................
    }
  }    
//****************************************************************************************
//****************************************************************************************


