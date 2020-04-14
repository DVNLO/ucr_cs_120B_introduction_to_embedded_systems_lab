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

enum { INIT, Q, TRANSITION_Q, Q_NOT, TRANSITION_Q_NOT } state;

void tick()
{
    switch(state)
    {
        case INIT:
            PORTB = set_bit(0, INIT_VAL_OUTPUT);
            state = Q;
            break;
        case Q:
            if(get_bit(0, PINA))
            {
                state = TRANSITION_Q_NOT;
            }
            break;
        case TRANSITION_Q_NOT:
            if(!get_bit(0, PINA))
            {
                PORTB = set_bit(1, 0x00);
                state = Q_NOT;
            }
            break;
        case Q_NOT:
            if(get_bit(0, PINA))
            {
                state = TRANSITION_Q;
            }
            break;
        case TRANSITION_Q:
            if(!get_bit(0, PINA))
            {
                PORTB = set_bit(0, 0x00);
                state = Q;
            }
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
