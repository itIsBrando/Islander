#include "shake.h"

#include "../defines.h"
#include "../entity/player.h"
#include <gb/gb.h>

int8_t shake_x = 0;


inline void shk_update()
{
    if(shake_x) {
        int8_t off;
        if(shake_x < 0) {
            off = -1;
            shake_x++;
        } else {
            off = 1;
            shake_x--;
        }

        int8_t xOff = shake_x * 2 + off;
        scroll_bkg(xOff, 0);
        scroll_sprite(plr_get_cursor_sprite_id(), xOff, 0);
        move_sprite(player.id, PLR_SCRN_X + shake_x, PLR_SCRN_Y);
        
        shake_x = -shake_x;
    }

}



/**
 * Screen shake
 * @param instensity value > 0
 */
inline void shake(uint8_t intensity)
{
    if(shake_x == 0)
    {
        shake_x = intensity;    
        SCX_REG -= intensity;
        move_sprite(player.id, PLR_SCRN_X - intensity, PLR_SCRN_Y);
        scroll_sprite(plr_get_cursor_sprite_id(), -intensity, 0);
    }
}
