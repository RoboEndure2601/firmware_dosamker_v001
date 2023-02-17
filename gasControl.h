#ifndef gasControl_h_
#define gasControl_h_

/*
   Gas Control - Prapotaitonal
   Dual Relay
   MLX90614 temprature sensor
   24 V fan - no control
   Update Time of Temprature - 0.5s to 5s
   Communication protocoal - I2C - data-time 170ms
*/

#include "Lib.h"

//temprature based functions and PID loop
//read temprature from  I2C device MLX90614
void I2CTempRead(){
  AmbTemp = TempI2C.readAmbientTempC();
  ObjTemp = TempI2C.readObjectTempC();
}

void StartGas(){
  /*
    if GasStartInput is HIGH that means Fire not started
    if GasStartInput is LOW that means Fire started
    3 try for Fire Start Each try take 5 sec time
    Total 15 sec for Fire Start
  */
  if (TryCount < 3){
    // Effect : wait for gas stop delay and start gas relay after that  
    if (isWaiting){
      if(millis() > gasStopDelay + lastDelayTime){
        isWaiting = false;
      }
    }
    else{
      //Effect : Just start relay once
      if (!state){ // if state is true then it stop working 
        CONTROL_RELAY_ON;
        CONSTANT_RELAY_ON;
        SPARK_ON;

        //Effect : disable this fuction for running again
        state = true;

        //Effect : update time for new millis funtion
        lastTryMillis = millis();//for 11 sec delay
        //lastDelayTime = millis();
      }
    }
    //Gas Start Tring
    if (millis() > GasSparkTime + lastTryMillis && !isWaiting){
      TryCount++;
      
      #if DEBUG
        Serial.print(F(" tryCount : "));
        Serial.println(TryCount);
      #endif

      #if ENABLE_DELAY
        delay(11000);
      #endif

      if (digitalRead(GasStartInput) == LOW){
        CONTROL_RELAY_OFF;
        CONSTANT_RELAY_OFF;
        SPARK_OFF;
        state = false;

        //DELAY for relay to discharge
        #if ENABLE_DELAY
          delay(1000);
        #else 
          //UNDER-DEVOLPMENT : wait for 1 sec without delay function
          isWaiting = true;
          lastDelayTime = millis();
        #endif

        #if DEBUG
          Serial.println(F(" turn off gas relays "));
        #endif
      }
      else{
        TryCount = 0;
        gasStart = true;
        gasStartIndication = false;
        state = false;
        #if DEBUG
          Serial.println(F(" gas started... "));
        #endif
      }
      lastTryMillis = millis();
    }
  }
  else{
    // print three try failed check gas input
    TryCount = 0;
    gasStartIndication = false;
  }
}

//Effect : Disable gas relay
void StopGas(){
  gasStart = false;
  gasStartIndication = false;
  CONTROL_RELAY_OFF;
  CONSTANT_RELAY_OFF;
  SPARK_OFF;
}

//Effect : Stop Spindle
void stopSpindle(){
  /*
     Sent RS485(serial1) for stop vfd drive
  */
  #if VFD_RS485
    VFD.writeSingleRegister(0x2001,6);RS485_delay;
    VFD.writeSingleRegister(0x2000,LOW_SPEED);RS485_delay;
  #else
    // write code relay based
    PORTx &= ~(1<<VFD_RELAY);
  #endif
}

#endif
