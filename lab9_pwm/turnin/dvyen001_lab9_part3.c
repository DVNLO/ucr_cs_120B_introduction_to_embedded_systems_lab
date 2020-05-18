/*	Author: Daniel Vyenielo
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 9 Exercise 3
 *	Exercise Description:
 *	Demo : https://drive.google.com/file/d/14kcN21iCc1U2ct4Wv1AYn21mXTHwPrgX/view?usp=sharing
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

unsigned char play = 0x00;

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

unsigned char notes[] = { 1, 2, 3, 4, 5, 6, 7, 7,
                          2, 3, 4, 5, 1, 2, 3, 5,
                          0, 1, 2, 3, 1, 0, 4, 7,
                          1, 3, 5, 6, 4, 2, 3, 1,
                          1, 1, 1, 1, 1, 2, 2, 2 };
unsigned char NOTES_SIZE = 40;
unsigned char note_idx = 0;

enum { WAIT, PLAY } speaker;

void speaker_out()
{
    switch(speaker)
    {
        case WAIT:
            if(play)
            {
                speaker = PLAY;
                note_idx = 0;
            }
            break;
        case PLAY:
            if(note_idx >= NOTES_SIZE)
            {
                speaker = WAIT;
            }
            break;
    }
    switch(speaker)
    {
        case WAIT:
            {
                set_PWM(STOP);
            }
            break;
        case PLAY:
            {
                set_PWM(tone_frequencies[notes[note_idx]]);
                ++note_idx;
            }
            break;
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
