#ifndef ITEM_H
#define ITEM_H

#include "../structs.h"

typedef enum {
    ITEM_STONE_PICK,
    ITEM_WOOD,
    ITEM_STONE,
    ITEM_COAL,
    ITEM_IRON_ORE,
    ITEM_BRIDGE,
    ITEM_WORKBENCH,
};


/**
 * @param _item `item_t *`
 */
#define ITEM_LOOKUP(_item) itm_data[(_item)->id]


typedef enum {
    DRAW_ITEM_NAME          = BIT(0),   // draws the name of item
    DRAW_ITEM_COUNT         = BIT(1),   // draws the number of items
    DRAW_ITEM_PIXEL_GRID    = BIT(2),   // draws a sprite icon for the item
    DRAW_ITEM_TILE_GRID     = BIT(3),   // draws a tile-based icon of the item
} itm_draw_flag_t;

item_t *itm_lookup(inv_t *inv, const uint8_t id);

uint8_t itm_get_tile_damage(const item_t *item);

bool itm_contains_id(const inv_t *inv, const uint8_t id, const int8_t count);
bool itm_contains(const inv_t *inv, const item_t *item, const int8_t count);

uint8_t itm_draw_icon(const item_t *item, uint8_t x, uint8_t y);
uint8_t itm_draw(const item_t *item, uint8_t x, uint8_t y, itm_draw_flag_t flags);

bool itm_can_interact(const item_t *item, tile_t tile);

tile_t itm_get_tile(uint8_t itemNumber);
const char *itm_get_name(const item_t *item);

bool itm_add_id_to_inventory(inv_t *inv, const uint8_t id, const uint8_t count);
bool itm_add_to_inventory(inv_t *inv, const item_t *item);
bool itm_sub_from_inventory(inv_t *inv, const uint8_t id, const int8_t count);


extern const item_data_t itm_data[];
#endif