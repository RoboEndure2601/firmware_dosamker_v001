#ifndef pinDefine_h_
#define pinDefine_h_

#include "config.h"

extern void sent_ok();

// function name declaretion
void preTransmission();
void postTransmission();


/*
 * VFD start, stop, hz. example
 * node.writeSingleRegister(0x2000, 5000); change Hz
 * node.writeSingleRegister(0x2001, 1); Start
 *  node.writeSingleRegister(0x2001, 6); Stop
 */
ModbusMaster VFD;

// instantiate Adafruit_MLX90614 object
Adafruit_MLX90614 TempI2C = Adafruit_MLX90614();

#if SMOOTH_MOTOR
  // Define a stepper and the pins it will use
  AccelStepper stepper_1(AccelStepper::DRIVER, STEPPER_PULSE_1, NULL); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
#endif

#if ENABLE_TEMP_SMOOTH
  Smoothed <int> TempSensor; 
#endif
/**
 * @brief 
 * 
 *  Serial port 0 to 2 enable
 *  Serial Port at 9600 baudrate - ESP8266(OTA port)
 *  Serial1 port at 115200 baudrate - DisplayPort
 *  Serial2 port at 9600 baudrate - VFDPort
 * 
 *  DDRn for direction
 *  PORTn for state change
 *  PINn for read state
 */
void board_init(void)
{
  // initialize timer1 
  #if ENABLE_GAS_TIMER
    Gas_timer();
  #endif

  #if STEPPER_SMOOTH
    stepper_timer();
  #endif
  
  #if DEBUG
    Serial.begin(115200);
    delay(100);
  #endif
  //Serial1.begin(115200);
  //delay(100);
  #if VFD_RS485
    Serial2.begin(57600);
    delay(100);
  #endif

  #if DEBUG
    /**
   * display helpful information
   */
    Serial.println(F("Company : Suzan Natural Tech Pvt. Ltd."));
    Serial.println(F("Website : www.suzantech.com"));
    Serial.println(F("Contact Number : +91-97149-69616"));
    Serial.print(F("DOSAMAKER : "));
    Serial.println(VERSION);
    Serial.println(F("MFD : 2022"));
    //Serial.print(F(""));
  #endif

  // 8-bit output port 24V-relay
  /*
     port A - arduino mega pin 22 - 29
     normal open
     Pin direction define here
  */
  DDRA = 0xFF;
  PORTA = 0x00;

  /**
   * @brief 
   * Stepper motor pinout defination
   */
  //pinMode(STEPPER_DIR_1,OUTPUT);
  //pinMode(STEPPER_EN_1,OUTPUT);
  DDRB |= (1 << STEPPER_PULSE_1);
  PORTC &= ~(1 << STEPPER_PULSE_1);
  DDRC |= (1 << STEPPER_DIR_1) | (1 << STEPPER_EN_1);
  PORTC &= ~(1 << STEPPER_DIR_1) | (1 << STEPPER_EN_1);
  
  //digitalWrite(STEPPER_EN_1,LOW);//set HIGH for disable motor
  PORTC &= ~(1 << STEPPER_DIR_1);
  PORTC &= ~(1 << STEPPER_EN_1);

  /*
   * rs485 pinout 
   */
  //pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode
  //digitalWrite(MAX485_DE, 0);
  DDRC |= (1 << MAX485_DE);
  PORTC &= ~(1 << MAX485_DE);

  // 8-bit input port 24V support
  /*
     port K - arduino mega A8 - A15
     normal Low
     Pin direction define here
  */
  DDRK = 0x00;
  PORTK = 0xFF;

  #if ENABLE_PCINT2
    /*
      Enable PCINT for INPUT if needed
      INT service define here
    */
    PCICR |= 0x02;
    PCMSK2 |= 0xFF;
  #endif

  /*
     How to read PIN state direct
    byte x = PINB & B000001;
    Serial.println(x,BIN);
  */


  //check connection for display
  nexInit();
  //Serial1.print("page 0");sent_ok();
  delay(10); //avoid collition

  #if DEBUG
    if (!nexInit()) {
      Serial.println(F("Display Communication Error, check wiring!!"));
    }
    else{
      Serial.println(F("Display Communication establish..."));
    }
  #else
    Serial1.print(" update to display ... ");
  #endif

  /*
    I2C for mlx90614 temprature sensor
  */
  TempI2C.begin();

  #if DEBUG
    if (!TempI2C.begin()) {
      Serial.println(F("Error connecting to Temprature sensor. Check wiring."));
      // sent error msg to display M101
    }
    else{
      Serial.println(F("Temprature sensor connection establish..."));
    }
  #else
    Serial1.print(" update display... ");
  #endif

  #if !STEPPER_SMOOTH
    // Change these to suit your stepper if you want
    stepper_1.setMaxSpeed(3000);
    stepper_1.setSpeed(2500);
  #endif
  
  #if ENABLE_TEMP_SMOOTH
    /*
      PID setup for gas contro system
    */
    TempSensor.begin(SMOOTHED_AVERAGE);
    TempSensor.clear();
  #endif
  
  //gcode_init()
  //
  #if VFD_RS485
    // Modbus slave ID 1
    VFD.begin(2, Serial2);
    // Callbacks allow us to configure the RS485 transceiver correctly
    VFD.preTransmission(preTransmission);
    VFD.postTransmission(postTransmission);
  #endif

  // load varialbles from eeprom
  #if ENABLE_DEFAULT
    if((soft.Temp == 0 || medium.Temp == 0) && (hard.Temp == 0 || custom.Temp == 0)){
      soft   = d_soft; 
      medium = d_medium;
      hard   = d_hard;
      custom = d_custom;
    }
    #if ENABLE_POPUP
     // sent error to display for setting up default parameter
    #endif
  #endif
}

#if VFD_RS485
  void preTransmission()
  {
    //digitalWrite(MAX485_DE, 1);
    PORTC |= (1 << MAX485_DE);
  }

  void postTransmission()
  {
    //digitalWrite(MAX485_DE, 0);
    PORTC &= ~(1 << MAX485_DE);
  }
#endif

#endif