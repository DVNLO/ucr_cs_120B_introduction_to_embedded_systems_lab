/*	Author: Daniel Vyenielo
 *      Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 2  Exercise 1
 *	Exercise Description:
 *	Illuminate PB0 if the garage door is open at night.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char get_bit(unsigned char const i, 
                      unsigned char const src)
// returns the i-th bit value found in src.
{
    unsigned char ret = src;
    ret >>= i;
    ret &= 0x01;
    return ret;
}

unsigned char set_bit(unsigned char const i, 
                      unsigned char const src)
// sets the i-th bit in src.
{
    unsigned char ret = src;
    ret |= (0x01 << i);
    return ret;
}

unsigned char clear_bit(unsigned char const i,
                        unsigned char const src)
// clears the i-th bit in src.
{
    unsigned char ret = src;
    ret &= ~(0x01 << i);
    return ret;
}

unsigned char toggle_bit(unsigned char const i,
                         unsigned char const src)
// toggles the i-th bit in src.
{
    unsigned char ret = src;
    ret ^= (0x01 << i);
    return ret;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00;    // configure PORTA's 8 pins as input
    PORTA = 0xFF;    // initialize PORTA
    DDRB = 0xFF;    // configure PORTB's 8 pins as output
    PORTB = 0x00;    // initialize PORTB
    /* Insert your solution below */
    unsigned char PA0;
    unsigned char PA1;
    unsigned char PB0;
    unsigned char PORTB_TMP = 0x00;
    while (1) 
    {
        PA0 = get_bit(0, PORTA);
        PA1 = get_bit(1, PORTA);
        if(PA0 && !PA1)
        {
            PORTB_TMP = set_bit(0, PORTB_TMP);
        }
	    else
        {
	        PORTB_TMP = clear_bit(0, PORTB_TMP);
        }
        PORTB = PORTB_TMP;
    }
    return 1;
}
