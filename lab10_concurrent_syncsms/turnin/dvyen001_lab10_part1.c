/*	Author: Daniel Vyenielo
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 10 Exercise 1
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
#include "button.h"
#include "io.h"
#include "music.h"
#include "pwm.h"
#include "timer.h"
#include "utilities.h"

typedef enum three_leds_states{ TLS_INIT, TLS0, TLS1, TLS2 } three_leds_state;
typedef enum blink_led_states{ BLS_INIT, BLS0, BLS1 } blink_led_state;

three_leds_state 
three_leds_transition(three_leds_state const current_state)
{
    three_leds_state next_state;
    switch(current_state)
    {
        case TLS_INIT:
            next_state = TLS0;
            break;
        case TLS0:
            next_state = TLS1;
            break;
        case TLS1:
            next_state = TLS2;
            break;
        case TLS2:
            next_state = TLS0;
            break;
        default:
            next_state = TLS_INIT;
            break;
    }
    return next_state;
}

unsigned char
three_leds_action(three_leds_state const current_state)
{
    unsigned char output;
    switch(current_state)
    {
        case TLS0:
            output = 0x01;
            break;
        case TLS1:
            output = 0x02;
            break;
        case TLS2:
            output = 0x04;
            break;
        default:
            output = 0xFF; // error
            break;
    }
    return output;
}

unsigned char
three_leds()
{
    static unsigned short t = 0;
    static unsigned short const PERIOD = 1000;
    static three_leds_state current_state = TLS_INIT;
    static unsigned char current_output = 0xFF;
    if(t >= PERIOD)
    {
        current_state = three_leds_transition(current_state);
        current_output = three_leds_action(current_state);
        t = 0;
    }
    else
    {
        ++t;
    }
    return current_output;
}

blink_led_state
blink_led_transition(blink_led_state const current_state)
{
    blink_led_state next_state;
    switch(current_state)
    {
        case BLS_INIT:
            next_state = BLS0;
            break;
        case BLS0:
            next_state = BLS1;
            break;
        case BLS1:
            next_state = BLS0;
            break;
        default:
            next_state = BLS_INIT;
            break;
    }
    return next_state;
}

unsigned char
blink_led_action(blink_led_state const current_state)
{
    unsigned char output;
    switch(current_state)
    {
        case BLS0:
            output = 0x08;
            break;
        case BLS1:
            output = 0x00;
            break;
        default:
            output = 0xFF;
            break;
    }
    return output;
}

unsigned char
blink_led()
{
    static unsigned short t = 0;
    static unsigned short const PERIOD = 1000;
    static blink_led_state current_state = BLS_INIT;
    static unsigned char current_output = 0xFF;
    if(t >= PERIOD)
    {
        current_state = blink_led_transition(current_state);
        current_output = blink_led_action(current_state);
        t = 0;
    }
    else
    {
        ++t;
    }
    return current_output;
}

unsigned char 
combine_leds(unsigned char const three_leds_output,
             unsigned char const blink_led_output)
{
    static unsigned short t = 0;
    static unsigned short const PERIOD = 1000;
    static unsigned char current_output = 0xFF;
    if(t >= PERIOD)
    {
        current_output = three_leds_output | blink_led_output;
        t = 0;
    }
    else
    {
        ++t;
    }
    return current_output;
}

void
update_output(unsigned char const next_output)
{
    static unsigned char current_output = 0xFF;
    if(current_output != next_output)
    {
        current_output = next_output;
        PORTB = current_output;
    }
}

void tick()
{
    static unsigned char three_leds_output;
    static unsigned char blink_led_output;
    static unsigned char combine_leds_output;
    three_leds_output = three_leds();
    blink_led_output = blink_led();
    combine_leds_output = combine_leds(three_leds_output, 
                                       blink_led_output);
    update_output(combine_leds_output);
}

int main(void) 
{
    initialize_port('A', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('B', DDR_OUTPUT, INIT_VAL_OUTPUT);
    TimerSet(1);
    TimerOn();
    while(1)
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
