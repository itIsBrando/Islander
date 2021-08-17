
#include "goal.h"
#include "item.h"
#include <string.h>



#define DEFINE_GOAL(_t, _item_id, _amount) {\
.type=_t,\
.item={.id=_item_id, .count=_amount}\
}

const goal_t all_goals[] = {
    DEFINE_GOAL(GOAL_TYPE_COLLECT, ITEM_WOOD, 12)
};


inline const goal_t *goal_get_current()
{
    return all_goals;
}

/**
 * Fills a buffer with a string representing a goal
 */
void goal_generate_name(goal_t *goal, char *buffer, uint8_t size)
{
    const char names[][8] = { "Craft", "Collect", "Mine" };

    memset(buffer, ' ', size);
    memcpy(buffer, names[goal->type], 7 * sizeof(char));
    
    buffer += 8;

    *buffer++ = 'x';

    char digits[5];
    uint8_t num = goal->item.count, i = 0;
    
    // put all digits into a buffer
    do {
        digits[i++] = (num % 10) + 0x90;
        num /= 10;
    } while(num > 0);

    do {
        *buffer = digits[--i];
        buffer++;
    } while(i);

    const char *name = itm_get_name(&goal->item);
    strcpy(++buffer, name);

    *(buffer + strlen(name)) = 0;
}