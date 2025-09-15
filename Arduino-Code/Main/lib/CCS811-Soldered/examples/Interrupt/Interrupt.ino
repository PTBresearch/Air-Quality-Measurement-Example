/**
 **************************************************
 *
 * @file        Interrupt.ino
 * @brief       This example shows use of interrupts in CCS811 sensors
 *
 * @authors     @ soldered.com
 * @copyright   [MIT License](http://opensource.org/licenses/MIT)
 *              
 *              www.Solde.red/333009
 ***************************************************/
#include <Wire.h>
#include "CCS811-SOLDERED.h"

#define PIN_NOT_WAKE 5
#define PIN_NOT_INT 6

CCS_811 myCCS811;

//Global sensor object
//---------------------------------------------------------------
void setup()
{
  //Start the serial
  Serial.begin(115200);
  Serial.println();
  Serial.println("...");

  Wire.begin();

  //This begins the CCS811 sensor and prints error status of .beginWithStatus()
  CCS_811::CCS811_Status_e returnCode = myCCS811.beginWithStatus();
  Serial.print("CCS811 begin exited with: ");
  //Pass the error code to a function to print the results
  Serial.println(myCCS811.statusString(returnCode));

  //This sets the mode to 60 second reads, and prints returned error status.
  returnCode = myCCS811.setDriveMode(2);
  Serial.print("Mode request exited with: ");
  Serial.println(myCCS811.statusString(returnCode));

  //Configure and enable the interrupt line,
  //then print error status
  pinMode(PIN_NOT_INT, INPUT_PULLUP);
  returnCode = myCCS811.enableInterrupts();
  Serial.print("Interrupt configuation exited with: ");
  Serial.println(myCCS811.statusString(returnCode));

  //Configure the wake line
  pinMode(PIN_NOT_WAKE, OUTPUT);
  digitalWrite(PIN_NOT_WAKE, 1); //Start asleep
}
//---------------------------------------------------------------
void loop()
{
  //Look for interrupt request from CCS811
  if (digitalRead(PIN_NOT_INT) == 0)
  {
    //Wake up the CCS811 logic engine
    digitalWrite(PIN_NOT_WAKE, 0);
    //Need to wait at least 50 us
    delay(1);
    //Interrupt signal caught, so cause the CCS811 to run its algorithm
    myCCS811.readAlgorithmResults(); //Calling this function updates the global tVOC and CO2 variables

    Serial.print("CO2[");
    Serial.print(myCCS811.getCO2());
    Serial.print("] tVOC[");
    Serial.print(myCCS811.getTVOC());
    Serial.print("] millis[");
    Serial.print(millis());
    Serial.print("]");
    Serial.println();

    //Now put the CCS811's logic engine to sleep
    digitalWrite(PIN_NOT_WAKE, 1);
    //Need to be asleep for at least 20 us
    delay(1);
  }
  delay(1); //cycle kinda fast
}

//printSensorError gets, clears, then prints the errors
//saved within the error register.
void printSensorError()
{
  uint8_t error = myCCS811.getErrorRegister();

  if (error == 0xFF) //comm error
  {
    Serial.println("Failed to get ERROR_ID register.");
  }
  else
  {
    Serial.print("Error: ");
    if (error & 1 << 5)
      Serial.print("HeaterSupply");
    if (error & 1 << 4)
      Serial.print("HeaterFault");
    if (error & 1 << 3)
      Serial.print("MaxResistance");
    if (error & 1 << 2)
      Serial.print("MeasModeInvalid");
    if (error & 1 << 1)
      Serial.print("ReadRegInvalid");
    if (error & 1 << 0)
      Serial.print("MsgInvalid");
    Serial.println();
  }
}