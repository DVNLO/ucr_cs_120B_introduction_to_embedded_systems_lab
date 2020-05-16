/*	Author: Daniel Vyenielo
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 8 Exercise 2
 *	Exercise Description:
 *	Demo : https://drive.google.com/open?id=14_U1kmzmokrk1snDNLx1WktWTpvJ2F7C
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

unsigned short const PHOTO_MIN = 0x0030;
unsigned short PHOTO_MAX = 0x00C0;

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
