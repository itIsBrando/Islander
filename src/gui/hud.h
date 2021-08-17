#ifndef HUD_H
#define HUD_H

#include "../structs.h"

extern uint8_t hud_cur;

void hud_draw_hotbar(player_t *);
void hud_move_cur(const direction_t dir);


void hud_remove_item();

#endif