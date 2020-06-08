#ifndef LCD_COL_COUNT
#define LCD_COL_COUNT 16
#endif
#ifndef LCD_ROW_COUNT
#define LCD_ROW_COUNT 2
#endif
#ifndef LCD_POS_OFFSET
#define LCD_POS_OFFSET 1
#endif


unsigned char  
lcd_get_cursor_position(unsigned char const row_idx,
                        unsigned char const row_size,
                        unsigned char const col_idx,
                        unsigned char const pos_offset)
// translats row_idx and col_idx to an lcd cursor position.
{
    return (row_size * row_idx) + (col_idx + pos_offset);
}

void
lcd_set_cursor_position(unsigned char const row_idx,
                        unsigned char const col_idx)
// sets the cursor position for the LCD used in this lab.
{
    unsigned char cursor_position = lcd_get_cursor_position(row_idx,
                                                            LCD_COL_COUNT,
                                                            col_idx,
                                                            LCD_POS_OFFSET);
    LCD_Cursor(cursor_position);
}

void
lcd_clear_screen()
{
    LCD_ClearScreen();
}

void
lcd_write_char(unsigned char const val)
{
    LCD_WriteData(val);
    delay_ms(1);
}

void 
lcd_write_chars(unsigned char const * const l,
                unsigned char const * const r)
{
    unsigned char const * i = l;
    while(i < r)
    {
        LCD_WriteData(*i);
        ++i;
        delay_ms(1);
    }
}

