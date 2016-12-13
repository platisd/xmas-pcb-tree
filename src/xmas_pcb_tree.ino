/**
   Xmas PCB Tree
   The ATtiny 25/45/85 microcontroller will be sleeping until a button is pressed.

   More info at: http://plat.is/xmaspcb
   Most code is based on Nick Gammon's example: http://www.gammon.com.au/forum/?id=11488&reply=9#reply9
*/

#include <avr/sleep.h>    // Sleep Modes
#include <avr/power.h>    // Power management

const int ledPins[4] = {PB0, PB1, PB4, PB3};
const int switchPin = PB2;

ISR (PCINT0_vect)
{
  // We leave this blank as we don't want to do anything specific in the interrupt, just wake the mcu up
}

void setup() {
  for (int i = 0; i < 4; i++){
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(switchPin, INPUT);
  // pin change interrupt for D2
  PCMSK  |= bit (PCINT2);  // want pin D2 (PB2)
  GIFR   |= bit (PCIF);    // clear any outstanding interrupts
  GIMSK  |= bit (PCIE);    // enable pin change interrupts

}

void loop() {
  goToSleep(); // Sleep until the button at PB2 is pressed
  // A simple lighting sequence
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
    digitalWrite(ledPins[i], LOW);
    delay(100);
  }
  for (int i = 3; i >= 0; i--) {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
    digitalWrite(ledPins[i], LOW);
    delay(100);
  }
}


void goToSleep() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  ADCSRA = 0;            // turn off ADC
  power_all_disable ();  // power off ADC, Timer 0 and 1, serial interface
  sleep_enable();
  sleep_cpu();          // Sleep here and wait for the interrupt
  sleep_disable();
  power_all_enable();    // power everything back on
}
