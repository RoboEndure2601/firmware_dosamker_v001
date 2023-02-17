#ifndef config_h_
#define config_h_

#define BOARD "MEGA_ESP"
#define VERSION "PRODUCTION_1.0.0"

/**
 * @brief 
 * DEBUG mode 
 * serial print all process after executation
 * serial 0 is debug serial do not use for other purpose 
 * 
 * INVALID CHAR error
 * 
 * un-used char print in serial monitor 
 */
#define DEBUG true
// #define INVALID_CHAR true

/**
 * @brief 
 * DEFAULT parameter
 * if Arduino not able to load parameter then use default parameter
 * 
 * OLD_SUPPORT function
 * this is used for pinMode and start up check
 */
#define ENABLE_DEFAULT true
//#define OLD_SUPPORT true
//#define ENABLE_DELAY true

/**
 * @brief 
 * DEMO mode for display model 
 *
 * TODO : Enable using display setting
 */
//#define ENABLE_DEMO true

/**
 * @brief 
 * STEPPER_TIMER mode
 * 
 * TODO : Enable timer for stepper motor pulse
 * 
 * 
 * SMOOTH STEPPER
 * using timer1 as pulse generator disable AccelStepper library
 * TODO
 * 
 * SECOND_STEPPER 
 * if there is need for another stepper motor just uncommect it
 * TODO
 */
//#define MOTOR_BLOCKED true
#define STEPPER_SMOOTH true
//#define SECOND_STEPPER true

/**
 * @brief 
 * WATCHDOG mode 
 *
 *  Available Enable with 8second loop
 */
#define ENABLE_WATCHDOG true

/**
 * @brief 
 * START_CHECK
 * 
 * stop all device from running before machine start of stop running in case of auto restart
 */
#define ADDITATIONAL_STOP true

/**
 * @brief 
 * PCINT for input 
 * 
 * ESTOP connection
 */
#define ENABLE_PCINT2 true

/**
 * @brief 
 * FIX LOOP TIME mode
 * 
 * fix time for each loop cycle 
 * ISSUE : unstable
 * 
 * TODO : repair stablity
 */
#define LOOP_TIME true

/**
 * @brief 
 * VFD RS485 mode
 * 
 * VFD drive connet using UART2 with RS485 module 
 * 
 * TODO : cmd take too much time
 */
#define VFD_RS485 true

/**
 * @brief 
 * POP UP create
 * 
 * TODO : undefine code
 *          pop-up all error and msg on display
 * 
 * PAPER STATE update
 * TODO : update all things happen in arduino when making paper
 * 
 * TEMP SMOOTHER value
 * change value smoothly
 */
//#define ENABLE_POPUP true
//#define ENABLE_STATE true
#define ENABLE_TEMP_SMOOTH true

/**
 * @brief 
 * ERROR generator
 * TODO : generat error
 * 
 */
#define ENABLE_ERROR true

/**
 * @brief 
 * GAS CONTROL timer
 * TODO : full scale
 * 
 */
//#define ENABLE_GAS_TIMER true

#endif