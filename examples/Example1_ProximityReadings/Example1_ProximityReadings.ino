/*
  Proximity Sensing with the VCNL4040 IR based sensor
  By: Nathan Seidle
  SparkFun Electronics
  Date: April 17th, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware License).

  This example outputs the IR Proximity Value from the VCNL4040 sensor.

  Hardware Connections:
  Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  Plug the sensor onto the shield
  Serial.print it out at 9600 baud to serial monitor.

   TODO:
   Set IR current to low/high
   is something there?
   Power on/off different parts of sensor
   output ambient light and white
   Set thresholds and print when it's close/far, dark/light
   fast i2c / wire port
   possibly convert log scale reading to linear distance
*/

#include <Wire.h>

#include "SparkFun_VCNL4040_Arduino_Library.h"
VCNL4040 distanceSensor;

void setup()
{
  Serial.begin(9600);
  Serial.println("Hello, world!");

  Wire.begin(); //Join i2c bus

  if (distanceSensor.begin() == false)
  {
    Serial.println("Device not found. Please check wiring.");
    while (1); //Freeze!
  }
}

void loop()
{
  //Get proximity value. The value ranges from 0 to 65535
  //so we need an unsigned integer or a long.
  unsigned int proxValue = distanceSensor.getProximity(); 

  Serial.print("Proximity Value: ");
  Serial.print(proxValue);
  Serial.println();

  delay(10);
}

