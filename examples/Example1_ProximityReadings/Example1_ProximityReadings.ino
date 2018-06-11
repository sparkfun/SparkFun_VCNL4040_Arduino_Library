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
*/

#include <Wire.h>

//Click here to get the library: http://librarymanager/All#SparkFun_VCNL4040
#include "SparkFun_VCNL4040_Arduino_Library.h"
VCNL4040 proximitySensor;

void setup()
{
  Serial.begin(9600);
  Serial.println("SparkFun VCNL4040 Example");

  Wire.begin(); //Join i2c bus

  if (proximitySensor.begin() == false)
  {
    Serial.println("Device not found. Please check wiring.");
    while (1); //Freeze!
  }
}

void loop()
{
  //Get proximity value. The value ranges from 0 to 65535
  //so we need an unsigned integer or a long.
  unsigned int proxValue = proximitySensor.getProximity(); 

  Serial.print("Proximity Value: ");
  Serial.print(proxValue);
  Serial.println();

  delay(10);
}

