#include "joypad.h"
#include <gb/gb.h>


/**
 * Better alternative of `waitpad`. Power efficient. Waits until button(s) is/are released
 */
void waitjoypad(const uint8_t mask)
{
    while((joypad() & mask) != 0)
        wait_vbl_done();
}


/**
 * Waits until one or multiple buttons are pressed and released
 * @param mask button or buttons to wait for depression and releasing
 */
void waitPressed(const uint8_t mask)
{
    waitjoypad(mask);

    while((joypad() & mask) == 0)
        wait_vbl_done();

    waitjoypad(mask);
}
