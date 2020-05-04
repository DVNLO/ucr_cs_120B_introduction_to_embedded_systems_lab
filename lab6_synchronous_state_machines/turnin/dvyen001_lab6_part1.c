/*	Author: Daniel Vyenielo
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 6  Exercise 1
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

enum { START, INIT, CYCLE } state;
unsigned char output;

void tick()
{
    switch(state)
    {
        case START:
            state = INIT;
            break;
        case INIT:
            state = CYCLE;
            break;
        case CYCLE:
            state = CYCLE;
            break;
        default:
            break;
    }
    switch(state)
    {
        case START:
            break;
        case INIT:
            output = 0x01;
            break;
        case CYCLE:
            if(output >= 0x04)
                output = 0x01;
            else
                output <<= 1;           
            break;
        default:
            break;
    }
    PORTB = output;
}

int main(void) 
{
//    initialize_port('A', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('B', DDR_OUTPUT, INIT_VAL_OUTPUT);
    state = START;
    TimerSet(1000);
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
