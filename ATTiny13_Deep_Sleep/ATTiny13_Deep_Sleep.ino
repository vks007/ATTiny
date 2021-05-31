/*
 * Source: https://forum.arduino.cc/t/solved-attiny13a-sleep-mode-high-current-consumption/431936/26
 * Uses the board Microcore from MCUDude : https://github.com/MCUdude/MicroCore or DIY ATTiny Board from 
 * This finally acheives a sleep current of 1.1uA. It could as well be 0.15 uA because I am measuring it with a DMM which is not very accurate for low currents
 * The difference here seems to be not using sleep_mode() , instead use statements as below 
 */

#include "avr/io.h"
#include "avr/interrupt.h"
#include "avr/sleep.h"
#include "avr/power.h"

// Define the below only if you are using the DIY ATTiny board , comment out if using Microcore board
#define BODCR _SFR_IO8(0x30)
#define BODSE 0
#define BODS 1

//Discard adc interrupt
EMPTY_INTERRUPT(ADC_vect);

void setup() {
//  DDRB  = 0b000001; // all but PB0 INPUT, want to use PB0 ...
//  PORTB = 0b000000; // all LOW

}

void loop() {
  system_sleep();

}


// set system into the sleep state
// system wakes up when watchdog times out
void system_sleep() {
  
  ACSR = ADMUX = ADCSRA = 0;  
  ACSR |= (1 << ACD);                  //Analog comparator off
  ADCSRA &= ~(1<<ADEN);                // switch Analog to Digitalconverter OFF
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  power_all_disable();
  sleep_enable();
  cli();
  BODCR = (1<<BODSE)|(1<<BODS);       //Disable BOD, step 1
  BODCR = (1<<BODS);                  //Second step
  sei();                              // 1 cycle
  sleep_cpu();  //Do not use sleep_mode(), contains sleep_enable(), sleep_cpu(),sleep_disable()
  sleep_disable();
  power_all_enable();
}
