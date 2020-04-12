/*	Author: Daniel Vyenielo
 *      Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 3  Exercise 5 
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
    initialize_port('B', ~(0x01), 0x01);
    initialize_port('D', DDR_INPUT, INIT_VAL_INPUT);
    unsigned short weight;
    unsigned char PORT_B_TMP;
    while (1) 
    {
        PORT_B_TMP = 0x00;
        weight = (PIND << 1) + get_bit(0, PINB);
        if(weight >= 70)
        {
            PORT_B_TMP = set_bit(1, PORT_B_TMP);
            PORT_B_TMP = clear_bit(2, PORT_B_TMP);
        }
        else if(weight > 5)
        {
            PORT_B_TMP = clear_bit(1, PORT_B_TMP);
            PORT_B_TMP = set_bit(2, PORT_B_TMP);
        }
        else
        {
            PORT_B_TMP = clear_bit(1, PORT_B_TMP);
            PORT_B_TMP = clear_bit(2, PORT_B_TMP);
        }
        PORTB = PORT_B_TMP;
    }
    return 1;
}
