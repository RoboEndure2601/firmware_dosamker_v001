#ifndef defaults_h
#define defaults_h

//// DosaMaker versioning system
#define DOSAMAKER_VERSION "V1.02"
#define DOSAMAKER_VERSION_BUILD "02112021"

// loop control
#define FIX_LOOP_TIME 125

// gas control setting
#define SensorTempLimit 35
#define MAX_TEMPRATURE 150
#define MIN_TEMPRATURE 100
#define IDEAL_TEMPRATURE 25

#define GasSpartTry   3
#define GasSparkTime  11000

#define RelayDefaultState HIGH

// spidle parameter
#define FULL_SPEED       1200
#define LOW_SPEED        500
#define STOP_SPEED       0

#define UpdateLoop       5000

#define GasDelayTime     1000

#define Default_Setpoint 100
#define windowTime       2000

/*
   INPUT pin default state
*/
#define DEFAULT_LIMIT_SWITCH_STATE LOW

// paper setting
#define DSP_TEMP 100
#define DMP_TEMP 110
#define DHP_TEMP 115
#define DCP_TEMP 120

#define DSP_WATER 2000
#define DMP_WATER 2000
#define DHP_WATER 2000
#define DCP_WATER 2000

#define DSP_OIL 2000
#define DMP_OIL 2000
#define DHP_OIL 2000
#define DCP_OIL 2000

#define DSP_SPREDER 5000
#define DMP_SPREDER 5000
#define DHP_SPREDER 5000
#define DCP_SPREDER 5000

#define DSP_BATTER 3500
#define DMP_BATTER 3500
#define DHP_BATTER 3500
#define DCP_BATTER 3500

#define DSP_ROAST 8000
#define DMP_ROAST 8500
#define DHP_ROAST 9000
#define DCP_ROAST 9500

/**
 * @brief 
 * PIN-configuration
 * 
 */

/*!
  We're using a MAX485-compatible RS485 Transceiver.
  Rx/Tx is hooked up to the hardware serial port at 'Serial'.
  The Data Enable and Receiver Enable pins are hooked up as follows:
*/
#define MAX485_DE 5 // PORTC 5 // D32

// relay name declare here (0-7)
#define SparkRelay    7
#define WATER_RELAY   2 //ok
#define OIL_RELAY     4 //ok
#define UPDOWN_RELAY  5
#define PUMP_RELAY    3
#define ConstantRelay 1
#define ControlRelay  6

//define all relay pre-define states
#define BUTTER_VALVE_ON     PORTA |=  (1 << PUMP_RELAY);
#define BUTTER_VALVE_OFF    PORTA &= ~(1 << PUMP_RELAY);
#define WATER_SPRAY_ON      PORTA |=  (1 << WATER_RELAY);
#define WATER_SPRAY_OFF     PORTA &= ~(1 << WATER_RELAY);
#define OIL_SPRAY_ON        PORTA |=  (1 << OIL_RELAY);
#define OIL_SPRAY_OFF       PORTA &= ~(1 << OIL_RELAY);
#define BUTTER_SPREDER_ON   PORTA |=  (1 << UPDOWN_RELAY);
#define BUTTER_SPREDER_OFF  PORTA &= ~(1 << UPDOWN_RELAY);

#define SPARK_ON            PORTA |=  (1 << SparkRelay);
#define SPARK_OFF           PORTA &= ~(1 << SparkRelay);
#define CONSTANT_RELAY_ON   PORTA |=  (1 << ControlRelay);
#define CONSTANT_RELAY_OFF  PORTA &= ~(1 << ControlRelay);
#define CONTROL_RELAY_ON    PORTA |=  (1 << ConstantRelay);
#define CONTROL_RELAY_OFF   PORTA &= ~(1 << ConstantRelay);

//input name declare here (0-7)
#define VERTICAL_MIN    4
#define VERTICAL_MAX    3
#define HORIZONTAL_MIN  A13
#define HORIZONTAL_MAX  A15
#define GasStartInput   A8
#define IN5_set         2
#define IN6_set         5
#define IN7_set         1

//states
#define BUTTER_SPREDER_LIMIT_MIN_STATE  digitalRead(VERTICAL_MIN)
#define BUTTER_SPREDER_LIMIT_MAX_STATE  digitalRead(VERTICAL_MAX)
#define PAPER_REMOVER_LIMIT_MIN_STATE   digitalRead(HORIZONTAL_MIN)
#define PAPER_REMOVER_LIMIT_MAX_STATE   digitalRead(HORIZONTAL_MAX)

/*
   stepper motor pin out
*/

#define STEPPER_PULSE_1 5 // PORTB 5 // D11
#define STEPPER_DIR_1   6  // PORTC 6 // D31
#define STEPPER_EN_1    7  // PORTC 7 // D30

#if SECOND_STEPPER
  #define STEPPER_PULSE_2 -1
  #define STEPPER_DIR_2   -1
  #define STEPPER_EN_2    -1
#endif

#endif
