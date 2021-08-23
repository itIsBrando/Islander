#ifndef GOAL_H
#define GOAL_H


#include "../structs.h"


typedef enum {
    GOAL_TYPE_CRAFT,
    GOAL_TYPE_COLLECT,
    GOAL_TYPE_MINE
} goal_type_t;


typedef struct {
    goal_type_t type;
    item_t item;
} goal_t;


const goal_t *goal_get_current();
void goal_generate_name(const goal_t *goal, char *buffer, uint8_t size);
void goal_print_active();

bool goal_check_completion();

#endif