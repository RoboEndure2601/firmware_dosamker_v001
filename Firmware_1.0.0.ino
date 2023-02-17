#include "Lib.h"

void setup() {
  // put your setup code here, to run once:

  delay(100);

  // ISSUE : 001 : manual define for Gcode init
  #if OLD_SUPPORT 
    pinSet();// "beta_0.1"
  #else 
    board_init();//"PRODUCTION_1.0.0"
  #endif

  #if ADDITATIONAL_STOP// && OLD_SUPPORT
    stopSpindle();
    StopGas();
    // ISSUE : SOLVE : 001
    ready();
  #endif

  #if DEBUG 
    Serial.println(F("DOSAMAKER Online..")); // inSerial.print("millis : ");dication for ESP-link device
    Serial.println(F("DONE arduino setup is complate...")); // indication for User. machine is ready to work
  #endif

  Serial1.print(F("start.val=1")); sent_ok();// sending cmd for setup process is done

  Serial1.print(F("page 1")); sent_ok();// set page 1. mostly used when arduino restart

  delay(25);
  
  #if ENABLE_WATCHDOG
    wdt_enable(WDTO_4S);
  #endif
}

void loop() {

  #if LOOP_TIME
    // control delay for cycle and mcu life
    start_point = millis();
  #endif

  #if ENABLE_ERROR
    // pop up on display which error occur
    Serial1.print(" ");
    sent_ok();
  #endif

  GcodeParser();

  if(qty)paper_prog();

  //Effect : Gas Start process begin when gas start gasStartIndication become false
  if (gasStartIndication && !gasStart)StartGas();

  // run motor if cmd 
  if(motor_move)accelStepper_move();

  #if MOTOR_BLOCKED
    if (!motor_move && gasStart)gasPID();
  #else
    if (gasStart || overControl){
      gasPID();
    }
  #endif

  #if LOOP_TIME
    // WARNING : DO NOT REMOVE THIS CODE
  //      1025     > 1000          125       (1125) 
    Serial.println("Waiting for fix loop to complate");
    while(!(millis() > start_point + FIX_LOOP_TIME)){
      delayMicroseconds(1);
    }
    Serial.println("fix loop has been complate");
  #endif

  #if ENABLE_WATCHDOG
    wdt_reset();
  #endif
}

/*
   ISR define here
   INT 2,3 - TOP priority INPUT
   PCINT2 PORT-K - MEDIUM priority INPUT
*/
#if ENABLE_PCINT2
  ISR(PCINT2_vect) {
    
    //   check which pin state change and update to globle variable
    
    if (PINB & 0x01)IN1 = true;
    else if (PINB & 0x02)IN2 = true;
    else if (PINB & 0x03)IN3 = true;
    else if (PINB & 0x04)IN4 = true;
    else if (PINB & 0x05)IN5 = true;
    else if (PINB & 0x06)IN6 = true;
    else if (PINB & 0x07)IN7 = true;
    else if (PINB & 0x08)IN8 = true;
  }
#endif

#if ENABLE_GAS_TIMER
  ISR(TIMER1_COMPA_vect){          // timer compare interrupt service routine
    if(gasStart){
      if(setPoint > _ObjTemp){
        PORTA |= (1<<ControlRelay);
      }
      else{
        PORTA &= ~(1<<ControlRelay);
      }
    }
  }
#endif