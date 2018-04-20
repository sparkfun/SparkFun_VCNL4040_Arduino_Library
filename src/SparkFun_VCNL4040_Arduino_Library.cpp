/*
  This is a library written for the VCNL4040 distance sensor.
  By Nathan Seidle @ SparkFun Electronics, April 17th, 2018

  The VCNL4040 is a simple IR presence and ambient light sensor. This 
  sensor is excellent for detecting if something has appeared in front 
  of the sensor. We often see this type of sensor on automatic towel 
  dispensers, automatic faucets, etc. You can detect objects qualitatively 
  up to 20cm away. This means you can detect if something is there, 
  and if it is closer or further away since the last reading, but it's 
  difficult to say it is 7.2cm away. If you need quantitative distance 
  readings (for example sensing that an object is 177mm away) check out 
  the SparkFun Time of Flight (ToF) sensors with mm accuracy.

  This library offers the full range of settings for the VCNL4040. Checkout
  the various examples provided with the library but also please give the datasheet
  a read.

  https://github.com/sparkfun/SparkFun_VCNL4040_Arduino_Library

  Development environment specifics:
  Arduino IDE 1.8.5

  SparkFun labored with love to create this code. Feel like supporting open
  source hardware? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14690
*/

#include <Wire.h>

#include "SparkFun_VCNL4040_Arduino_Library.h"

const uint8_t VCNL4040_ADDR = 0x60; //7-bit unshifted I2C address of VCNL4040

//Used to select between upper and lower byte of command register
#define LOWER true
#define UPPER false

//VCNL4040 Command Codes
#define VCNL4040_ALS_CONF 0x00
#define VCNL4040_ALS_THDH 0x01
#define VCNL4040_ALS_THDL 0x02
#define VCNL4040_PS_CONF1 0x03 //Lower
#define VCNL4040_PS_CONF2 0x03 //Upper
#define VCNL4040_PS_CONF3 0x04 //Lower
#define VCNL4040_PS_MS 0x04 //Upper
#define VCNL4040_PS_CANC 0x05
#define VCNL4040_PS_THDL 0x06
#define VCNL4040_PS_THDH 0x07
#define VCNL4040_PS_DATA 0x08
#define VCNL4040_ALS_DATA 0x09
#define VCNL4040_WHITE_DATA 0x0A
#define VCNL4040_INT_FLAG 0x0B //Upper
#define VCNL4040_ID 0x0C

//Class constructor
VCNL4040::VCNL4040(void) {}

//Check comm with sensor and set it to default init settings
boolean VCNL4040::begin(TwoWire &wirePort) {
  _i2cPort = &wirePort; //Grab which port the user wants us to use

  //We expect caller to begin their I2C port, with the speed of their choice external to the library
  //But if they forget, we start the hardware here.
  _i2cPort->begin();

  //Check connection
  if (isConnected() == false) return (false); //I2C comm failure

  if (getID() != 0x0186) return (false); //Check default ID value

  //Configure the various parts of the sensor
  setLEDCurrent(200); //Max IR LED current

  setIRDutyCycle(40); //Set to highest duty cycle

  setProxIntegrationTime(8); //Set to max integration

  setProxResolution(16); //Set to 16-bit output
  
  enableSmartPersistance(); //Turn on smart presistance

  powerOnProximity(); //Turn on prox sensing

  //setAmbientIntegrationTime(VCNL4040_ALS_IT_80MS); //Keep it short
  //powerOnAmbient(); //Turn on ambient sensing

  return (true);
}

//Test to see if the device is responding
boolean VCNL4040::isConnected(void) {
  _i2cPort->beginTransmission((uint8_t)VCNL4040_ADDR);
  return (_i2cPort->endTransmission() == 0);
}

//Set the duty cycle of the IR LED. The higher the duty
//ratio, the faster the response time achieved with higher power
//consumption. For example, PS_Duty = 1/320, peak IRED current = 100 mA,
//averaged current consumption is 100 mA/320 = 0.3125 mA.
void VCNL4040::setIRDutyCycle(uint16_t dutyValue)
{
  if(dutyValue > 320 - 1) dutyValue = VCNL4040_PS_DUTY_320;
  else if(dutyValue > 160 - 1) dutyValue = VCNL4040_PS_DUTY_160;
  else if(dutyValue > 80 - 1) dutyValue = VCNL4040_PS_DUTY_80;
  else dutyValue = VCNL4040_PS_DUTY_40;
  
  bitMask(VCNL4040_PS_CONF1, LOWER, VCNL4040_PS_DUTY_MASK, dutyValue);
}

