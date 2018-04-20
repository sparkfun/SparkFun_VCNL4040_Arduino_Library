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

#ifndef SPARKFUN_VCNL4040_ARDUINO_LIBRARY_H
#define SPARKFUN_VCNL4040_ARDUINO_LIBRARY_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

static const uint8_t VCNL4040_ALS_IT_MASK = (uint8_t)~((1 << 7) | (1 << 6));
static const uint8_t VCNL4040_ALS_IT_80MS = 0;
static const uint8_t VCNL4040_ALS_IT_160MS = (1 << 7);
static const uint8_t VCNL4040_ALS_IT_320MS = (1 << 6);
static const uint8_t VCNL4040_ALS_IT_640MS = (1 << 7) | (1 << 6);

static const uint8_t VCNL4040_ALS_PERS_MASK = (uint8_t)~((1 << 3) | (1 << 2));
static const uint8_t VCNL4040_ALS_PERS_1 = 0;
static const uint8_t VCNL4040_ALS_PERS_2 = (1 << 2);
static const uint8_t VCNL4040_ALS_PERS_4 = (1 << 3);
static const uint8_t VCNL4040_ALS_PERS_8 = (1 << 3) | (1 << 2);

static const uint8_t VCNL4040_ALS_INT_EN_MASK = (uint8_t)~((1 << 1));
static const uint8_t VCNL4040_ALS_INT_DISABLE = 0;
static const uint8_t VCNL4040_ALS_INT_ENABLE = (1 << 1);

static const uint8_t VCNL4040_ALS_SD_MASK = (uint8_t)~((1 << 0));
static const uint8_t VCNL4040_ALS_SD_POWER_ON = 0;
static const uint8_t VCNL4040_ALS_SD_POWER_OFF = (1 << 0);

static const uint8_t VCNL4040_PS_DUTY_MASK = (uint8_t)~((1 << 7) | (1 << 6));
static const uint8_t VCNL4040_PS_DUTY_40 = 0;
static const uint8_t VCNL4040_PS_DUTY_80 = (1 << 6);
static const uint8_t VCNL4040_PS_DUTY_160 = (1 << 7);
static const uint8_t VCNL4040_PS_DUTY_320 = (1 << 7) | (1 << 6);

static const uint8_t VCNL4040_PS_PERS_MASK = (uint8_t)~((1 << 5) | (1 << 4));
static const uint8_t VCNL4040_PS_PERS_1 = 0;
static const uint8_t VCNL4040_PS_PERS_2 = (1 << 4);
static const uint8_t VCNL4040_PS_PERS_3 = (1 << 5);
static const uint8_t VCNL4040_PS_PERS_4 = (1 << 5) | (1 << 4);

static const uint8_t VCNL4040_PS_IT_MASK = (uint8_t)~((1 << 3) | (1 << 2) | (1 << 1));
static const uint8_t VCNL4040_PS_IT_1T = 0;
static const uint8_t VCNL4040_PS_IT_15T = (1 << 1);
static const uint8_t VCNL4040_PS_IT_2T = (1 << 2);
static const uint8_t VCNL4040_PS_IT_25T = (1 << 2) | (1 << 1);
static const uint8_t VCNL4040_PS_IT_3T = (1 << 3);
static const uint8_t VCNL4040_PS_IT_35T = (1 << 3) | (1 << 1);
static const uint8_t VCNL4040_PS_IT_4T = (1 << 3) | (1 << 2);
static const uint8_t VCNL4040_PS_IT_8T = (1 << 3) | (1 << 2) | (1 << 1);

static const uint8_t VCNL4040_PS_SD_MASK = (uint8_t)~((1 << 0));
static const uint8_t VCNL4040_PS_SD_POWER_ON = 0;
static const uint8_t VCNL4040_PS_SD_POWER_OFF = (1 << 0);

static const uint8_t VCNL4040_PS_HD_MASK = (uint8_t)~((1 << 3));
static const uint8_t VCNL4040_PS_HD_12_BIT = 0;
static const uint8_t VCNL4040_PS_HD_16_BIT = (1 << 3);

static const uint8_t VCNL4040_PS_INT_MASK = (uint8_t)~((1 << 1) | (1 << 0));
static const uint8_t VCNL4040_PS_INT_DISABLE = 0;
static const uint8_t VCNL4040_PS_INT_CLOSE = (1 << 0);
static const uint8_t VCNL4040_PS_INT_AWAY = (1 << 1);
static const uint8_t VCNL4040_PS_INT_BOTH = (1 << 1) | (1 << 0);

static const uint8_t VCNL4040_PS_SMART_PERS_MASK = (uint8_t)~((1 << 4));
static const uint8_t VCNL4040_PS_SMART_PERS_DISABLE = 0;
static const uint8_t VCNL4040_PS_SMART_PERS_ENABLE = (1 << 1);

