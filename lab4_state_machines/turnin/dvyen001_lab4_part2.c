/*	Author: Daniel Vyenielo
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 4  Exercise 2
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

enum { INIT, WAIT, DECREMENT, INCREMENT, RESET } state;
unsigned char A0;
unsigned char A1;
unsigned char count_val;

void tick()
{
    A0 = get_bit(0, PINA);
    A1 = get_bit(1, PINA);
    switch(state)
    {
        case INIT:
            PORTC = 7;
            state = WAIT;
            break;
        case WAIT:
            if(A0 && A1)
            {
                state = RESET;
            }
            else if(!A0 && A1)
            {
                state = DECREMENT;
            }
            else if(A0 && !A1)
            {
                state = INCREMENT;
            }
            state = WAIT;
            break;
        case RESET:
            count_val = 0;
            PORTC = count_val;
            state = WAIT;
        case DECREMENT:
            if(count_val)
            {
                --count_val;
                PORTC = count_val;
            }
            state = WAIT;
            break;
        case INCREMENT:
            if(count_val < 9)
            {
                ++count_val;
                PORTC = count_val;
            }
            state = WAIT;
            break;
        default:
            break;
    }
}

int main(void) 
{
    initialize_port('A', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('C', DDR_OUTPUT, INIT_VAL_OUTPUT);
    state = INIT;
    while (1) 
    {
        tick();
    }
    return 1;
}
