/*	Author: Daniel Vyenielo
 *  	Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 11 Exercise 5
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
#include "keypad.h"
#include "lcd.h"

typedef struct 
{
    signed char state;
    unsigned long int period;
    unsigned long int elapsed_time;
    int (*start)(int);
}
task;

#ifndef TASK_COUNT
#define TASK_COUNT 3
#endif
static task tasks[TASK_COUNT];

//TASK START_STOP_BUTTON CONFIG
static unsigned char const TASK_START_STOP_BUTTON_ID = 0;
static unsigned long int const TASK_START_STOP_BUTTON_PERIOD = 50;  // miliseconds
static unsigned char const START_STOP_SIGNAL = 'S';
#ifndef START_STOP_BUTTON_PIN
#define START_STOP_BUTTON_PIN 0
#endif

//TASK UP_BUTTON CONFIG
static unsigned char const TASK_UP_BUTTON_ID = 1;
static unsigned long int const TASK_UP_BUTTON_PERIOD = 50;  // miliseconds
static unsigned char const UP_BUTTON_SIGNAL = 'U';
#ifndef UP_BUTTON_PIN
#define UP_BUTTON_PIN 1
#endif 

//TASK DOWN_BUTTON CONFIG
static unsigned char const TASK_DOWN_BUTTON_ID = 2;
static unsigned long int const TASK_DOWN_BUTTON_PERIOD = 50;    // miliseconds
static unsigned char const DOWN_BUTTON_SIGNAL = 'D';
#ifndef DOWN_BUTTON_PIN
#define DOWN_BUTTON_PIN 2
#endif

int
start_stop_button_handler(int const current_state)
{
    unsigned char const is_button_pressed = is_pressed(START_STOP_BUTTON_PIN, 
                                                       PINA);
    button_state next_state = change_button_state(current_state,
                                                  is_button_pressed);
    if(next_state == BUTTON_PRESS)
    {
        lcd_write_char(START_STOP_SIGNAL);
    }
    return next_state;
}

int
up_button_handler(int const current_state)
{
    return 0;
}

int 
down_button_handler(int const current_state)
{
    return 0;
}

void 
schedule(unsigned long int const SYSTEM_PERIOD)
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

void 
initialize_ports()
// initializes the system ports
{
    initialize_port('A', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('B', DDR_OUTPUT, INIT_VAL_OUTPUT);
    initialize_port('C', DDR_KEYPAD, INIT_VAL_KEYPAD);
    initialize_port('D', DDR_OUTPUT, INIT_VAL_OUTPUT);
}

void 
initialize_task(unsigned char const task_id,
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
// initializes system tasks
{
    initialize_task(TASK_START_STOP_BUTTON_ID,
                    BUTTON_INIT,
                    TASK_START_STOP_BUTTON_PERIOD,
                    TASK_START_STOP_BUTTON_PERIOD,
                    start_stop_button_handler);   // initialize the start stop button task
    initialize_task(TASK_UP_BUTTON_ID,
                    BUTTON_INIT,
                    TASK_UP_BUTTON_PERIOD,
                    TASK_UP_BUTTON_PERIOD,
                    up_button_handler);
    initialize_task(TASK_DOWN_BUTTON_ID,
                    BUTTON_INIT,
                    TASK_DOWN_BUTTON_PERIOD,
                    TASK_DOWN_BUTTON_PERIOD,
                    down_button_handler);
}

int 
main(void) 
{
    initialize_ports();
    initialize_tasks();
    unsigned long int const SYSTEM_PERIOD = get_system_period();
    TimerSet(SYSTEM_PERIOD);
    TimerOn();
    LCD_init();
    LCD_ClearScreen();
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
