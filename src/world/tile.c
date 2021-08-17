

#include "tile.h"
#include "../item/item.h"

#define DEFINE_TILE(_id, _can_pass, _can_mine, _hp, ...) {\
.id=_id,\
.passable=_can_pass,\
.mineable=_can_mine,\
.hp=_hp,\
__VA_ARGS__\
}


const tile_data_t tile_data[] = {
    DEFINE_TILE(TILE_WATER, false, false, 0),
    DEFINE_TILE(TILE_TREE_TOP, false, true, 10, .item_id=ITEM_WOOD),
    DEFINE_TILE(TILE_TREE_BOTTOM, false, true, 10, .item_id=ITEM_WOOD),
    DEFINE_TILE(TILE_STONE, false, true, 10),
    DEFINE_TILE(TILE_COAL, false, true, 13),
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