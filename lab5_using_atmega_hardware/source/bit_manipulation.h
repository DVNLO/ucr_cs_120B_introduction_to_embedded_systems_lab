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
