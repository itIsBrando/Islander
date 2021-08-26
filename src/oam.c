
#include "defines.h"
#include "oam.h"
#include "main.h"

#include <gb/gb.h>
#include <string.h>

static bool spr_candidates[40];
static uint8_t spr_tiles[40];

/**
 * Frees all sprites
 */
void spr_flush() {
    memset(spr_candidates, 0, 40);
}


/**
 * Hides a sprite. Reappear by `spr_show`
 * @note assumes that tile 0 is blank
 */
void spr_hide(const uint8_t id)
{
    spr_tiles[id] = get_sprite_tile(id);
    set_sprite_tile(id, 0);
}


/**
 * Shows a sprite. `Call only if hidden.`
 * @note assumes the sprite is already hidden
 * @see spr_hide
 */
void spr_show(const uint8_t id)
{
    set_sprite_tile(id, spr_tiles[id]);
}


/**
 * Grabs a free index in oam
 * - call spr_free() to free this sprite
 * @returns a free sprite index in OAM
 */
uint8_t spr_allocate() {
    char i = -1;
    while(spr_candidates[++i])
        if(i > 39)
            print("Out of sprite indexes", 2, 2);

    spr_candidates[i] = true;

    return i;
}


/**
 * Grabes a new OAM index and initializes the sprite's properties
 * @param x unadjusted x coordinate
 * @param y unadjusted y coordinate
 * @param tile tile number for sprite
 */
uint8_t spr_new(const uint8_t x, const uint8_t y, const uint8_t tile)
{
    const u8 id = spr_allocate();

    move_sprite(id, x, y);
    set_sprite_tile(id, tile);

    return id;
}

/**
 * Frees a sprite that is allocated from spr_allocate()
 * - no effect if the sprite is already freed
 * - does not visually clear sprite
 */
inline void spr_free(uint8_t i) {
    move_sprite(i, 0, 0);
    spr_candidates[i] = false;
}