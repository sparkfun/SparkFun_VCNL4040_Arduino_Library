/*
  Proximity Sensing with the VCNL4040 IR based sensor
  By: Nathan Seidle
  SparkFun Electronics
  Date: April 17th, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware License).

  This example takes an initial reading at power on. If the reading changes
  by a significant amount the sensor reports that something is present.

  Point the sensor up and start the sketch. Then bring your hand infront of the sensor.

  Hardware Connections:
  Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  Plug the sensor onto the shield
  Serial.print it out at 9600 baud to serial monitor.
*/

#include <Wire.h>

#include "SparkFun_VCNL4040_Arduino_Library.h"
VCNL4040 distanceSensor;

long startingProxValue = 0;
boolean nothingThere = false;
// http://librarymanager/All#SparkFun_Simultaneous_RFID_Tag_Reader_Library

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

  //Set the current used to drive the IR LED - 50mA to 200mA is allowed.
  distanceSensor.setLEDCurrent(200); //For this example, let's do max.

  //The sensor will average readings together by default 8 times.
  //Reduce this to one so we can take readings as fast as possible
  distanceSensor.setProxIntegrationTime(8); //1 to 8 is valid

  //Take 8 readings and average them
  for(byte x = 0 ; x < 8 ; x++)
  {
    startingProxValue += distanceSensor.getProximity();
  }
  startingProxValue /= 8;
}

void loop()
{
  unsigned int proxValue = distanceSensor.getProximity(); 

  Serial.print("Prox: ");
  Serial.print(proxValue);
  Serial.print(" ");

  //Let's only trigger if we detect a 100 value change from the starting value
  //Otherwise, values at the edge of the read range can cause false triggers
  if(proxValue > (startingProxValue + 100))
  {
    Serial.print("Something is there!");
    nothingThere = false;
  }
  else
  {
    if(nothingThere == false) Serial.print("I don't see anything");
    nothingThere = true;
  }

  Serial.println();
  delay(10);
}

