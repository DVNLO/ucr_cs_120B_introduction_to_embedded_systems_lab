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

enum { START, CYCLE, PRESS1, RELEASE, PRESS2 } state;
unsigned char output[] = { 0x01, 0x02, 0x04, 0x02 };
unsigned char i = -1;

unsigned char is_pressed()
{
    return !get_bit(0,PINA);
}

void tick()
{
    switch(state)
    {
        case START:
            state = CYCLE;
            break;
        case CYCLE:
            if(is_pressed())
            {
				state = PRESS1;
            }
            else 
			{
                ++i;
                if(i > 3)
                    i = 0;
            }
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
                state = CYCLE;
            }
            break;
        default:
            break;
    }
    PORTB = output[i];
}

int main(void) 
{
    initialize_port('A', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('B', DDR_OUTPUT, INIT_VAL_OUTPUT);
    state = START;
    TimerSet(300);
    TimerOn();
    while (1) 
    {
		tick();
		while(!TimerFlag)
        {
            ;
        }
        TimerFlag = 0;
    }
    return 1;
}
