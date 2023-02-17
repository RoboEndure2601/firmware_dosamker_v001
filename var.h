#ifndef var_h_
#define var_h_

#include "Lib.h"

#if LOOP_TIME
   /**
    * @brief 
    * loop time setup
    * wait until this value is start_point + FIX_LOOP_TIME equal to mills 
    */
    unsigned long start_point;
    //unsigned long _lastLoopTime;
#endif

/**
 * @brief 
 * EEPROM var
 */
//bool eeprom_update = false;

/*
   Use bool(true/false) for fast condition compare and low processing time
   varialbe define here are for system checking 
   this variable for gas control system
   Error - if any error occurs then enable error loop
   P
   Relay one
   Relay two
   Relay three
   Sensor (mlx90614)
   Fan Control
   FlameSensor 1
   FlameSensor 2
   ShutDown - this function use for stopping machine to damage it self
*/
#if ENABLE_ERROR
   bool Error = false;
   bool P = false;
   bool enable_relay = false;
   bool enable_sensor = false;
   bool Force_Shutdown = false;
#endif
/*
   this variable for motion and relay control for normal opretion
   Relay four
   Relay five
   Relay Six
   Relay seven
   Relay eight
   Stepper motor a
   Stepper motor b
   VFD X1
   VFD X2
   VFD 0-10V
   Forward Limit
   Backward Limit
   Up Limit
   Down Limit
*/

bool isStepperRunning = false;

bool displayState = false;
/*-----------------------------------------------------------------------------------------------------------------*/
/*

*/
int AmbTemp, ObjTemp, _ObjTemp;

uint8_t TryCount ;

#define gasStopDelay 1000
bool isWaiting = false;
bool state = false, gasStartIndication = false, gasStart = false;
bool stop_motor = false;
unsigned long nextTime, lastDelayTime, lastMillis, lastTryMillis, lastUpdateMillis;

//Effect : P variables
// user settings
int setPoint;
int Controled;// = setPoint + 20;
int InputRead;
bool overControl = false;

int8_t task = -1;// tasking loop
bool check = false;

//unsigned long stop_point;// control loop

int address;// EEPROM address for data saving 

//for paper remover
#define PAPER_REMOVER_FORWARD   paper_remover_motor_move(1)
#define PAPER_REMOVER_BACKWARD  paper_remover_motor_move(0)

#define RS485_delay delayMicroseconds(5)
uint8_t page_number;

// stepper motor control parameter
bool motor_move  = false;// motor ask for move
bool fwd         = false;// move to this direction
bool bwd         = false;// move to this direction

// auto mode parameter
bool paperProgre   = false;
uint8_t typePaper;
unsigned long currentTime;

int qty;
int doneQty;

// user data & default user data
typedef struct
{
  int Temp;//max limit ~32000
  int water;//max limit 0 to 65,535
  int oil;//max limit 0 to 65,535
  int spreder;//max limit 0 to 65,535
  int batterValve;//max limit 0 to 65,535
  int CookingTime;//max limit 0 to 4,294,967,295
} Paper;

Paper *paperPtr;

//default parameter//default
Paper d_soft    = {DSP_TEMP, DSP_WATER, DSP_OIL, DSP_SPREDER, DSP_BATTER, DSP_ROAST};
Paper d_medium  = {DMP_TEMP, DMP_WATER, DMP_OIL, DMP_SPREDER, DMP_BATTER, DMP_ROAST};
Paper d_hard    = {DHP_TEMP, DHP_WATER, DHP_OIL, DHP_SPREDER, DHP_BATTER, DHP_ROAST};
Paper d_custom  = {DCP_TEMP, DCP_WATER, DCP_OIL, DCP_SPREDER, DCP_BATTER, DCP_ROAST};

//user setup parameter
Paper soft, medium, hard, custom;


/*
   PCINT2 PORT-K gloable variable to state cheking
   False : LOW state
   True  : HIGH state
*/
#if ENABLE_PCINT2
   volatile bool IN1 = false;
   volatile bool IN2 = false;
   volatile bool IN3 = false;
   volatile bool IN4 = false;
   volatile bool IN5 = false;
   volatile bool IN6 = false;
   volatile bool IN7 = false;
   volatile bool IN8 = false;
#endif

#endif
