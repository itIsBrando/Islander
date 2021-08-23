#include <gb/gb.h>
#include <gb/console.h>
#include <gb/font.h>

#include <stdio.h>
#include <string.h>
#include <rand.h>

#include "structs.h"
#include "main.h"
#include "oam.h"
#include "gui/hud.h"
#include "entity/player.h"
#include "entity/particle.h"
#include "world/chunk.h"
#include "world/spawn.h"


void vbl_main_loop();
uint8_t main_counter = 0;



void main(void) {

    // load the font
    set_bkg_1bit_data(0x80, 102, font_ibm + 130, 3);

    // OBP0_REG = 0b10110100; // set other sprite palette
    set_sprite_data(0, 128, SPRITE_DATA);
    set_bkg_data(0, 128, SPRITE_DATA);

    STAT_REG = 0;
    SHOW_BKG; SHOW_SPRITES;
    OBP1_REG = 0b10110000; // set other sprite palette

    // set vblank interrupt
    set_interrupts(VBL_IFLAG);
    enable_interrupts();

    spr_flush();
    clear_bg();
    initGame();
}


/**
 * Starts the game
 */
void initGame() {
    move_bkg(0, 0);

    print("Generating world", 0, 0);
    
    for(u8 y = 0; y < 3; y++)
    {
        for(u8 x = 0; x < 3; x++)
            cnk_island_load(0, x, y);
    }

    cnk_init();
    plr_init();
    hud_init();

    cnk_draw_active();

    add_VBL(vbl_main_loop);

    do {
        uint8_t j = joypad();

        plr_update(j);
    
        wait_vbl_done();
    } while(true);
}


int8_t shake_x = 0;

/**
 * Default interrupt handler
 */
void vbl_main_loop() {
    if(main_counter++ == 0)
        spwn_do_tick();

    eff_update();

    if(shake_x) {
        int8_t off;
        if(shake_x < 0) {
            off = -1;
            shake_x++;
        } else {
            off = 1;
            shake_x--;
        }

        int8_t xOff = shake_x * 2 + off;
        scroll_bkg(xOff, 0);
        move_sprite(player.id, PLR_SCRN_X + shake_x, PLR_SCRN_Y);
        
        shake_x = -shake_x;
    }
}


/**
 * Screen shake
 * @param instensity value > 0
 */
inline void shake(uint8_t intensity)
{
    if(shake_x == 0)
    {
        shake_x = intensity;    
        SCX_REG -= intensity;
        move_sprite(player.id, PLR_SCRN_X - intensity, PLR_SCRN_Y);
    }
}

/**
 * Better alternative of `waitpad`. Power efficient. Waits until button(s) is/are released
 */
void waitjoypad(const uint8_t mask)
{
    while((joypad() & mask) != 0)
        wait_vbl_done();
}


/**
 * Waits until one or multiple buttons are pressed and released
 * @param mask button or buttons to wait for depression and releasing
 */
void waitPressed(const uint8_t mask)
{
    waitjoypad(mask);

    while((joypad() & mask) == 0)
        wait_vbl_done();

    waitjoypad(mask);
}


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
    const uint8_t len = strlen(str);
    char c;

    for(uint8_t i = 0; i < len; i++)
    {
        c = str[i];
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

// prints stuff to screen
inline void debug(char *s) {
    print(s, 0, 10);
}

/**
 * Empties all of the sprites in the shadow OAM
 */
void empty_oam() {
    memset(shadow_OAM, 0, 40 * 4);
}

/**
 * Clears the visible background
 */
void clear_bg() {
    fill_bkg_rect(0, 0, 20, 18, 0);
}


static const int8_t dx[] = {-1, 1, 0, 0};
static const int8_t dy[] = {0, 0, -1, 1};


inline int8_t dir_get_x(const direction_t dir)
{
    return dx[dir];
}


inline int8_t dir_get_y(const direction_t dir)
{
    return dy[dir];
}
