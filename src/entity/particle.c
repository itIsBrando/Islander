#include <gb/gb.h>
#include <string.h>

#include "../structs.h"
#include "../oam.h"
#include "particle.h"


typedef struct {
    uint8_t id1; // for the `+` sign
    uint8_t id2; // for the number
    uint8_t frames_active; // each particle dies after 120 frames
} particle_t;

#define EFFECTS_MAX 3

particle_t eff_list[EFFECTS_MAX];
uint8_t eff_active = 0;

/**
 * Creates a new particle effect for the collection of an item
 * @param count number of items collected [0-4]
 */
void eff_new(const uint8_t x, const uint8_t y, const uint8_t count)
{
    if(eff_active >= EFFECTS_MAX - 1)
        return;

    particle_t *p = &eff_list[eff_active++];

    p->id1 = spr_new(x + 4, y + 6, (u8)FONT_BASE - (u8)FONT_START + (u8)'+');
    p->id2 = spr_new(x + 10, y + 6, FONT_BASE - FONT_START + '0' + count);
    p->frames_active = 0;
}


/**
 * Should be called every frame
 */
void eff_update()
{
    particle_t *p = eff_list;
    
    for(uint8_t i = 0; i < eff_active; i++)
    {
        if(p->frames_active++ >= 100) {
            // delete my dude
            eff_active--;
            move_sprite(p->id1, 0, 0);
            move_sprite(p->id2, 0, 0);
            spr_free(p->id1);
            spr_free(p->id2);
            memcpy(&eff_list[i], &eff_list[i + 1], (eff_active - i) * sizeof(eff_list[0]));
        } else if((p->frames_active & 3) == 3) {
                scroll_sprite(p->id1, 0, -1);
                scroll_sprite(p->id2, 0, -1);
        }

        p++;
    }
}