/*	Author: Daniel Vyenielo
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 7  Exercise 2
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

#include "avr_configuration.h"
#include "bit_manipulation.h"
#include "io.h"
#include "timer.h"
#include "utilities.h"

enum { START, CYCLE, PRESS0, HOLD0, WAIT } state;

unsigned char is_pressed()
{
    return !get_bit(0,PINA);
}

void tick()
{
}

int main(void) 
{
    initialize_port('A', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('D', DDR_OUTPUT, INIT_VAL_OUTPUT);
    state = START;
    TimerSet(300);
    TimerOn();
    LCD_init();
    LCD_WriteData('0' + score);
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
