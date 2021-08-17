#ifndef PLAYER_H
#define PLAYER_H

#include "../structs.h"

#define PLR_SCRN_X ((160 / 2 - 4) + 8)
#define PLR_SCRN_Y ((144 / 2 - 4) + 16)

void plr_init();
void plr_update(const uint8_t);
void plr_move(const direction_t dir);
void plr_interact();

#endif