//Set the Prox interrupt persistance value
//The PS persistence function (PS_PERS, 1, 2, 3, 4) helps to avoid
//false trigger of the PS INT. It defines the amount of
//consecutive hits needed in order for a PS interrupt event to be triggered.
void VCNL4040::setProxInterruptPersistance(uint8_t persValue)
{
  bitMask(VCNL4040_PS_CONF1, LOWER, VCNL4040_PS_PERS_MASK, persValue);
}

//Set the Ambient interrupt persistance value
//The ALS persistence function (ALS_PERS, 1, 2, 4, 8) helps to avoid
//false trigger of the ALS INT. It defines the amount of
//consecutive hits needed in order for a ALS interrupt event to be triggered.
void VCNL4040::setAmbientInterruptPersistance(uint8_t persValue)
{
  bitMask(VCNL4040_ALS_CONF, LOWER, VCNL4040_ALS_PERS_MASK, persValue);
}

void VCNL4040::enableAmbientInterrupts(void)
{
  bitMask(VCNL4040_ALS_CONF, LOWER, VCNL4040_ALS_INT_EN_MASK, VCNL4040_ALS_INT_ENABLE);
}
void VCNL4040::disableAmbientInterrupts(void)
{
  bitMask(VCNL4040_ALS_CONF, LOWER, VCNL4040_ALS_INT_EN_MASK, VCNL4040_ALS_INT_DISABLE);
}

//Power on or off the ambient light sensing portion of the sensor
void VCNL4040::powerOnAmbient(void)
{
  bitMask(VCNL4040_ALS_CONF, LOWER, VCNL4040_ALS_SD_MASK, VCNL4040_ALS_SD_POWER_ON);
}
void VCNL4040::powerOffAmbient(void)
{
  bitMask(VCNL4040_ALS_CONF, LOWER, VCNL4040_ALS_SD_MASK, VCNL4040_ALS_SD_POWER_OFF);
}

//Sets the integration time for the ambient light sensor
void VCNL4040::setAmbientIntegrationTime(uint16_t timeValue)
{
  if(timeValue > 640 - 1) timeValue = VCNL4040_ALS_IT_640MS;
  else if(timeValue > 320 - 1) timeValue = VCNL4040_ALS_IT_320MS;
  else if(timeValue > 160 - 1) timeValue = VCNL4040_ALS_IT_160MS;
  else timeValue = VCNL4040_ALS_IT_80MS;

  bitMask(VCNL4040_ALS_CONF, LOWER, VCNL4040_ALS_IT_MASK, timeValue);
}

//Sets the integration time for the proximity sensor
void VCNL4040::setProxIntegrationTime(uint8_t timeValue)
{
  if(timeValue > 8 - 1) timeValue = VCNL4040_PS_IT_8T;
  else if(timeValue > 4 - 1) timeValue = VCNL4040_PS_IT_4T;
  else if(timeValue > 3 - 1) timeValue = VCNL4040_PS_IT_3T;
  else if(timeValue > 2 - 1) timeValue = VCNL4040_PS_IT_2T;
  else timeValue = VCNL4040_PS_IT_1T;

  bitMask(VCNL4040_PS_CONF1, LOWER, VCNL4040_PS_IT_MASK, timeValue);
}

//Power on the prox sensing portion of the device
void VCNL4040::powerOnProximity(void)
{
  bitMask(VCNL4040_PS_CONF1, LOWER, VCNL4040_PS_SD_MASK, VCNL4040_PS_SD_POWER_ON);
}

//Power off the prox sensing portion of the device
void VCNL4040::powerOffProximity(void)
{
  bitMask(VCNL4040_PS_CONF1, LOWER, VCNL4040_PS_SD_MASK, VCNL4040_PS_SD_POWER_OFF);
}

//Sets the proximity resolution
void VCNL4040::setProxResolution(uint8_t resolutionValue)
{
	if(resolutionValue > 16 - 1) resolutionValue = VCNL4040_PS_HD_16_BIT;
	else resolutionValue = VCNL4040_PS_HD_12_BIT;
	
  bitMask(VCNL4040_PS_CONF2, UPPER, VCNL4040_PS_HD_MASK, resolutionValue);
}

//Sets the proximity interrupt type
void VCNL4040::setProxInterruptType(uint8_t interruptValue)
{
  bitMask(VCNL4040_PS_CONF2, UPPER, VCNL4040_PS_INT_MASK, interruptValue);
}

//Enable smart persistance
//To accelerate the PS response time, smart
//persistence prevents the misjudgment of proximity sensing
//but also keeps a fast response time.
void VCNL4040::enableSmartPersistance(void)
{
  bitMask(VCNL4040_PS_CONF3, LOWER, VCNL4040_PS_SMART_PERS_MASK, VCNL4040_PS_SMART_PERS_ENABLE);
}
void VCNL4040::disableSmartPersistance(void)
{
  bitMask(VCNL4040_PS_CONF3, LOWER, VCNL4040_PS_SMART_PERS_MASK, VCNL4040_PS_SMART_PERS_DISABLE);
}

