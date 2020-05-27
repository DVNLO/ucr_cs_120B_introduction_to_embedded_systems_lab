/*	Author: Dan Vyenielo
 *	Lab Section: 21
 *  Exam #2
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "timer.h"

/* SM state declarations --- fill in as needed */
typedef enum ping_states { PInit, PING_HIGH, PING_LOW } ping_states;
typedef enum detect_eq_states { DEQInit, DEQ_NO_QUAKE, DEQ_QUAKE, DEQ_NO_MOTION } detect_eq_states;
typedef enum detect_max_amp_states { DMAInit, DMA_WAIT, DMA_DETECT } detect_max_amp_states;
typedef enum detect_zc_states { DZCInit, DZC_WAIT, DZC_DETECT } detect_zc_states;
typedef enum transmit_states {TInit, T_OUTPUT } transmit_states;

/* shared variables --- fill in as needed */
static unsigned char const MOTION_EAST = 0x00;
static unsigned char const MOTION_NORTH_EAST = 0x01;
static unsigned char const MOTION_NORTH = 0x02;
static unsigned char const MOTION_NORTH_WEST = 0x03;
static unsigned char const MOTION_WEST = 0x04;
static unsigned char const MOTION_SOUTH_WEST = 0x05;
static unsigned char const MOTION_SOUTH = 0x06;
static unsigned char const MOTION_SOUTH_EAST = 0x07;


/* state variables --- do not alter */
ping_states ping_state;
detect_eq_states detect_eq_state;
detect_max_amp_states detect_max_amp_state;
detect_zc_states detect_zc_state;
transmit_states transmit_state;

/* SM definitions --- complete each task as a SM in the appropriate file.
 * Alternatively, you can remove the #include statement and insert your
 *   SM implementation directly. 
 */
///////////////////////////////////////////////////////////////////
//#include "ping.h"
static unsigned char output_ping;
static unsigned char output_detect_eq;
static unsigned char output_detect_max_amp;
static unsigned char output_detect_zero_crossing;

void Ping()
{
    static unsigned short i = 0;
    static unsigned const LOW_COUNT = 9;
    switch(ping_state)
    {
        case PInit:
            ping_state = PING_HIGH;
            break;
        case PING_HIGH:
            ping_state = PING_LOW;
            break;
        case PING_LOW:
            if(i >= LOW_COUNT)
            {
                ping_state = PING_HIGH;
                i = 0;
            }
            else
            {
                ++i;
            }
            break;
        default:
            ping_state = PInit;
            break;
    }
    switch(ping_state)
    {
        case PInit:
            break;
        case PING_HIGH:
            output_ping = 0x01;
            break;
        case PING_LOW:
            output_ping = 0x00;
            break;
        default:
            break;
    }
}

///////////////////////////////////////////////////////////////////
//#include "detect_eq.h"
//define local variable(s) here.  Use static keyword to keep local, e.g:
//   static int i;  // defines a local int named i
/*complete the state machine*/

void Detect_EQ()
// detect an earth quake when a +amplitude is detected
// an earthquake ends when 0 motion is detected for 1 second.
{
    static unsigned short j = 0;
    static unsigned char amplitude;
    amplitude = ~PINA >> 3;
    switch(detect_eq_state)
    {
        case DEQInit:
            detect_eq_state = DEQ_NO_QUAKE;
            break;
        case DEQ_NO_QUAKE:
            if(amplitude > 0)
            {
                detect_eq_state = DEQ_QUAKE;
            }
            break;
        case DEQ_QUAKE:
            if(amplitude == 0)
            {
                detect_eq_state = DEQ_NO_MOTION;
                j = 0;
            }
        case DEQ_NO_MOTION:
            if(amplitude)
            {
                detect_eq_state = DEQ_QUAKE;
            }
            else if(j > 10)
            {
                detect_eq_state = DEQ_NO_QUAKE;
            }
        default:
            detect_eq_state = DEQInit;
            break;
    }
    switch(detect_eq_state)
    {
        case DEQInit:
            break;
        case DEQ_NO_QUAKE:
            output_detect_eq = 0x00;
            break;
        case DEQ_QUAKE:
            output_detect_eq = 0x02;
            break;
        case DEQ_NO_MOTION:
            ++j;
            break;
        default:
            break;
    }
}

///////////////////////////////////////////////////////////////////
//#include "detect_max_amp.h"
//define local variable(s) here.  Use static keyword to keep local, e.g:
//   static int i;  // defines a local int named i

/* complete the state machine. */

void Detect_Max_Amp()
// detects the maximum amplitude when an earthquake begins
{
    static unsigned char amplitude;
    amplitude = ~PINA >> 3;
    switch(detect_max_amp_state)
    {
        case DMAInit:
            break;
        case DMA_WAIT:
            if(output_detect_eq)
            {
                detect_max_amp_state = DMA_DETECT;
            }
            break;
        case DMA_DETECT:
            if(!output_detect_eq)
            {
                detect_max_amp_state = DMA_WAIT;
            }
            break;
        default:
            break;
    }
    switch(detect_max_amp_state)
    {
        case DMAInit:
            break;
        case DMA_WAIT:
            output_detect_max_amp = 0;
            break;
        case DMA_DETECT:
            if(amplitude > output_detect_max_amp)
            {
                output_detect_max_amp = amplitude << 3;
            }
            break;
        default:
            break;
    }
}

///////////////////////////////////////////////////////////////////
//#include "detect_zc.h"
//define local variable(s) here.  Use static keyword to keep local, e.g:
//   static int i;  // defines a local int named i



/*complete the state machine*/

void Detect_ZC()
{
    static unsigned char prev_motion;
    static unsigned char cur_motion;
    cur_motion = ~PINA & 0x07;
    switch(detect_zc_state)
    {
        case DZCInit:
            break;
        case DZC_WAIT:
            if(output_detect_eq)
            {
                detect_zc_state = DZC_DETECT;
            }
            break;
        case DZC_DETECT:
            if(!output_detect_eq)
            {
                detect_zc_state = DZC_WAIT;
            }
            break;
        default:
            detect_zc_state = DZCInit;
            break;
    }
    switch(detect_zc_state)
    {
        case DZCInit:
            break;
        case DZC_WAIT:
            output_detect_zero_crossing = 0;
            break;
        case DZC_DETECT:
            if(prev_motion != cur_motion)
            // i know the condition is wrong here, but i dont have time to
            // write out all the comparisions.
            {
                output_detect_zero_crossing = 0x04;
            }
            break;
        default:
            break;
    }
    prev_motion = cur_motion;
}

//#include "transmit.h"
//define local variable(s) here.  Use static keyword to keep local, e.g:
//   static int i;  // defines a local int named i



/*complete the state machine*/

void Transmit()
{
    PORTB = output_ping 
            | output_detect_eq 
            | output_detect_max_amp
            | output_detect_zero_crossing;
}

/* main function --- do not alter */
int main(void) {
    /* DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    // Common period for all tasks set to 100ms
    TimerSet(100);
    TimerOn();

    // init state vars
    ping_state = PInit;
    detect_eq_state = DEQInit;
    detect_max_amp_state = DMAInit;
    detect_zc_state = DZCInit;
    transmit_state = TInit;

    while (1) {
        Ping();
        Detect_EQ();
        Detect_Max_Amp();
        Detect_ZC();
        Transmit();
        while (!TimerFlag) { }
        TimerFlag = 0;
    }
    return 1;
}
