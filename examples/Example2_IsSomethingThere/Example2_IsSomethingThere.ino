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

  if (proximitySensor.begin() == false)
  {
    Serial.println("Device not found. Please check wiring.");
    while (1); //Freeze!
  }

  //Set the current used to drive the IR LED - 50mA to 200mA is allowed.
  proximitySensor.setLEDCurrent(200); //For this example, let's do max.

  //The sensor will average readings together by default 8 times.
  //Reduce this to one so we can take readings as fast as possible
  proximitySensor.setProxIntegrationTime(8); //1 to 8 is valid

  //Take 8 readings and average them
  for(byte x = 0 ; x < 8 ; x++)
  {
    startingProxValue += proximitySensor.getProximity();
  }
  startingProxValue /= 8;

  deltaNeeded = (float)startingProxValue * 0.05; //Look for 5% change
  if(deltaNeeded < 5) deltaNeeded = 5; //Set a minimum
}

void loop()
{
  unsigned int proxValue = proximitySensor.getProximity(); 

  Serial.print("Prox: ");
  Serial.print(proxValue);
  Serial.print(" ");

  //Let's only trigger if we detect a 5% change from the starting value
  //Otherwise, values at the edge of the read range can cause false triggers
  if(proxValue > (startingProxValue + deltaNeeded))
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

