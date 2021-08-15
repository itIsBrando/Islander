#include <gb/gb.h>

#include "hud.h"
#include "../main.h"
#include "../oam.h"
#include "../item/item.h"


static uint8_t hud_cur = 0;


uint8_t hud_cur_id;

// @todo draw sprite of the player's active item
uint8_t hud_item_id;

/**
 * Draws the player's units onto the window
 */
void hud_draw_hotbar(player_t *plr)
{
    const uint8_t t[] = {7, 10, 8, 9};
    uint8_t i;

    hud_cur_id = spr_allocate();
    move_win(7, 144 - 24);

    // draw item icon borders
    for(i = 0; i <  10; i++) {
        set_win_tiles(i << 1, 1, 2, 2, t);
    }

    // draws the inventory
    for(i = 0; i < plr->inventory.size; i++) {
        itm_draw_icon(&plr->inventory.items[i], 3 + (i << 4), 10);
    }

    SHOW_WIN;

    set_sprite_tile(hud_cur_id, 11);
    set_sprite_prop(hud_cur_id, BIT(4));
    hud_move_cur(DIRECTION_RIGHT);
}


/**
 * @param dir LEFT or RIGHT only
 */
void hud_move_cur(const direction_t dir)
{
    const int8_t dx = dir_get_x(dir);

    if((hud_cur + dx) >= 10)
        return;

    hud_cur += dx;

    move_sprite(hud_cur_id, 10 + (hud_cur << 4), 150);
}