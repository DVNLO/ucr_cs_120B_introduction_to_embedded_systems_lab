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

enum { INIT, UNLOCK, LOCK, INTER } state;
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
            state = UNLOCK;
            break;
        case UNLOCK:
            if(!X && !Y && !LB && L)
            {
                PORTB = 0x01;
                state = LOCK;
            }
            break;
        case LOCK:
            if(!X && !Y && LB && !L)
                state = INTER;
            break;
        case INTER:
            if(!X && Y && !LB && !L)
            {
                PORTB = 0x00;
                state = UNLOCK;
            }
            else if(!X && !Y && LB && !L)
            {
                state = INTER;
            }
            else
                state = LOCK;
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
