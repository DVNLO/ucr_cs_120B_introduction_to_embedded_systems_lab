/*	Author: Daniel Vyenielo
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 8 Exercise 4
 *	Exercise Description:
 *	Demo:  
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

unsigned char i = 0;
unsigned char output = 0x01;

void tick(unsigned short const * const ranges, 
            unsigned short const ranges_size)
{
    read_val = ADC;
	i = 0;
	output = 0x01;
    while(i < ranges_size 
            && read_val > ranges[i])
    {
        ++i;
        output <<= 1;
        ++output;
    }
    PORTB = output;
}

int main(void) 
{
    initialize_port('A', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('B', DDR_OUTPUT, INIT_VAL_OUTPUT);
    initialize_port('D', DDR_OUTPUT, INIT_VAL_OUTPUT);
    ADC_init();
    TimerSet(10);
    TimerOn();
    static unsigned short const PHOTO_MIN = 0x0030;
    static unsigned short const PHOTO_MAX = 0x00C0;
    static unsigned short const PHOTO_RANGE = PHOTO_MAX - PHOTO_MIN;
    static unsigned short const PHOTO_LED_COUNT = 8;
    static unsigned short const PHOTO_RANGE_PER_LED = PHOTO_RANGE / PHOTO_LED_COUNT;
    unsigned short ranges[PHOTO_LED_COUNT];
    unsigned short const * const ranges_handle = ranges;
    unsigned short i = 0;
    ranges[i] = PHOTO_MIN;
    ++i;
    while(i < PHOTO_LED_COUNT)
    {
        ranges[i] = ranges[i - 1] + PHOTO_RANGE_PER_LED;
        ++i;
    }
    while (1) 
    {
		tick(ranges_handle, PHOTO_LED_COUNT);
        while(!TimerFlag)
        {
            continue;
        }
        TimerFlag = 0;
    }
    return 1;
}
