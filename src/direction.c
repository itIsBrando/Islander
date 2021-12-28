#include "direction.h"


static const int8_t dx[] = {-1, 1, 0, 0};
static const int8_t dy[] = {0, 0, -1, 1};


inline int8_t dir_get_x(const direction_t dir)
{
    return dx[dir];
}


inline int8_t dir_get_y(const direction_t dir)
{
    return dy[dir];
}
