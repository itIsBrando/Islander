#ifndef TYPES_H
#define TYPES_H

#include "defines.h"
#include <asm/types.h>
#include <stdint.h>
#include <stdbool.h>

#define TILE_ICON_START 26 // 26-28
#define TILE_ICON_A     29
#define TILE_ICON_B     30
#define TILE_ICON_LEFT  31
#define TILE_ICON_UP    32
#define TILE_ICON_RIGHT 33
#define TILE_ICON_DOWN  34
#define TILE_ARROW      35

typedef uint8_t u8;


#define BIT(b) (1 << b)

typedef enum {
    TILE_WATER = 0,
    TILE_PLAYER_BASE = 1,
    TILE_GROUND = 21,
    TILE_COAL = 27,
    TILE_STONE = 32,
    TILE_TREE_TOP = 47,
    TILE_TREE_BOTTOM = 63,
    TILE_BRIDGE = 35,
} tile_t;

typedef enum {
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_UP,
    DIRECTION_DOWN
} direction_t;


typedef struct {
    uint8_t x, y;
    uint8_t type;
} ent_t;


typedef struct {
    tile_t tiles[8 * 8];
    uint8_t entSize;
    ent_t entities[CHUNK_MAX_ENTITIES];
} chunk_t;


typedef struct {
    chunk_t *chunk;
    u8 xChunk, yChunk; // [0-9) represents the chunk's position relative to the ENTIRE world
    u8 xOffset, yOffset;  // screen offset to draw chunks
} active_chunk_t;

/**
 * Stores a 3x3 array of chunks representing one island
 */
typedef struct {
    chunk_t chunks[9];
} island_chunk_t; // 9 chunks per island (3x3)


typedef struct {
    uint8_t tiles[24 * 24 / 8];
} stored_island_t;


typedef struct {
    /**
     * Organized such that there are 3x3 islands. each island is 3x3 chunks
     *  so there is a 9x9 world of chunks.
     */
    chunk_t chunks[WORLD_MAX_SIZE * CHUNK_PER_ISLAND * CHUNK_PER_ISLAND];
} world_t;


typedef struct {
    const char *name;
    const bool (*placable)(tile_t);
    const bool is_tool : 1;
    tile_t tile;
    uint8_t level;
    
} item_data_t;


typedef struct {
    uint8_t id;
    int8_t count;
} item_t;


typedef struct {
    item_t items[10];
    uint8_t size;
} inv_t;


typedef struct {
    uint8_t x;      // absolute tile (x, y). On the interval [0-72]
    uint8_t y;      // absolute tile (x, y). On the interval [0-72]
    uint8_t id;     // sprite index
    inv_t inventory;
    uint8_t active_item; // index in `inventory` of the active item
    direction_t dir;
} player_t;


extern player_t player;

#endif