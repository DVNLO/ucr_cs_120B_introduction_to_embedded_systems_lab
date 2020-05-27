/*	Author: Daniel Vyenielo
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Exam 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "timer.h"

#include <stdlib.h>
#include "queue.h"

/* SM state declarations --- fill in as needed */
typedef enum output_states { OInit, Ooutput } output_states;
typedef enum read_states { RInit, Rread } read_states;

/* shared variables --- fill in as needed */
unsigned char PA;
queue * q;

/* state variables --- do not alter */
output_states output_state;
read_states read_state;

/* SM definitions --- complete each task as a SM */
//#include "output.h"
//#include "read.h"

void Output()
{
    switch(output_state)
    {
        case OInit:
            output_state = Ooutput;
            PORTB = 0x00;
            break;
        case Ooutput:
            break;
        default:
            output_state = OInit;
            break;
    }
    switch(output_state)
    {
        case OInit:
            PORTB = 0x00;
            break;
        case Ooutput:
            if(!queue_empty(q))
            {
                PORTB = queue_pop(q);
            }
            break;
        default:
            break;
    }
}

void Read()
{
    switch(read_state)
    {
        case RInit:
            read_state = Rread;
            PA = 0;
            break;
        case Rread:
            break;
        default:
            read_state = RInit;
            break;
    }
    switch(read_state)
    {
        case RInit:
            PA = 0;
            break;
        case Rread:
            if(!queue_full(q))
            {
                queue_push(q, ~PINA);
            }
            if(!queue_full(q))
            {
                queue_push(q, ~PINA + 1);
            }
            break;
        default:
            break;
    }
}


/* main function --- do not alter */
int main(void) 
{
    /* DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    // Common period for all tasks set to 100ms
    TimerSet(100);
    TimerOn();

    // init state vars
    output_state = OInit;
    read_state = RInit;
    q = (queue *)malloc(sizeof(queue));
    queue_construct(q, 4);

    while (1) {
        Read();
        Output();
        while (!TimerFlag) { }
        TimerFlag = 0;
    }
    return 1;
}
