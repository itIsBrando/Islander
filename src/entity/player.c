#include <gb/gb.h>
#include <string.h>
#include <rand.h>

#include "player.h"
#include "particle.h"
#include "../main.h"
#include "../oam.h"
#include "../item/item.h"
#include "../item/goal.h"
#include "../world/tile.h"
#include "../world/chunk.h"
#include "../world/collision.h"
#include "../gui/hud.h"
#include "../gui/crafting.h"



player_t player;


uint8_t cur_id, cur_x, cur_y;
uint8_t anim_counter = 0;

uint8_t tile_damage = 0; // Amount of damage for the attacking tile. This variable is reset whenever `plr_move` is called & the player moves successfully

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
    itm_add_to_inventory(&player.inventory, &pick);
    goal_print_active();
}


/**
 * Prints the name and quantity of the currently selected item
 */
inline void plr_draw_active_item()
{
    itm_draw(plr_get_active_item(), 0, 0, DRAW_ITEM_COUNT | DRAW_ITEM_NAME);
}


/**
 * Call every frame
 * @param j joypad
 */
void plr_update(const uint8_t j)
{
    // deals with inventory scrolling and drawing active goal
    if(j & J_B) {
        if(WY_REG > WINDOW_Y - 8) {
            hud_scroll_north();
        }

        if(j & J_RIGHT)
            hud_move_cur(DIRECTION_RIGHT);
        else if (j & J_LEFT)
            hud_move_cur(DIRECTION_LEFT);

        plr_draw_active_item();
        waitjoypad(J_LEFT | J_RIGHT);
        return;
    } else if(WY_REG < WINDOW_Y) {
        hud_scroll_south();
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
        if(((player.x - 1) & 63) == 0) {
            if(cnk_load_row(DIRECTION_LEFT))
                plr_move(DIRECTION_LEFT);
        } else {
            plr_move(DIRECTION_LEFT);
        }
    }
    
    if(j & J_RIGHT) {
        if(((player.x + 1) & 63) == 0) {
            if(cnk_load_row(DIRECTION_RIGHT))
                plr_move(DIRECTION_RIGHT);
        } else {
            plr_move(DIRECTION_RIGHT);
        }
    }

    if(j & J_START)
        cft_open_menu(WORKBENCH);

    if(j & J_A) {
        plr_interact();
    }

}


/**
 * Hides the player and the player's cursor
 */
void plr_hide()
{
    spr_hide(player.id);
    spr_hide(cur_id);
}

/**
 * Shows the player and player's cursor sprite if hidden.
 * @note only call if hidden
 */
void plr_show()
{
    spr_show(player.id);
    spr_show(cur_id);
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
 * Adds an item from the player's inventory. This function
 *  calls `itm_add_to_inventory` but also invokes necesssary
 *  drawing functions for the hotbar
 */
void plr_add_id_to_inventory(const uint8_t id, const int8_t count)
{
    item_t *item = itm_lookup(&player.inventory, id);

    itm_add_id_to_inventory(&player.inventory, id, count);
    
    // if item was not in inventory previously, draw it
    if(!item) {
        hud_add_item(id);
    }

    eff_new(PLR_SCRN_X + (dir_get_x(player.dir) << 3), PLR_SCRN_Y + (dir_get_y(player.dir) << 3), count);
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
    const tile_data_t *tile_data = tile_data_lookup(*t);

    if(ITEM_LOOKUP(item).is_tool && tile_data->mineable) {
        shake(5);
        tile_damage += itm_get_tile_damage(item);

        // damages the tile that we are looking at
        if(tile_damage >= tile_data->hp) {
            x >>= 3, y >>= 3;
            plr_add_id_to_inventory(tile_data->item_id, 2 + (rand() & 0x1));
            if(tile_data->onmine)
                tile_data->onmine(x, y, *t);
            
            cnk_active_write(x, y, TILE_GROUND);
            
            goal_check_completion();
        }
    } else if(itm_can_interact(item, *t)) {
        cnk_active_write(x >> 3, y >> 3, TILE_BRIDGE);
        plr_sub_from_inventory(plr_get_active_item()->id, 1);
        plr_draw_active_item();
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