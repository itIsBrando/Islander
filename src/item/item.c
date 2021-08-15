#include <gb/gb.h>
#include <string.h>

#include "item.h"
#include "../oam.h"
#include "../main.h"


#define DEFINE_ITEM(_name, _placable, _is_tool, _level, _tile) {\
    .name=_name,\
    .placable=_placable,\
    .is_tool=_is_tool,\
    .level=_level,\
    .tile=_tile\
}


/**
 * @param _item `item_t *`
 */
#define ITEM_LOOKUP(_item) itm_data[(_item)->id]

const item_data_t itm_data[] = {
    [ITEM_STONE_PICK] = DEFINE_ITEM("PICK", false, true, 0, 17),
    [ITEM_WOOD] = DEFINE_ITEM("WOOD", false, false, 0, 33)
};


/**
 * @returns the graphical tile of an item
 */
inline tile_t itm_get_tile(uint8_t itemNumber) {
    return itm_data[itemNumber].tile;
}


/**
 * Draws an item at (x, y)
 * @param x pixel x coordinate 0-159
 * @param y pixel y coordinate 0-143
 * @returns sprite id
 */
uint8_t itm_draw_icon(const item_t *item, uint8_t x, uint8_t y)
{
    uint8_t i = spr_allocate();

    move_sprite(i, x + WX_REG, y + 16 + WY_REG);
    set_sprite_tile(i, itm_get_tile(item->id));

    return i;
}


void itm_free_icon(uint8_t i)
{
    spr_free(i);
}


/**
 * @param item item to draw
 * @param x tile x coordinate
 * @param y tile y coordinate
 * @param flags `DRAW_ITEM_...`
 * @see itm_draw_flag_t
 */
uint8_t itm_draw(const item_t *item, uint8_t x, uint8_t y, itm_draw_flag_t flags)
{
    const char *name = ITEM_LOOKUP(item).name;
    uint8_t id = 255;
    
    if(flags & DRAW_ITEM_ICON) {
        id = itm_draw_icon(item, x << 3, y << 3);
        x++;
    }

    if(flags & DRAW_ITEM_NAME)
    {
        print_window(name, x, y);
        x += strlen(name) + 1;
    }
    
    if(flags & DRAW_ITEM_COUNT)
        printInt(item->count, x, y, true);


    
    return id;
}


/**
 * @returns true if `inv` has `item` with at least `count` items
 */
bool itm_contains(inv_t *inv, const item_t *item, uint8_t count)
{
    if(!inv->size)
        return false;

    item_t *invItem = inv->items;

    for(u8 i = 0; i < inv->size; i++)
    {
        if(invItem->id == item->id)
        {
            if(invItem->count >= count)
                return true;
            else
                return false;
        }

        invItem++;
    }

    return false;
}



/**
 * @returns false if the inventory is full
 */
bool itm_add_to_inventory(inv_t *inv, const item_t *item)
{
    if(itm_contains(inv, item, 1))
    {
        // find and append
    } else if(inv->size < 10) {
        inv->items[inv->size++] = *item;
    } else {
        return false;
    }

    return true;
}