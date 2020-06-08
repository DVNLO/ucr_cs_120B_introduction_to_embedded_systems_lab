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

void
swap(unsigned long int * const lhs, 
     unsigned long int * const rhs)
// swaps the values found at lhs and rhs
// assuming lhs and rhs are not nullptr.
{
    unsigned long int t = *lhs;
    *lhs = *rhs;
    *rhs = t;
}

unsigned long int
greatest_common_divisor(unsigned long int const a,
                        unsigned long int const b)
// returns the greatest common divisor between two
// numbers a and b. 
{
    unsigned long int x;
    unsigned long int y;
    unsigned long int z;
    if(a >= b)
    {
        x = a;
        y = b;
    }
    else
    {
        x = b;
        y = a;
    }
    while(y)
    {
        z = y;
        y = x % y;
        x = z;
    }
    return x;
}
