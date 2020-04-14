/*	Author: Daniel Vyenielo
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 4  Exercise 1
 *	Exercise Description:
 *  Make a sr - latch state machine
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

enum { INIT, PB0_ON, TO_PB1, PB1_ON, TO_PB0 } state;
unsigned char A0;
unsigned char const B0 = 0;
unsigned char const B1 = 1;

void tick()
{
    A0 = get_bit(0, PINA);
    switch(state)
    {
        case INIT:
            PORTB = set_bit(B0, 0x00);
            state = PB0_ON;
            break;
        case PB0_ON:
            if(A0)
            {
                state = TO_PB1;
            }
            break;
        case TO_PB1:
            if(!A0)
            {
                state = PB1_ON;
            }
            break;
        case PB1_ON:
            if(A0)
            {
                state = TO_PB0;
            }
            break;
        case TO_PB0:
            if(!A0)
            {
                state = PB0;
            }
            break;
        default:
            break;
    }
    switch(state)
    {
        case INIT:
            break;
        case PB0_ON:
            break;
        case TO_PB1:
            PORTB = set_bit(B1, 0x00);
            break;
        case PB1_ON:
            break;
        case TO_PB0:
            PORTB = set_bit(B0, 0x00);
            break;
        default:
            break;
    }
}

int main(void) 
{
    initialize_port('A', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('B', DDR_OUTPUT, INIT_VAL_OUTPUT);
    state = INIT;
    while (1) 
    {
        tick();
    }
    return 1;
}
