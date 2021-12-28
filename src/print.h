#ifndef PRINT_H
#define PRINT_H

#include "structs.h"

void print_char_window(char, uint8_t, uint8_t);
void print(const unsigned char *, uint8_t, uint8_t);
void print_window(const unsigned char *, uint8_t, uint8_t);
void printInt(uint16_t, uint8_t, uint8_t, const bool);
void print_small_window(const unsigned char *str, uint8_t tx, uint8_t ty);


#endif