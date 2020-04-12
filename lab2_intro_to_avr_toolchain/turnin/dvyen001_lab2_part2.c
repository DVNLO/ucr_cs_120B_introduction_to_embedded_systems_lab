/*	Author: Daniel Vyenielo
 *      Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 2  Exercise 2 
 *	Exercise Description:
 *  Output the number of available spaces on port C
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

int main(void) 
{
    /* Insert DDR and PORT initializations */
    DDRA = 0x00;    // configure PORTA's 8 pins as input
    PORTA = 0xFF;    // initialize PORTA
    DDRC = 0x8F;    // configure PORTC's pins as output on P7, and P3...P0
    PORTC = 0x00;    // initialize PORTC
    /* Insert your solution below */
    unsigned char i;
    unsigned char PORT_C_TMP;
    unsigned char const SPACES_COUNT = 4;
    while (1) 
    {
        PORT_C_TMP = 0x00;
        for(i = 0; i < SPACES_COUNT; ++i)
        {
            PORT_C_TMP += get_bit(i, PINA);
        }
        PORT_C_TMP = SPACES_COUNT - PORT_C_TMP;
        PORTC = PORT_C_TMP;
    }
    return 1;
}
