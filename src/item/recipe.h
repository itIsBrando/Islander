#ifndef RECIPE_H
#define RECIPE_H

#include "../structs.h"

typedef struct {
    item_t requirements[3];
    item_t product;
} recipe_t;

typedef recipe_t bench_t[CRAFTING_BENCH_SIZE];

extern const bench_t WORKBENCH;

bool rcpe_can_craft(const inv_t *inv, const recipe_t *recipe);
bool rcpe_craft(const recipe_t *recipe);
void rcpe_draw(const recipe_t *recipe, uint8_t x, uint8_t y);


#endif