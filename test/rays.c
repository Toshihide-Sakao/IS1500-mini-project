#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "vector.h"

#define PI 3.14159265535
#define sqS 4
#define DOFLIMIT 8

#define FOV PI / 3.0

const uint8_t const enemy_dist[7] = {
    30, 26, 21, 14, 11, 8, 7};

float smallest(float a, float b)
{
    if (a < b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

float fix_angle(float angle)
{
    if (angle < 0.0)
    {
        angle += 2.0 * PI;
    }
    else if (angle >= 2.0 * PI)
    {
        angle -= 2.0 * PI;
    }

    return angle;
}

float abs_myting(float x)
{
    if (x < 0)
    {
        return -x;
    }
    else
    {
        return x;
    }
}

void draw_rays_3d(vec2 player_pos, double player_angle, uint8_t map2d[8][16], uint32_t *map)
{
    int r, mx, my, dof, enemy_hit = 0, enemy_rendered = 0;
    float rx, ry, ra, xo, yo, disT, disEnemy;
    uint8_t number_of_enem_rays = 0;

    ra = (float)(player_angle - (FOV / 2)); // fix back 30 degrees
    // ra = player_angle;
    // printf("first fix: ");
    ra = fix_angle(ra);
    // printf("start ra: %f\n", ra);

    float CosPa = cos(player_angle);
    float SinPa = sin(player_angle);

    // for (r = 17; r < 18; r++)
    for (r = 0; r < 30; r++)
    {
        // Horizontal
        dof = 0;                                                    // depth of field
        float disH = 1000000, hx = player_pos.x, hy = player_pos.y; // super high number

        float disEnemyH = 1000000; // super high number

        float Tan = tan(ra);
        if (r == 17)
        {
            printf("ra: %f\n", ra);
        }

        // Check if angle is pi/2 or 0/2pi (then division by 0 can happen)

        if (ra > PI + 0.001 && ra < 2.0 * PI - 0.001) // ray looking up
        {
            float pmy = floor(player_pos.y / 4);
            ry = pmy * 4.0 - 0.001;
            rx = (ry - player_pos.y) / Tan + player_pos.x;
            yo = -4;
            xo = yo / Tan;
            if (r == 17)
            {
                printf("17: ");
                printf("up... yo: %f, xo: %f, rx: %f, ry: %f, ra: %f\n", yo, xo, rx, ry, ra);
                printf("Tan: %f\n", Tan);
            }
        }
        else if (ra < PI - 0.001 && ra > 0.001) // ray looking down
        {
            float pmy = floor(player_pos.y / 4);
            ry = pmy * 4.0 + 4;
            rx = (ry - player_pos.y) / Tan + player_pos.x;
            yo = 4;
            xo = yo / Tan;

            // if (r == 17)
            // {
            //     printf("17: ");
            //     printf("down... yo: %f, xo: %f, rx: %f, ry: %f, ra: %f\n", yo, xo, rx, ry, ra);
            // }
        }
        else // ray looking left/right
        {
            // printf("looking left/right\n");
            rx = player_pos.x;
            ry = player_pos.y;
            dof = DOFLIMIT;
        }

        while (dof < DOFLIMIT)
        {
            mx = (int)(rx) / 4;
            my = (int)(ry) / 4;
            // printf("rx: %f, ry: %f, mx: %d, my: %d\n", rx, ry, mx, my);

            int check = (mx < 16 && my < 8 && mx >= 0 && my >= 0 && map2d[my][mx] == 1) || (mx >= 16 || mx <= 0 || my >= 8 || my <= 0);
            // int check = (mx < 16 && my < 8 && mx >= 0 && my >= 0 && map2d[my][mx] == 1);
            if (check) // hit wall
            {
                // printf("hit wall\n");
                dof = DOFLIMIT;
                hx = rx;
                hy = ry;
                disH = CosPa * (rx - player_pos.x) + SinPa * (ry - player_pos.y);
                // disH = (hx - player_pos.x) * cos(ra) - (hy - player_pos.y) * sin(ra);
                if (r == 17)
                {
                    printf("17: ");
                    printf("disH: %f, dx: %f, rx: %f\n", disH, (rx - player_pos.x), rx);
                }
            }
            else
            {
                // printf("didnt hit wall\n");
                rx += xo;
                ry += yo;
                dof += 1;

                if (r == 17)
                {
                    printf("17: ");
                    printf("dx: %f\n", (rx - player_pos.x));
                }
            }

            if (mx < 16 && my < 8 && mx >= 0 && my >= 0 && map2d[my][mx] == 2 && enemy_hit == 0)
            {
                // draw enemy on 3d game screen
                // printf("enemy hit\n");
                // printf("rx: %f, ry: %f, mx: %d, my: %d\n", rx, ry, mx, my);
                disEnemyH = CosPa * (mx - (player_pos.x / 4)) + SinPa * (my - (player_pos.y / 4));
                // disEnemyH = cos(ra) * (mx - (player_pos.x / 4)) - sin(ra) * (my - (player_pos.y / 4));

                enemy_hit = 1;
            }
        }

        // Vertical
        dof = 0;
        float disV = 1000000, vx = player_pos.x, vy = player_pos.y; // super high number
        float disEnemyV = 1000000;                                  // super high number

        if (ra > PI / 2.0 + 0.0001 && ra < 3.0 * PI / 2.0 - 0.0001) // ray looking left
        {
            float pmx = floor(player_pos.x / 4);
            rx = pmx * 4.0 - 0.001;
            ry = (rx - player_pos.x) * Tan + player_pos.y;
            xo = -4;
            yo = xo * Tan;

            // printf("left... yo: %f, xo: %f, rx: %f, ry: %f, ra: %f\n", yo, xo, rx, ry, ra);
        }
        else if (ra < PI / 2.0 || ra > 3.0 * PI / 2.0) // ray looking right
        {
            float pmx = floor(player_pos.x / 4);
            rx = pmx * 4.0 + 4.0;
            ry = (rx - player_pos.x) * Tan + player_pos.y;
            xo = 4;
            yo = xo * Tan;

            // printf("right... yo: %f, xo: %f, rx: %f, ry: %f, ra: %f\n", yo, xo, rx, ry, ra);
        }
        else // looking up or down
        {
            rx = player_pos.x;
            ry = player_pos.y;
            dof = DOFLIMIT;
        }

        while (dof < DOFLIMIT)
        {
            mx = (int)(rx) / 4;
            my = (int)(ry) / 4;
            // printf("rx: %f, ry: %f, mx: %d, my: %d\n", rx, ry, mx, my);

            int check = (mx < 16 && my < 8 && mx >= 0 && my >= 0 && map2d[my][mx] == 1) || (mx >= 16 || mx <= 0 || my >= 8 || my <= 0);
            // int check = (mx < 16 && my < 8 && mx >= 0 && my >= 0 && map2d[my][mx] == 1);
            if (check) // hit wall
            {
                // printf("hit wall\n");
                dof = DOFLIMIT;
                vx = rx;
                vy = ry;
                disV = CosPa * (rx - player_pos.x) + SinPa * (ry - player_pos.y);
                // disV = ((double)vx - player_pos.x) * cos(ra) - ((double)vy - player_pos.y) * sin(ra);
            }
            else
            {
                // printf("didnt hit wall\n");
                rx += xo;
                ry += yo;
                dof += 1;
            }

            if (mx < 16 && my < 8 && mx >= 0 && my >= 0 && map2d[my][mx] == 2 && enemy_hit == 0)
            {
                // draw enemy on 3d game screen
                disEnemyV = CosPa * (mx - (player_pos.x / 4)) + SinPa * (my - (player_pos.y / 4));
                // disEnemyV = cos(ra) * (mx - (player_pos.x / 4)) - sin(ra) * (my - (player_pos.y / 4));

                enemy_hit = 1;
            }
        }

        // choose shortest distance
        int choose_texture = 1;
        disV = abs_myting(disV);
        disH = abs_myting(disH);
        if (disH < disV)
        {
            rx = hx;
            ry = hy;
            disT = disH;
        }
        else
        {
            rx = vx;
            ry = vy;
            disT = disV;
            choose_texture = 2;
        }

        // GLTINGS
        // glColor3f(1.0, 0.3, 0.3); // red
        // glLineWidth(10);
        // glBegin(GL_LINES);
        // glVertex2i((int)(player_pos.x / 2.0 + 96) * 8, (int)(player_pos.y / 2.0 + 1) * 8);
        // glVertex2i((rx / 2.0 + 96) * 8, (ry / 2.0 + 1) * 8);
        // glEnd();

        // make sure angle is between 0 and 2PI
        // printf("ca fix: ");
        float ca = fix_angle(player_angle - ra);
        disT = disT * cos(ca); // fix fisheye

        float lineH = 96 / disT;     // sq size * screen hight
        lineH = smallest(lineH, 31); // max line height to half of screen

        int lineO = 16 - lineH / 2; // half of screen - half of line height
        // printf("lineH: %f, lineO: %d\n", lineH, lineO);
        int col;
        for (col = 0; col < 3; col++)
        {
            map[r * 3 + col] = 0;
        }
        draw_rects(r * 3, (int)lineO, r * 3 + choose_texture, (int)(lineH + lineO), map);

        disEnemy = smallest(disEnemyH, disEnemyV);
        // if enemy is hit
        if (enemy_hit && disEnemy < 7)
        {
            number_of_enem_rays = enemy_dist[(int)disEnemy];

            int o;
            // printf("number_of_enem_rays: %d, disEnemy: %d\n", number_of_enem_rays, (int)disEnemy);
            // TODO: fix scaling with enemy.
            // printf("enemy_rendered: %d, number_of_enem_rays: %d\n", enemy_rendered, number_of_enem_rays);
            if (number_of_enem_rays == 30 && enemy_rendered <= 28 * 2)
            {
                float scale = 0.5;
                for (o = 0; o < 3; o++)
                {
                    draw_enemy_x(r * 3 + o, (int)(scale * enemy_rendered), map);
                    enemy_rendered++;
                    printf("fat:  number_of_enem_rays: %d, enemy_rende: %d, index: %d\n", number_of_enem_rays, enemy_rendered, (int)(scale * enemy_rendered));
                }
            } 
            else if (enemy_rendered == number_of_enem_rays - 3)
            {
                int sc = (int)((26 - number_of_enem_rays) / 2.0) / 2;
                for (o = 0; o < 3; o++)
                {
                    draw_enemy_scalable(r * 3 + o, sc, 25 + o, map);
                    enemy_rendered++;
                    printf("last:  number_of_enem_rays: %d, index: %d\n", number_of_enem_rays, 27 + o);
                }
            }

            else if (number_of_enem_rays > enemy_rendered)
            {
                int sc = (int)((26 - number_of_enem_rays) / 2.0) / 2;
                // printf("enemy_rendered: %d, number_of_enem_rays: %d\n", enemy_rendered, number_of_enem_rays);
                for (o = 0; o < 3; o++)
                {
                    draw_enemy_scalable(r * 3 + o, sc, (int)((30.0 / (number_of_enem_rays + 1)) * enemy_rendered) + 2, map);
                    enemy_rendered++;
                    printf("not:   number_of_enem_rays: %d, enemy_rende: %d, index: %d\n", number_of_enem_rays, enemy_rendered, (int)((30.0 / (number_of_enem_rays + 1)) * enemy_rendered) + 2);
                }
            }
        }

        // enemy on 5,7
        // d: 6 is    7, *3
        //    5       8
        //    4.6     9
        //    4.35    10
        //    3.75    11
        //    3.3     12
        //    3.04    13
        //    3.74    14
        //    2.77    15
        //    2.62    16
        //    2.5     17
        //    2.3     18
        //    2.2     19
        //    2.1     20
        //    1.95    22
        //    1.8     24
        // ------------------------------

        ra += FOV / 30.0;
        // printf("diff: %f\n", PI / 3.0 / 40.0);

        // make sure angle is between 0 and 2PI
        // printf("last fix: ");
        ra = fix_angle(ra);
        enemy_hit = 0;
    }
    // enemy_rendered = 0;
    // printf("last ra: %f\n", ra);
}