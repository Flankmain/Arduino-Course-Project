#define F_CPU 16000000UL

#ifndef __AVR_ATmega328__
#define __AVR_ATmega328__
#endif

// standard libraries
#include <stdio.h>

// arduino-related libraries
#include <avr/io.h>
#include <util/delay.h>

/*
 * A simple function for testing. If the on-board led blinks at 1hz, everything works.
 * */

void blink(void) {
    PORTB |= (1 << PORTB7);     // turn onboard led on
    _delay_ms(50);
    
    PORTB &= ~ (1 << PORTB7);   // turn onboard led off
    _delay_ms(950);
}

/*
 * A function invoked before the loop.
 * Mimics the format of the arduino.
 * */

void setup(void) {
    DDRB |= (1 << DDB7); // make the onboard led act as an output 
}

void loop() {
    blink();
}

int main(void) {

    setup();

    //in all modern compilers it doesn't matter if the while(1) is inside the function.
    //both should produce the same assembly.
    while(1) {
        loop();
    }
}