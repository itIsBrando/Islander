#include <gb/gb.h>
#include <string.h>

#include "item.h"
#include "../oam.h"
#include "../main.h"


/**
 * @param _name name of item
 * @param _placable true if it can placed on surfaces
 * @param _tile tile of ITEM representation
 */
#define DEFINE_ITEM(_name, _placable, _is_tool, _level, _tile, ...) {\
    .name=_name,\
    .placable=_placable,\
    .is_tool=_is_tool,\
    .level=_level,\
    .tile=_tile,\
    __VA_ARGS__\
}


bool bridge_can_place(tile_t tile) {
    return tile == TILE_WATER;
}


const item_data_t itm_data[] = {
    [ITEM_STONE_PICK] = DEFINE_ITEM("PICK", NULL, true, 0, 17),
    [ITEM_WOOD] = DEFINE_ITEM("WOOD", bridge_can_place, false, 0, 33)
};


/**
 * @returns the amount of tile damage that a tool will do to a tile
 */
inline uint8_t itm_get_tile_damage(const item_t *item)
{
    return ITEM_LOOKUP(item).level + 3;
}


bool itm_can_interact(const item_t *item, tile_t tile)
{
    item_data_t *d = &ITEM_LOOKUP(item);

    if(d->placable)
        return d->placable(tile);
    else
        return false;
}


/**
 * @returns the graphical background tile of an item
 */
inline tile_t itm_get_tile(uint8_t itemNumber) {
    return itm_data[itemNumber].tile;
}


/**
 * @returns the string name of the item
 */
inline const char *itm_get_name(const item_t *item)
{
    return itm_data[item->id].name;
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


inline void itm_free_icon(uint8_t i)
{
    spr_free(i);
}


/**
 * Draws data about an item. Very customizable function. You can draw sprites and text to shox information about an item
 * @param item item to draw
 * @param x tile x coordinate
 * @param y tile y coordinate
 * @param flags `DRAW_ITEM_...`
 * @returns -1 if no sprite is generated, otherwise returns the sprite id
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
 * @param inv inventory to search in
 * @param item item to look for
 * @returns NULL if not found, otherwise a pointer to the item
 */
item_t *itm_lookup(inv_t *inv, const uint8_t id)
{
    for(uint8_t i = 0; i < inv->size; i++)
    {
        if(inv->items[i].id == id)
            return &inv->items[i];
    }

    return NULL;
}


/**
 * Checks to see if an item is present in an inventory
 * @returns true if `inv` has `id` with at least `count` items
 */
bool itm_contains_id(inv_t *inv, const uint8_t id, const int8_t count)
{
    if(!inv->size)
        return false;

    item_t *invItem = inv->items;

    for(u8 i = 0; i < inv->size; i++)
    {
        if(invItem->id == id)
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
 * Checks to see if an item is present in an inventory
 * @returns true if `inv` has `item` with at least `count` items
 * @see `itm_contains_id`
 */
inline bool itm_contains(inv_t *inv, const item_t *item, const int8_t count)
{
    return itm_contains_id(inv, item->id, count);
}


inline bool itm_add_id_to_inventory(inv_t *inv, const uint8_t id)
{
    item_t item = {.id = id, .count = 1};
    return itm_add_to_inventory(inv, &item);
}

/**
 * Adds an item to an inventory. Does nothing if inventory is full
 * @returns false if the inventory is full
 */
bool itm_add_to_inventory(inv_t *inv, const item_t *item)
{
    if(itm_contains(inv, item, 1))
    {
        // find item already in the inventory
        itm_lookup(inv, item->id)->count += item->count;
        return true;
    } else if(inv->size < 10) {
        inv->items[inv->size++] = *item;
    } else {
        return false;
    }

    return true;
}


/**
 * Removes/subtracts an item from an inventory
 * @returns true if an item is removed, false if an item is just subtracted 
 * @see `plr_sub_from_inventory`
 */
bool itm_sub_from_inventory(inv_t *inv, const uint8_t id, const int8_t count)
{
    for(uint8_t i = 0; i < inv->size; i++)
    {
        if(inv->items[i].id == id)
        {
            inv->items[i].count -= count;

            if(inv->items[i].count <= 0)
            {
                memcpy(&inv->items[i], &inv->items[i + 1], (10 - i) * sizeof(item_t));
                inv->size--;

                return true;
            }

            return false;
        }
    }

    return false;
}