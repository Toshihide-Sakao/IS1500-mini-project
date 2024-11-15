#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "vector.h"
#include "player.h"

vec2 last_pos = {0, 0};
vec2 last_dir = {0, 0};

void draw_player(vec2 pos, double angle, uint32_t *map, uint8_t map2d[8][16])
{
    clr_pos((int)last_pos.x, (int)last_pos.y, map);
    clr_pos((int)last_dir.x, (int)last_dir.y, map);

    vec2 player_mpos = {(pos.x * 2) / 4 + 96, (pos.y *2) / 4 + 8};
    set_pos((int)player_mpos.x, (int)player_mpos.y, map);
    
    // // draw line for player direction
    vec2 dir = {player_mpos.x + (cos(angle) * 2), player_mpos.y + (sin(angle) * 2)};
    set_pos((int)dir.x, (int)dir.y, map);
    
    draw_rays_3d(pos, angle, map2d, map);

    last_pos = player_mpos;
    last_dir = dir;
}

void move_player(vec2 *pos, double angle, uint32_t *map) {
    pos->x += cos(angle) / 5;
    pos->y += sin(angle) / 5;
}

void rotate_player(double *angle, int whichway){
    *angle += 0.05 * whichway;
}

