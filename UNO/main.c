/*
 * uno_I2C_slave_basics.c
 *
 * Created: 18.7.2019 9:20:14
 * Author : Glutex
 */ 

#ifndef __AVR_ATmega328p__
#define __AVR_ATmega328p__
#endif

#define F_CPU 16000000UL
#define FOSC 16000000UL // Clock Speed
#define BAUD 9600
#define MYUBRR (FOSC/16/BAUD-1)

#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lcd.h"

#define SLAVE_ADDRESS 85 // 0b1010101

// the only real global variable in this program


static void
USART_init(uint16_t ubrr) // unsigned int
{
    /* Set baud rate in the USART Baud Rate Registers (UBRR) */
    UBRR0H = (unsigned char) (ubrr >> 8);
    UBRR0L = (unsigned char) ubrr;
    
    /* Enable receiver and transmitter on RX0 and TX0 */
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0); //NOTE: the ATmega328p has 1 UART: 0
    
    /* Set frame format: 8 bit data, 2 stop bit */
    UCSR0C |= (1 << USBS0) | (3 << UCSZ00);
    
}

static void
USART_Transmit(unsigned char data, FILE *stream)
{
    /* Wait until the transmit buffer is empty*/
    while(!(UCSR0A & (1 << UDRE0)))
    {
        ;
    }
    
    /* Put the data into a buffer, then send/transmit the data */
    UDR0 = data;
}

static char
USART_Receive(FILE *stream)
{
    /* Wait until the transmit buffer is empty*/
    while(!(UCSR0A & (1 << UDRE0)))
    {
        ;
    }
    
    /* Get the received data from the buffer */
    return UDR0;
}



void
lcd_puts_line(int line, char* content)
{
    lcd_gotoxy(0, line);
    lcd_puts("                "); // empty the line
    lcd_gotoxy(0, line);
    lcd_puts(content); // replace content
    lcd_gotoxy(0,0); // return to default coordinates
}

void
lcd_setup()
{
    lcd_clrscr();
    lcd_gotoxy(0,1);
    lcd_puts("Hint: BOC song");
    lcd_gotoxy(0,0);
}


//I just couldn't get pwm to work.
//It got stuck and made a horrible noise
void
alarmRoutine()
{
    lcd_puts_line(0, " ALARM TRIPPED!");

    for(int i=3; i<3; i++)
    {
        PORTB |= (1 << PORTB5);     // turn beeper on
        PORTB |= (1 << PORTB7);     // turn onboard led on
        _delay_ms(300);

        PORTB &= ~ (1 << PORTB5);   // turn beeper off
        PORTB &= ~ (1 << PORTB7);   // turn onboard led off
        _delay_ms(300);

        
    }
}

void
timeoutRoutine()
{
    lcd_puts_line(0, "Time's up.");
    lcd_puts_line(0, "Try again.");
    _delay_ms(1000);
}

void
correctRoutine()
{
    printf("correct password!\n");
    lcd_puts_line(0, "Correct!");
    lcd_puts_line(1, "Congratulations!");
    _delay_ms(3000);
}

//Should be excluded from timeout
void
interpret_keycode(char* code, char* inputtedPassword)
{
    static int inputted = 0; // number of digits 
    printf("keycode '%c'\n", code[0]);

    // handle numbers
    if(isdigit(code[0]))
    {
        printf("'%c' is digit\n", code[0]);

        // final number already there
        // therefore don't input anything else
        if (inputted == 4) {
            return;
        }
        
        //todo add input to inputtedpassword
        inputtedPassword[inputted] = code[0];

        printf("INPUTTED:%s\n", inputtedPassword);
        lcd_puts_line(0, inputtedPassword);

        inputted++;
        return;
    }

    //alarm
    if(code[0] == 'A')
    {
        printf("alarm!\n");
        alarmRoutine();
        lcd_setup();
        lcd_puts_line(0, inputtedPassword);
        return;
    }

    //backspace
    //'B' to not have to translate anything
    if(code[0] == 'B')
    {
        printf("backspace!\n");
        
        //no character left to remove
        if (inputted == 0){
            return;
        }

        //remove one digit
        inputtedPassword[inputted-1] = '\0';
        lcd_puts_line(0, inputtedPassword);
        inputted--;
        return;
    }

    //submit code
    //'A' to not have to translate anything
    if(code[0] == 'C')
    {
        printf("submit!\n");
        return;
    }
}

