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

unsigned char is_pressed(unsigned char pin, 
                         unsigned char src)
{
    return !get_bit(pin, src);
}
