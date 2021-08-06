
#include "spawn.h"
#include "chunk.h"

// #include <stdlib.h>
#include <rand.h>

#define PERCENT * 255/100

typedef enum {
    TILE_TYPE_ROCK=50,
    TILE_TYPE_TREE=64,
    TILE_TYPE_COAL=28,
    TILE_TYPE_IRON,
    TILE_TYPE_GOLD,
    TILE_TYPE_DIAMOND,
} tile_t;


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
        {TILE_TYPE_ROCK, 50 PERCENT},
        {TILE_TYPE_TREE, 25 PERCENT},
        {TILE_TYPE_COAL, 25 PERCENT}
    };


    const u8 candidates = sizeof(spawnables) / sizeof(spawnables[0]);

    u8 x = rand() & 31, y = rand() & 31;

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

    return true;
}