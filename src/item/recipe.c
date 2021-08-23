#include <gb/gb.h>

#include "recipe.h"
#include "item.h"
#include "../entity/player.h"


#define MAKE_RECIPE(i1_id, i1_count, i2_id, i2_count, i3_id, i3_count, product_id, product_count) {\
.requirements={\
[0]={.id=i1_id, .count=i1_count},\
[1]={.id=i2_id, .count=i2_count},\
[2]={.id=i3_id, .count=i3_count},\
},\
.product={.id=product_id, .count=product_count}\
}

const bench_t WORKBENCH = {
    MAKE_RECIPE(0, 1, 0, 1, 0, 1, ITEM_COAL, 3),
    MAKE_RECIPE(ITEM_WOOD, 6, 0, 0, 0, 0, ITEM_BRIDGE, 2),
};


/**
 * @returns true if the player can craft a recipe
 */
bool rcpe_can_craft(const inv_t *inv, const recipe_t *recipe)
{
    const item_t *item = recipe->requirements;

    for(uint8_t i = 0; i < 3; i++)
    {
        if(item->count != 0) {
            return true;
        } else if(!itm_contains(inv, item, item->count)) {
            return false;
        }

        item++;
    }

    return true;
}


/**
 * Crafts a recipe if possible
 * @returns whether or not crafting was possible
 */
bool rcpe_craft(const recipe_t *recipe)
{
    if(!rcpe_can_craft(&player.inventory, recipe))
        return false;

    item_t *item = recipe->requirements;

    for(uint8_t i = 0; i < 3; i++)
    {
        if(item->count != 0)
            plr_sub_from_inventory(item->id, item->count);

        item++;
    }

    plr_add_id_to_inventory(recipe->product.id, recipe->product.count);

    return true;
}


/**
 * @param recipe recipe to draw
 * @param x tile x
 * @param y tile y
 */
void rcpe_draw(const recipe_t *recipe, uint8_t x, uint8_t y)
{
    for(u8 i = 0; i < 3; i++)
    {
        if(recipe->requirements[i].count)
            itm_draw(&recipe->requirements[i], x, y + i, DRAW_ITEM_TILE_GRID | DRAW_ITEM_NAME | DRAW_ITEM_COUNT);
        else
            fill_win_rect(x, y + i, 9, 1, 0);
    }
}