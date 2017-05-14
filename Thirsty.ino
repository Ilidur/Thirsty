#include "SoilMoistureSensor.h"

#define DEBUG

using u_int = unsigned int;

// Pins
const u_int uINFORM_LED_PIN = 13;
const u_int uTRANSISTOR_PIN = 2;

// Trigger value for notification. Needs more experimentation.
const float fMAX_DRY_MEASUREMENT = 0.64f;

// Sensors
const u_int uSENSOR_COUNT = 1;
const size_t uSENSOR_BUFFER = 10;
SoilMoistureSensor<uSENSOR_BUFFER> axSMSensors[uSENSOR_COUNT];

// Delays
const u_int uTRANSISTOR_WAKEUP_DELAY = 100;
const u_int uSAMPLE_DELAY = 50;
const u_int uLOOP_DELAY = 10000;
 
void setup() 
{
  for (int i=0 ; i<uSENSOR_COUNT; i++)
  {
    axSMSensors[i].SetPin(i);
  }

   //Power sensor control pin
   pinMode(uTRANSISTOR_PIN, OUTPUT);
   digitalWrite(uTRANSISTOR_PIN, LOW); 

   //Notification LED 
   pinMode(uINFORM_LED_PIN, OUTPUT);
   digitalWrite(uINFORM_LED_PIN, LOW); 

   #ifdef DEBUG
   Serial.begin(9600);  
   #endif
}
 
void loop()
{
  // Turn on the sensors
  digitalWrite(uTRANSISTOR_PIN,HIGH);
  // Wait for power to stabilise. (probably not needed?)
  delay(uTRANSISTOR_WAKEUP_DELAY);

  for (u_int j=0 ; j<uSENSOR_BUFFER; j++)
  {
    // read the value from the sensors
    for (u_int i=0 ; i<uSENSOR_COUNT; i++)
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
          digitalWrite(uINFORM_LED_PIN, HIGH);
        }
        else
        {
          digitalWrite(uINFORM_LED_PIN, LOW); 
        }
        
        axSMSensors[i].ClearStoredValue();
      }  
    }
    
    delay(uSAMPLE_DELAY);
  }
  
  // We're done so turn off the sensors
  digitalWrite(uTRANSISTOR_PIN,LOW);

  // The data shouldn't change that often so leave it alone for a while.
  delay(uLOOP_DELAY);
}
