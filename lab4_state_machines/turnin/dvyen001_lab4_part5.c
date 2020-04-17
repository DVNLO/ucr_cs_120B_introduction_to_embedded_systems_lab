/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
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
enum { INIT, LOCK, UNLOCK } state;
unsigned char sequence[] = { 0x04, 0x01, 0x02, 0x01 };
unsigned char i;
unsigned char A;

void tick()
{
    A = PINA;
    switch(state)
    {
        case INIT:
            i = 0;
            state = LOCK;
            break;
        case LOCK:
            if(i > 3)
            {
                i = 0;
                state = UNLOCK;
            }
            break;
        case UNLOCK:
            if(get_bit(7, PINA) || i > 3)
            {
                i = 0;
                state = LOCK;
            }
            break;
        default:
            break;
    }
    switch(state)
    {
        case INIT:
            break;
        case LOCK:
            if(A)
            {
                if(A == sequence[i])
                    ++i;
            }
            PORTB = 0x00;
            break;
        case UNLOCK:
            if(A)
            {
                if(A == sequence[i])
                    ++i;
            }
            PORTB = 0x01;
            break;
        default:
            break;
    }
    PORTC = i;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    //DDRA = 0x00; PORTA = 0xFF;
    initialize_port('A', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('B', DDR_OUTPUT, INIT_VAL_OUTPUT);
    initialize_port('C', DDR_OUTPUT, INIT_VAL_OUTPUT);
    //DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
    state = INIT;
    while (1) 
    {
        tick();
    }
    return 1;
}
