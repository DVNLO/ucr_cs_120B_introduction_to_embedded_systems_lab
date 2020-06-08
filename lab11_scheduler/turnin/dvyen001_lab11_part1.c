/*	Author: Daniel Vyenielo
 *  	Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 11 Exercise 1
 *	Exercise Description:
 *	Demo:  https://drive.google.com/open?id=16b62txIbGlnQH7dalKaiXQaqblDMtCZ9
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
#include "keypad.h"

typedef struct 
{
    signed char state;
    unsigned long int period;
    unsigned long int elapsed_time;
    int (*start)(int);
}
task;

#define TASK_COUNT 1
static task tasks[TASK_COUNT];
static unsigned char const KEYPAD_TASK_ID = 0;
static unsigned long int KEYPAD_PERIOD = 1;

enum { KEYPAD_INIT, KEYPAD_SAMPLE } keypad_sampling_state;

int
keypad_handler(int const current_state)
{
    int next_state;
    unsigned char pressed_key;
    switch(current_state)
    {
        case KEYPAD_INIT:
            next_state = KEYPAD_SAMPLE;
            break;
        case KEYPAD_SAMPLE:
            next_state = KEYPAD_SAMPLE;
            break;
        default:
            next_state = KEYPAD_INIT;
    }
    switch(next_state)
    {
        case KEYPAD_INIT:
            break;
        case KEYPAD_SAMPLE:
            pressed_key = get_pressed_key();
            display_pressed_key(pressed_key);
            break;
    }
    return next_state;
}

void schedule(unsigned long int const SYSTEM_PERIOD)
// iterates through all tasks executing the i-th task 
// if its elapsed time is greater than or equal too its
// period. 
{
    static task * t;
    for(unsigned char i = 0; i < TASK_COUNT; ++i)
    {
        t = &tasks[i];
        if(t->elapsed_time >= t->period)
        {
            t->state = t->start(t->state);
            t->elapsed_time = 0;
        }
        else
        {
            t->elapsed_time += SYSTEM_PERIOD;
        }
    }
}

void initialize_ports()
// initializes the system ports
{
    initialize_port('B', DDR_OUTPUT, INIT_VAL_OUTPUT);
    initialize_port('C', DDR_KEYPAD, INIT_VAL_KEYPAD);
}

void initialize_task(unsigned char const task_id,
                     signed char const start_state,
                     unsigned long int period,
                     unsigned long int elapsed_time,
                     int (*start)(int))
{
    task * t = &tasks[task_id];
    t->state = start_state;;
    t->period = period;
    t->elapsed_time = elapsed_time;
    t->start = start;
}

unsigned long int
get_system_period()
// returns the system period. The system period is the 
// greatest common divisor of all task periods assuming
// there exists atleast one task. 
{
    unsigned long int ret = tasks[0].period;
    for(unsigned char i = 1; i < TASK_COUNT; ++i)
    {
        ret = greatest_common_divisor(ret, 
                                      tasks[i].period);
    }
    return ret;
}

void
initialize_tasks()
// initializes the system tasks
{
    initialize_task(KEYPAD_TASK_ID, 
                    KEYPAD_INIT,
                    KEYPAD_PERIOD,
                    KEYPAD_PERIOD,
                    keypad_handler);    // initialize the keypad task 
}

int 
main(void) 
{
    initialize_ports();
    initialize_tasks();
    unsigned long int const SYSTEM_PERIOD = get_system_period();
    TimerSet(SYSTEM_PERIOD);
    TimerOn();
    while(1)
    {
        schedule(SYSTEM_PERIOD);
        while(!TimerFlag)
        {
            continue;
        }
        TimerFlag = 0x00;
    }
    return 1;
}
