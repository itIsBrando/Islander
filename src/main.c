#include <gb/gb.h>
#include <gb/console.h>
#include <gb/font.h>

#include <stdio.h>
#include <string.h>
#include <rand.h>

#include "structs.h"
#include "main.h"
#include "oam.h"
#include "print.h"
#include "joypad.h"
#include "gui/hud.h"
#include "entity/player.h"
#include "entity/particle.h"
#include "world/chunk.h"
#include "world/spawn.h"
#include "world/shake.h"



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

/**
 * Default interrupt handler
 */
void vbl_main_loop() {
    if(main_counter++ == 0)
        spwn_do_tick();

    eff_update();

    shk_update();
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