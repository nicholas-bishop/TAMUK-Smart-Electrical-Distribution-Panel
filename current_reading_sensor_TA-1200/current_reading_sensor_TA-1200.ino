// 
// Author(s): Nicholas Bishop, Jose Martinez 
// Last modified: 2/10/2020
//

// Sources/references
// Code to read current is adapted from Henry's Bench
// Henry's Bench TA-1200 AC Current Sensor Tutorial


const byte NUM_INPUTS = 12; 
const byte pvi[NUM_INPUTS] = { A0, A1, A2, A3, A4, A5 };
float current; 

float nVPP;   // Voltage measured across resistor
float nCurrThruResistorPP; // Peak Current Measured Through Resistor
float nCurrThruResistorRMS; // RMS current through Resistor
float nCurrentThruWire;     // Actual RMS current in Wire
float temp; 

 void setup() 
 {
     Serial.begin(115200); 
     pinMode(pvi[NUM_INPUTS], INPUT); 
 }
 
 
 void loop() 
 {
     populateArray(pvi, NUM_INPUTS);
    
 }


 void populateArray(const byte arr[], const byte arrSize)
 {
    float tempValue; 
    // populating array with voltage and current readings 
    for (int i = 0; i < 2; i++)
    {
      //arr[i] = getCurrent(pvi, i);
      tempValue = getCurrent(pvi, i);
      Serial.print(tempValue);
      Serial.print(",");
    }
    Serial.println(" ");
 }



 float getCurrent(const byte arr[], int arrIndex)
 {
    int readValue;             //value read from the sensor
    int maxValue = 0;          // store max value here
     uint32_t start_time = millis();
     while((millis()-start_time) < 1000) //sample for 1 Sec
     {
         readValue = analogRead(arr[arrIndex]);
         // see if you have a new maxValue
         if (readValue > maxValue) 
         {
             /*record the maximum sensor value*/
             maxValue = readValue;
         }
     }
   
     // Convert the digital data to a voltage
     nVPP = (maxValue * 5.)/1024.;

      /*
     Use Ohms law to calculate current across resistor
     and express in mA 
     */
     nCurrThruResistorPP = (nVPP/200.) * 1000.;
     /* 
     Use Formula for SINE wave to convert
     to RMS 
     */
     nCurrThruResistorRMS = nCurrThruResistorPP * 0.707;
     /* 
     Current Transformer Ratio is 1000:1...
     
     Therefore current through 200 ohm resistor
     is multiplied by 1000 to get input current
     */
     nCurrentThruWire = nCurrThruResistorRMS * 1000;

     return nCurrentThruWire; 
     
 }


 /************************************ 
In order to calculate RMS current, we need to know
the peak to peak voltage measured at the output across the
200 Ohm Resistor

The following function takes one second worth of samples
and returns the peak value that is measured
 *************************************/
 
 
float getVPP()
{
  float result;
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
   uint32_t start_time = millis();
   while((millis()-start_time) < 1000) //sample for 1 Sec
   {
       readValue = analogRead(A0);
       // see if you have a new maxValue
       if (readValue > maxValue) 
       {
           /*record the maximum sensor value*/
           maxValue = readValue;
       }
   }
   
   // Convert the digital data to a voltage
   result = (maxValue * 5.0)/1024.0;
  
   return result;
 }
