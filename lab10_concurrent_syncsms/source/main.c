/*	Author: Daniel Vyenielo
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 9 Exercise 3
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


void button_play()
{
    static button_state current_state = INIT;
    static unsigned char is_button_pressed;
    static unsigned char const PLAY_BUTTON_PIN_IDX = 0;
    is_button_pressed = is_pressed(PLAY_BUTTON_PIN_IDX, 
                                   PINA);
    current_state = change_button_state(current_state, 
                                        is_button_pressed);
    if(current_state == PRESS)
    {
        play = set_bit(0, play);
    }
    else if(current_state == PRESSED 
            || current_state == RELEASE)
    {
        play = clear_bit(0, play);
    }
}


void tick()
{
    button_play();
    speaker_out();
}

int main(void) 
{
    initialize_port('A', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('B', DDR_OUTPUT, INIT_VAL_OUTPUT);
    PWM_on();
    TimerSet(125);
    TimerOn();
    speaker = WAIT;
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
