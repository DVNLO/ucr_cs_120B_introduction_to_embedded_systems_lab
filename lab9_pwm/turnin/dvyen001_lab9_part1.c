/*	Author: Daniel Vyenielo
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 9 Exercise 1
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
#include "pwm.h"
#include "timer.h"
#include "utilities.h"

double tone_frequencies[] = { 261.63,   // C4
                              293.66,   // D4
                              329.63,   // E4
                              349.23,   // F4
                              392.20,   // G4
                              440.00,   // A4
                              493.88,   // B4
                              523.25    // C5
                            };
unsigned char const TONE_FREQUENCIES_SIZE = 8;
unsigned char const C4_IDX = 0;
unsigned char const D4_IDX = 1;
unsigned char const E4_IDX = 2;
unsigned char const F4_IDX = 3;
unsigned char const G4_IDX = 4;
unsigned char const A4_IDX = 5;
unsigned char const B4_IDX = 6;
unsigned char const C5_IDX = 7;
double const STOP = 0;

unsigned char is_pressed(unsigned char pin, 
                         unsigned char src)
{
    return !get_bit(pin, src);
}

unsigned char pa0_is_pressed;
unsigned char pa1_is_pressed;
unsigned char pa2_is_pressed;

void tick()
{
    pa0_is_pressed = is_pressed(0, PINA);
    pa1_is_pressed = is_pressed(1, PINA);
    pa2_is_pressed = is_pressed(2, PINA);
    if(pa0_is_pressed 
        && !pa1_is_pressed
        && !pa2_is_pressed)
    {
        set_PWM(tone_frequencies[C4_IDX]);
    }
    else if(!pa0_is_pressed 
            && pa1_is_pressed
            && !pa2_is_pressed)
    {
        set_PWM(tone_frequencies[D4_IDX]);
    }
    else if(!pa0_is_pressed 
            && !pa1_is_pressed
            && pa2_is_pressed)
    {
        set_PWM(tone_frequencies[E4_IDX]);
    } 
    else
    {
        set_PWM(STOP);
    }
}

int main(void) 
{
    initialize_port('A', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('B', DDR_OUTPUT, INIT_VAL_OUTPUT);
    PWM_on();
    while(1)
    {
        tick();
    }
    return 1;
}
