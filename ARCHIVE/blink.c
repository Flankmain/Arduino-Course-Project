
#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "lcd1602.h"

int main(void) {
    // Set port B7 as output
    DDRB |= (1 << DDB7);

    while(1) {
        // turn LED on
        PORTB |= (1 << PORTB7);

        // wait 1 second
        _delay_ms(1000);

        // turn LED off
        PORTB = PORTB & ~ (1 << PORTB7);
        
        // wait 1 second again
        _delay_ms(1000);
    }
}