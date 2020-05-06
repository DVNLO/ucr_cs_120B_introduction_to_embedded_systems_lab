/*	Author: Daniel Vyenielo
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 6  Exercise 2
 *	Exercise Description:
 *
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

enum { START, INIT, OUT0, OUT1_UP, OUT2, OUT1_DOWN, PRESS1, RELEASE, PRESS2 } state;
unsigned char output;
unsigned short i;
unsigned short PERIOD = 300;

unsigned char is_pressed()
{
    return !get_bit(0,PINA);
}

void tick()
{
    switch(state)
    {
        case START:
            state = OUT0;
            i = 0;
            break;
        case OUT0:
            if(is_pressed())
            {
                state = PRESS1;
            }
            else if(i > PERIOD)
            {
                i = 0;
                state = OUT1_UP;
            }
            ++i;
            break;
        case OUT1_UP:
            if(is_pressed())
            {
                state = PRESS1;
            }
            else if(i > PERIOD)
            {
                i = 0;
                state = OUT2;
            }
            ++i;
            break;
        case OUT2:
            if(is_pressed())
            {
                state = PRESS1;
            }
            else if(i > PERIOD)
            {
                i = 0;
                state = OUT1_DOWN;
            }
            ++i;
            break;
        case OUT1_DOWN:
            if(is_pressed())
            {
                state = PRESS1;
            }
            else if(i > PERIOD)
            {
                i = 0;
                state = OUT0;
            }
            ++i;
            break;
        case PRESS1:
            if(is_pressed())
            {
                state = PRESS1;
            }
            else
            {
                state = RELEASE;
            }
            break;
        case RELEASE:
            if(is_pressed())
            {
                state = PRESS2;
            }
            else
            {
                state = RELEASE;
            }
            break;
        case PRESS2:
            if(is_pressed())
            {
                state = PRESS2;
            }
            else
            {
                state = OUT0;
                i = 0;
            }
            break;
        default:
            break;
    }
    switch(state)
    {
        case START:
            break;
        case OUT0:
            output = 0x01;
            break;
        case OUT1_UP: 
        case OUT1_DOWN:
            output = 0x02;
            break;
        case OUT2:
            output = 0x04;
            break;
        case PRESS1:
            break;
        case RELEASE: 
            break;
        case PRESS2:
            break;
        default:
            break;
    }
    PORTB = output;
}

int main(void) 
{
    initialize_port('A', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('B', DDR_OUTPUT, INIT_VAL_OUTPUT);
    state = START;
    TimerSet(1);
    TimerOn();
    while (1) 
    {
        while(!TimerFlag)
        {
            ;
        }
        TimerFlag = 0;
        tick();
    }
    return 1;
}
