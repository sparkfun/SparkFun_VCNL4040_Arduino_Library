/*
  Proximity Sensing with the VCNL4040 IR based sensor
  By: Nathan Seidle
  SparkFun Electronics
  Date: April 17th, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware License).

  This example outputs IR, ambient and white light readings to the terminal.

  Along with proximity and ambient light sensing the VCNL4040 has a 'white light' 
  sensor as well. Point the sensor up and start the sketch. Then cover the 
  sensor with your hand. 
    IR readings increase as the reflected IR light increases
    Ambient light readings decrease as less ambient light can get to the sensor
    White light readings decrease as less white light is detected
  
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

  proximitySensor.begin(); //Initialize the sensor

  //Turn on everything
  proximitySensor.powerOnProximity();
  proximitySensor.powerOnAmbient();
  proximitySensor.enableWhiteChannel();
}

void loop()
{
  unsigned int proxValue = proximitySensor.getProximity();
  Serial.print("Prox value[");
  Serial.print(proxValue);
  
  unsigned int ambientValue = proximitySensor.getAmbient(); 
  Serial.print("] Ambient light level[");
  Serial.print(ambientValue);

  unsigned int whiteValue = proximitySensor.getWhite(); 
  Serial.print("] White level[");
  Serial.print(whiteValue);
  Serial.print("]");

  Serial.println();

  delay(10);
}

