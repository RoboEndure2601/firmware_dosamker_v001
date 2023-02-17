#ifndef paper_h_
#define paper_h_

#include "Lib.h"


void paper_prog(){

  if (paperProgre){
    String type;// = "";
    paperProgre = false;

    //paper type selection
    switch (typePaper){
      case 0: type = "Soft";   paperPtr = &soft; break;
      case 1: type = "Medium"; paperPtr = &medium; break;
      case 2: type = "Hard";   paperPtr = &hard; break;
      case 3: type = "Custom"; paperPtr = &custom; break;
      default: break;
    }

    #if DEBUG
        Serial.print(F("Paper Type selected : "));
        Serial.println(type);
        Serial.print(F("Input paper Qty : "));
        Serial.println(qty);
        Serial.print(F("Temprature : "));
        Serial.println(paperPtr->Temp);
        Serial.print(F("water spray : "));
        Serial.println(paperPtr->water);
        Serial.print(F("oil : "));
        Serial.println(paperPtr->oil);
        Serial.print(F("spreder : "));
        Serial.println(paperPtr->spreder);
        Serial.print(F("bettervalve : "));
        Serial.println(paperPtr->batterValve);
        Serial.print(F("CookingTime : "));
        Serial.println(paperPtr->CookingTime);
    #endif

    // Effect : update display value
    sent_ok();
    Serial1.print(F("paperType.txt="));
    Serial1.print("\"" + type + "\"");
    sent_ok();
    Serial1.print(F("setPoint.val="));
    Serial1.print(paperPtr->Temp);
    Controled = paperPtr->Temp + 15;
    sent_ok();
    #if ENABLE_STATE
        Serial1.print(F("t1.txt="));
        Serial1.print("\"<write state here>\"");
        sent_ok();
    #endif

    #if !ENABLE_DEMO    
        // wait here for staring gas and spindle rotation
        gasStartIndication = true;
        // StartGas(); // take upto 35 sec
        
        /*
        sent VFD node single for start
        */

        VFD.writeSingleRegister(0x2000,LOW_SPEED);RS485_delay;
        VFD.writeSingleRegister(0x2001,1);RS485_delay;
        //if(gasStart){
            task = 0;
        //}
    #else
        VFD.writeSingleRegister(0x2000,LOW_SPEED);RS485_delay;
        VFD.writeSingleRegister(0x2001,1);RS485_delay;
        task = 1;
    #endif

    delayMicroseconds(25);
  }
    
  switch (task)
  {
    case 0:
      //currentTemp = ObjTemp;
      //delay(200);
      if (_ObjTemp > paperPtr->Temp)
      {
        // change spindle speed to high
        VFD.writeSingleRegister(0x2000,FULL_SPEED);RS485_delay;
        // assign next task
        task = 1;
        //set time
        currentTime = millis();
      }
      break;
    case 1:
    
      #if ENABLE_DEMO
        VFD.writeSingleRegister(0x2000,FULL_SPEED);RS485_delay;
      #else
        //water spray
        WATER_SPRAY_ON;
      #endif
      
      // assign next task
      task = 2;

      //set time
      currentTime = millis();
      break;
    case 2:
      if (millis() > currentTime + paperPtr->water)
      {
        WATER_SPRAY_OFF;
        BUTTER_SPREDER_ON;

        // assign next task
        task = 3;

        //set time
        currentTime = millis();
      }
      break;
    case 3:
      //water spray
      BUTTER_VALVE_ON;

      // assign next task
      task = 4;

      //set time
      currentTime = millis();
      break;
    case 4:
      if (millis() > currentTime + paperPtr->batterValve)
      {
        BUTTER_VALVE_OFF;
        // assign next task
        task = 6;

        //set time
        currentTime = millis();
      }
      break;
    /*    
    case 5:
        // assign next task
        task = 6;

        //set time
        currentTime = millis();
      break;
      */
    case 6:
      if (millis() > currentTime + paperPtr->spreder)
      {
        BUTTER_SPREDER_OFF;

        // assign next task
        task = 7;

        //set time
        currentTime = millis();
      }
      break;
    case 7:
      //water spray
      #if !ENABLE_DEMO 
        OIL_SPRAY_ON;
      #endif
      // assign next task
      task = 8;

      //set time
      currentTime = millis();
      break;
    case 8:
      if (millis() > currentTime + paperPtr->oil)
      {
        OIL_SPRAY_OFF;

        // change spindle speed to normal
        VFD.writeSingleRegister(0x2000, LOW_SPEED);
        RS485_delay;
        //ISSUE : serial cmd not working

        // assign next task
        task = 9;
        //set time

        currentTime = millis();
      }
      break;
    case 9:
    #if ENABLE_DEMO
      if (millis() > currentTime + 2000){
    #else
      if (millis() > currentTime + long(paperPtr->CookingTime) * 10){
    #endif
        check = false;
        motor_move = true;
        fwd = true;
        task = 10;
        //currentTime = millis();
      }
      break;
    case 10:
      if (check){
        doneQty += 1;
        if (qty != doneQty){
          Serial1.print("qtyMade.val=" + String(doneQty)); sent_ok(); // enter for nextion display
          delayMicroseconds(25);
          task = 0;
          currentTime = millis();
        }
        else{
          qty = 0;
          doneQty = 0;
          paperProgre = true;
          Serial1.print(F("page 4")); sent_ok(); // enter for nextion display

          // turn off gas and stop spindle
          VFD.writeSingleRegister(0x2001,6);RS485_delay;
          StopGas();
        }
      }
      break;
    default:
      break;
  }
}


#endif 