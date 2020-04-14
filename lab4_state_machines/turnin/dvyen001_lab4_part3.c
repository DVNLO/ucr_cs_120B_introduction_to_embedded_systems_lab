/*	Author: Daniel Vyenielo
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 4  Exercise 3
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

enum { INIT, LOCK, TO_UNLOCK, UNLOCK } state;
unsigned char X;
unsigned char Y;
unsigned char LB;
unsigned char L;

void tick()
{
    X = get_bit(0, PINA);
    Y = get_bit(1, PINA);
    LB = get_bit(2, PINA);
    L = get_bit(7, PINA);
    switch(state)
    {
        case INIT:
            state = LOCK;
            break;
        case LOCK:
            if(!X && !Y && LB && !L)
                state = TO_UNLOCK;
            break;
        case TO_UNLOCK:
            if((!X && !Y && LB && !L ) 
                || (!X && !Y && !LB && !L))
                state = TO_UNLOCK;
            else if(!X && Y && !LB && !L)
                state = UNLOCK;
            else
                state = LOCK;
            break;
        case UNLOCK:
            if(L)
                state = LOCK;
            break;
         default:
            break;
    }
    switch(state)
    {
        case INIT:
            break;
        case LOCK:
            PORTB = 0x00;
            break;
        case TO_UNLOCK:
            break;
        case UNLOCK:
            PORTB = 0x01;
            break;
        default:
            break;
    }
    PORTC = (unsigned char)(state);
}

int main(void) 
{
    initialize_port('A', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('B', DDR_OUTPUT, INIT_VAL_OUTPUT);
    initialize_port('C', DDR_OUTPUT, INIT_VAL_OUTPUT);
    state = INIT;
    while (1) 
    {
        tick();
    }
    return 1;
}
