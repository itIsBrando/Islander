#include <string.h>
#include <gb/gb.h>

#include "goal.h"
#include "item.h"
#include "../gui/hud.h"
#include "../main.h"



#define MAKE_GOAL(_t, _item_id, _amount) {\
.type=_t,\
.item={.id=_item_id, .count=_amount}\
}

const goal_t all_goals[] = {
    MAKE_GOAL(GOAL_TYPE_COLLECT, ITEM_WOOD, 12),
    MAKE_GOAL(GOAL_TYPE_CRAFT, ITEM_BRIDGE, 4),
    MAKE_GOAL(GOAL_TYPE_COLLECT, ITEM_STONE, 8),
    MAKE_GOAL(GOAL_TYPE_COLLECT, ITEM_STONE, 99), // @todo remove
};

static const goal_t *cur_goal = all_goals;


inline const goal_t *goal_get_current()
{
    return cur_goal;
}


/**
 * Fills a buffer with a string representing a goal
 * @param size of `buffer` in bytes
 * @note does not detect buffer overflow. Goal will never exceed 20chars
 */
void goal_generate_name(const goal_t *goal, char *buffer, uint8_t size)
{
    const char const names[][8] = { "Craft", "Collect", "Mine" };
    const uint8_t len = strlen(names[goal->type]);

    memset(buffer, ' ', size);
    memcpy(buffer, names[goal->type], len);
    
    buffer += len + 1;

    *buffer++ = 'x';

    char digits[5];
    uint8_t num = goal->item.count, i = 0;
    
    // put all digits into a buffer
    do {
        digits[i++] = (num % 10) + FONT_BASE - FONT_START + '0';
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


void goal_print_active()
{
    char const goal_str[20];

    goal_generate_name(goal_get_current(), goal_str, sizeof(goal_str));
    print_window(goal_str, 0, 3);
}


/**
 * Checks to see if the current goal is satified.
 *  Called whenever:
 *   - player mines a tile
 *   - crafts an item (@todo)
 * @returns true if the active goal is completed
 */
bool goal_check_completion()
{
    const goal_t *goal = goal_get_current();

    if(itm_contains(&player.inventory, &goal->item, goal->item.count))
    {
        u8 i = 0;
        cur_goal++;
        
        print_window("Goal complete!   ", 0, 3);

        while(WY_REG > WINDOW_Y - 8)
        {
            hud_scroll_north();
            wait_vbl_done();
        }

        do {
            wait_vbl_done();
        } while(i++ < 90 && !(joypad() & (J_B | J_START)));

        goal_print_active();
        waitjoypad(J_B | J_START);
        
        goal_check_completion(); // recursion :O
        return true;
    } else {
        return false;
    }
}