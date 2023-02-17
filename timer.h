#ifndef timer_h_
#define timer_h_

#include "Lib.h"

// TODO : look up table for stepper motor pulse genrator

bool stepper_timer(){ 
    bool timer_enabled = false;
    // initialize timer1 for stepper motor pulse generator 
    noInterrupts();           // disable all interrupts
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;
    /**
     * @brief 
     * How to calculat timer frequncy
     * Timer1 is 16bit timer = 65536 timer overflow value = TOV
     * TOV - (crystal on arduino board) / prescaler / Hz you want for output
     * 
     * we using 10kHz for stepper motor
     * pulse time for motor = 1/10kHz = 100us(96us)
     * so we are using 256 prescaler
     * 16M / 256 = 62.5kHz
     * pulse time timer = 1/62.5kHz = 16us
     * count up to = 100/16 = 6.25 OCR value not valid
     * count valid = 96/16 = 6 OCR value used
     */
    OCR1A   = 46;
    TCCR1B |= (1 << WGM12);   // CTC mode
    TCCR1B |= (1 << CS12);    // 256 prescaler 
    TIMSK1 |= (1 << OCIE1A);

    interrupts();             // enable all interrupts

    return timer_enabled=true;
}

bool Gas_timer(){
  bool isGasStart = false;
  noInterrupts();           // disable all interrupts

  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2  = 0;
  /**
   * @brief 
   * How to calculat timer frequncy
   * Timer2 is 8bit timer = 255 timer overflow value = TOV
   * TOV - (crystal on arduino board) / prescaler / Hz you want for output
   * 
   * we using 10kHz for stepper motor
   * pulse time for motor = 1/10kHz = 100us(96us)
   * so we are using 256 prescaler
   * 16M / 256 = 62.5kHz
   * pulse time timer = 1/62.5kHz = 16us
   * count up to = 100/16 = 6.25 OCR value not valid
   * count valid = 96/16 = 6 OCR value used
   */

  OCR2A = 255;            // compare match register 16MHz/256/2Hz
  TCCR2B |= (1 << WGM22);   // CTC mode
  TCCR2B |= (1 << CS22);    // 256 prescaler 
  TIMSK2 |= (1 << OCIE2A);  // enable timer compare interrupt
  
  interrupts();             // enable all interrupts
  return isGasStart=true;
}

ISR(TIMER1_COMPA_vect){ // interrupt service routine that wraps a user defined function supplied by attachInterrupt
  //TCNT1 = 0;            // preload timer
  // generat pulse 
  PORTB ^= (1 << STEPPER_PULSE_1);
}

ISR(TIMER2_COMPA_vect){ // interrupt service routine that wraps a user defined function supplied by attachInterrupt
  TCNT2 = 0;            // preload timer
  // enable gas relay based on state 
}

#endif