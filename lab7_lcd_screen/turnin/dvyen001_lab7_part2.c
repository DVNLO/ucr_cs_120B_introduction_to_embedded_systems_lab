/*	Author: Daniel Vyenielo
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 7  Exercise 2
 *	Exercise Description:
 *	Demo : https://drive.google.com/file/d/13xldNsNIf44QnlN1hXj8SgBD2knuw6QR/view?usp=sharing
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
unsigned char output[] = { 0x01, 0x02, 0x04, 0x02 };
unsigned char i;
unsigned char has_released = 1U;
unsigned char score = 5;

unsigned char is_pressed()
{
    return !get_bit(0,PINA);
}

void tick()
{
	unsigned char press = is_pressed();
    switch(state)
    {
        case START:
            i = 0;
            state = CYCLE;
            break;
        case CYCLE:
			if(!press)
				has_released = 1U;
            if(has_released && press)
            {
				if(output[i] == 0x02)
                {
                    ++score;
                }
                else
                {
                    if(score > 0)
                        --score;
                }
                LCD_ClearScreen();
                if(score >= 9)
                {
                    LCD_DisplayString(1, "Victory");
                }
                else
                {
                    LCD_WriteData('0' + score);
                }
                state = PRESS0;
            }
            else 
			{
                ++i;
                if(i > 3)
                    i = 0;
            }
            break;
        case PRESS0:
            if(press)
            {
				state = HOLD0;
            }
            else
            {
                state = WAIT;
            }
            break;
		case HOLD0:
            if(press)
            {
				state = HOLD0;
            }
            else
            {
                state = WAIT;
            }
            break;
	case WAIT:
            if(press)
            {
				i = 0;
				has_released = 0U;
                state = CYCLE;
                if(score >= 9)
                {
                    score = 5;
                    LCD_ClearScreen();
                    LCD_WriteData('0' + score);
                }
            }
            else
            {
                state = WAIT;
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
    initialize_port('C', DDR_OUTPUT, INIT_VAL_OUTPUT);
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
