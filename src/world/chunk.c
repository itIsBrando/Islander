#include <gb/gb.h>
#include <string.h>

#include "chunk.h"
#include "../main.h"
#include "../structs.h"

world_t world;
/**
 * So there are 9 islands total.
 * Islands are in a 3x3 grid
 * Each island is made of 3x3 array of chunks
 * Meaning there is a 9x9 array of chunks
 * Each chunk is 8x8 tiles
 */


active_chunk_t active_chunks[16];


inline void cnk_draw(const u8 x, const u8 y, const chunk_t *chunk)
{
    set_bkg_tiles((x & 3) << 3, (y & 3) << 3, 8, 8, chunk->tiles);
}


const stored_island_t islands[] = {
    {
        {
            0b00000000, 0b00000000, 0b00000000,
            0b00000000, 0b00000000, 0b00000000,
            0b00000000, 0b00011100, 0b00000000,
            0b00000001, 0b11111111, 0b11100000,
            0b00000011, 0b11111111, 0b11110000,
            0b00000111, 0b11111111, 0b11111000,
            0b00001111, 0b11111111, 0b11111000,
            0b00001111, 0b11111111, 0b11111000,
            0b00001111, 0b11111111, 0b11111000,
            0b00001111, 0b11111111, 0b11111000,
            0b00001111, 0b11111111, 0b11111000,
            0b00001111, 0b11111111, 0b11111000,
            0b00000111, 0b11111111, 0b11111000,
            0b00000011, 0b11111111, 0b11110000,
            0b00000000, 0b11111111, 0b11000000,
            0b00000000, 0b00000000, 0b00000000,
        }
    }
};


/**
 * Writes a tile on the active chunks
 * @param x relative to the active chunk
 * @param y relative to the active chunk
 */
void cnk_active_write(u8 x, u8 y, u8 tile)
{
    active_chunk_t *c = cnk_get_relative(x, y);

    c->chunk->tiles[(x & 7) + (y & 7) * 8] = tile;

    cnk_draw(c->xOffset, c->yOffset, c->chunk);
}



/**
 * Creates a chunk out of an island
 * @param islandNumber island data
 * @param x [0-3) x coordinate for the placement of this island
 * @param y [0-3) y coordinate for the placement of this island
 */
void cnk_island_load(const uint8_t islandNumber, u8 xx, u8 yy)
{
    xx *= 3;
    
    u8 xChunk, yChunk = yy *= 3;
    u8 tile = islands[islandNumber].tiles[0], offset = 0;

    for(u8 y = 0; y < 24; y++)
    {
        xChunk = xx;
        
        for(u8 x = 0; x < 24; x++)
        {
            world.chunks[xChunk + yChunk * 9].tiles[(x & 7) + ((y & 7) << 3)] = (tile & 0x80) ? TILE_GROUND : TILE_WATER;

            tile <<= 1;

            if((x & 7) == 7) {
                xChunk++;
                tile = islands[islandNumber].tiles[++offset];
            }
        }

        if((y & 7) == 7)
            yChunk++;
    }
}


// min and max chunks that are loaded. Correspond to `xChunk`/`yChunk`
static uint8_t cnk_xMin, cnk_xMax, cnk_yMin, cnk_yMax;

/**
 * Loads the first island onto the screen
 */
void cnk_init()
{
    memset(active_chunks, NULL, sizeof(active_chunks));
    u8 offset = 0;

    for(u8 y = 0; y < 4; y++)
    {
        for(u8 x = 0; x < 4; x++)
        {
            active_chunk_t *c = &active_chunks[x + y * 4];
            c->chunk = &world.chunks[x + y * 9];
            c->xOffset = x;
            c->xChunk = x + 3;
            c->yOffset = y;
            c->yChunk = y + 3;
        }
    }

    cnk_xMin = 3, cnk_xMax = 6;
    cnk_yMin = 3, cnk_yMax = 6;
}


/**
 * Loads a row of chunks north of current set up
 * @returns false if could not load (out of bounds)
 */
bool cnk_load_row(const direction_t dir)
{
    if((dir == DIRECTION_UP && cnk_yMin == 0) || (dir == DIRECTION_DOWN && cnk_yMax == 8))
        return false;

    active_chunk_t *ac = active_chunks;
    const int8_t dx = dir_get_x(dir);
    const int8_t dy = dir_get_y(dir);

    for(uint8_t i = 0; i < 16; i++)
    {
        if(
            (dir == DIRECTION_UP && ac->yChunk == cnk_yMax) ||
            (dir == DIRECTION_DOWN && ac->yChunk == cnk_yMin) ||
            (dir == DIRECTION_LEFT && ac->xChunk == cnk_xMax) ||
            (dir == DIRECTION_RIGHT && ac->xChunk == cnk_xMin)
        ) {
            if(dir == DIRECTION_LEFT)
                ac->xChunk = cnk_xMin - 1;
            else if(dir == DIRECTION_RIGHT)
                ac->xChunk = cnk_xMax + 1;
            else if(dir == DIRECTION_UP)
                ac->yChunk = cnk_yMin - 1;
            else if(dir == DIRECTION_DOWN)
                ac->yChunk = cnk_yMax + 1;

            //   unload chunk
            // cnk_unload(ac);
            // ac->xOffset = (ac->xChunk - 3) & 3;//(ac->xOffset + dx) & 3;
            // ac->yOffset = (ac->yChunk - 3) & 3;//(ac->yOffset + dy) & 3;
            ac->chunk = &world.chunks[ac->xChunk + (ac->yChunk) * 9];
            
            cnk_draw(
                ac->xOffset,
                ac->yOffset,
                ac->chunk
            );

        }

        ac++;
    }

    cnk_yMax += dy;
    cnk_yMin += dy;

    cnk_xMax += dx;
    cnk_xMin += dx;
    
    return true;
}


/**
 * Graphically draws all of the active chunks
 */
void cnk_draw_active()
{
    active_chunk_t *c = active_chunks;

    // for(active_chunk_t *c = active_chunks; c < active_chunks + sizeof(active_chunks); c++)
    for(u8 i = 0; i < 16; i++)
    {
        if(c->chunk)
            cnk_draw(c->xOffset, c->yOffset, c->chunk);
        
        c++;
    }
}


/**
 * Returns an active chunk given a tile coordinate
 * @param tx x coordinate between 0 and 31
 * @param ty y coordinate between 0 and 31
 * @returns chunk at (tx, ty)
 */
active_chunk_t *cnk_get_relative(u8 tx, u8 ty)
{
    tx >>= 3;
    ty >>= 3;

    active_chunk_t *ac = active_chunks;

    for(u8 i = 0; i < 16; i++) {
        
        if(ac->xOffset == tx && ac->yOffset == ty)
        {
            return ac;
        }

        ac++;
    }

    return NULL;
}