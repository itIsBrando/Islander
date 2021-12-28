#include <gb/gb.h>

#include "../oam.h"
#include "../print.h"
#include "../joypad.h"
#include "../item/item.h"
#include "../item/recipe.h"
#include "../item/goal.h"
#include "../entity/player.h"
#include "crafting.h"
#include "hud.h"


#define WX_TILE_OFFSET 1
#define WY_TILE_OFFSET 6


void cft_draw_requirements(const recipe_t *recipe)
{
    rcpe_draw(recipe, WX_TILE_OFFSET + 9, WY_TILE_OFFSET + 2);       

    print_window("CRAFT", WX_TILE_OFFSET + 10, WY_TILE_OFFSET + 10);
    
    // vvv prints out the current amount of item
    print_window("HAVE:", WX_TILE_OFFSET + 8, WY_TILE_OFFSET + 8);
    printInt(
        itm_get_count(itm_lookup(&player.inventory, recipe->product.id)),
        WX_TILE_OFFSET + 13,
        WY_TILE_OFFSET + 8,
        true
    );
    print_window("need:", WX_TILE_OFFSET + 15 - 3, WY_TILE_OFFSET + 12);

    fill_win_rect(
        WX_TILE_OFFSET + 9,
        WY_TILE_OFFSET + 10,
        1,
        1,
        rcpe_can_craft(&player.inventory, recipe) ? 64 : 65
    );

}


/**
 * Opens the crafting menu
 * @param bench crafting bench that is populated with recipes
 */
void cft_open_menu(const bench_t bench)
{
    uint8_t j, i, maxCurY;
    uint8_t cur_id = spr_allocate();
    const recipe_t *r = bench;

    fill_win_rect(0, WY_TILE_OFFSET - 2, 20, 1, 224);
    print_window("CRAFTING", 10 - 4, WY_TILE_OFFSET - 1);

    for(i = 0; i < CRAFTING_BENCH_SIZE; i++)
    {
        if(!r->product.count)
            break;
        
        itm_draw(&r->product, WX_TILE_OFFSET, WY_TILE_OFFSET + i, CRAFTING_ITEM_FLAGS);
        r++;
    }

    maxCurY = i - 1;

    plr_hide();

    for(i = 0; i < WINDOW_Y / 2; i++)
    {
        hud_scroll_north();
        hud_scroll_north();
        wait_vbl_done();
    }

    cft_draw_requirements(bench);
    move_sprite(cur_id, 8, WY_TILE_OFFSET * 8 + 16);
    set_sprite_tile(cur_id, (u8)FONT_BASE - (u8)FONT_START + (u8)'>');

    i = 0;

    do {
        j = joypad();

        if((j & J_UP) && i > 0) {
            cft_draw_requirements(&bench[--i]);
            scroll_sprite(cur_id, 0, -8);
        } else if((j & J_DOWN) && i < maxCurY) {
            cft_draw_requirements(&bench[++i]);
            scroll_sprite(cur_id, 0, 8);
        }

        // check craftable
        if((j & J_A) && rcpe_can_craft(&player.inventory, &bench[i]))
        {
            rcpe_craft(&bench[i]);
            cft_draw_requirements(&bench[i]);
            goal_check_completion();
        }

        waitjoypad(J_DOWN | J_UP | J_A);

        wait_vbl_done();
    } while(!(j & J_B));

    spr_free(cur_id);
    plr_draw_active_item();

    plr_show();

    for(i = 0; i < WINDOW_Y / 2; i++)
    {
        hud_scroll_south();
        hud_scroll_south();
        wait_vbl_done();
    }
}