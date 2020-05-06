/*	Author: Daniel Vyenielo
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 5  Exercise 2
 *	Exercise Description:
 *  Make a sr - latch state machine
 *      https://drive.google.com/open?id=1-qczAqNHtoBGfwIkwvLt1P2fbmTy6UGQ
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "avr_configuration.h"
#include "bit_manipulation.h"
#include "timer.h"
#include "utilities.h"

enum { INIT, WAIT, INCREMENT_WAIT, DECREMENT_WAIT, RESET } state;
unsigned char A0;
unsigned char A1;
unsigned char PORTB_VAL;
unsigned char i;

void tick()
{
    A0 = !get_bit(0, PINA);
    A1 = !get_bit(1, PINA);
    switch(state)
    {
        case INIT:
            PORTB_VAL = 7;
            i = 0;
            state = WAIT;
            break;
        case WAIT:
            if(!A0 && !A1)
            {
                state = WAIT;
            }
            else if(!A0 && A1)
            {
                if(PORTB_VAL)
                    --PORTB_VAL;
                i = 0;
                state = DECREMENT_WAIT;
            }
            else if(A0 && !A1)
            {
                if(PORTB_VAL < 9)
                    ++PORTB_VAL;
                i = 0;
                state = INCREMENT_WAIT;
            }
            else if(A0 && A1)
            {
                PORTB_VAL = 0;
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
                if(PORTB_VAL)
                    --PORTB_VAL;
                i = 0;
                state = DECREMENT_WAIT;
            }
            else if(A0 && !A1)
            {
                ++i;
                if(i > 10 && PORTB_VAL < 9)
                {
                    ++PORTB_VAL;
                    i = 0;
                }
                state = INCREMENT_WAIT;
            }
            else if(A0 && A1)
            {
                PORTB_VAL = 0;
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
                ++i;
                if(i > 10 && PORTB_VAL > 0)
                {
                    --PORTB_VAL;
                    i = 0;
                }
                state = DECREMENT_WAIT;
            }
            else if(A0 && !A1)
            {
                if(PORTB_VAL < 9)
                    ++PORTB_VAL;
                i = 0;
                state = INCREMENT_WAIT;
            }
            else if(A0 && A1)
            {
                PORTB_VAL = 0;
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
    PORTB = PORTB_VAL;
}

int main(void) 
{
    initialize_port('A', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('B', DDR_OUTPUT, INIT_VAL_OUTPUT);
    state = INIT;
    TimerSet(100);
    TimerOn();
    while (1) 
    {
        tick();
        while(!TimerFlag){;}
        TimerFlag = 0;
    }
    return 1;
}
