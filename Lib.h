#ifndef Lib_h_
#define Lib_h_


#include "config.h"
//#define __AVR_ATmega2560__

/*
   Build library call
*/
#include <Arduino.h>
#include <avr/io.h>
#include <avr/iom2560.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/wdt.h>
#include <Wire.h>//wire.h library give access to use I2C channel

HardwareSerial *MySerial = &Serial2;

/*
   third party library call
*/
#include <Nextion.h>
#if ENABLE_TEMP_SMOOTH
   #include <Smoothed.h>
#endif
#if SMOOTH_STEPPER
   #include <AccelStepper.h>//mlx90614 tempreture sensor
#endif
#include <ModbusMaster.h>
#include <Adafruit_MLX90614.h>
#include <Smoothed.h>
#include <EEPROM.h>
/*
   user define library call
*/
#include "defaults.h"
#include "timer.h"
#include "var.h"
#include "eeprom.h"
#include "pinDefine.h"
#include "gasControl.h"
#include "func.h"
#include "gcode.h"
#include "paper.h"

#endif
