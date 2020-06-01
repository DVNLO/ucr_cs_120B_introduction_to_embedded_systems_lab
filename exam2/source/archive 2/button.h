typedef enum button_states{ BUTTON_INIT, 
                            BUTTON_RELEASED, 
                            BUTTON_PRESS, 
                            BUTTON_PRESSED, 
                            BUTTON_RELEASE } button_state;

button_state 
change_button_state(button_state const current_state,
                    unsigned char const is_pressed)
// returns the next button_state depending on the inputs
// current_state and is_pressed.
{
    button_state next_state;
    switch(current_state)
    {
        case BUTTON_INIT:
            next_state = BUTTON_RELEASED;
            break;
        case BUTTON_RELEASED:
            if(is_pressed)
                next_state = BUTTON_PRESS;
            else
                next_state = BUTTON_RELEASED;
            break;
        case BUTTON_PRESS:
            if(is_pressed)
                next_state = BUTTON_PRESSED;
            else
                next_state = BUTTON_RELEASE;
            break;
        case BUTTON_PRESSED:
            if(is_pressed)
                next_state = BUTTON_PRESSED;
            else
                next_state = BUTTON_RELEASE;
            break;
        case BUTTON_RELEASE:
            if(is_pressed)
                next_state = BUTTON_PRESS;
            else
                next_state = BUTTON_RELEASED;
            break;
        default:
            next_state = BUTTON_INIT;
            break;
    }
    return next_state;
}

unsigned char is_pressed(unsigned char pin, 
                         unsigned char src)
{
    return !get_bit(pin, src);
}