// Setup the stream functions for UART
FILE uart_output = FDEV_SETUP_STREAM(USART_Transmit, NULL, _FDEV_SETUP_WRITE);
FILE uart_input = FDEV_SETUP_STREAM(NULL, USART_Receive, _FDEV_SETUP_READ);

int 
main(void)
{
    // set onboard led as output
    DDRB |= _BV(DDB5);

    // set buzzer pin as output
    DDRB |= (1 << DDB5);

    // initialize display, cursor off
    lcd_init(LCD_DISP_ON);
    
    // set starting information
    lcd_setup();

    // initialize the UART with 9600 BAUD
    USART_init(MYUBRR);
    
    // redirect the stdin and stdout to UART functions
    // these are both global variables
    stdout = &uart_output;
    stdin = &uart_input;
    
    char twi_receive_data[20];

    // variables to keep track of password
    char inputtedPassword[5]  = "\0\0\0\0\0";   // storage for inputs
    const char correctPassword[5]   = "1969\0";
    const char resetPassword[5] = "\0\0\0\0\0"; // will be reset to this

    char test_char_array[16]; // 16-bit array, assumes that the int given is 16-bits
    uint8_t twi_index = 0;
    uint8_t twi_status = 0;
    
    // slave address
    TWAR = 0b10101010; // same as 170 DEC
    // Slave address 85 + TWI General Call Recognition Enable Bit '0' LSB ---> 170
    
    // Initialize TWI slave
    TWCR |= (1 << TWEA) | (1 << TWEN);
    TWCR &= ~(1 << TWSTA) & ~(1 << TWSTO);
    
    
    while (1) 
    {
        // wait for TWINT to set, meaning that we are waiting for a transmission
        while(!(TWCR & (1 << TWINT))) 
        {
            ;
        }
        
        // get TWI status
        twi_status = (TWSR & 0xF8);	
        
        // "clear" TWINT and generate acknowledgment ACK (TWEA)
        TWCR |=  (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
        
        // wait for the the TWINT to set
        // use waiting time to check for timeout (dumb solution, I know)
        while(!(TWCR & (1 << TWINT)))
        {
            if (/*timeout*/){
                timeoutRoutine();

                //reset password
                for (int i=0; i<5; i++){
                    inputtedPassword[i] = '\0';
                }

                lcd_setup();
            }
        }
        
        // get TWI status
        twi_status = (TWSR & 0xF8);

        // if status indicates that previous response was either slave address or general call and ACK was returned
        // store the data register value to twi_receive_data
        if((twi_status == 0x80) || (twi_status == 0x90))
        {
            twi_receive_data[twi_index] = TWDR; 
            twi_index++;
        }     
        else if((twi_status == 0x88) || (twi_status == 0x98))
        {           
            // if status indicates that previous response was either slave address or general call and NOT ACK was returned
            // store the data register value to twi_receive_data
            twi_receive_data[twi_index] = TWDR;
            twi_index++;
        }
        else if(twi_status == 0xA0)
        {
            // Stop condition or repeated start was received
            // Clear interrupt flag
            TWCR |= (1 << TWINT);
        }        

        // if twi_index indicates that the twi_receive_data is full, handle message.
        if (20 <= twi_index)
        {
            printf("DATA:");
            printf(twi_receive_data);
            printf("\n");
            
            twi_index = 0;

            interpret_keycode(twi_receive_data, inputtedPassword);
            
            printf("inputted: %s\n", inputtedPassword);

            //correct password
            if (0 == strcmp(inputtedPassword, correctPassword))
            {
                correctRoutine();
                
                for (int i=0; i<5; i++){
                    inputtedPassword[i] = '\0';
                }

                lcd_setup();
            }
        }
    }
    
    return 0;
}