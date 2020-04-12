/*	Author: Daniel Vyenielo
 *      Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 3  Exercise 1
 *	Exercise Description:
 *  output : the number of 1s on ports A and B on port C
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
    unsigned char const INIT_VAL_INPUT = 0xFF;
    unsigned char const DDR_OUTPUT = 0xFF;
    unsigned char const INIT_VAL_OUTPUT = 0x00;
    initialize_port('A', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('B', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('C', DDR_OUTPUT, INIT_VAL_OUTPUT);
    //initialize_port('D', DDR_OUTPUT, INIT_VAL_OUTPUT); 
    unsigned char PORT_C_TMP;
    unsigned char i;
    while (1) 
    {
        PORT_C_TMP = 0x00;
        for(i = 0; i < CHAR_BIT; ++i)
            PORT_C_TMP += get_bit(i, PINA);
        for(i = 0; i < CHAR_BIT; ++i)
            PORT_C_TMP += get_bit(i, PINB);
        PORTC = PORT_C_TMP;
    }
    return 1;
}
