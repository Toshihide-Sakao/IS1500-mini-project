#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "vector.h"
#include "player.h"


void draw_player(vec2 pos, double angle, uint32_t *map, uint8_t map2d[8][16])
{
	// set_pos((int)pos.x, (int)pos.y, map);
    // // printf("player pos: %f, %f\n", pos.x, pos.y);
    
    // // draw line for player direction
    // vec2 dir = {pos.x + (cos(angle) * 5), pos.y + (sin(angle) * 5)};
    // draw_line(pos, dir, map);
    draw_rays_3d(pos, angle, map2d, map);
}

void move_player(vec2 *pos, double angle, uint32_t *map) {
    clr_pos((int)pos->x, (int)pos->y, map);
    pos->x += cos(angle);
    pos->y += sin(angle);
}

void rotate_player(double angle){
    angle += 0.1;
}

