/*
 * mega_I2C_master_basics.c
 *
 * Created: 18.7.2019 9:21:00
 * Author : Glutex
 */ 

#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#endif

#define F_CPU 16000000UL
#define FOSC 16000000UL // Clock Speed
#define BAUD 9600
#define MYUBRR (FOSC/16/BAUD-1)
//define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "keypad.h"

#define SLAVE_ADDRESS 85 // 0b1010101

static void
USART_init(uint16_t ubrr)
{
    /* Set baud rate in the USART Baud Rate Registers (UBRR) */
    UBRR0H = (unsigned char) (ubrr >> 8);
    UBRR0L = (unsigned char) ubrr;
    
    /* Enable receiver and transmitter on RX0 and TX0 */
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0); //NOTE: the ATmega2560 has 4 UARTs: 0,1,2,3
    
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
    
    /* Puts the data into a buffer, then sends/transmits the data */
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

// Setup the stream functions for UART
FILE uart_output = FDEV_SETUP_STREAM(USART_Transmit, NULL, _FDEV_SETUP_WRITE);
FILE uart_input = FDEV_SETUP_STREAM(NULL, USART_Receive, _FDEV_SETUP_READ);

int 
main(void)
{
    // initialize keypad
    KEYPAD_Init();

    // initialize the UART with 9600 BAUD
    USART_init(MYUBRR);
    
    // redirect the stdin and stdout to UART functions
    stdout = &uart_output;
    stdin = &uart_input;
    
    unsigned char twi_send_data[20] = "master to slave\n";
    //unsigned char twi_send_data_2[20] = "\0\0";
    //unsigned char* twi_send_data = twi_send_data_2;

    unsigned char twi_send_char = '\0';
    
    char print_char_array[16]; // 16-bit array, assumes that the int given is 16-bits
    uint8_t twi_status = 0;
    
    // Initialize TWI 
    // set SCL frequency to 400 kHz, using equation in p. 242 of ATmega2560 datasheet
    TWSR = 0x00; // TWI status register prescaler value set to 1
    TWBR = 0x03; // TWI bit rate register.
    TWCR |= (1 << TWEN); // enable TWI
    
    while (1) 
    {
        // Start transmission by sending START condition
        TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); 
        
        // wait for the TWINT to set
        while (!(TWCR & (1 << TWINT))) 
        {
            ;    
        }
        
        // read the status from TWI status register, 0xF8 is used to mask prescaler bits so that 
        // only the status bits are read
        #if 0
        twi_status = (TWSR & 0xF8); 
        
        // print the status bits to the UART for monitoring
        itoa(twi_status, print_char_array, 16);
        printf(print_char_array);
        printf(" ");
        #endif
        
        // Send slave address and write command to enter MT mode
        TWDR = 0b10101010; // load slave address and write command
        // Slave address = 85 + write bit '0' as a LSB ---> 170 
        
        // clear TWINT to start transmitting the slave address + write command
        TWCR = (1 << TWINT) | (1 << TWEN); 
        
        // wait for the TWINT to set
        while (!(TWCR & ( 1<< TWINT)))
        {
            ;
        }
        
        // read the status from TWI status register, 0xF8 is used to mask prescaler bits so that
        // only the status bits are read
        #if 0 
        twi_status = (TWSR & 0xF8);
        
        itoa(twi_status, print_char_array, 16);
        printf(print_char_array);
        printf(" ");
        #endif
       
        
        #if 1

        // wait until there is something to transmit
        twi_send_char = '\0';
        while(twi_send_char == '\0') {
            twi_send_char = KEYPAD_GetKey();
            //todo add other choices
        }
        
        printf("CODE: '%c'\n", twi_send_char);
        printf("STARTING TRANSACTION\n");
        twi_send_data[0] = twi_send_char;//this is just quick to test

        #endif
        

        // transmit data to the slave
        for(int8_t twi_data_index = 0; twi_data_index < sizeof(twi_send_data); twi_data_index++)
        {
            //send data one character at a time.
            TWDR = twi_send_data[twi_data_index]; // load data 
            
            // "clear" TWINT to start transmitting the data
            TWCR = (1 << TWINT) | (1 << TWEN);
            
            // wait for the TWINT to set
            while (!(TWCR & ( 1<< TWINT)))
            {
                ;
            }

            #if 0
            // read the status from TWI status register, 0xF8 is used to mask prescaler bits so that
            // only the status bits are read
            twi_status = (TWSR & 0xF8);
            itoa(twi_status, print_char_array, 16);
            printf(print_char_array);
            printf(" ");
            #endif
        }

        printf("\n");
        
        // stop transmission by sending STOP
        TWCR = (1 << TWINT) | (1 << TWSTO) |(1 << TWEN);

        _delay_ms(1000);
    }

    return 0;
}