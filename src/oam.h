#ifndef OAM_H
#define OAM_H

#include "structs.h"

uint8_t spr_allocate();
uint8_t spr_new(uint8_t x, uint8_t y, uint8_t tile);
void spr_free(uint8_t i);
void spr_flush();

void spr_hide(const uint8_t id);
void spr_show(const uint8_t id);
#endif