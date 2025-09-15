/**
 **************************************************
 *
 * @file        BasicReadings.ino
 * @brief       Basic example for using CCS811 Sensor with easyC/I2C comunication
 *              It shows how to use our API wrapper to take some basic readings
 *
 *
 * @authors     @ soldered.com
 * @copyright   [MIT License](http://opensource.org/licenses/MIT)
 *              www.Solde.red/333009
 ***************************************************/

#include "CCS811-SOLDERED.h"
#include "Wire.h"

CCS_811 ccs811Sensor;

void setup()
{
    Wire.begin();
    Serial.begin(115200);
    ccs811Sensor.begin();
    Serial.println("CCS811 Sensor Basic Example");
}

void loop()
{
    // Check to see if data is ready with .dataAvailable()
    if (ccs811Sensor.dataAvailable())
    {
        // If so, have the sensor read and calculate the results.
        // Get them later
        //Read Alorithm only reads data into internal registers of sensor, doesnt return anything
        ccs811Sensor.readAlgorithmResults();

        Serial.print("CO2[");
        // Returns calculated CO2 reading
        Serial.print(ccs811Sensor.getCO2());
        Serial.println("]");

        Serial.print("tVOC[");
        // Returns calculated TVOC reading
        Serial.print(ccs811Sensor.getTVOC());
        Serial.println("]");

        Serial.print("millis[");
        // Display the time since program start
        Serial.print(millis());
        Serial.print("]");
        Serial.println();
        Serial.println();
    }

    delay(5000); // Don't spam the I2C bus
}
