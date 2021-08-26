#include "tile.h"
#include "../world/chunk.h"
#include "../item/item.h"

#define MAKE_TILE(_id, _can_pass, _can_mine, _hp, ...) {\
.id=_id,\
.passable=_can_pass,\
.mineable=_can_mine,\
.hp=_hp,\
__VA_ARGS__\
}


static void tree_onmine(uint8_t x, uint8_t y, tile_t tile)
{
        cnk_active_write(x, y + (tile == TILE_TREE_BOTTOM ? -1 : 1), TILE_GROUND);
}


static void workbench_onmine(uint8_t x, uint8_t y, tile_t tile) {
    
}


const tile_data_t tile_data[] = {
    MAKE_TILE(TILE_WATER, false, false, 0),
    MAKE_TILE(TILE_TREE_TOP, false, true, 10, .item_id=ITEM_WOOD, .onmine=tree_onmine),
    MAKE_TILE(TILE_TREE_BOTTOM, false, true, 10, .item_id=ITEM_WOOD, .onmine=tree_onmine),
    MAKE_TILE(TILE_STONE, false, true, 10, .item_id=ITEM_STONE),
    MAKE_TILE(TILE_COAL, false, true, 15, .item_id=ITEM_COAL),
    MAKE_TILE(TILE_WORKBENCH, false, true, 9, .item_id=ITEM_WORKBENCH, .onmine=workbench_onmine),
};


/**
 * @returns data about a tile id
 */
const tile_data_t *tile_data_lookup(tile_t id)
{
    const tile_data_t *d = tile_data;
    for(u8 i = 0; i < sizeof(tile_data) / sizeof(tile_data[0]); i++)
    {
        if(id == d->id)
            return d;
        
        d++;
    }

    return NULL;
}


/**
 * Checks to see if a tile is passable
 * @returns true if player can pass, otherwise false
 */
inline bool tile_can_pass(tile_t tile)
{
    if(tile == TILE_GROUND)
        return true;
    else
        return tile_data_lookup(tile)->passable;
}