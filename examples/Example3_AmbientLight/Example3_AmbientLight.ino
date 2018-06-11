/*
  Proximity Sensing with the VCNL4040 IR based sensor
  By: Nathan Seidle
  SparkFun Electronics
  Date: April 17th, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware License).

  This example outputs ambient light readings to the terminal. 
  
  Point the sensor up and start the sketch. Then cover the sensor with your hand.
  The readings decrease in value because there is less light detected.

  Hardware Connections:
  Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  Plug the sensor onto the shield
  Serial.print it out at 9600 baud to serial monitor.
*/

#include <Wire.h>

//Click here to get the library: http://librarymanager/All#SparkFun_VCNL4040
#include "SparkFun_VCNL4040_Arduino_Library.h"
VCNL4040 proximitySensor;

long startingProxValue = 0;
long deltaNeeded = 0;
boolean nothingThere = false;

void setup()
{
  Serial.begin(9600);
  Serial.println("SparkFun VCNL4040 Example");

  Wire.begin(); //Join i2c bus

  proximitySensor.begin(); //Initialize the sensor

  proximitySensor.powerOffProximity(); //Power down the proximity portion of the sensor
  
  proximitySensor.powerOnAmbient(); //Power up the ambient sensor
}

void loop()
{
  unsigned int ambientValue = proximitySensor.getAmbient(); 

  Serial.print("Ambient light level: ");
  Serial.println(ambientValue);

  delay(10);
}

