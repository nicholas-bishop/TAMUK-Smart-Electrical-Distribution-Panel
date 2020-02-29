//
//  Description: code to control relay and monitor current transformers on arduino controller
//  Authors: Jose Martinez, Nicholas Bishop
//

#define utilityrelay 2
#define batteryrelay 7
#define load2 3
#define load3 4
#define load4 5
//#define load5 6
const int pvi = A0;
const int sensor0 = A1;
const int sensor1 = A2;
const int sensor2 = A3;
const int sensor3 = A4;
const int sensor4 = A5;

float nCurrentThruWire;
float nCurrThruResistorPP;
float nCurrThruResistorRMS;
float nCurrentThruWire1;
float nCurrThruResistorPP1;
float nCurrThruResistorRMS1;
float nCurrentThruWire2;
float nCurrThruResistorPP2;
float nCurrThruResistorRMS2;
float nCurrentThruWire3;
float nCurrThruResistorPP3;
float nCurrThruResistorRMS3;
float nCurrentThruWire4;
float nCurrThruResistorPP4;
float nCurrThruResistorRMS4;

float nVPP;
float nVPP1;
float nVPP2;
float nVPP3;
float nVPP4;
float nVPP5;
float power;
float power1;
float power2;
float power3;
float power4;

// voltage delay variables
boolean utilFlag = false; // flag for when utility power is off
float utilVoltage;

// for timing delay interrupt
unsigned long time_now = 0;
int period = 30000;

// for overcurrent stuff
int RESET;
String strVar;

// Overcurrent flag, can only be reset on PI !!
boolean currFlag = false; 



void setup() {
  Serial.begin(115200);
  pinMode(utilityrelay, OUTPUT);   //SSR switching on off pin
  pinMode(batteryrelay, OUTPUT);   //SSR switching on off pin
  pinMode(load2, OUTPUT);
  pinMode(load3, OUTPUT);
  pinMode(load4, OUTPUT);
  //pinMode(load5, OUTPUT);
  pinMode(sensor0, INPUT);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);

  // voltage delay upon startup
  // while utility had no power
  while (!utilFlag)
  {
    // get analog voltage reading
    utilVoltage = initVoltReading();
    // if reading > 2.0 volts
    if (utilVoltage > 1.0)
    {
      // wait 1 second
      delay(900);
      // if reading still > 2.0 volts
      utilVoltage = initVoltReading();
      if (utilVoltage > 1.0)
      {
        utilFlag = true;
      } // end inner if
    } // end outer if
  } // end while loop

}

void loop()
{

  // while utility power is lost
  while (!utilFlag)
  {
    Current(); // !!!
    // get analog voltage reading
    utilVoltage = initVoltReading(); // reusing same init voltage function
    // if voltage is greater than 1 volt
    if (utilVoltage > 1.0)
    {
      Current(); // !!!!!
      // wait conecutive 30 seconds for util to comeback
      time_now = millis(); // get the time right now
      // keep waiting until 30 seconds have passed with consecutive utility voltage
      while (millis() < time_now + period)
      {
        // if voltage is seen (<1v) reset timer to 30 seconds
        if (initVoltReading() < 1.0)
        {
          time_now = millis(); // grab the n
        }
        Current(); // ********* THIS WAS ADDED HERE FROM BOTTOM IF STATEMENT
      }
      // if AFTER 30 consecutive seconds, reading still greater than 1 volt
      utilFlag = true;
    }
  }

  // ORIGINAL function calls
  // normal operation
  if (utilFlag)
  {
    volts();
    Current();
  }


}


//**********************************************************************
//   This function is just to get an initial utility voltage reading   *
//   used in the setup()                                               *
//**********************************************************************

float initVoltReading()
{
  float initVoltage; // temp value
  int vReading = analogRead(pvi);
  initVoltage = (5. / 1023.) * vReading;
  return initVoltage;
}


void volts(void)
{
  float Volt;

  int value = analogRead ( pvi);
  Volt = (5. / 1023.) * value;

  // utility power loss, now going to battery backup
  if (Volt < 1.0) {

    digitalWrite(utilityrelay, LOW);
    digitalWrite(batteryrelay, HIGH);
    digitalWrite(load2, HIGH);
    digitalWrite(load4, HIGH);
    //digitalWrite(load3, LOW);
    //digitalWrite(load5, LOW);
    utilFlag = false;
  }


  // utility power is back, return from battery
  if ( (Volt >= 1.0) && (!currFlag) ) 
  {
    digitalWrite(utilityrelay, HIGH);
    digitalWrite(batteryrelay, LOW);
    digitalWrite(load2, HIGH);
    digitalWrite(load3, HIGH);
    digitalWrite(load4, HIGH);
    //digitalWrite(load5, HIGH);
    utilFlag = true;
  }
  // This is the case where the current flag is raised
  else if ( (Volt >= 1.0) && (currFlag) )
  {
    digitalWrite(load3, LOW);
  }
  
}



