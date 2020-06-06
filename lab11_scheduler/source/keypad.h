unsigned char const DDR_KEYPAD = 0xF0;
unsigned char const INIT_VAL_KEYPAD = 0x0F;

unsigned char 
get_pressed_key()
// returns the key pressed on a keypad connected to PORTC
{
    static unsigned char PINC_SAMPLE;
    PORTC = 0xEF;   // enable col 1 on keypad
    asm("nop");
    PINC_SAMPLE = PINC;
    if(is_pressed(0, PINC_SAMPLE))
    {
        return '1';
    }
    else if(is_pressed(1, PINC_SAMPLE))
    {
        return '4';
    }
    else if(is_pressed(2, PINC_SAMPLE))
    {
        return '7';
    }
    else if(is_pressed(3, PINC_SAMPLE))
    {
        return '*';
    }
    PORTC = 0xDF;   // enable col 2 on keypad
    asm("nop");
    PINC_SAMPLE = PINC;
    if(is_pressed(0, PINC_SAMPLE))
    {
        return '2';
    }
    else if(is_pressed(1, PINC_SAMPLE))
    {
        return '5';
    }
    else if(is_pressed(2, PINC_SAMPLE))
    {
        return '8';
    }
    else if(is_pressed(3, PINC_SAMPLE))
    {
        return '0';
    }
    PORTC = 0xBF;   // enable col 3 on keypad
    asm("nop");
    PINC_SAMPLE = PINC;
    if(is_pressed(0, PINC_SAMPLE))
    {
        return '3';
    }
    else if(is_pressed(1, PINC_SAMPLE))
    {
        return '6';
    }
    else if(is_pressed(2, PINC_SAMPLE))
    {
        return '9';
    }
    else if(is_pressed(3, PINC_SAMPLE))
    {
        return '#';
    }
    PORTC = 0x7F;   // enable col 4 on keypad
    asm("nop");
    PINC_SAMPLE = PINC;
    if(is_pressed(0, PINC_SAMPLE))
    {
        return 'A';
    }
    else if(is_pressed(1, PINC_SAMPLE))
    {
        return 'B';
    }
    else if(is_pressed(2, PINC_SAMPLE))
    {
        return 'C';
    }
    else if(is_pressed(3, PINC_SAMPLE))
    {
        return 'D';
    }
    return '\0';
}

void 
display_pressed_key(unsigned char val)
// displays the pressed key value on PORTB
{
    unsigned char output;
    switch(val)
    {
        case '\0':
            output = 0x1F;
            break;
        case '0':
            output = 0x00;
            break;
        case '1':
            output = 0x01;
            break;
        case '2':
            output = 0x02;
            break;
        case '3':
            output = 0x03;
            break;
        case '4':
            output = 0x04;
            break;
        case '5':
            output = 0x05;
            break;
        case '6':
            output = 0x06;
            break;
        case '7':
            output = 0x07;
            break;
        case '8':
            output = 0x08;
            break;
        case '9':
            output = 0x09;
            break;
        case 'A':
            output = 0x0A;
            break;
        case 'B':
            output = 0x0B;
            break;
        case 'C':
            output = 0x0C;
            break;
        case 'D':
            output = 0x0D;
            break;
        case '*':
            output = 0x0E;
            break;
        case '#':
            output = 0x0F;
            break;
        default:
            output = 0x1B;
            break;
    }
    // mask the output on PORTB to prevent 
    // control interference with the LCD display.
    PORTB = (PORTB & 0xE0) | (output & 0x1F);
}

