#ifndef TILE_H
#define TILE_H

#include "../structs.h"


typedef struct {
    bool passable : 1; // true if entities can pass through
    bool mineable : 1; // true if this tile can be mined with a pickaxe
    uint8_t hp; // strength of items
    tile_t id;
    uint8_t item_id; // id of item to drop when mined
    void (*onmine)(uint8_t, uint8_t, tile_t); /** optional event. Called when a tile is destroyed by a player @param x i assume absolute x????*/
} tile_data_t;


const tile_data_t *tile_data_lookup(tile_t id);


bool tile_can_pass(tile_t tile);


#endif