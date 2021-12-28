#include "print.h"

#include <gb/gb.h>

void print_char_window(char c, uint8_t x, uint8_t y)
{
    if(c >= 'A' && c <= 'Z')
        c = c - 'A' + 0xA1;
    else if(c >= 'a' && c <= 'z')
        c = c - 'a' + 0xC1;
    else if(c >= '!' && c <= '@')
        c = c - '!' + 0x81;
    else if(c == ' ')
        c = 0;

    fill_win_rect(x, y, 1, 1, c);
}

/**
 * For internal operation
 */
static void _print_(const char *str, uint8_t x, uint8_t y, bool isWindow)
{
    char c;
    uint8_t i = 0;

    while(str[i] != 0)
    {
        c = str[i++];
        if(c >= 'A' && c <= 'Z')
            c = c - 'A' + 0xA1;
        else if(c >= 'a' && c <= 'z')
            c = c - 'a' + 0xC1;
        else if(c >= '!' && c <= '@')
            c = c - '!' + 0x81;
        else if(c == ' ')
            c = 0;
        
        if(c == '\n' || x >= 20)
            x = 0, y++;

        if(isWindow)
            fill_win_rect(x++, y, 1, 1, c);
        else
            fill_bkg_rect(x++, y, 1, 1, c);

    }

}


/**
 * Prints small text to the window. Each character is 3x4
 */
void print_small_window(const unsigned char *str, uint8_t tx, uint8_t ty)
{
    uint8_t t;
    while(*str != 0)
    {
        if(*str >= 'A' && *str <= 'Z')
            t = *str - 'A' + 93;
        else if(*str >= 'a' && *str <= 'z')
            t = *str - 'a' + 93;
        else if(*str >= '0' && *str <= '9')
            t = *str - '0' + 119;
        else
            t = 0;
        
        fill_win_rect(tx++, ty, 1, 1, t);
        str++;
    }
}


/**
 * Prints a string at (x, y). Checks for newlines
 * @param tx x coordinate of string
 * @param ty y coordinate of string
 */
inline void print(const unsigned char *str, uint8_t tx, uint8_t ty)
{
    _print_(str, tx, ty, false);
}


/**
 * Prints a string at (x, y). Checks for newlines
 * @param tx x coordinate of string
 * @param ty y coordinate of string
 */
inline void print_window(const unsigned char *str, uint8_t tx, uint8_t ty)
{
    _print_(str, tx, ty, true);
}


/**
 * Prints a 16-bit integer
 * @param num 16-bit unsigned integer
 * @param tx x tile to draw the number at
 * @param ty y tile to draw the number at
 * @param onWindow true to draw on window, or false to draw on bg
 */
void printInt(uint16_t num, uint8_t tx, uint8_t ty, const bool onWindow) {
    uint8_t digits[5];
    int8_t i = 0;
    
    // put all digits into a buffer
    do {
        digits[i++] = (num % 10) + FONT_BASE - FONT_START + '0';
        num /= 10;
    } while(num > 0);

    // work backwards in the buffer to place them onto the screen
    do {
        if(onWindow)
            set_win_tiles(tx++, ty, 1, 1, &digits[--i]);
        else
            set_bkg_tiles(tx++, ty, 1, 1, &digits[--i]);
    } while(i > 0);
}