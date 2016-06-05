
template < int iBUFFER_SIZE>
class SoilMoistureSensor
{
  
     
  private:
    int m_aiSensorRawData[iBUFFER_SIZE];
    int m_iSensorPin;
    float m_fSensorValue;
    int m_bSensorHasValue;
    int m_iCurrentSamplingIndex;
    
  public:
    
    // Must set pin separately
    SoilMoistureSensor ()
    {
      m_iSensorPin = 0;
      m_fSensorValue = 0.0f; 
      m_bSensorHasValue = false; 
      
      m_iCurrentSamplingIndex = 0;
    }
    
    // Takes an analog input pin.
    SoilMoistureSensor (int iSensorPortNumber)
    {
      m_iSensorPin = iSensorPortNumber;
      m_fSensorValue = 0.0f; 
      m_bSensorHasValue = false; 
      
      m_iCurrentSamplingIndex = 0;
    }
    
    void SetPin(int iPin)
    {
      m_iSensorPin = iPin;
    }
    
    boolean DoesSensorHaveData()
    {
      return m_bSensorHasValue;
    }
    
    void Sample()
    {
      m_aiSensorRawData[m_iCurrentSamplingIndex] = analogRead(m_iSensorPin);
      m_iCurrentSamplingIndex++;
      
      if (m_iSensorPin == iBUFFER_SIZE)
      {
        m_fSensorValue = 0;
        for (int i = 0; i < iBUFFER_SIZE; i++)
        {
          m_fSensorValue += m_aiSensorRawData[i];
        }
        m_fSensorValue /= iBUFFER_SIZE;
        m_bSensorHasValue = true;
      }
      
      m_iCurrentSamplingIndex %= iBUFFER_SIZE;
   }
   
   float GetValue()
   {
     return m_fSensorValue;
   }
   
   void ClearStoredValue()
   {
     m_bSensorHasValue = false;
     m_fSensorValue = 0.0f;
   }
};

const int iSENSOR_COUNT = 2;
SoilMoistureSensor<10> axSMSensors[iSENSOR_COUNT];
 
void setup() 
{
  
  // declare the ledPin as an OUTPUT:
   Serial.begin(9600);  
}
 
void loop() {
  // read the value from the sensor:
  
  for (int i=0 ; i<iSENSOR_COUNT; i++)
  {
    axSMSensors[i].Update(); 
    delay(1000);
  
    if (axSMSensors[i].DoesSensorHaveValue())
    {
      Serial.print("sensor");
      Serial.print(i);
      Serial.print(" = ");                       
      Serial.println(axSMSensors[i].GetValue());
      
      axSMSensors[i].ClearStoredValue();
    }  
  }   
}
