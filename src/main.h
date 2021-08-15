#ifndef MAIN_H
#define MAIN_H

#include "defines.h"
#include "data/sprite.h"
#include "structs.h"

void initGame();

void empty_oam();
void clear_bg();
void waitjoypad(const uint8_t);
void waitPressed(const uint8_t);

void print(const unsigned char *, uint8_t, uint8_t);
void print_window(const unsigned char *, uint8_t, uint8_t);
void printInt(uint16_t, uint8_t, uint8_t, const bool);
void debug(char *);
void shake(uint8_t);

int8_t dir_get_x(const direction_t dir);
int8_t dir_get_y(const direction_t dir);

#endif