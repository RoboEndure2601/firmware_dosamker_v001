#ifndef eeprom_h_
#define eeprom_h_

#include "Lib.h"
//define write and read function for parameter

bool  EEPROM_write_data(uint8_t _paperType= -1){
  bool update = false;
  switch (_paperType) 
  {
    case 0: paperPtr = &soft; address = 0;
      // adress 0, 5, 10, 15, 20, 25
      break;
    case 1: paperPtr = &medium; address = 30;
      // adress 30, 35, 40, 45, 50, 55
      break;
    case 2: paperPtr = &hard; address = 60;
      // adress 60, 65, 70, 75, 80, 85
      break;
    case 3: paperPtr = &custom; address = 90;
      // adress 90, 95, 100, 105, 110, 115
      break;
    default:
      if(_paperType > 3){
        return update=false;
      }
      else{
        /**
         * @todo 
         * write all data to eeprom
         * 
         */
        #if DEBUG
          Serial.println(F("You not able to write data on EEPROM!!"));
          Serial.println(F("This function not available this time"));
        #endif

        return update=false;
      }
  }
  //total address 4*6 = 24
  //address = address + 5

  /**
   * @todo paragraph describing what is to be done 
   * Add waiting POP_UP on display for prevent user for 
   * giving another input at data saving time
   */

  EEPROM.put(address, paperPtr->Temp);
  EEPROM.put(address + 5, paperPtr->water);
  EEPROM.put(address + 10, paperPtr->oil);
  EEPROM.put(address + 15, paperPtr->spreder);
  EEPROM.put(address + 20, paperPtr->batterValve);
  EEPROM.put(address + 23, paperPtr->CookingTime);

  return update=true;
}


void EEPROM_read_data()
{
  for (int i = 0; i < 4; i++)
  {
    switch (i)
    {
      case 0: paperPtr = &soft; address = 0;
        // adress 0, 5, 10, 15, 20, 25
        break;
      case 1: paperPtr = &medium; address = 30;
        // adress 30, 35, 40, 45, 50, 55
        break;
      case 2: paperPtr = &hard; address = 60;
        // adress 60, 65, 70, 75, 80, 85
        break;
      case 3: paperPtr = &custom; address = 90;
        // adress 90, 95, 100, 105, 110, 115
        break;
    }
    //total address 4*6 = 24
    //address = address + 5
    EEPROM.get(address, paperPtr->Temp);
    EEPROM.get(address + 5, paperPtr->water);
    EEPROM.get(address + 10, paperPtr->oil);
    EEPROM.get(address + 15, paperPtr->spreder);
    EEPROM.get(address + 20, paperPtr->batterValve);
    EEPROM.get(address + 23, paperPtr->CookingTime);
  }
}

#endif