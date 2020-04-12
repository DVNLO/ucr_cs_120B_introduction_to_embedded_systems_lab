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

