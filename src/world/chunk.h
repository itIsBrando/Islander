#ifndef CHUNK_T
#define CHUNK_T

#include "../structs.h"

void cnk_island_load(const uint8_t islandNumber, u8 xx, u8 yy);

void cnk_init();
void cnk_draw_active();


bool cnk_north();
bool cnk_load_row(const direction_t dir);


void cnk_active_write(u8 x, u8 y, u8 tile);

#endif