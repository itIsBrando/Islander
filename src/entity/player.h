#ifndef PLAYER_H
#define PLAYER_H

#include "../structs.h"

#define PLR_SCRN_X ((160 / 2 - 4) + 8)
#define PLR_SCRN_Y ((144 / 2 - 4) + 16)

void plr_init();
void plr_update(const uint8_t);
void plr_move(const direction_t dir);
void plr_interact();
void plr_hide();
void plr_show();

uint8_t plr_get_cursor_sprite_id();

item_t *plr_get_active_item();
void plr_sub_from_inventory(const uint8_t id, const int8_t count);
void plr_add_id_to_inventory(const uint8_t id, const int8_t count);

#endif