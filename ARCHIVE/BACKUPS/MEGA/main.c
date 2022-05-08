#define F_CPU 16000000UL

#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif


#include <stdio.h>

#include <avr/io.h>
#include <util/delay.h>
#include "megalcd.h"

//#include "Arduino.h"

int main(void) {

    //initialize display, with blinking cursor
    lcd_init(LCD_DISP_ON); //CURSOR_BLINK is appendable to here
    _delay_ms(2000);

    lcd_clrscr(); //clear display
    _delay_ms(2000);

    lcd_puts("Hello World!"); //print string
    _delay_ms(5000);

    //lets create a test number (int) and convert it to char array
    int16_t test_number = 1023;
    char test_char_array[16]; // 16-bit array, assumes that the int given is 16-bits
    itoa(test_number, test_char_array, 10);
    
    
    lcd_gotoxy(0,1); //move cursor to start of  second line
    lcd_puts(test_char_array); //print test number


    // Set port B7 as output
    DDRB |= (1 << DDB7);
    while(1) {

        //just blink the onboard led for the rest of eternity
        PORTB |= (1 << PORTB7);
        _delay_ms(500);
        PORTB &= ~ (1 << PORTB7);
        _delay_ms(500);
    }
}