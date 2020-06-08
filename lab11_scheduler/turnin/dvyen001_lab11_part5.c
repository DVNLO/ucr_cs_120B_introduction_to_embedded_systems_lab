/*	Author: Daniel Vyenielo
 *  	Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 11 Exercise 5
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
#include "lcd.h"
#include "queue.h"

typedef struct 
{
    signed char state;
    unsigned long int period;
    unsigned long int elapsed_time;
    int (*start)(int);
}
task;

#ifndef TASK_COUNT
#define TASK_COUNT 4
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
static unsigned char const UP_SIGNAL = 'U';
#ifndef UP_BUTTON_PIN
#define UP_BUTTON_PIN 1
#endif 

//TASK DOWN_BUTTON CONFIG
static unsigned char const TASK_DOWN_BUTTON_ID = 2;
static unsigned long int const TASK_DOWN_BUTTON_PERIOD = 50;    // miliseconds
static unsigned char const DOWN_SIGNAL = 'D';
#ifndef DOWN_BUTTON_PIN
#define DOWN_BUTTON_PIN 2
#endif

// CONTROL QUEUE CONFIG
static Queue control_queue;
#ifndef CONTROL_QUEUE_SIZE
#define CONTROL_QUEUE_SIZE 32
#endif

// TASK GAME CONFIG
static unsigned char const TASK_GAME_ID = 3;
static unsigned long int const TASK_GAME_PERIOD = 500;
enum { GAME_INIT, GAME_RESET, GAME_WAIT, GAME_PLAY, GAME_OVER };
static unsigned char GAME_OVER_SIGNAL = 'G';

int
start_stop_button_handler(int const current_state)
{
    unsigned char const is_button_pressed = is_pressed(START_STOP_BUTTON_PIN, 
                                                       PINA);
    button_state next_state = change_button_state(current_state,
                                                  is_button_pressed);
    if(next_state == BUTTON_PRESS)
    {
       QueueEnqueue(control_queue, START_STOP_SIGNAL); 
    }
    return next_state;
}

int
up_button_handler(int const current_state)
{
    unsigned char const is_button_pressed = is_pressed(UP_BUTTON_PIN, 
                                                       PINA);
    button_state next_state = change_button_state(current_state,
                                                  is_button_pressed);
    if(next_state == BUTTON_PRESS)
    {
       QueueEnqueue(control_queue, UP_SIGNAL); 
    }
    return next_state;
}

int 
down_button_handler(int const current_state)
{
    unsigned char const is_button_pressed = is_pressed(DOWN_BUTTON_PIN, 
                                                       PINA);
    button_state next_state = change_button_state(current_state,
                                                  is_button_pressed);
    if(next_state == BUTTON_PRESS)
    {
       QueueEnqueue(control_queue, DOWN_SIGNAL); 
    }
    return next_state;
}

typedef struct
{
    unsigned char r;
    unsigned char c;
} 
actor;

#ifndef OBSTACLE_COUNT
#define OBSTACLE_COUNT 2
#endif

unsigned char
queue_contains(Queue q, 
               unsigned char const size, 
               unsigned char const val)
// returns non-zero if the Queue q containing size
// values contains val. Assumes there are size values
// in the queue.
{
    unsigned char found = 0x00;
    if(!size)
        return found;
    unsigned char tmp = ~val;
    for(unsigned char i = 0; i < size; ++i)
    {
        tmp = QueueDequeue(q);
        if(!found && tmp == val)
            found = !found;
        QueueEnqueue(q, tmp);
    }
    return found;
}

void
queue_clear(Queue q)
{
    QueueMakeEmpty(q);
}

unsigned char
queue_size(Queue q)
{
    return QueueSize(q);
}

unsigned char 
queue_is_empty(Queue q)
{
    return QueueIsEmpty(q);
}

static actor player;
static actor obstacles[OBSTACLE_COUNT];

void
initialize_player()
{
    player.r = 0;
    player.c = 4;
}

void
update_player(unsigned char const r,
              unsigned char const c)
{
    player.r = r;
    player.c = c;
    lcd_set_cursor_position(r, c);
}

void
initialize_obstacles()
{
    obstacles[0].r = 0;
    obstacles[0].c = 10;
    obstacles[1].r = 1;
    obstacles[1].c = 14;         
}

void
update_obstacle(unsigned char const obstacle_idx,
                unsigned char r,
                unsigned char c)
{
    if(r > 1)
        return;
    if(c > LCD_COL_COUNT)
        c = LCD_COL_COUNT - 1;
    lcd_set_cursor_position(obstacles[obstacle_idx].r, 
                            obstacles[obstacle_idx].c);
    lcd_write_char(' ');
    obstacles[obstacle_idx].r = r;
    obstacles[obstacle_idx].c = c;
    lcd_set_cursor_position(obstacles[obstacle_idx].r, 
                            obstacles[obstacle_idx].c);
    lcd_write_char('#');
}


int
game_handler(int const current_state)
{
    static unsigned char game_over_msg[] = "Game Over!";
    static unsigned char game_over_msg_size = sizeof(game_over_msg) / sizeof(game_over_msg[0]) - 1;
    int next_state;
    unsigned char request;
    switch(current_state)
    {
        case GAME_INIT:
            next_state = GAME_RESET;
            break;
        case GAME_RESET:
            next_state = GAME_WAIT;
            break;
        case GAME_WAIT:
            if(queue_contains(control_queue, 
                              queue_size(control_queue), 
                              START_STOP_SIGNAL))
            {
                next_state = GAME_PLAY;
            }
            else
            {
                next_state = GAME_WAIT;
            }
            break;
        case GAME_PLAY:
            if(queue_contains(control_queue, 
                              queue_size(control_queue), 
                              START_STOP_SIGNAL))
            {
                next_state = GAME_RESET;
            }
            else if(queue_contains(control_queue,
                                   queue_size(control_queue),
                                   GAME_OVER_SIGNAL))
            {
                next_state = GAME_OVER;
                lcd_clear_screen();
                lcd_write_chars(game_over_msg, 
                                game_over_msg + game_over_msg_size);
            }
            else
            {
                next_state = GAME_PLAY;
            }
            break;
        case GAME_OVER:
            if(queue_contains(control_queue,
                              queue_size(control_queue),
                              START_STOP_SIGNAL))
            {
                next_state = GAME_RESET;
            }
            else
                next_state = GAME_OVER;
            break;
        default:
            next_state = GAME_INIT;
    }
    switch(next_state)
    {
        case GAME_INIT:
            break;
        case GAME_RESET:
            lcd_clear_screen();
            initialize_player();
            initialize_obstacles();
            update_obstacle(0, obstacles[0].r, obstacles[0].c);
            update_obstacle(1, obstacles[1].r, obstacles[1].c);
            update_player(player.r, player.c);
            queue_clear(control_queue);
            break;
        case GAME_WAIT:
            queue_clear(control_queue);
            break;
        case GAME_PLAY:
            update_obstacle(0, obstacles[0].r, obstacles[0].c - 1);
            update_obstacle(1, obstacles[1].r, obstacles[1].c - 1);
            while(!queue_is_empty(control_queue))
            {
                request = QueueDequeue(control_queue);
                if(request == UP_SIGNAL)
                    update_player(0, player.c);
                else if(request == DOWN_SIGNAL)
                    update_player(1, player.c);
            }
            update_player(player.r, player.c);
            if((player.r == obstacles[0].r && player.c == obstacles[0].c)
                || (player.r == obstacles[1].r && player.c == obstacles[1].c))
            {
                QueueEnqueue(control_queue, GAME_OVER_SIGNAL);
            }
            break;
        case GAME_OVER:
            queue_clear(control_queue);
            break;
    }
    return next_state;
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
                    up_button_handler); // initialize the up button task
    initialize_task(TASK_DOWN_BUTTON_ID,
                    BUTTON_INIT,
                    TASK_DOWN_BUTTON_PERIOD,
                    TASK_DOWN_BUTTON_PERIOD,
                    down_button_handler);   // initialize the down button task
    initialize_task(TASK_GAME_ID,
                    GAME_INIT,
                    TASK_GAME_PERIOD,
                    TASK_GAME_PERIOD,
                    game_handler); // initialize the game task
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
    control_queue = QueueInit(CONTROL_QUEUE_SIZE);
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
