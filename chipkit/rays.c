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

// so the angle is between 0 and 2PI
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
// THE REAL SHIT
void draw_rays_3d(vec2 player_pos, double player_angle, uint8_t shot, int *player_score, uint32_t *map, uint8_t map2d[8][16])
{
    // m stands for map
    int mx, my, dof, enemy_hit = 0, enemy_rendered = 0;
    float rx, ry, ra, x_offset, y_offset, dist, disEnemy;

    // how many rays to render the enemy (so how scaled it should be) (how thick it is)
    uint8_t number_of_enem_rays = 0;
    int which_enemy_x = 1000, which_enemy_y = 1000; // big number
    int enemy_done = 1; // when one enemy is drawn enemy_done is 1 and then the next enemy can be drawn

    ra = (float)(player_angle - (FOV / 2)); // fix back 30 degrees
    ra = fix_angle(ra);

    float CosPa = cos(player_angle); // saves values
    float SinPa = sin(player_angle);

    float disEnemyV = 1000000; // super high number
    float disEnemyH = 1000000; // super high number

    int killed = 0; // checks if this frame killed an enemy

    int r;
    for (r = 0; r < 30; r++)
    {
        // Horizontal
        dof = 0;                                                    // depth of field
        float disH = 1000000, hx = player_pos.x, hy = player_pos.y; // super high number

        float Tan = sin(ra) / cos(ra);                // tan does not work in chipkit lmao!!!!!!!!!!!!!!!!!
        if (ra > PI + 0.001 && ra < 2.0 * PI - 0.001) // ray looking up
        {
            float pmy = floor(player_pos.y / 4);            // player map y
            ry = pmy * 4.0 - 0.001;                         // height/width in map2d for one position is 4
            rx = (ry - player_pos.y) / Tan + player_pos.x;
            y_offset = -4;                                  // how much to move in y every check
            x_offset = y_offset / Tan;                      // how much to move in x every check
        }
        else if (ra < PI - 0.001 && ra > 0.001) // ray looking down
        {
            float pmy = floor(player_pos.y / 4);
            ry = pmy * 4.0 + 4;
            rx = (ry - player_pos.y) / Tan + player_pos.x;
            y_offset = 4;
            x_offset = y_offset / Tan;
        }
        else // ray looking left/right
        {
            rx = player_pos.x;
            ry = player_pos.y;
            dof = DOFLIMIT;
        }

        // while the ray has not hit a wall or not found any wall after 8 searches
        while (dof < DOFLIMIT)
        {
            // get map equivalent of ray position
            mx = (int)(rx) / 4;
            my = (int)(ry) / 4;

            // checks if the ray hit a wall or if it is outside the map
            int check = (mx < 16 && my < 8 && mx >= 0 && my >= 0 && map2d[my][mx] == 1) || (mx >= 16 || mx <= 0 || my >= 8 || my <= 0);
            if (check) // hit wall
            {
                // set the depth of field to the limit so it breaks out of loop
                dof = DOFLIMIT;
                hx = rx;
                hy = ry;
                disH = CosPa * (rx - player_pos.x) + SinPa * (ry - player_pos.y); // distance to wall
            }
            else
            {
                // if the ray hit an enemy
                if (mx < 16 && my < 8 && mx >= 0 && my >= 0 && map2d[my][mx] == 2 && enemy_hit == 0)
                {
                    // if the ray is a shot and the enemy is in the right place (kill the enemy)
                    if (shot == 1 && (r >= 14 && r <= 15))
                    {
                        killed = 1;
                        map2d[my][mx] = 0;
                    }
                    // if the ray is not a shot (draw the enemy)
                    else
                    {
                        enemy_hit = 1; // draw a column of the enemy

                        // if the last enemy hit has finished drawing and the enemy hit is not the same as the last enemy hit
                        // then start with a new enemy
                        if (enemy_done == 1 && which_enemy_x != mx && which_enemy_y != my)
                        {
                            disEnemyH = CosPa * (mx - (player_pos.x / 4)) + SinPa * (my - (player_pos.y / 4));
                            enemy_done = 0;
                            enemy_rendered = 0;
                            which_enemy_x = mx;
                            which_enemy_y = my;
                        }
                    }
                }

                // move ray to next position
                rx += x_offset;
                ry += y_offset;
                dof += 1;
            }
        }

        // Vertical (same as horizontal but with different offsets)
        dof = 0;
        float disV = 1000000, vx = player_pos.x, vy = player_pos.y; // super high number

        if (ra > PI / 2.0 + 0.0001 && ra < 3.0 * PI / 2.0 - 0.0001) // ray looking left
        {
            float pmx = floor(player_pos.x / 4);
            rx = pmx * 4.0 - 0.001;
            ry = (rx - player_pos.x) * Tan + player_pos.y;
            x_offset = -4;
            y_offset = x_offset * Tan;
        }
        else if (ra < PI / 2.0 || ra > 3.0 * PI / 2.0) // ray looking right
        {
            float pmx = floor(player_pos.x / 4);
            rx = pmx * 4.0 + 4.0;
            ry = (rx - player_pos.x) * Tan + player_pos.y;
            x_offset = 4;
            y_offset = x_offset * Tan;
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

            int check = (mx < 16 && my < 8 && mx >= 0 && my >= 0 && map2d[my][mx] == 1) || (mx >= 16 || mx <= 0 || my >= 8 || my <= 0);
            if (check) // hit wall
            {
                dof = DOFLIMIT;
                vx = rx;
                vy = ry;
                disV = CosPa * (rx - player_pos.x) + SinPa * (ry - player_pos.y);
            }
            else
            {
                if (mx < 16 && my < 8 && mx >= 0 && my >= 0 && map2d[my][mx] == 2 && enemy_hit == 0)
                {
                    if (shot == 1 && (r >= 14 && r <= 15) && killed == 0)
                    {
                        killed = 1;
                        map2d[my][mx] = 0;
                    }
                    else
                    {
                        // draw enemy on 3d game screen
                        enemy_hit = 1;
                        if (enemy_done == 1 && which_enemy_x != mx && which_enemy_y != my)
                        {
                            disEnemyV = CosPa * (mx - (player_pos.x / 4)) + SinPa * (my - (player_pos.y / 4));
                            enemy_done = 0;
                            enemy_rendered = 0;
                            which_enemy_x = mx;
                            which_enemy_y = my;
                        }
                    }
                }

                rx += x_offset;
                ry += y_offset;
                dof += 1;
            }
        }

        // choose shortest distance to wall and chooses texture
        int choose_texture = 1; // 1 = vertical, 2 = horizontal
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

        // make sure angle is between 0 and 2PI
        float ca = fix_angle(player_angle - ra);
        dist = dist * cos(ca); // fix fisheye

        // Setting wall height
        float lineH = 96.0 / dist;   // constant / dist
        lineH = smallest(lineH, 31); // max line height to half of screen

        int lineO = 16 - (int)lineH / 2; // half of screen - half of line height

        // -----------------draw wall ------------------------
        int col;
        for (col = 0; col < (3 - choose_texture); col++)
        {
            map[r * 3 + choose_texture + col] = 0; // clears wall from last frame (if it was another texture it could be left)
        }
        draw_rects(r * 3, (int)lineO, r * 3 + choose_texture, (int)(lineH + lineO), map); // draws the actual wall
        // ---------------------------------------------------

        // if enemy is hit
        disEnemy = smallest(disEnemyH, disEnemyV);
        // if the enemy is hit and the enemy is close enough to be rendered
        if (enemy_hit && disEnemy < 7 && enemy_done == 0)
        {
            // sets how many rays the enemy will take up (how thick it is)
            number_of_enem_rays = enemy_dist[(int)disEnemy];

            int o;
            // if the enemy is very near and big and the enemy is not done
            if (number_of_enem_rays == 30 && enemy_rendered <= 28 * 2)
            {
                float scale = 0.5;
                for (o = 0; o < 3; o++)
                {
                    draw_enemy_x(r * 3 + o, (int)(scale * enemy_rendered), map);
                    enemy_rendered++;
                }
                if (enemy_rendered == 56) // it is done
                {
                    enemy_rendered = 0;
                    enemy_done = 1;
                    // printf("big: reset enemy_rendered r: %d\n", r);
                }
            }
            else if (enemy_rendered == number_of_enem_rays - 3) // normal enemy size and at end
            {
                // sets how mmany to remove from top/bot
                int sc = (int)((26 - number_of_enem_rays) / 2.0);
                for (o = 0; o < 3; o++)
                {
                    draw_enemy_scalable(r * 3 + o, sc, 25 + o, map);
                    enemy_rendered++;
                }
                // enemy is done after this
                enemy_rendered = 0;
                enemy_done = 1;
            }
            else if (number_of_enem_rays > enemy_rendered) // normal enemy size and not at end
            {
                // sets how mmany to remove from top/bot
                int sc = (int)((26 - number_of_enem_rays) / 2.0);
                for (o = 0; o < 3; o++)
                {
                    draw_enemy_scalable(r * 3 + o, sc, (int)((30.0 / (number_of_enem_rays + 1)) * enemy_rendered) + 2, map);
                    enemy_rendered++;
                }
                if (enemy_rendered >= number_of_enem_rays)
                {
                    // DONE
                    enemy_rendered = 0;
                    enemy_done = 1;
                    // printf("normal: reset enemy_rendered r: %d\n", r);
                }
            }
        }

        // adds to angle and fixes it
        ra += FOV / 30.0;
        ra = fix_angle(ra);
        enemy_hit = 0; // resets enemy hit
    }

    // after every frame check if a enemy was killed
    if (killed == 1)
    {
        *player_score += 1;
    }
}