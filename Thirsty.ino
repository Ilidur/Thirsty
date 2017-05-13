#include "SoilMoistureSensor.h"

//#define DEBUG

const int iINFORM_LED_PIN = 13;
const int iTRANSISTOR_PIN = 2;

// Trigger value for notification. Needs more experimentation.
const float fMAX_DRY_MEASUREMENT = 0.64f;

const int iSENSOR_COUNT = 1;
SoilMoistureSensor<10> axSMSensors[iSENSOR_COUNT];
 
void setup() 
{
   for (int i=0 ; i<iSENSOR_COUNT; i++)
  {
    axSMSensors[i].SetPin(i);
  }

   //Power sensor control pin
   pinMode(iTRANSISTOR_PIN, OUTPUT);
   digitalWrite(iTRANSISTOR_PIN, LOW); 

   //Notification LED 
   pinMode(iINFORM_LED_PIN, OUTPUT);
   digitalWrite(iINFORM_LED_PIN, LOW); 

   #ifdef DEBUG
   Serial.begin(9600);  
   #endif
}
 
void loop() {

  bool bIsReadyToSleep = false;
  // Turn on the sensors
  digitalWrite(iTRANSISTOR_PIN,HIGH);
  // Wait for power to stabilise. (probably not needed?)
  delay(50);
  
  // read the value from the sensors
  for (int i=0 ; i<iSENSOR_COUNT; i++)
  {
    axSMSensors[i].Sample();
  
    if (axSMSensors[i].DoesSensorHaveValue())
    {

      const float fValue = axSMSensors[i].GetNormalisedValue(); //Between [0, 1)

      #ifdef DEBUG
      Serial.print("sensor");
      Serial.print(i);
      Serial.print(" = ");                       
      Serial.println(fValue);
      #endif

      if ( fValue < fMAX_DRY_MEASUREMENT )
      {
        digitalWrite(iINFORM_LED_PIN, HIGH);
      }
      else
      {
        digitalWrite(iINFORM_LED_PIN, LOW); 
      }
      
      axSMSensors[i].ClearStoredValue();
    }  
  }   
    // We're done so turn off the sensors
    digitalWrite(iTRANSISTOR_PIN,LOW);
    delay(200);
}
