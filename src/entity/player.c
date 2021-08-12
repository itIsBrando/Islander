#include <gb/gb.h>

#include "../main.h"
#include "player.h"
#include "../oam.h"
#include "../world/chunk.h"



player_t player;


uint8_t cur_id, cur_x, cur_y;

/**
 * Initializes player sprite and variables
 */
void plr_init()
{
    player.id = spr_allocate();

    move_sprite(player.id, PLR_SCRN_X, PLR_SCRN_Y);
    set_sprite_tile(player.id, 2);

    player.x = 1, player.y = 1;
    move_bkg(player.x, player.y);

    set_sprite_tile(cur_id = spr_allocate(), 17);
}


/**
 * Call every frame
 * @param j joypad
 */
void plr_update(const uint8_t j)
{
    printInt(player.x, 0, 0, false);
    printInt(player.y, 0, 1, false);

    if(j & J_UP) {
        if(((player.y - 1) & 63) == 0) {
            if(cnk_load_row(DIRECTION_UP))
                plr_move(DIRECTION_UP);

        } else {
            plr_move(DIRECTION_UP);
        }
    }

    if(j & J_DOWN) {
        if(((player.y + 1) & 63) == 0) {
            if(cnk_load_row(DIRECTION_DOWN))
                plr_move(DIRECTION_DOWN);

        } else {
            plr_move(DIRECTION_DOWN);
        }

    }

    if(j & J_LEFT) {
        if((player.x & 63) == 0)
            cnk_load_row(DIRECTION_LEFT);

        plr_move(DIRECTION_LEFT);
    }
    
    if(j & J_RIGHT) {
        if((player.x & 63) == 0)
            cnk_load_row(DIRECTION_RIGHT);

        plr_move(DIRECTION_RIGHT);
    }

    if(j & J_B)
    {
        cnk_draw_active();
        waitjoypad(J_B);
    }
}


/**
 * Moves the player
 */
void plr_move(const direction_t dir)
{
    const int8_t dx = dir_get_x(dir), dy = dir_get_y(dir);

    player.dir = dir;
    player.x += dx;
    player.y += dy;
    scroll_bkg(dx, dy);

    cur_x = PLR_SCRN_X + 4 + (dx << 3);
    cur_y = PLR_SCRN_Y + 4 + (dy << 3);

    move_sprite(cur_id, cur_x - (player.x & 7), cur_y - (player.y & 7));
}