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
 * Initializes and draws the player's hotbar
 */
inline void hud_init()
{
    hud_cur_id = spr_new(10, 150, 11);
    move_win(WINDOW_X, WINDOW_Y);
    set_sprite_prop(hud_cur_id, BIT(4));

    hud_print_hotbar();

    SHOW_WIN;
}


/**
 * Draws the player's inventory onto the window
 */
void hud_print_hotbar()
{
    const uint8_t t[] = {7, 10, 8, 9};
    uint8_t i;


    // draw item icon borders
    for(i = 0; i <  10; i++) {
        set_win_tiles(i << 1, 1, 2, 2, t);
    }

    // draws the inventory
    for(i = 0; i < player.inventory.size; i++) {
        hud_item_ids[i] = itm_draw_icon(&player.inventory.items[i], 3 + (i << 4), 10);
    }
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
        if(hud_cur >= 10)
            return;

        hud_cur++;
    }
    
    scroll_sprite(hud_cur_id, dir_get_x(dir) << 4, 0);
    plr_draw_active_item();
}


/**
 * @param index index of item removed.
 */
void hud_remove_item(const uint8_t index)
{
    const u8 id = hud_item_ids[index];
    
    for(uint8_t i = index; i < player.inventory.size; i++) {
        hud_item_ids[i] = hud_item_ids[i + 1];
        scroll_sprite(hud_item_ids[i], -16, 0);
    }

    spr_free(id);
}


/**
 * Adds an item icon to the player's hotbar
 */
void hud_add_item(const uint8_t id)
{
    const u8 index = player.inventory.size - 1;
    
    hud_item_ids[index] = itm_draw_icon(itm_lookup(&player.inventory, id), 3 + (index << 4), 10);
}


/**
 * Moves the window & HUD related sprites north
 */
inline void hud_scroll_north()
{
    WY_REG--;

    for(u8 i = 0; i < player.inventory.size; i++) {
        scroll_sprite(hud_item_ids[i], 0, -1);
    }

    scroll_sprite(hud_cur_id, 0, -1);
}


/**
 * Moves the window & HUD related sprites south
 */
inline void hud_scroll_south()
{
    WY_REG++;

    for(u8 i = 0; i < player.inventory.size; i++) {
        scroll_sprite(hud_item_ids[i], 0, 1);
    }

    scroll_sprite(hud_cur_id, 0, 1);
}