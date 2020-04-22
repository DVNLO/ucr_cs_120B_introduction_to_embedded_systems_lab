/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "avr_configuration.h"
#include "bit_manipulation.h"
#include "utilities.h"

void tick()
// input: fuel level sensor on [PA3:PA0] with values in 
// binary from 0 to 15 (full).
// output: LEDS on [PC5:PC0] light to indicate fuel level.
{
    static unsigned char output[] = { 0x40,
                                      0x60, 0x60,
                                      0x70, 0x70,
                                      0x38, 0x38,
                                      0x3C, 0x3C, 0x3C,
                                      0x3E, 0x3E, 0x3E,
                                      0x3F, 0x3F, 0x3F };
    PORTC = output[~PINA & 0x0F];
}

int main(void) {
    initialize_port('A', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('C', DDR_OUTPUT, INIT_VAL_OUTPUT);
    while (1) 
    {
        tick();
    }
    return 1;
}
