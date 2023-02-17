#ifndef gcode_h_
#define gcode_h_

#include "Lib.h"

//------------------------------------------------------------------------------
// GLOBALS
//------------------------------------------------------------------------------

#define MAX_BUF 64

char  buffer[MAX_BUF];  // where we store the message until we get a newline
int   idx;            // how much is in the buffer
char  code_1;
int * trans;
//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------
/**
   prepares the input buffer to receive a new message and tells the serial connected device it is ready for more.
*/
void ready()
{
  idx = 0; // clear input buffer
}



float parseNumber(char code, float val)
{
  code_1 = buffer[0];
  char *ptr = buffer; // start at the beginning of buffer
  while ((long)ptr > 1 && (*ptr) && (long)ptr < (long)buffer + idx)
  { // walk to the end
    if (*ptr == code)
    {
      // if you find code on your walk,
      return atof(ptr + 1); // convert the digits that follow into a float and return it
    }
    ptr = strchr(ptr, ' ') + 1; // take a step from here to the letter after the next space
  }
  return val;  // end reached, nothing found, return default val.
}

/**
   Read the input buffer and find any recognized commands.  One G or M command per line.
*/
void processCommand()
{  
  int temp_1 = parseNumber('T', -1);
  // cmd value between temprature support range
  if ((70 < temp_1) && (temp_1 < 320)) {
    setPoint = temp_1;
    Controled = setPoint + 15;
    //Effect : put GasStart function to working condition
    gasStartIndication = true;
  }
  else if (temp_1 == 0) {
    StopGas();
    temp_1 = -1;
  }
  //cmd = parseNumber('A', -1);

// Effect : on/off better pump relay
  int pump = parseNumber('B',-1);
  switch (pump)
  {
      case 0:PORTA &= ~(1 << PUMP_RELAY);break;
      case 1:PORTA |= (1 << PUMP_RELAY);break;
      default:break;
  }
  
  //cmd = parseNumber('C', -1);

  // Effect : paper type and set qty
 if(code_1 == 'D'){
  typePaper = parseNumber('D', -1);
  qty = parseNumber('Q', -1);
  paperProgre = true;

  #if DEBUG
    Serial.print(F("paper type : "));
    Serial.println(typePaper);
    Serial.print(F("qty : "));
    Serial.println(qty);
  #endif
 }

 // Effect : stop machine effect based on situation
  int _error = parseNumber('E', -1);
  switch (_error)
  {
    /*
       ERROR menu
       Exx - E = Error, xx = Error number
       E0 -> under volt
       E1 -> gas start break!!
       E2 -> gas input error!!
       E3 -> temprature sensor fail
       E4 -> motor homing fail
       E5 -> motor start fail
       E6 -> spindle communication fail
       E7 -> spindle power issue
       E8 -> spindle other issue
       E9 -> display communication fail
       E10 -> booting fail
       E11 -> eeprom read fail
       E12 -> sd card not detect
       E13 -> RTC start fail
       E14 -> esp-link link broken
       E15 -> vfd link broken
       E16 -> display link broken
       E17 -> update fail - esp-update-link-broken
       E18 -> temprature sensor over heating
       E19 -> estop enable not able to work
       E20 -> other error
    */
    case 0: /* reset paper parameter to default */reset_parameter(); break;
    case 1: /* auto page quit button error for stop process */auto_quit(); break;
    case 2: /* manual page back buttong for stop spindle and gas*/StopGas();stopSpindle(); break;
    case 3:       break;
    case 4:       break;
    case 5:       break;
    case 6:       break;
    case 7:       break;
    case 8:       break;
    case 9:       break;
    case 10:      break;
    case 11:      break;
    case 12:      break;
    case 13:      break;
    case 14:      break;
    case 15:      break;
    case 16:      break;
    case 17:      break;
    case 18:      break;
    case 19:      break;
    case 20:      break;
    default:      break;
  }

  //cmd = parseNumber('F', -1);
  //cmd = parseNumber('G', -1);
  //cmd = parseNumber('H', -1);

  // Effect : update parameter for specify paper
  int para = parseNumber('I', -1); 
   if(code_1 == 'I'){
    switch(para) {
        case 0: trans = &soft.Temp; break;
        case 1: trans = &soft.water; break;
        case 2: trans = &soft.oil; break;
        case 3: trans = &soft.spreder; break;
        case 4: trans = &soft.batterValve; break;
        case 5: trans = &soft.CookingTime; break;

        case 6: trans = &medium.Temp; break;
        case 7: trans = &medium.water; break;
        case 8: trans = &medium.oil; break;
        case 9: trans = &medium.spreder; break;
        case 10: trans = &medium.batterValve; break;
        case 11: trans = &medium.CookingTime; break;
        
        case 12: trans = &hard.Temp; break;
        case 13: trans = &hard.water; break;
        case 14: trans = &hard.oil; break;
        case 15: trans = &hard.spreder; break;
        case 16: trans = &hard.batterValve; break;
        case 17: trans = &hard.CookingTime; break;
        
        case 18: trans = &custom.Temp; break;
        case 19: trans = &custom.water; break;
        case 20: trans = &custom.oil; break;
        case 21: trans = &custom.spreder; break;
        case 22: trans = &custom.batterValve; break;
        case 23: trans = &custom.CookingTime; break;

        default: break;
    }
    switch (para)
    {
    case 0:
    case 6:
    case 12:
    case 18:
      *trans = parseNumber('V',-1);
      break;
    default:
      *trans = (parseNumber('V',-1)*100);
      break;
    }
    

    #if DEBUG
      Serial.println(F("*trans : "));
      Serial.println(*trans);

      Serial.print("soft water : ");
      Serial.println(soft.water);
      Serial.print("soft temp : ");
      Serial.println(soft.Temp);
      Serial.print("soft spreder : ");
      Serial.println(soft.spreder);
      Serial.print("soft oil : ");
      Serial.println(soft.oil);
      Serial.print("soft cookingTime : ");
      Serial.println(soft.CookingTime);
      Serial.print("soft batterValve : ");
      Serial.println(soft.batterValve);

      Serial.print("medium water : ");
      Serial.println(medium.water);
      Serial.print("medium temp : ");
      Serial.println(medium.Temp);
      Serial.print("medium spreder : ");
      Serial.println(medium.spreder);
      Serial.print("medium oil : ");
      Serial.println(medium.oil);
      Serial.print("medium cookingTime : ");
      Serial.println(medium.CookingTime);
      Serial.print("medium batterValve : ");
      Serial.println(medium.batterValve);
      
      Serial.print("hard water : ");
      Serial.println(hard.water);
      Serial.print("hard temp : ");
      Serial.println(hard.Temp);
      Serial.print("hard spreder : ");
      Serial.println(hard.spreder);
      Serial.print("hard oil : ");
      Serial.println(hard.oil);
      Serial.print("hard cookingTime : ");
      Serial.println(hard.CookingTime);
      Serial.print("hard batterValve : ");
      Serial.println(hard.batterValve);
      
      Serial.print("custom water : ");
      Serial.println(custom.water);
      Serial.print("custom temp : ");
      Serial.println(custom.Temp);
      Serial.print("custom spreder : ");
      Serial.println(custom.spreder);
      Serial.print("custom oil : ");
      Serial.println(custom.oil);
      Serial.print("custom cookingTime : ");
      Serial.println(custom.CookingTime);
      Serial.print("custom batterValve : ");
      Serial.println(custom.batterValve);
    #endif
    //EEPROM_write_data();
  }

  //cmd = parseNumber('J', -1);
  //cmd = parseNumber('K', -1);
  //cmd = parseNumber('L', -1);
  

  // Effect : start/stop and change spindle speed based on user input
  int _motor = parseNumber('M', -1);
  switch (_motor)
  {
    case 0:
      VFD.writeSingleRegister(0x2000, LOW_SPEED);// spindle start at 10.00 Hz
      RS485_delay;
      VFD.writeSingleRegister(0x2001, 6);//stop spindle
      RS485_delay;
      #if DEBUG
        Serial.println(F("STOP"));
      #endif
      break;

    case 1:
      VFD.writeSingleRegister(0x2001, 1);// spindle start at 10.00 Hz
      RS485_delay;
      VFD.writeSingleRegister(0x2000, LOW_SPEED);// spindle start at 10.00 Hz
      RS485_delay;
      #if DEBUG
        Serial.println(F("low speed run"));
      #endif
      break;

    case 2:
      VFD.writeSingleRegister(0x2000, FULL_SPEED);// spindle start at 10.00 Hz
      RS485_delay;
      VFD.writeSingleRegister(0x2001, 1);// spindle start at 10.00 Hz
      RS485_delay;
      #if DEBUG
        Serial.println(F("full speed run"));
      #endif
      break;
    default:
      break;
  }
  
  //cmd = parseNumber('N', -1);
  
  // Effect : on/off oil relay via manual page oil button
  int oil = parseNumber('O', -1);
  switch (oil)
  {
      case 0:PORTA &= ~(1 << OIL_RELAY);break;
      case 1:PORTA |= (1 << OIL_RELAY);break;
      default:break;
  }
  

  // Effect : update page number of display in local system
  page_number = parseNumber('P', -1);
  switch (page_number){
    case 6: sent_para(1); break;// send soft paper parameter to display
    case 7: sent_para(2); break;// send medium paper parameter to display
    case 8: sent_para(3); break;// send hard paper parameter to display
    case 9: sent_para(4); break;// send custom paper parameter to display

    #if DEBUG
      Serial.print(F("page_number : "));
      Serial.println(page_number);
    #endif
  }

 
  // update qty value for machine process
  //qty = parseNumber('Q', -1);

  // remove paper from plate
  int stepper = parseNumber('R', -1);
  /*
  * test using accelStepper library
  */
  switch (stepper)
  {
  case 1:/* move motor fwd to bwd and return */
    if(!isStepperRunning){
      motor_move = true;
      fwd = true;
      isStepperRunning = true;
    }
    break;
  default: break;
  }

  int updown = parseNumber('S', -1);
  switch (updown)
  {
      case 0:PORTA &= ~(1 << UPDOWN_RELAY);break;
      case 1:PORTA |= (1 << UPDOWN_RELAY);break;
      default:break;
  }
  

  //cmd = parseNumber('U', -1);
  //cmd = parseNumber('V', -1);
  
  int water = parseNumber('W', -1);
  switch (water)
  {
      case 0:PORTA &= ~(1 << WATER_RELAY);break;
      case 1:PORTA |= (1 << WATER_RELAY);break;
      default:break;
  }

  //cmd = parseNumber('X', -1);
  //cmd = parseNumber('Y', -1);
  //cmd = parseNumber('Z', -1);
}

/**
   After setup() this machine will repeat loop() forever.
*/
void GcodeParser()
{
  // listen for serial commands
  while (Serial1.available() > 0)
  {
    // if something is available
    char c = Serial1.read(); // get it
    //enable for echo debug
    #if DEBUG && INVALID_CHAR
      Serial.print(c);  // repeat it back so I know you got the message
    #endif

    /*
     * remove unwanted char input from serial
     */
    if(!((c > 47 && c < 58 ) || (c > 64 && c < 91 ) || (c == '\n') || (c == '\r') || (c == ' ') )){
      #if DEBUG && INVALID_CHAR
        Serial.println(F("this char is invalid"));
      #endif
    }
    else{
      #if DEBUG
        Serial.print(c);
      #endif
      if (idx < MAX_BUF - 1) buffer[idx++] = c; // store it
      if ((c == '\n') || (c == '\r'))
      {
        // entire message received
        buffer[idx] = 0; // end the buffer so string functions work right

        processCommand();  // do something with the command
        ready();
      }
    }
  }
}
#endif
