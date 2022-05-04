
#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif

#include <stdio.h>

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

int main(void) {
    // Set port B7 as output
    DDRB |= (1 << DDB7);

    lcd1602_init();

    while(1) {
        
        PORTB |= (1 << PORTB7);
        lcd1602_send_string("hello");
        printf("Hello\n");

        _delay_ms(500);

        PORTB &= ~ (1 << PORTB7);
        lcd1602_send_string("world");
        printf("world!\n");

        _delay_ms(500);
    }
}