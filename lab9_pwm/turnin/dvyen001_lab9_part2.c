/*	Author: Daniel Vyenielo
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 9 Exercise 2
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

typedef enum button_state{ INIT, RELEASED, PRESS, PRESSED, RELEASE } button_state;

button_state 
change_button_state(button_state const current_state,
                    unsigned char const is_pressed)
// returns the next button_state depending on the inputs
// current_state and is_pressed.
{
    button_state next_state;
    switch(current_state)
    {
        case INIT:
            next_state = RELEASED;
            break;
        case RELEASED:
            if(is_pressed)
                next_state = PRESS;
            else
                next_state = RELEASED;
            break;
        case PRESS:
            if(is_pressed)
                next_state = PRESSED;
            else
                next_state = RELEASE;
            break;
        case PRESSED:
            if(is_pressed)
                next_state = PRESSED;
            else
                next_state = RELEASE;
            break;
        case RELEASE:
            if(is_pressed)
                next_state = PRESS;
            else
                next_state = RELEASED;
            break;
        default:
            next_state = INIT;
            break;
    }
    return next_state;
}

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
unsigned char const TONE_FREQUENCIES_TERMINAL_IDX = 7;
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

unsigned char enable_output;
unsigned char tone_idx;

void button_enable()
{
    static button_state current_state = INIT;
    static unsigned char is_button_pressed;
    static unsigned char const ENABLE_BUTTON_PIN_IDX = 0;
    is_button_pressed = is_pressed(ENABLE_BUTTON_PIN_IDX, 
                                   PINA);
    current_state = change_button_state(current_state, 
                                        is_button_pressed);
    if(current_state == PRESS)
    {
        enable_output = toggle_bit(0, 
                                   enable_output);
    }
}

void button_up()
{
    static button_state current_state = INIT;
    static unsigned char is_button_pressed;
    static unsigned char const UP_BUTTON_PIN_IDX = 1;
    is_button_pressed = is_pressed(UP_BUTTON_PIN_IDX, 
                                   PINA);
    current_state = change_button_state(current_state, 
                                        is_button_pressed);
    if(current_state == PRESS)
    {
        if(tone_idx < TONE_FREQUENCIES_TERMINAL_IDX)
        {
            ++tone_idx;
        }
    }
}

void button_down()
{
    static button_state current_state = INIT;
    static unsigned char is_button_pressed;
    static unsigned char const DOWN_BUTTON_PIN_IDX = 2;
    is_button_pressed = is_pressed(DOWN_BUTTON_PIN_IDX, 
                                   PINA);
    current_state = change_button_state(current_state, 
                                        is_button_pressed);
    if(current_state == PRESS)
    {
        if(tone_idx > 0)
        {
            --tone_idx;
        }
    }
}

void tick()
{
    button_enable();
    button_up();
    button_down();
    if(enable_output)
    {
        set_PWM(tone_frequencies[tone_idx]);
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
    TimerSet(125);
    TimerOn();
    while(1)
    {
        tick();
    }
    return 1;
}
