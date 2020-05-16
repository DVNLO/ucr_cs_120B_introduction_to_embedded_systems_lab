/*	Author: Daniel Vyenielo
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 8 Exercise 1
 *	Exercise Description:
 *	Demo : https://drive.google.com/file/d/13iT8MnMpBZRbNtXrm4287qTV1njehFa5/view?usp=sharing
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "adc.h"
#include "avr_configuration.h"
#include "bit_manipulation.h"
#include "io.h"
#include "timer.h"
#include "utilities.h"

unsigned short read_val;

void tick()
{
    read_val = ADC;
    PORTB = (unsigned char)read_val;
    PORTD = (read_val >> 8) & 0x03;
}

int main(void) 
{
    initialize_port('A', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('B', DDR_OUTPUT, INIT_VAL_OUTPUT);
    initialize_port('D', DDR_OUTPUT, INIT_VAL_OUTPUT);
    ADC_init();
    TimerSet(10);
    TimerOn();
    while (1) 
    {
		tick();
        while(!TimerFlag)
        {
            continue;
        }
        TimerFlag = 0;
    }
    return 1;
}
