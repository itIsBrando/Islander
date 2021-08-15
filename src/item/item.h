#ifndef ITEM_H
#define ITEM_H

#include "../structs.h"

typedef enum {
    ITEM_STONE_PICK,
    ITEM_WOOD,
    ITEM_STONE,
    ITEM_COAL_ORE,
    ITEM_IRON_ORE,
};


typedef enum {
    DRAW_ITEM_NAME = BIT(0),    // draws the name of item
    DRAW_ITEM_COUNT = BIT(1),   // draws the number of items
    DRAW_ITEM_ICON = BIT(2),    // draws a sprite icon for the item
} itm_draw_flag_t;


uint8_t itm_draw_icon(const item_t *item, uint8_t x, uint8_t y);
uint8_t itm_draw(const item_t *item, uint8_t x, uint8_t y, itm_draw_flag_t flags);


bool itm_add_to_inventory(inv_t *inv, const item_t *item);


extern const item_data_t itm_data[];
#endif