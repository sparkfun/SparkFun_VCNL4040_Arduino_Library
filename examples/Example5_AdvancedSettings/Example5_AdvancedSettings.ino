/*
  Proximity Sensing with the VCNL4040 IR based sensor
  By: Nathan Seidle
  SparkFun Electronics
  Date: April 17th, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware License).

  This example shows how to use different Wire ports, fast I2C, and various advanced
  settings that are supported by the library.
  
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
  Wire.setClock(400000); //Run the I2C bus at fat 400kHz
  proximitySensor.begin(); //Initialize the sensor

  proximitySensor.powerOnAmbient(); //Turn on the ambient sensor
  
  //If you have a platform with multiple Wire buses (for example SAMD21 with Wire1)
  //you can pass them into the library and it will communicate on that port. For example:
  //Wire1.begin();
  //proximitySensor.begin(Wire1);

  //Set the integration time for the proximity sensor
  //1 to 8 is valid
  proximitySensor.setProxIntegrationTime(8); 

  //Set the integration time for the ambient light sensor in milliseconds
  //80 to 640ms is valid
  proximitySensor.setAmbientIntegrationTime(80);

  //If sensor sees more than this, interrupt pin will go low
  proximitySensor.setProxHighThreshold(2000); 

  //The int pin will stay low until the value goes below the low threshold value
  proximitySensor.setProxLowThreshold(150); 

  //Enable both 'away' and 'close' interrupts
  proximitySensor.setProxInterruptType(VCNL4040_PS_INT_BOTH); 

  //This causes the int pin to go low every time a reading is outside the thresholds
  //Get a multimeter and probe the INT pin to see this feature in action
  proximitySensor.enableProxLogicMode(); 
}

void loop()
{
  unsigned int proxValue = proximitySensor.getProximity();
  Serial.print("Prox value[");
  Serial.print(proxValue);
  
  unsigned int ambientValue = proximitySensor.getAmbient(); 
  Serial.print("] Ambient light level[");
  Serial.print(ambientValue);
  Serial.print("]");

  Serial.println();

  delay(10);
}

