
#include "spawn.h"
#include "chunk.h"
#include "collision.h"

// #include <stdlib.h>
#include <rand.h>

#define PERCENT * 255/100

typedef struct {
    u8 tile;
    u8 rate;
} spawn_rate_t;


/**
 * @returns true if we can spawn, otherwise false
 */
bool spwn_do_tick()
{
    const spawn_rate_t spawnables[] = {
        {TILE_TREE_TOP, 50 PERCENT},
        {TILE_STONE, 25 PERCENT},
        {TILE_COAL, 25 PERCENT}
    };


    const u8 candidates = sizeof(spawnables) / sizeof(spawnables[0]);

    u8 x = rand() & 31, y = rand() & 31;

    if(cnk_get_relative(x, y)->chunk->tiles[(x & 7) + ((y & 7) << 3)] != TILE_GROUND)
        return false;

    u8 spawn_type = rand();

    u8 c = 0, i;

    for(i = 0; i < candidates; i++)
    {
        c += spawnables[i].rate;

        if(spawn_type < c)
        {
            break;
        }
    }

    cnk_active_write(x, y, spawnables[i].tile);
    if(spawnables[i].tile == TILE_TREE_TOP)
        cnk_active_write(x, y + 1, TILE_TREE_BOTTOM);

    return true;
}