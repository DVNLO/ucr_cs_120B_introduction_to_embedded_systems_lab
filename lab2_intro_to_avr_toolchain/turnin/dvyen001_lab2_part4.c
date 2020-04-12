/*	Author: Daniel Vyenielo
 *      Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 2  Exercise 4 
 *	Exercise Description:
 *  in: 8-bit weight on A, B, C (0-255kg)
 *  out: PD0 = 1 if total weight exceeds 140kg
 *       PD1 = 1 if absolute difference on A and C exceeds 80kg
         PD[7:2] = approx weight in cart. True weight / 4
 *  
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char get_bit(unsigned char const i, 
                      unsigned char const src)
// Returns the i-th bit value found in src.
{
    unsigned char ret = src;
    ret >>= i;
    ret &= 0x01;
    return ret;
}

unsigned char set_bit(unsigned char const i, 
                      unsigned char const src)
// Sets the i-th bit in src, and returns the result, ret.
{
    unsigned char ret = src;
    ret |= (0x01 << i);
    return ret;
}

unsigned char clear_bit(unsigned char const i,
                        unsigned char const src)
// Clears the i-th bit in src and returns the result, ret.
{
    unsigned char ret = src;
    ret &= ~(0x01 << i);
    return ret;
}

unsigned char toggle_bit(unsigned char const i,
                         unsigned char const src)
// Toggles the i-th bit in src and returns the result, ret.
{
    unsigned char ret = src;
    ret ^= (0x01 << i);
    return ret;
}

void initialize_port(unsigned char const port,
                     unsigned char const ddr_val,
                     unsigned char const init_val)
// Initializes port, port. Sets port's data direction register to a data
// direction value, ddr_val, and assigns an initial value, init_val, to the 
// port.
{
    switch(port)
    {
        case 'A':
            DDRA = ddr_val;
            PORTA = init_val;
            break;
        case 'B':
            DDRB = ddr_val;
            PORTB = init_val;
            break;
        case 'C':
            DDRC = ddr_val;
            PORTC = init_val;
            break;
        case 'D':
            DDRD = ddr_val;
            PORTD = init_val;
            break;
        default:
            break;
    }
}

unsigned char min(unsigned char const lhs, 
                  unsigned char const rhs)
{
    return lhs < rhs ? lhs : rhs;
}

unsigned char max(unsigned char const lhs,
                  unsigned char const rhs)
{
    return lhs > rhs ? lhs : rhs;
}

int main(void) 
{
    unsigned char const DDR_INPUT = 0x00;
    unsigned char const INIT_VAL_INPUT = 0xFF;
    unsigned char const DDR_OUTPUT = 0xFF;
    unsigned char const INIT_VAL_OUTPUT = 0x00;
    initialize_port('A', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('B', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('C', DDR_INPUT, INIT_VAL_INPUT);
    initialize_port('D', DDR_OUTPUT, INIT_VAL_OUTPUT); 
    unsigned char PORT_D_TMP;
    unsigned short cart_weight;
    while (1) 
    {
        PORT_D_TMP = 0x00;
        cart_weight = (unsigned short)(PINA);
        cart_weight += (unsigned short)(PINB);
        cart_weight += (unsigned short)(PINC);
		if(cart_weight > UCHAR_MAX)
			PORT_D_TMP = UCHAR_MAX;
		else
			PORT_D_TMP = (unsigned char)(cart_weight);
        if(PORT_D_TMP > 140)
            PORT_D_TMP = set_bit(0, PORT_D_TMP);
        else
            PORT_D_TMP = clear_bit(0, PORT_D_TMP);
        if(max(PINA, PINC) - min(PINA, PINC) > 80)
            PORT_D_TMP = set_bit(1, PORT_D_TMP);
        else
            PORT_D_TMP = clear_bit(1, PORT_D_TMP);
        PORTD = PORT_D_TMP;
    }
    return 1;
}
