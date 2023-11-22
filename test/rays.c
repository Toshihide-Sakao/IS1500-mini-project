#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "vector.h"

#define PI 3.14159265535
#define P2 PI / 2
#define P3 3* PI / 2

void draw_rays_3d(vec2 player_pos, double player_angle, uint8_t map2d[8][16], uint32_t *map)
{
    int r, mx, my, mp, dof;
    float rx, ry, ra, xo, yo;

    ra = (float)player_angle;

    for (r = 0; r < 1; r++)
    {
        // Horizontal
        dof = 0;
        float aTan = -1.0 / tan(ra);
        if (ra > PI) // ray looking up
        {
            ry = player_pos.y - (((int)player_pos.x / 16) * 16) - 0.0001;
            rx = (player_pos.y - ry) * aTan + player_pos.x;
            yo = -4;
            xo = -yo * aTan;
        }
        else if (ra < PI) // ray looking down
        {
            ry = player_pos.y - (((int)player_pos.x / 16) * 16) + 8;
            rx = (player_pos.y - ry) * aTan + player_pos.x;
            yo = 4;
            xo = -yo * aTan;
        }
        else // ray looking left/right
        {
            rx = player_pos.x;
            ry = player_pos.y;
            dof = 8;
        }

        while (dof < 8)
        {
            mx = (int)(rx) / 4;
            my = (int)(ry) / 4;
            printf("e rx: %f, ry: %f\n", rx, ry);
            // printf("e mx: %d, my: %d\n", mx, my);
            printf("pos info map: %d, 2d: %d\n", get_pos(mx * 4, my * 4, map), map2d[my][mx]);
            if (mx < 96 && my < 32 && mx >= 0 && my >= 0 && get_pos(mx * 4, my * 4, map) == 1) // hit wall
            {
                printf("hit %d, %d\n", mx, my);
                dof = 8;
            }
            else
            {
                printf("no  %d, %d\n", mx, my);
                rx += xo;
                ry += yo;
                dof += 1;
            }
            printf("rx: %f, ry: %f\n", rx, ry);
        }
        // printf("mx: %f, my: %f\n", mx, my);

        // Vertical
        // dof = 0;
        // float nTan = -tan(ra);
        // if (ra > P2 && ra < P3) // ray looking left
        // {
        //     printf("left: %f\n", ra);
        //     rx = player_pos.x - (((int)player_pos.y / 8) * 8) - 0.0001;
        //     ry = (player_pos.x - rx) * nTan + player_pos.y;
        //     xo = -4;
        //     yo = -xo * nTan;
        // }
        // else if (ra < P2 || ra > P3) // ray looking right
        // {
        //     printf("right: %f\n", ra);
        //     rx = player_pos.x - (((int)player_pos.y / 8) * 8) + 16;
        //     ry = (player_pos.x - rx) * nTan + player_pos.y;
        //     xo = 4;
        //     yo = -xo * nTan;
        // }
        // else // looking up or down
        // {
        //     printf("up or down: %f\n", ra);
        //     rx = player_pos.x;
        //     ry = player_pos.y;
        //     dof = 8;
        // }

        // while (dof < 8)
        // {
        //     mx = (int)(rx) / 4;
        //     my = (int)(ry) / 4;
        //     printf("e rx: %f, ry: %f\n", rx, ry);
        //     // printf("e mx: %d, my: %d\n", mx, my);
        //     printf("pos info map: %d, 2d: %d\n", get_pos(mx * 4, my * 4, map), map2d[my][mx]);
        //     if (mx < 96 && my < 32 && mx >= 0 && my >= 0 && get_pos(mx * 4, my * 4, map) == 1) // hit wall
        //     {
        //         printf("hit %d, %d\n", mx, my);
        //         dof = 8;
        //     }
        //     else
        //     {
        //         printf("no  %d, %d\n", mx, my);
        //         rx += xo;
        //         ry += yo;
        //         dof += 1;
        //     }
        //     printf("rx: %f, ry: %f\n", rx, ry);
        // }
        // printf("mx: %f, my: %f\n", mx, my);
        draw_line(player_pos, (vec2){rx, ry}, map);
    }
}