void Current() {
  nVPP = getVPP();
  nVPP1 = getVPP1();
  nVPP2 = getVPP2();
  nVPP3 = getVPP3();
  nVPP4 = getVPP4();


  // ***** Current from Relay 1  *****
  nCurrThruResistorPP = (nVPP / 200.0) * 1000.0;
  nCurrThruResistorRMS = nCurrThruResistorPP * 0.707;
  nCurrentThruWire = nCurrThruResistorRMS * 1;
  power = nCurrentThruWire * 120;

  // ***** Current from Relay 2  *****
  nCurrThruResistorPP1 = (nVPP1 / 200.0) * 1000.0;
  nCurrThruResistorRMS1 = nCurrThruResistorPP1 * 0.707;
  nCurrentThruWire1 = nCurrThruResistorRMS1 * 1;
  power1 = nCurrentThruWire1 * 120;

  // **** Current from Relay 3  *****
  nCurrThruResistorPP2 = (nVPP2 / 200.0) * 1000.0;
  nCurrThruResistorRMS2 = nCurrThruResistorPP2 * 0.707;
  nCurrentThruWire2 = nCurrThruResistorRMS2 * 1;
  power2 = nCurrentThruWire1 * 120;

  // **** Current from Relay 4  *****
  nCurrThruResistorPP3 = (nVPP3 / 200.0) * 1000.0;
  nCurrThruResistorRMS3 = nCurrThruResistorPP3 * 0.707;
  nCurrentThruWire3 = nCurrThruResistorRMS3 * 1;
  power3 = nCurrentThruWire1 * 120;

  // **** Current from Relay 5  *****
  nCurrThruResistorPP4 = (nVPP4 / 200.0) * 1000.0;
  nCurrThruResistorRMS4 = nCurrThruResistorPP4 * 0.707;
  nCurrentThruWire4 = nCurrThruResistorRMS4 * 1;
  power4 = nCurrentThruWire4 * 120;


  //***************************************************
  //   Over Current Case                              *
  //***************************************************

  if (nCurrentThruWire2 > 0.6)
  {
    currFlag = true; // latch
    strVar = "";
    if (Serial.available())
    {
      strVar = Serial.readString();
    }
    if(strVar)
    {
      digitalWrite(load3, HIGH);
      currFlag = false;
    }
  }

  //***************************************************
  //   End of Over Current Case                       *
  //***************************************************


  Serial.print(nCurrentThruWire);
  //Serial.print(" R1 ");
  Serial.print(",");
  Serial.print(power);
  Serial.print(",");

  Serial.print(nCurrentThruWire1);
  //Serial.print(" R2 ");
  Serial.print(",");
  Serial.print(power1);
  Serial.print(",");
  Serial.print(nCurrentThruWire2);
  //Serial.print(" R3 ");
  Serial.print(",");

  Serial.print(power2);
  Serial.print(",");
  Serial.print(nCurrentThruWire3);
  // Serial.print(" R4");
  Serial.print(",");

  Serial.print(power3);
  Serial.print(",");
  Serial.print(nCurrentThruWire4);
  //Serial.print(" R5 ");
  Serial.print(",");

  Serial.print(power4);
  Serial.print(",");

  Serial.println();
}

float getVPP()
{
  float result;
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
  uint32_t start_time = millis();
  while ((millis() - start_time) < 25) //sample for 1 Sec
  {
    readValue = analogRead(sensor0);
    // see if you have a new maxValue
    if (readValue > maxValue)
    {
      /*record the maximum sensor value*/
      maxValue = readValue;
    }
  }

  // Convert the digital data to a voltage
  result = (maxValue * 5.0) / 1024.0;

  return result;
}

float getVPP1()

{
  float result1;
  int readValue1;
  int maxValue1 = 0;          // store max value here
  uint32_t start_time = millis();
  while ((millis() - start_time) < 25) //sample for 1 Sec
  {
    readValue1 = analogRead(sensor1);


    // see if you have a new maxValue
    if (readValue1 > maxValue1)
    {
      /*record the maximum sensor value*/
      maxValue1 = readValue1;
    }
  }

  // Convert the digital data to a voltage
  result1 = (maxValue1 * 5.0) / 1024.0;

  return result1;
}
float getVPP2()

{

  float result2;
  int readValue2;
  int maxValue2 = 0;          // store max value here
  uint32_t start_time = millis();
  while ((millis() - start_time) < 25) //sample for 1 Sec
  {
    readValue2 = analogRead(sensor2);


    // see if you have a new maxValue
    if (readValue2 > maxValue2)
    {
      /*record the maximum sensor value*/
      maxValue2 = readValue2;
    }
  }

  // Convert the digital data to a voltage
  result2 = (maxValue2 * 5.0) / 1024.0;

  return result2;
}

float getVPP3()
{
  float result3;
  int readValue3;
  int maxValue3 = 0;          // store max value here
  uint32_t start_time = millis();
  while ((millis() - start_time) < 25) //sample for 1 Sec
  {
    readValue3 = analogRead(sensor3);


    // see if you have a new maxValue
    if (readValue3 > maxValue3)
    {
      /*record the maximum sensor value*/
      maxValue3 = readValue3;
    }
  }

  // Convert the digital data to a voltage
  result3 = (maxValue3 * 5.0) / 1024.0;

  return result3;
}

float getVPP4()

{
  float result4;
  int readValue4;
  int maxValue4 = 0;          // store max value here
  uint32_t start_time = millis();
  while ((millis() - start_time) < 25) //sample for 1 Sec
  {
    readValue4 = analogRead(sensor4);


    // see if you have a new maxValue
    if (readValue4 > maxValue4)
    {
      /*record the maximum sensor value*/
      maxValue4 = readValue4;
    }
  }

  // Convert the digital data to a voltage
  result4 = (maxValue4 * 5.0) / 1024.0;

  return result4;
}
