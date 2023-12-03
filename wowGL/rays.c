#include <stdio.h>
#include <stdint.h>
#include <math.h>
// #include<GLUT/glut.h>
#include <GL/glut.h>

#include "vector.h"
#include "main.h"

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

void draw_rays_3d(vec2 player_pos, double player_angle, uint8_t map2d[8][16], uint8_t enemy_poses[32][2])
{
    int r, mx, my, dof, enemy_hit = 0, enemy_rendered = 0;
    float rx, ry, ra, x_offset, y_offset, dist, disEnemy;
    uint8_t number_of_enem_rays = 0;

    ra = (float)(player_angle - (FOV / 2)); // fix back 30 degrees
    // ra = player_angle;
    // printf("first fix: ");
    ra = fix_angle(ra);
    // printf("start ra: %f\n", ra);

    float CosPa = cos(player_angle);
    float SinPa = sin(player_angle);

    for (r = 0; r < 30; r++)
    {
        // Horizontal
        dof = 0;                                                    // depth of field
        float disH = 1000000, hx = player_pos.x, hy = player_pos.y; // super high number
        float disEnemyH = 1000000;                                  // super high number

        float Tan = tan(ra);

        if (ra > PI + 0.001 && ra < 2.0 * PI - 0.001) // ray looking up
        {
            float pmy = floor(player_pos.y / 4);
            ry = pmy * 4.0 - 0.001;
            rx = (ry - player_pos.y) / Tan + player_pos.x;
            y_offset = -4;
            x_offset = y_offset / Tan;

            // printf("up... y_offset: %f, x_offset: %f, rx: %f, ry: %f, ra: %f\n", y_offset, x_offset, rx, ry, ra);
        }
        else if (ra < PI - 0.001 && ra > 0.001) // ray looking down
        {
            float pmy = floor(player_pos.y / 4);
            ry = pmy * 4.0 + 4;
            rx = (ry - player_pos.y) / Tan + player_pos.x;
            y_offset = 4;
            x_offset = y_offset / Tan;

            // printf("down... y_offset: %f, x_offset: %f, rx: %f, ry: %f, ra: %f\n", y_offset, x_offset, rx, ry, ra);
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
                // disH = (hx - player_pos.x) * cos(ra) - (hy - player_pos.y) * sin(ra); // maybe abs?
                disH = CosPa * (rx - player_pos.x) + SinPa * (ry - player_pos.y);
                // disH = (rx - player_pos.x) / sin(ra);
            }
            else
            {
                // printf("didnt hit wall\n");
                rx += x_offset;
                ry += y_offset;
                dof += 1;
            }

            if (mx < 16 && my < 8 && mx >= 0 && my >= 0 && map2d[my][mx] == 2 && enemy_hit == 0)
            {
                // draw enemy on 3d game screen
                // printf("enemy hit\n");
                disEnemyH = CosPa * (mx - (player_pos.x / 4)) + SinPa * (my - (player_pos.y / 4));
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
            x_offset = -4;
            y_offset = x_offset * Tan;

            // printf("left... y_offset: %f, x_offset: %f, rx: %f, ry: %f, ra: %f\n", y_offset, x_offset, rx, ry, ra);
        }
        else if (ra < PI / 2.0 || ra > 3.0 * PI / 2.0) // ray looking right
        {
            float pmx = floor(player_pos.x / 4);
            rx = pmx * 4.0 + 4.0;
            ry = (rx - player_pos.x) * Tan + player_pos.y;
            x_offset = 4;
            y_offset = x_offset * Tan;

            // printf("right... y_offset: %f, x_offset: %f, rx: %f, ry: %f, ra: %f\n", y_offset, x_offset, rx, ry, ra);
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
                // disV = ((double)vx - player_pos.x) * cos(ra) - ((double)vy - player_pos.y) * sin(ra);
                disV = CosPa * (rx - player_pos.x) + SinPa * (ry - player_pos.y);
                // disV = (rx - player_pos.x) / cos(ra);
            }
            else
            {
                // printf("didnt hit wall\n");
                rx += x_offset;
                ry += y_offset;
                dof += 1;
            }

            if (mx < 16 && my < 8 && mx >= 0 && my >= 0 && map2d[my][mx] == 2 && enemy_hit == 0)
            {
                // draw enemy on 3d game screen
                // printf("enemy hit\n");
                disEnemyV = CosPa * (mx - (player_pos.x / 4)) + SinPa * (my - (player_pos.y / 4));
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
            dist = disH;
        }
        else
        {
            rx = vx;
            ry = vy;
            dist = disV;
            choose_texture = 2;
        }

        // printf("x_offset: %f, y_offset: %f, dist: %f\n", (rx - player_pos.x), (ry - player_pos.y), dist);

        // GLTINGS
        glColor3f(1.0, 0.3, 0.3); // red
        glLineWidth(10);
        glBegin(GL_LINES);
        glVertex2i((int)(player_pos.x / 2.0 + 96) * 8, (int)(player_pos.y / 2.0 + 1) * 8);
        glVertex2i((rx / 2.0 + 96) * 8, (ry / 2.0 + 1) * 8);
        glEnd();

        // make sure angle is between 0 and 2PI
        // printf("ca fix: ");
        float ca = fix_angle(player_angle - ra);
        dist = dist * cos(ca); // fix fisheye

        float lineH = 96 / dist;     // sq size * screen hight
        lineH = smallest(lineH, 32); // max line height to half of screen

        int lineO = 16 - lineH / 2; // half of screen - half of line height
        // printf("lineH: %f, lineO: %d\n", lineH, lineO);

        draw_rects(r * 3, (int)lineO, r * 3 + choose_texture, (int)(lineH + lineO));

        disEnemy = smallest(disEnemyH, disEnemyV);
        // DEBUG-----------------------
        // if (enemy_hit)
        // {
        //     int delx = (int)(rx / 4) - (player_pos.x / 4);
        //     int dely = (int)(ry / 4) - (player_pos.y / 4);
        //     printf("disEnemy: %f, delx: %d, dely: %d\n", disEnemy, delx, dely);
        // }
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

        // if enemy is hit
        if (enemy_hit && disEnemy < 7)
        {
            number_of_enem_rays = enemy_dist[(int)disEnemy];

            int o;
            // printf("number_of_enem_rays: %d, disEnemy: %d\n", number_of_enem_rays, (int)disEnemy);
            if (number_of_enem_rays > enemy_rendered)
            {
                printf("enemy_rendered: %d, number_of_enem_rays: %d\n", enemy_rendered, number_of_enem_rays);
                for (o = 0; o < 3; o++)
                {
                    glColor3f(1.0, 1.0, 0.3); // red
                    glLineWidth(10);
                    glBegin(GL_LINES);
                    glVertex2i((int)(r * 3) * 8, (int)lineO * 8);
                    glVertex2i((int)(r * 3) * 8 + 1, (int)lineO * 8 + 40);
                    glEnd();
                    printf("hel: %d\n", (30 / (int)(number_of_enem_rays +1)) * enemy_rendered + 2);
                    
                    enemy_rendered++;
                }
            }
        }

        ra += FOV / 30.0;

        // make sure angle is between 0 and 2PI
        // printf("last fix: ");
        ra = fix_angle(ra);
        enemy_hit = 0;
    }
    // printf("last ra: %f\n", ra);
}