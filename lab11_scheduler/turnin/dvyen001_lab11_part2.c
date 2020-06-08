/*	Author: Daniel Vyenielo
 *  	Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 11 Exercise 2
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

#ifndef TASK_COUNT
#define TASK_COUNT 2
#endif
#ifndef LCD_COL_COUNT
#define LCD_COL_COUNT 16
#endif
static task tasks[TASK_COUNT];
static unsigned char const TASK_KEYPAD_ID = 0;
static unsigned long int const TASK_KEYPAD_PERIOD = 250;

static unsigned char const TASK_LCD_ID = 1;
static unsigned long int const TASK_LCD_PERIOD = 250;
#ifndef LCD_COL_COUNT
#define LCD_COL_COUNT 16
#endif
#ifndef LCD_ROW_COUNT
#define LCD_ROW_COUNT 2
#endif
static unsigned char const LCD_POS_OFFSET = 1;
 
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

unsigned char  
lcd_get_cursor_position(unsigned char const row_idx,
                        unsigned char const row_size,
                        unsigned char const col_idx,
                        unsigned char const pos_offset)
// translats row_idx and col_idx to an lcd cursor position.
{
    return (row_size * row_idx) + (col_idx + pos_offset);
}

void
lcd_set_cursor_position(unsigned char const row_idx,
                        unsigned char const col_idx)
// sets the cursor position for the LCD used in this lab.
{
    unsigned char cursor_position = lcd_get_cursor_position(row_idx,
                                                            LCD_COL_COUNT,
                                                            col_idx,
                                                            LCD_POS_OFFSET);
    LCD_Cursor(cursor_position);
}

enum { LCD_INIT, LCD_RESET, LCD_MSG_START, LCD_MSG_MID, LCD_MSG_END } lcd_update_state;

void
lcd_clear_screen()
{
    LCD_ClearScreen();
}

void
lcd_write_char(unsigned char const val)
{
    LCD_WriteData(val);
    delay_ms(1);
}

void 
lcd_write_chars(unsigned char const * const l,
                unsigned char const * const r)
{
    unsigned char const * i = l;
    while(i < r)
    {
        LCD_WriteData(*i);
        ++i;
        delay_ms(1);
    }
}

int 
lcd_handler(int const current_state)
{
    static unsigned char msg[] = "CS120B is Legend... wait for it DARY!";
    static unsigned char * l;   // beginning of msg substr, inclusive
    static unsigned char * r;   // ending of the msg substr, exclusive
    int next_state;
    switch(current_state)
    {
        case LCD_INIT:
            next_state = LCD_RESET;
            break;
        case LCD_RESET:
            next_state = LCD_MSG_START;
            break;
        case LCD_MSG_START:
            if(r - l < LCD_COL_COUNT)
                next_state = LCD_MSG_START;
            else
                next_state = LCD_MSG_MID;
            break;
        case LCD_MSG_MID:
            if(*r)
                next_state = LCD_MSG_MID;
            else
                next_state = LCD_MSG_END;
            break;
        case LCD_MSG_END:
            if(*l)
                next_state = LCD_MSG_END;
            else
                next_state = LCD_RESET;
            break;
        default:
            next_state = LCD_INIT;
    }
    switch(next_state)
    {
        case LCD_INIT:
            break;
        case LCD_RESET:
            l = msg;
            r = msg + 1;
            lcd_clear_screen();
            lcd_set_cursor_position(0, 2 * LCD_COL_COUNT);
            break;
        case LCD_MSG_START:
            lcd_set_cursor_position(0, LCD_COL_COUNT - (r - l));
            lcd_write_chars(l, r);
            ++r;
            break;
        case LCD_MSG_MID:
            lcd_set_cursor_position(0, 0);
            lcd_write_chars(l, r);
            ++l;
            ++r;
            break;
        case LCD_MSG_END:
            lcd_set_cursor_position(0, 0);
            lcd_write_chars(l, r);
            for(unsigned char i = r - l; i < LCD_COL_COUNT; ++i)
                lcd_write_char(' ');
            ++l;
            break;
        default:
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
    initialize_port('D', DDR_OUTPUT, INIT_VAL_OUTPUT);
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
    initialize_task(TASK_KEYPAD_ID, 
                    KEYPAD_INIT,
                    TASK_KEYPAD_PERIOD,
                    TASK_KEYPAD_PERIOD,
                    keypad_handler);    // initialize the keypad task 
    initialize_task(TASK_LCD_ID,
                    LCD_INIT,
                    TASK_LCD_PERIOD,
                    TASK_LCD_PERIOD,
                    lcd_handler);   // initialize the lcd task
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
