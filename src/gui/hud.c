#include <gb/gb.h>

#include "hud.h"
#include "../main.h"
#include "../oam.h"
#include "../item/item.h"


uint8_t hud_cur = 0;

static uint8_t hud_cur_id;

// These are sprite ids corresponding to each of the player's inventory slots
static uint8_t hud_item_ids[10];

/**
 * Draws the player's inventory onto the window
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
        hud_item_ids[i] = itm_draw_icon(&plr->inventory.items[i], 3 + (i << 4), 10);
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
    if(dir == DIRECTION_LEFT)
    {
        if(hud_cur == 0)
            return;
        
        hud_cur--;
    } else {
        if(hud_cur > player.inventory.size)
            return;

        hud_cur++;
    }
    
    move_sprite(hud_cur_id, 10 + (hud_cur << 4), 150);
}


/**
 * @param id id of item removed. (not really necessary)
 */
void hud_remove_item()
{
    const u8 index = player.inventory.size;
    const u8 id = hud_item_ids[index];

    move_sprite(id, 0, 0);

    if(hud_cur == index)
        hud_move_cur(DIRECTION_LEFT);

    spr_free(id);
}