/*	Author: Daniel Vyenielo
 *      Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 3  Exercise 2
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
    unsigned char const INIT_VAL_INPUT = 0xFF;
    unsigned char const DDR_OUTPUT = 0xFF;
    unsigned char const INIT_VAL_OUTPUT = 0x00;
    initialize_port('A', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('C', DDR_OUTPUT, INIT_VAL_OUTPUT);
    unsigned char PORT_C_TMP;
    //output maps the input configuration to the output pin configuration.
    unsigned char output[] = { 0x40,
                               0x60, 0x60,
                               0x70, 0x70,
                               0x38, 0x38,
                               0x3C, 0x3C, 0x3C,
                               0x3E, 0x3E, 0x3E,
                               0x3F, 0x3F, 0x3F };
    while (1) 
    {
        PORT_C_TMP = output[PINA & 0x0F];
        PORTC = PORT_C_TMP;
    }
    return 1;
}
