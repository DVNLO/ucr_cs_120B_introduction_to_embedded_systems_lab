unsigned char 
min(unsigned char const lhs, 
    unsigned char const rhs)
// returns the minimum of lhs and rhs values.
{
    return lhs < rhs ? lhs : rhs;
}

unsigned char 
max(unsigned char const lhs,
    unsigned char const rhs)
// returns the maximum of lhs and rhs values.
{
    return lhs > rhs ? lhs : rhs;
}

unsigned char 
is_odd(unsigned char const val)
// returns a non-zero value if val is odd. Recall that an 
// odd value has a one in the least significant binary digit.
{
    return val & 0x01;
}

unsigned char 
is_even(unsigned char const val)
// returns a non-zero value if val is even. Recall
// that a an even value is not odd.
{
    return !is_odd(val);
}

void 
ranged_copy(unsigned char * begin,
            unsigned char * end,
            unsigned char const val)
// performs ranged copy of a value val into all elements
// between begin and end, where begin is inclusive and
// end is exclusive.
{
    unsigned char * itr = begin;
    while(itr < end)
    {
        *itr = val;
        ++itr;
    }
}
