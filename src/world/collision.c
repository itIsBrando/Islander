
#include "chunk.h"
#include "collision.h"


/**
 * @param px screen x coordinate
 * @param py screen y coordinate
 * @returns pointer to tile
 */
tile_t *map_get_tile_relative(uint8_t px, uint8_t py)
{
    px >>= 3, py >>= 3;
    
    chunk_t *c = cnk_get_relative(px, py)->chunk;

    px &= 7;
    py &= 7;

    return &c->tiles[px + (py << 3)];
}
