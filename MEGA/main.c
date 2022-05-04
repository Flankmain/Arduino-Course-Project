#define F_CPU 16000000UL

#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif


// standard libraries
#include <stdio.h>

// arduino-related libraries
#include <avr/io.h>
#include <util/delay.h>
#include "arduinolibs/megalcd.h"

#include "arduinolibs/keypad.h"

//#include "Arduino.h"


/*
 * A simple sanity check function to see that the lcd works.
 * expected result:
 *  --------------
 *   Hello world!
 *   1234
 *  --------------
 * */

void testLCD(void) {
    lcd_puts("Hello World!"); //print string
    _delay_ms(2000);

    // lets create a test number (int) and convert it to char array
    int16_t test_number = 1023;
    char test_char_array[16]; // 16-bit array, assumes that the int given is 16-bits
    itoa(test_number, test_char_array, 10);
    
    lcd_gotoxy(0,1); // move cursor to start of second line
    lcd_puts(test_char_array); // print test number

    _delay_ms(2000);
}

/*
 * The first
 * 
 * 
 * 
 * 
 * */


void blink(void) {
    PORTB |= (1 << PORTB7);     // turn onboard led on
    _delay_ms(500);
    
    PORTB &= ~ (1 << PORTB7);   // turn onboard led off
    _delay_ms(500);
}

/*
 * A function invoked before the loop.
 * */

void setup(void) {
    
    lcd_init(LCD_DISP_ON); // required to use the LCD
    
    lcd_clrscr(); 
    
    #if 0
    testLCD();
    lcd_clrscr();
    #endif

    KEYPAD_Init();

    DDRB |= (1 << DDB7); // make the onboard led an output 
}

int main(void) {

    setup();

    //test for keycode function, char array for easy handling
    char keycode[] = " ";

    while(1) {
        
        keycode[0] = KEYPAD_GetKey(); //function only returns when key is pressed, therefore 
        lcd_puts(keycode);
        _delay_ms(50);

        //blink();
    }
}