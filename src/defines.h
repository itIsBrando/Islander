#ifndef DEFINES_H
#define DEFINES_H

#include <types.h>

#define CHUNK_PER_ISLAND        (3)
#define CHUNK_MAX_ENTITIES      (4)
#define WORLD_MAX_SIZE          (3*3)
#define CHUNK_INITIAL_SPAWNS    (25)    // number of trees, rocks, and other stuff to spawn whenever a world is loaded

#define FONT_START      ('!'-1) // first ASCII character that we load into VRAM
#define FONT_BASE       (0x80)  // the base tile number where the font is loaded into

#define WINDOW_X        (7)
#define WINDOW_Y        (144 - 24)

#define CRAFTING_BENCH_SIZE 10 // number of recipes that each crafting bench has
#define CRAFTING_ITEM_FLAGS (DRAW_ITEM_COUNT | DRAW_ITEM_NAME | DRAW_ITEM_TILE_GRID) // item_flag_t standard for drawing recipe items


#endif