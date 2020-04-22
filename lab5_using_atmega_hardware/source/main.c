/*	Author: Daniel Vyenielo
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 5  Exercise 3
 *	Exercise Description:
 *  Allows the user to input A0 or A1 to left or right shift a display
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

enum { START, INIT, WAIT_PRESS, WAIT_RELEASE } state;
unsigned char output;
unsigned char A0;
unsigned char A1;

void tick()
{
    A0 = !get_bit(0, PINA);
    A1 = !get_bit(1, PINA);
    switch(state)
    {
        case START:
            output = 0x01;
            state = WAIT_PRESS;
            break;
        case WAIT_PRESS:
            if(A0 && !A1)
            {
                output <<= 1;
                if(output > 0x20)
                    output = 0x01;
                state = WAIT_RELEASE;
            }
            else if(!A0 && A1)
            {
                output >>= 1;
                if(!output)
                    output = 0x20;
                state = WAIT_RELEASE;
            }
            break;
        case WAIT_RELEASE:
            if(A0 || A1)
            {
                state = WAIT_RELEASE;
            }
            else
            {
                state = WAIT_PRESS;
            }
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
    while (1) 
    {
        tick();
    }
    return 1;
}
