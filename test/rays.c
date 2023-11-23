#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "vector.h"

#define PI 3.14159265535
#define P2 PI / 2
#define P3 3 * PI / 2

void draw_rays_3d(vec2 player_pos, double player_angle, uint8_t map2d[8][16], uint32_t *map)
{
    int r, mx, my, dof;
    float rx, ry, ra, xo, yo, disT;

    ra = (float)(player_angle - PI / 6);
    if (ra < 0)
    {
        ra += 2 * PI;
    }
    else
    {
        ra -= 2 * PI;
    }

    for (r = 0; r < 10; r++)
    {
        // Horizontal
        dof = 0;
        float disH = 1000000, hx = player_pos.x, hy = player_pos.y; // super high number

        float aTan = -1.0 / tan(ra);
        if (ra > PI + 0.0001) // ray looking up
        {
            ry = player_pos.y - (((int)player_pos.x / 16) * 16) - 0.0001;
            rx = (player_pos.y - ry) * aTan + player_pos.x;
            yo = -4;
            xo = -yo * aTan;
        }
        else if (ra < PI - 0.0001) // ray looking down
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
            // printf("e rx: %f, ry: %f\n", rx, ry);
            // printf("e mx: %d, my: %d\n", mx, my);
            // printf("pos info map: %d, 2d: %d\n", get_pos(mx * 4, my * 4, map), map2d[my][mx]);
            if (mx < 96 && my < 32 && mx >= 0 && my >= 0 && get_pos(mx * 4, my * 4, map) == 1) // hit wall
            {
                // printf("hit %d, %d\n", mx, my);
                dof = 8;
                hx = rx;
                hy = ry;
                // disH = vec_dist(player_pos, (vec2){hx, hy});
                disH = (hx - player_pos.x) * cos(ra) - (hy - player_pos.y) * sin(ra);

            }
            else
            {
                // printf("no  %d, %d\n", mx, my);
                rx += xo;
                ry += yo;
                dof += 1;
            }
            // printf("rx: %f, ry: %f\n", rx, ry);
        }

        // Vertical
        dof = 0;
        float disV = 1000000, vx = player_pos.x, vy = player_pos.y; // super high number

        float nTan = -tan(ra);
        if (ra > P2 && ra < P3) // ray looking left
        {
            // printf("left: %f\n", ra);
            rx = player_pos.x - (((int)player_pos.y / 8) * 8) - 0.0001;
            ry = (player_pos.x - rx) * nTan + player_pos.y;
            xo = -4;
            yo = -xo * nTan;
        }
        else if (ra < P2 || ra > P3) // ray looking right
        {
            // printf("right: %f\n", ra);
            rx = player_pos.x - (((int)player_pos.y / 8) * 8) + 16;
            ry = (player_pos.x - rx) * nTan + player_pos.y;
            xo = 4;
            yo = -xo * nTan;
        }
        else // looking up or down
        {
            // printf("up or down: %f\n", ra);
            rx = player_pos.x;
            ry = player_pos.y;
            dof = 8;
        }

        while (dof < 8)
        {
            mx = (int)(rx) / 4;
            my = (int)(ry) / 4;
            // printf("e rx: %f, ry: %f\n", rx, ry);
            // printf("e mx: %d, my: %d\n", mx, my);
            // printf("pos info map: %d, 2d: %d\n", get_pos(mx * 4, my * 4, map), map2d[my][mx]);
            if (mx < 96 && my < 32 && mx >= 0 && my >= 0 && map2d[my][mx] == 1) // hit wall
            {
                dof = 8;
                vx = rx;
                vy = ry;
                // disV = vec_dist(player_pos, (vec2){vx, vy});
                disV = ((double)vx - player_pos.x) * cos(ra) - ((double)vy - player_pos.y) * sin(ra);

            }
            else
            {
                rx += xo;
                ry += yo;
                dof += 1;
            }
            // printf("rx: %f, ry: %f\n", rx, ry);
        }

        // drawing
        if (disH < disV)
        {
            disT = disH;
            // draw_line(player_pos, (vec2){hx, hy}, map);
        }
        else
        {
            disT = disV;
            // draw_line(player_pos, (vec2){vx, vy}, map);
        }

        //Draw 3D
        float ca = player_angle - ra;
        disT = disT * cos(ca);

        if (ca < 0)
        {
            ca += 2 * PI;
        }
        else if (ca > 2 * PI)
        {
            ca -= 2 * PI;
        }
        
        float lineH = (4 * 32) / disT;
        if (lineH > 32)
        {
            lineH = 32;
        }
        int lineO = 16 - lineH / 2;
        printf("r: %d, disT: %f, lineH: %f, lineO: %d\n", r, disT, lineH, lineO);
        draw_rects(r*8, (int)lineO, r*8 +8, (int)(lineH + lineO), map);        

        ra += PI / 30.0;
        if (ra < 0)
        {
            ra += 2 * PI;
        }
        else
        {
            ra -= 2 * PI;
        }
    }
}