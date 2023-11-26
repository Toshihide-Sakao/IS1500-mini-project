#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <GL/glut.h>

#include "vector.h"

#define PI 3.14159265535
#define P2 PI / 2
#define P3 3 * PI / 2

float smallest(float a, float b)
{
    if (a < b) { return a; }
    else { return b; }
}

float fix_angle(float angle)
{
    if (angle < 0) { angle += 2 * PI; }
    else if (angle > 2 * PI) { angle -= 2 * PI; }
    return angle;
}

float abs_myting(float x)
{
    if (x < 0) { return -x; }
    else { return x; }
}

void draw_rays_3d(vec2 player_pos, double player_angle, uint8_t map2d[8][16])
{
    int r, mx, my, dof;
    float rx, ry, ra, xo, yo, disT;

    ra = (float)(player_angle - (PI / 6)); // fix back 30 degrees
    ra = fix_angle(ra);

    for (r = 0; r < 41; r++)
    {
        // Horizontal
        dof = 0; // depth of field
        float disH = 1000000, hx = player_pos.x, hy = player_pos.y; // super high number

        float aTan = -1.0 / tan(ra);
        if (ra > PI + 0.0001) // ray looking up
        {
            ry = (((int)player_pos.y / 4) * 4) - 0.01; // unsure could be 8, I think it is square size so 4
            rx = (player_pos.y - ry) * aTan + player_pos.x;
            yo = -4; // square size
            xo = -yo * aTan;
        }
        else if (ra < PI - 0.0001) // ray looking down
        {
            ry = (((int)player_pos.y / 4) * 4) + 8; // I think these numbers are right
            rx = (player_pos.y - ry) * aTan + player_pos.x;
            yo = 4; // square size
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

            // int check = (mx < 16 && my < 8 && mx >= 0 && my >= 0 && map2d[my][mx] == 1) || (mx >= 16 || mx <= 0 || my >= 8 || my <= 0);
            int check = (mx < 16 && my < 8 && mx >= 0 && my >= 0 && map2d[my][mx] == 1);
            if (check) // hit wall
            {
                dof = 8;
                hx = rx;
                hy = ry;
                disH = (hx - player_pos.x) * cos(ra) - (hy - player_pos.y) * sin(ra);
            }
            else
            {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }

        // Vertical
        dof = 0;
        float disV = 1000000, vx = player_pos.x, vy = player_pos.y; // super high number

        float nTan = -tan(ra);
        if (ra > P2 && ra < P3) // ray looking left
        {
            rx = (((int)player_pos.x / 4) * 4) - 0.01;
            ry = (player_pos.x - rx) * nTan + player_pos.y;
            xo = -4;
            yo = -xo * nTan;
        }
        else if (ra < P2 || ra > P3) // ray looking right
        {
            // printf("right: %f\n", ra);
            rx = (((int)player_pos.x / 4) * 4) + 16;
            ry = (player_pos.x - rx) * nTan + player_pos.y;
            xo = 4;
            yo = -xo * nTan;
        }
        else // looking up or down
        {
            rx = player_pos.x;
            ry = player_pos.y;
            dof = 8;
        }

        while (dof < 8)
        {
            mx = (int)(rx) / 4;
            my = (int)(ry) / 4;
            printf("rx: %f, ry: %f\n", rx, ry);
            printf("mx: %d, my: %d\n", mx, my);
            
            // int check = (mx < 16 && my < 8 && mx >= 0 && my >= 0 && map2d[my][mx] == 1) || (mx >= 16 || mx <= 0 || my >= 8 || my <= 0);
            int check = (mx < 16 && my < 8 && mx >= 0 && my >= 0 && map2d[my][mx] == 1);
            if (check) // hit wall
            {
                dof = 8;
                vx = rx;
                vy = ry;
                disV = ((double)vx - player_pos.x) * cos(ra) - ((double)vy - player_pos.y) * sin(ra);
            }
            else
            {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }

        // choose shortest distance
        disV = abs_myting(disV);
        disH = abs_myting(disH);
        disT = smallest(disV, disH);
        printf("disT: %f, disV: %f, disH: %f\n", disT, disV, disH);

        //make sure angle is between 0 and 2PI
        float ca = fix_angle(player_angle - ra);

        disT = disT * cos(ca); // fix fisheye
        // printf("disT: %f\n", disT);
        
        float lineH = (4 * 32) / disT; // sq size * screen hight
        lineH = smallest(lineH, 31); // max line height to half of screen

        int lineO = 16 - lineH / 2; // half of screen - half of line height
        printf("lineH: %f, lineO: %d\n", lineH, lineO);

        //GLTINGS
        glColor3f(1.0, 0.3, 0.3);
        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex2i((int)(player_pos.x / 2.0 + 96) * 8, (int)(player_pos.y / 2.0 +1) * 8);
        glVertex2i((rx / 2.0 + 96) * 8, (ry / 2.0 +1) * 8);
        glEnd();

        // glColor3f(0.3, 1.0, 0.3);
        // glLineWidth(3);
        // glBegin(GL_LINES);
        // glVertex2i((int)(player_pos.x / 2.0 + 96) * 8, (int)(player_pos.y / 2.0 +1) * 8);
        // glVertex2i((vx / 2.0 + 96) * 8, (vy / 2.0 +1) * 8);
        // glEnd();

        draw_rects(r*2, (int)lineO, r*2 +2, (int)(lineH + lineO));

        ra += PI / 30.0 / 4.0;

        // make sure angle is between 0 and 2PI
        ra = fix_angle(ra);
    }
}