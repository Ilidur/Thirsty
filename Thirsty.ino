#include "SoilMoistureSensor.h"

const int iSENSOR_COUNT = 1;
SoilMoistureSensor<10> axSMSensors[iSENSOR_COUNT];
 
void setup() 
{
   for (int i=0 ; i<iSENSOR_COUNT; i++)
  {
    axSMSensors[i].SetPin(i);
  }
  // declare the ledPin as an OUTPUT:
   Serial.begin(9600);  
}
 
void loop() {
  // read the value from the sensor:
  
  for (int i=0 ; i<iSENSOR_COUNT; i++)
  {
    axSMSensors[i].Sample();
  
    if (axSMSensors[i].DoesSensorHaveValue())
    {
      Serial.print("sensor");
      Serial.print(i);
      Serial.print(" = ");                       
      Serial.println(axSMSensors[i].GetIntValue());
      
      axSMSensors[i].ClearStoredValue();
    }  
  }   
  
    delay(200);
}
