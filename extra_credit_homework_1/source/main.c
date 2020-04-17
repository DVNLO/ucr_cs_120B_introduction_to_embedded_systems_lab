/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <limits.h>
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "avr_configuration.h"


enum { S00, S01, S10, S11 } state;
unsigned short adj_mat[] = { 0, 0, 0, 0,
                             0, 0, 0, 0,
                             0, 0, 0, 0,
                             0, 0, 0, 0 };

unsigned short r;

void update_adj_matrix(unsigned char u, unsigned char v)
{
    size_t idx = 4 * u + v;
    if(adj_mat[idx] == USHRT_MAX)
    {
        // I am not sure how to handle overflow. 
        for(unsigned short i = 0; i < 16; ++i)
            adj_mat[i] = 0;
    }
    else
    {
        ++adj_mat[idx];
    }
}

unsigned char most_likely(unsigned char u)
{
    size_t idx = 4 * u;
    unsigned short j = 0;
    for(unsigned short i = 1; i < 4; ++i)
    {
        if(adj_mat[idx + i] > adj_mat[idx + j])
            j = i;
    }
    return j;
}

void tick()
{
    r = PINA & 0x03;
    update_adj_matrix(state, r);
    state = r;
    PORTB = most_likely(state);
}

int main(void) 
{
    initialize_port('A', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('B', DDR_OUTPUT, INIT_VAL_OUTPUT);
    state = S00;
    while (1) 
    {
        tick();
    }
    return 1;
}
