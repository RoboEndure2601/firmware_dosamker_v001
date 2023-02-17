#ifndef func_h_
#define func_h_

#include "Lib.h"

void butter_spreder_relay(int position)
{
  if (position == 0) //pump at highest position and going to lowest position
  {
    //Serial.println("inside the function");
    if ((BUTTER_SPREDER_LIMIT_MIN_STATE == DEFAULT_LIMIT_SWITCH_STATE) && (BUTTER_SPREDER_LIMIT_MAX_STATE == !DEFAULT_LIMIT_SWITCH_STATE))
    {
      BUTTER_SPREDER_ON;
    }
  }
  else if (position == 1) //pump at lowest position and going to hightest position
  {
    if ((BUTTER_SPREDER_LIMIT_MIN_STATE == !DEFAULT_LIMIT_SWITCH_STATE) && (BUTTER_SPREDER_LIMIT_MAX_STATE == DEFAULT_LIMIT_SWITCH_STATE))
    {
      BUTTER_SPREDER_OFF;
    }
  }
  //else //pump stuct in between some position is not proper
}

void accelStepper_move(){
    if (fwd){ 
      //isStepperRunning = true;
      PORTC |= (1 << STEPPER_EN_1);
      PORTC &= ~(1 << STEPPER_DIR_1);
      delayMicroseconds(5);
      fwd = false;
    }
    if (PAPER_REMOVER_LIMIT_MAX_STATE == HIGH){
      bwd = true;
    }
    //moving backward
    if (bwd){
      PORTC |= (1 << STEPPER_DIR_1);
      delayMicroseconds(5);
      bwd = false;
      stop_motor = true;
    }
    if (PAPER_REMOVER_LIMIT_MIN_STATE == HIGH && stop_motor){
      //task = 10;
      PORTC &= ~(1 << STEPPER_EN_1);
      check = true;
      stop_motor = false;
      motor_move = false;
      isStepperRunning = false;
    }
    #if DEBUG 
      Serial.println(PAPER_REMOVER_LIMIT_MAX_STATE);
      Serial.println(PAPER_REMOVER_LIMIT_MIN_STATE);
    #endif
        
    #if !STEPPER_SMOOTH
      stepper_1.runSpeed();
    #endif
}

void sent_ok()
{
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
}

void reset_parameter(){
  Serial.println(/* clear all things and reset default value */ );
    soft = d_soft;
    medium = d_medium;
    hard = d_hard;
    custom = d_custom;

    EEPROM_write_data();
    EEPROM_read_data();

    Serial1.print(F("page 13"));sent_ok();

    delay(200);
}

void auto_quit()
{
  qty = 0;
  BUTTER_SPREDER_OFF;
  BUTTER_VALVE_OFF;
  OIL_SPRAY_OFF;
  WATER_SPRAY_OFF;
  StopGas();
  stopSpindle();
  paperProgre = 1;
  task = -1;
  delayMicroseconds(25);
}

//define variable for communication
void sent_para(uint8_t type)
{
  // TODO : recreate better version with return "somthing;"
  /*
     type 1 = soft paper
     type 2 = medium paper
     type 3 = hard paper
     type 4 = custom paper
  */
  switch (type)
  {
    case 1: paperPtr = &soft;
      // adress 0, 5, 10, 15, 20, 2
      break;
    case 2: paperPtr = &medium;
      // adress 30, 35, 40, 45, 50, 55
      break;
    case 3: paperPtr = &hard;
      // adress 60, 65, 70, 75, 80, 85
      break;
    case 4: paperPtr = &custom;
      // adress 90, 95, 100, 105, 110, 115
      break;
    default:
      break;
  }
  delay(250);
  Serial1.print(F("n0.val=0"));
  sent_ok();
  Serial1.print("n0.val=" + String(paperPtr->Temp));
  sent_ok();
  Serial1.print("x0.val=" + String(paperPtr->water / 100));
  sent_ok();
  Serial1.print("x1.val=" + String(paperPtr->oil / 100));
  sent_ok();
  Serial1.print("x2.val=" + String(paperPtr->spreder / 100));
  sent_ok();
  Serial1.print("x3.val=" + String(paperPtr->batterValve / 100));
  sent_ok();
  Serial1.print("x4.val=" + String(paperPtr->CookingTime / 100));
  sent_ok();
}

// TODO : update with "static inline"
void  gasPID(){
   
  unsigned long msNow = millis();	

  if(msNow > nextTime){
    I2CTempRead();

    #if ENABLE_TEMP_SMOOTH
      TempSensor.add(ObjTemp);
      _ObjTemp = TempSensor.get();

      // this value show real temprature readed by sensor 
      //int lastValueStoredExp = TempSensor.getLast();
    #else
      _ObjTemp = ObjTemp;
    #endif

    #if !ENABLE_GAS_TIMER
      if(Controled > ObjTemp){
        if(setPoint > ObjTemp){
          if(overControl){
            //restarting gas after overControl state happen
            StartGas();
            overControl = false;
          }
          else{
            PORTA |= (1<<ControlRelay);
            delayMicroseconds(5);
          }
        }
        else{
          PORTA &= ~(1<<ControlRelay);
          delayMicroseconds(5);
        }
      }
      else{
        //TODO : disable gas kit, beacues it's out of control
        //gas controller is out of control
        //gas off    
        StopGas();
        overControl = true;
      }
    #endif

    // Serial1.write(0xff);
    // Serial1.write(0xff);
    // Serial1.write(0xff);
    Serial1.print(F("temp.val="));
    Serial1.print(ObjTemp);
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);

    #if DEBUG
      Serial.print(F("Temprature : "));
      Serial.println(ObjTemp);
    #endif    
    nextTime = msNow + windowTime;
  }
}

void Default_pararmeter()
{
  soft   = d_soft;
  medium = d_medium;
  hard   = d_hard;
  custom = d_custom;
}

#endif