//Enable active force mode
//An extreme power saving way to use PS is to apply PS active force mode.
//Anytime host would like to request one proximity measurement,
//enable the active force mode. This
//triggers a single PS measurement, which can be read from the PS result registers.
//VCNL4040 stays in standby mode constantly.
void VCNL4040::enableActiveForceMode(void)
{
  bitMask(VCNL4040_PS_CONF3, LOWER, VCNL4040_PS_AF_MASK, VCNL4040_PS_AF_ENABLE);
}
void VCNL4040::disableActiveForceMode(void)
{
  bitMask(VCNL4040_PS_CONF3, LOWER, VCNL4040_PS_AF_MASK, VCNL4040_PS_AF_DISABLE);
}

//Set trigger bit so sensor takes a force mode measurement and returns to standby
void VCNL4040::takeSingleProxMeasurement(void)
{
  bitMask(VCNL4040_PS_CONF3, LOWER, VCNL4040_PS_TRIG_MASK, VCNL4040_PS_TRIG_TRIGGER);
}

//Enable the white measurement channel
void VCNL4040::enableWhiteChannel(void)
{
  bitMask(VCNL4040_PS_MS, UPPER, VCNL4040_WHITE_EN_MASK, VCNL4040_WHITE_ENABLE);
}
void VCNL4040::disableWhiteChannel(void)
{
  bitMask(VCNL4040_PS_MS, UPPER, VCNL4040_WHITE_EN_MASK, VCNL4040_WHITE_ENABLE);
}

//Enable the proximity detection logic output mode
//When this mode is selected, the INT pin is pulled low when an object is
//close to the sensor (value is above high
//threshold) and is reset to high when the object moves away (value is
//below low threshold). Register: PS_THDH / PS_THDL
//define where these threshold levels are set.
void VCNL4040::enableProxLogicMode(void)
{
  bitMask(VCNL4040_PS_MS, UPPER, VCNL4040_PS_MS_MASK, VCNL4040_PS_MS_ENABLE);
}
void VCNL4040::disableProxLogicMode(void)
{
  bitMask(VCNL4040_PS_MS, UPPER, VCNL4040_PS_MS_MASK, VCNL4040_PS_MS_DISABLE);
}

//Set the IR LED sink current to one of 8 settings
void VCNL4040::setLEDCurrent(uint8_t currentValue)
{
	if(currentValue > 200 - 1) currentValue = VCNL4040_LED_200MA;
	else if(currentValue > 180 - 1) currentValue = VCNL4040_LED_180MA;
	else if(currentValue > 160 - 1) currentValue = VCNL4040_LED_160MA;
	else if(currentValue > 140 - 1) currentValue = VCNL4040_LED_140MA;
	else if(currentValue > 120 - 1) currentValue = VCNL4040_LED_120MA;
	else if(currentValue > 100 - 1) currentValue = VCNL4040_LED_100MA;
	else if(currentValue > 75 - 1) currentValue = VCNL4040_LED_75MA;
	else currentValue = VCNL4040_LED_50MA;

	bitMask(VCNL4040_PS_MS, UPPER, VCNL4040_LED_I_MASK, currentValue);
}

//Set the proximity sensing cancelation value - helps reduce cross talk
//with ambient light
void VCNL4040::setProxCancellation(uint16_t cancelValue)
{
  writeCommand(VCNL4040_PS_CANC, cancelValue);
}

//Value that ALS must go above to trigger an interrupt
void VCNL4040::setALSHighThreshold(uint16_t threshold)
{
  writeCommand(VCNL4040_ALS_THDH, threshold);
}

//Value that ALS must go below to trigger an interrupt
void VCNL4040::setALSLowThreshold(uint16_t threshold)
{
  writeCommand(VCNL4040_ALS_THDL, threshold);
}

//Value that Proximity Sensing must go above to trigger an interrupt
void VCNL4040::setProxHighThreshold(uint16_t threshold)
{
  writeCommand(VCNL4040_PS_THDH, threshold);
}

//Value that Proximity Sensing must go below to trigger an interrupt
void VCNL4040::setProxLowThreshold(uint16_t threshold)
{
  writeCommand(VCNL4040_PS_THDL, threshold);
}

//Read the Proximity value
uint16_t VCNL4040::getProximity()
{
  return (readCommand(VCNL4040_PS_DATA));
}

//Read the Ambient light value
uint16_t VCNL4040::getAmbient()
{
  return (readCommand(VCNL4040_ALS_DATA));
}