static const uint8_t VCNL4040_PS_AF_MASK = (uint8_t)~((1 << 3));
static const uint8_t VCNL4040_PS_AF_DISABLE = 0;
static const uint8_t VCNL4040_PS_AF_ENABLE = (1 << 3);

static const uint8_t VCNL4040_PS_TRIG_MASK = (uint8_t)~((1 << 3));
static const uint8_t VCNL4040_PS_TRIG_TRIGGER = (1 << 2);

static const uint8_t VCNL4040_WHITE_EN_MASK = (uint8_t)~((1 << 7));
static const uint8_t VCNL4040_WHITE_ENABLE = 0;
static const uint8_t VCNL4040_WHITE_DISABLE = (1 << 7);

static const uint8_t VCNL4040_PS_MS_MASK = (uint8_t)~((1 << 6));
static const uint8_t VCNL4040_PS_MS_DISABLE = 0;
static const uint8_t VCNL4040_PS_MS_ENABLE = (1 << 6);

static const uint8_t VCNL4040_LED_I_MASK = (uint8_t)~((1 << 2) | (1 << 1) | (1 << 0));
static const uint8_t VCNL4040_LED_50MA = 0;
static const uint8_t VCNL4040_LED_75MA = (1 << 0);
static const uint8_t VCNL4040_LED_100MA = (1 << 1);
static const uint8_t VCNL4040_LED_120MA = (1 << 1) | (1 << 0);
static const uint8_t VCNL4040_LED_140MA = (1 << 2);
static const uint8_t VCNL4040_LED_160MA = (1 << 2) | (1 << 0);
static const uint8_t VCNL4040_LED_180MA = (1 << 2) | (1 << 1);
static const uint8_t VCNL4040_LED_200MA = (1 << 2) | (1 << 1) | (1 << 0);

static const uint8_t VCNL4040_INT_FLAG_ALS_LOW = (1 << 5);
static const uint8_t VCNL4040_INT_FLAG_ALS_HIGH = (1 << 4);
static const uint8_t VCNL4040_INT_FLAG_CLOSE = (1 << 1);
static const uint8_t VCNL4040_INT_FLAG_AWAY = (1 << 0);

class VCNL4040 {

  public:
    VCNL4040();

    boolean begin(TwoWire &wirePort = Wire);
    boolean isConnected(void); //True if sensor responded to I2C query

    void setIRDutyCycle(uint16_t dutyValue);

    void setProxInterruptPersistance(uint8_t persValue);
    void setAmbientInterruptPersistance(uint8_t persValue);

    void setProxIntegrationTime(uint8_t timeValue); //Sets the integration time for the proximity sensor
    void setAmbientIntegrationTime(uint16_t timeValue);

    void powerOnProximity(void); //Power on the prox sensing portion of the device
    void powerOffProximity(void); //Power off the prox sensing portion of the device

    void powerOnAmbient(void); //Power on the ALS sensing portion of the device
    void powerOffAmbient(void); //Power off the ALS sensing portion of the device

    void setProxResolution(uint8_t resolutionValue);//Sets the proximity resolution to 12 or 16 bit

    void enableAmbientInterrupts(void);
    void disableAmbientInterrupts(void);

    void enableSmartPersistance(void);
    void disableSmartPersistance(void);

    void enableActiveForceMode(void);
    void disableActiveForceMode(void);
    void takeSingleProxMeasurement(void);

    void enableWhiteChannel(void);
    void disableWhiteChannel(void);

    void enableProxLogicMode(void);
    void disableProxLogicMode(void);

    void setLEDCurrent(uint8_t currentValue);

    void setProxCancellation(uint16_t cancelValue);
    void setALSHighThreshold(uint16_t threshold);
    void setALSLowThreshold(uint16_t threshold);
    void setProxHighThreshold(uint16_t threshold);
    void setProxLowThreshold(uint16_t threshold);

    uint16_t getProximity();
    uint16_t getAmbient();
    uint16_t getWhite();
    uint16_t getID();

    void setProxInterruptType(uint8_t interruptValue); //Enable four prox interrupt types
    boolean isClose(); //Interrupt flag: True if prox value greater than high threshold
    boolean isAway(); //Interrupt flag: True if prox value lower than low threshold
    boolean isLight(); //Interrupt flag: True if ALS value higher than high threshold
    boolean isDark(); //Interrupt flag: True if ALS value lower than low threshold

    uint16_t readCommand(uint8_t commandCode);
    uint8_t readCommandLower(uint8_t commandCode);
    uint8_t readCommandUpper(uint8_t commandCode);

    boolean writeCommand(uint8_t commandCode, uint16_t value);
    boolean writeCommandLower(uint8_t commandCode, uint8_t newValue);
    boolean writeCommandUpper(uint8_t commandCode, uint8_t newValue);

    void bitMask(uint8_t commandAddress, boolean commandHeight, uint8_t mask, uint8_t thing);

  private:
    TwoWire *_i2cPort; //The generic connection to user's chosen I2C hardware

};
#endif
