#include <stdio.h>
#include <stdint.h>

#include "vector.h"
#include "player.h"


void draw_player(vec2 pos, vec2 dir, uint32_t *map)
{
	set_pos((int)pos.y, (int)pos.x, map);
    // printf("player pos: %f, %f\n", pos.x, pos.y);
    for (int i = 1; i < 5; i++)
    {
        set_pos((int)(pos.y + dir.y *i), (int)(pos.x + dir.x * i), map);
    }
}

void move_player(vec2 *pos, vec2 dir, uint32_t *map) {

}

void rotate_player(vec2 *dir, float angle){

}