//Read the White light value
uint16_t VCNL4040::getWhite()
{
  return (readCommand(VCNL4040_WHITE_DATA));
}

//Read the sensors ID
uint16_t VCNL4040::getID()
{
  return (readCommand(VCNL4040_ID));
}

//Returns true if the prox value rises above the upper threshold
boolean VCNL4040::isClose()
{
  uint8_t interruptFlags = readCommandUpper(VCNL4040_INT_FLAG);
  return (interruptFlags & VCNL4040_INT_FLAG_CLOSE);
}

//Returns true if the prox value drops below the lower threshold
boolean VCNL4040::isAway()
{
  uint8_t interruptFlags = readCommandUpper(VCNL4040_INT_FLAG);
  return (interruptFlags & VCNL4040_INT_FLAG_AWAY);
}

//Returns true if the prox value rises above the upper threshold
boolean VCNL4040::isLight()
{
  uint8_t interruptFlags = readCommandUpper(VCNL4040_INT_FLAG);
  return (interruptFlags & VCNL4040_INT_FLAG_ALS_HIGH);
}

//Returns true if the ALS value drops below the lower threshold
boolean VCNL4040::isDark()
{
  uint8_t interruptFlags = readCommandUpper(VCNL4040_INT_FLAG);
  return (interruptFlags & VCNL4040_INT_FLAG_ALS_LOW);
}

//Reads two consecutive bytes from a given 'command code' location
uint16_t VCNL4040::readCommand(uint8_t commandCode)
{
  _i2cPort->beginTransmission(VCNL4040_ADDR);
  _i2cPort->write(commandCode);
  if (_i2cPort->endTransmission(false) != 0) //Send a restart command. Do not release bus.
  {
    return (0); //Sensor did not ACK
  }

  _i2cPort->requestFrom((uint8_t)VCNL4040_ADDR, (uint8_t)2);
  if (_i2cPort->available())
  {
    uint8_t lsb = _i2cPort->read();
    uint8_t msb = _i2cPort->read();
    return ((uint16_t)msb << 8 | lsb);
  }

  return (0); //Sensor did not respond
}

//Write two bytes to a given command code location (8 bits)
boolean VCNL4040::writeCommand(uint8_t commandCode, uint16_t value)
{
  _i2cPort->beginTransmission(VCNL4040_ADDR);
  _i2cPort->write(commandCode);
  _i2cPort->write(value & 0xFF); //LSB
  _i2cPort->write(value >> 8); //MSB
  if (_i2cPort->endTransmission() != 0)
  {
    return (false); //Sensor did not ACK
  }

  return (true);
}

//Given a command code (address) write to the lower byte without affecting the upper byte
boolean VCNL4040::writeCommandLower(uint8_t commandCode, uint8_t newValue)
{
  uint16_t commandValue = readCommand(commandCode);
  commandValue &= 0xFF00; //Remove lower 8 bits
  commandValue |= (uint16_t)newValue; //Mask in
  return (writeCommand(commandCode, commandValue));
}

//Given a command code (address) write to the upper byte without affecting the lower byte
boolean VCNL4040::writeCommandUpper(uint8_t commandCode, uint8_t newValue)
{
  uint16_t commandValue = readCommand(commandCode);
  commandValue &= 0x00FF; //Remove upper 8 bits
  commandValue |= (uint16_t)newValue << 8; //Mask in
  return (writeCommand(commandCode, commandValue));
}

//Given a command code (address) read the lower byte
uint8_t VCNL4040::readCommandLower(uint8_t commandCode)
{
  uint16_t commandValue = readCommand(commandCode);
  return (commandValue & 0xFF);
}

//Given a command code (address) read the upper byte
uint8_t VCNL4040::readCommandUpper(uint8_t commandCode)
{
  uint16_t commandValue = readCommand(commandCode);
  return (commandValue >> 8);
}

//Given a register, read it, mask it, and then set the thing
//commandHeight is used to select between the upper or lower byte of command register
//Example:
//Write dutyValue into PS_CONF1, lower byte, using the Duty_Mask
//bitMask(VCNL4040_PS_CONF1, LOWER, VCNL4040_PS_DUTY_MASK, dutyValue);
void VCNL4040::bitMask(uint8_t commandAddress, boolean commandHeight, uint8_t mask, uint8_t thing)
{
  // Grab current register context
  uint8_t registerContents;
  if (commandHeight == LOWER) registerContents = readCommandLower(commandAddress);
  else registerContents = readCommandUpper(commandAddress);

  // Zero-out the portions of the register we're interested in
  registerContents &= mask;

  // Mask in new thing
  registerContents |= thing;

  // Change contents
  if (commandHeight == LOWER) writeCommandLower(commandAddress, registerContents);
  else writeCommandUpper(commandAddress, registerContents);
}
