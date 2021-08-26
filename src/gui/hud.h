#ifndef HUD_H
#define HUD_H

#include "../structs.h"

extern uint8_t hud_cur;

void hud_init();
void hud_print_hotbar();
void hud_move_cur(const direction_t dir);


void hud_remove_item(const uint8_t index);
void hud_add_item(const uint8_t id);

void hud_scroll_north();
void hud_scroll_south();

#endif