#include <gb/gb.h>
#include <string.h>

#include "player.h"
#include "../main.h"
#include "../oam.h"
#include "../item/item.h"
#include "../item/goal.h"
#include "../world/tile.h"
#include "../world/chunk.h"
#include "../world/collision.h"
#include "../gui/hud.h"



player_t player;


uint8_t cur_id, cur_x, cur_y;
uint8_t anim_counter = 0;

// @todo implement tile destruction
uint8_t tile_damage = 0; // amount of damage for the attacking tile

/**
 * Initializes player sprite and variables
 */
void plr_init()
{
    player.id = spr_allocate();

    // initialize inventory
    memset(&player.inventory, 255, sizeof(inv_t));
    player.inventory.size = 0;

    move_sprite(player.id, PLR_SCRN_X, PLR_SCRN_Y);
    set_sprite_tile(player.id, 2);
    set_sprite_prop(player.id, BIT(4));

    player.x = 1, player.y = 1;
    move_bkg(player.x, player.y);

    set_sprite_tile(cur_id = spr_allocate(), 16);

    item_t pick = {.id = ITEM_STONE_PICK};
    item_t wood = {.id = ITEM_WOOD, 2};
    itm_add_to_inventory(&player.inventory, &pick);
    itm_add_to_inventory(&player.inventory, &wood);

    hud_draw_hotbar(&player);
}


/**
 * Call every frame
 * @param j joypad
 */
void plr_update(const uint8_t j)
{
    if(j & J_B) {
        char buf[20];
        goal_generate_name(goal_get_current(), buf, 20);

        print_window(buf, 0, 0);

        if(j & J_RIGHT)
            hud_move_cur(DIRECTION_RIGHT);
        else if (j & J_LEFT)
            hud_move_cur(DIRECTION_LEFT);

        waitjoypad(J_LEFT | J_RIGHT);
        return;
    }


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
        if(((player.x - 1) & 63) == 0)
            cnk_load_row(DIRECTION_LEFT);

        plr_move(DIRECTION_LEFT);
    }
    
    if(j & J_RIGHT) {
        if(((player.x + 1) & 63) == 0)
            cnk_load_row(DIRECTION_RIGHT);

        plr_move(DIRECTION_RIGHT);
    }

    if(j & J_A)
    {
        plr_interact();
    }

}


/**
 * Removes an item from the player's inventory. This function
 *  calls `itm_sub_from_inventory` but also invokes necesssary
 *  drawing functions for the hotbar
 */
void plr_sub_from_inventory(const uint8_t id, const int8_t count)
{
    item_t *item = itm_lookup(&player.inventory, id);

    if(itm_sub_from_inventory(&player.inventory, id, count)) {
        hud_remove_item();
        item->id = 0xFF; // remove item from recognizable ids
    }
}


/**
 * @returns returns a pointer to the player active item
 */
inline item_t *plr_get_active_item()
{
    return &player.inventory.items[hud_cur];
}


/**
 * @todo add proper item interaction
 */
inline void plr_interact()
{
    u8 x = player.x + cur_x - 4, y = player.y + cur_y - 10;
    tile_t *t = map_get_tile_relative(x, y);
    item_t *item = plr_get_active_item();
    tile_data_t *tile_data = tile_data_lookup(*t);

    if(ITEM_LOOKUP(item).is_tool && tile_data->mineable) {
        shake(5);
        tile_damage += itm_get_tile_damage(item);

        // @todo improve versitility
        if(tile_damage >= tile_data->hp) {
            cnk_active_write(x >> 3, y >> 3, TILE_GROUND);
            itm_add_id_to_inventory(&player.inventory, tile_data->item_id);
            /** add optional events for tiles to
             *   allow for trees to be mined correctly
             *   and for other unnecessary functionality
             */
        }
    } else if(itm_can_interact(item, *t)) {
        cnk_active_write(x >> 3, y >> 3, TILE_BRIDGE);
        plr_sub_from_inventory(plr_get_active_item()->id, 1);
    }

    waitjoypad(J_A);
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

    if(dir == DIRECTION_LEFT)
        set_sprite_prop(player.id, BIT(5) | BIT(4));
    else if(dir == DIRECTION_RIGHT)
        set_sprite_prop(player.id, BIT(4));


    cur_x = PLR_SCRN_X + 4 + (dx << 3) - (player.x & 7);
    cur_y = PLR_SCRN_Y + 4 + (dy << 3) - (player.y & 7);

    move_sprite(cur_id, cur_x, cur_y);

    const tile_t *tile = map_get_tile_relative(player.x + dx + PLR_SCRN_X - 4, PLR_SCRN_Y - 10 + player.y + dy);

    if(!tile_can_pass(*tile))
    {
        player.x -= dx;
        player.y -= dy;
    } else {
        scroll_bkg(dx, dy);

        // @todo improve perfomance
        if((anim_counter++ & 3) == 3) {
            set_sprite_tile(player.id, TILE_PLAYER_BASE + ((anim_counter & 0b1100) % 3));
        }

        tile_damage = 0;
    }
}