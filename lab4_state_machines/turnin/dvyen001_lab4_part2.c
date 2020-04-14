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

enum { INIT, WAIT, INCREMENT_WAIT, DECREMENT_WAIT, RESET } state;
unsigned char A0;
unsigned char A1;
unsigned char PORTC_VAL;

void tick()
{
    A0 = get_bit(0, PINA);
    A1 = get_bit(1, PINA);
    switch(state)
    {
        case INIT:
            PORTC_VAL = 7;
            state = WAIT;
            break;
        case WAIT:
            if(!A0 && !A1)
            {
                state = WAIT;
            }
            else if(!A0 && A1)
            {
                if(PORTC_VAL)
                    --PORTC_VAL;
                state = DECREMENT_WAIT;
            }
            else if(A0 && !A1)
            {
                if(PORTC_VAL < 9)
                    ++PORTC_VAL;
                state = INCREMENT_WAIT;
            }
            else if(A0 && A1)
            {
                PORTC_VAL = 0;
                state = RESET;
            }
            break;
        case INCREMENT_WAIT:
            if(!A0 && !A1)
            {
                state = WAIT;
            }
            else if(!A0 && A1)
            {
                if(PORTC_VAL)
                    --PORTC_VAL;
                state = DECREMENT_WAIT;
            }
            else if(A0 && !A1)
            {
                state = INCREMENT_WAIT;
            }
            else if(A0 && A1)
            {
                PORTC_VAL = 0;
                state = RESET;
            }
            break; 
        case DECREMENT_WAIT:
           if(!A0 && !A1)
            {
                state = WAIT;
            }
            else if(!A0 && A1)
            {
                state = DECREMENT_WAIT;
            }
            else if(A0 && !A1)
            {
                if(PORTC_VAL < 9)
                    ++PORTC_VAL;
                state = INCREMENT_WAIT;
            }
            else if(A0 && A1)
            {
                PORTC_VAL = 0;
                state = RESET;
            }
            break; 
        case RESET:
            if(A0 && A1)
            {
                state = RESET;
            }
            else
            {
                state = WAIT;
            }
            break;
    }
    PORTC = PORTC_VAL;
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
