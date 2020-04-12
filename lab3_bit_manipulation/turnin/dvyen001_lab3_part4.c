/*	Author: Daniel Vyenielo
 *      Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 3  Exercise 4
 *	Exercise Description:
 *  output : complicated, look at the code. 
 *  
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "avr_configuration.h"
#include "bit_manipulation.h"
#include "utilities.h"

#define UCHAR_MAX 255
#define CHAR_BIT 8

int main(void) 
{
    unsigned char const DDR_INPUT = 0x00;
    unsigned char const DDR_OUTPUT = 0xFF;
    unsigned char const INIT_VAL_INPUT = 0xFF;
    unsigned char const INIT_VAL_OUTPUT = 0x00;
    initialize_port('A', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('B', DDR_OUTPUT, INIT_VAL_OUTPUT);
    initialize_port('C', DDR_OUTPUT, INIT_VAL_OUTPUT);
    while (1) 
    {
        PORTB = (0xF0 & PINA) >> 4;
        PORTC = (0x0F & PINA) << 4;
    }
    return 1;
}
