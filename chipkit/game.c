#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "chipkit_funcs.h"

#include "vector.h"

#define PI 3.14159265535

vec2 player_pos = { 9, 17 };
double player_angle = PI * (0/4.0);

// x: 96, y: 32
uint8_t map2d[8][16] =
	{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1},
		{1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

uint8_t map2d_overlay[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // kommer använda för att visa var man befinner sig i mappen vid sidan.

void set_pos(int x, int y, uint32_t *map)
{
	uint32_t mask = 0;
	mask = 1 << (y);
	map[x] |= mask;
}

void clr_pos(int x, int y, uint32_t *map)
{
	uint32_t mask = 0;
	mask = 1 << (y);
	map[x] &= ~mask;
}

uint8_t get_pos(int x, int y, uint32_t *map)
{
	uint32_t mask = 0;
	mask = 1 << (y);
	return (map[x] & mask) >> y;
}

uint8_t set_map2d_ol(int x, int y, uint8_t *map2d_overlay)
{
	uint8_t mask = 0;
	mask = 1 << (y);
	map2d_overlay[x] |= mask;
}

uint8_t get_map2d_ol(int x, int y, uint8_t *map2d_overlay)
{
	uint8_t mask = 0;
	mask = 1 << (y);
	return (map2d_overlay[x] & mask) >> y;
}

void draw_line(vec2 p1, vec2 p2, uint32_t *map)
{
	int i;
	double dx = p2.x - p1.x;
	double dy = p2.y - p1.y;
	double x = p1.x;
	double y = p1.y;

	for (i = 0; i < 100; i++)
	{
		if (x < 0 || x > 95 || y < 0 || y > 31) { break; } // not needed if we are not dumb
		
		set_pos((int)x, (int)y, map);
		x += dx / 100;
		y += dy / 100;
	}
}

void draw_rects(int startX, int startY, int endX, int endY, uint32_t *map)
{
	int i;
	uint32_t tmp = ~0;
	tmp = tmp >> (31 - (endY - startY));
	tmp = tmp << startY;
	for (i = startX; i < endX; i++)
	{
		map[i] = tmp;
	}
}

void conv_2d_to_map(uint8_t map2d[8][16], uint32_t *map)
{
	int i, j;
	for (i = 0; i < 8 * 2; i++)
	{
		for (j = 0; j < 16 * 2; j++)
		{
			// int tmp = get_map2d_ol(j / 2, i / 2, map2d_overlay);
			int tmp = map2d[i / 2][j / 2];
			if (tmp == 1)
			{
				set_pos(j+96, i+1, map);
			}
		}
	}
}

void init_game(uint32_t *map)
{
	conv_2d_to_map(map2d, map);
	// draw_player(player_pos, player_angle, map);
}

void player_inputs(vec2 *player_pos, double *player_angle, uint32_t *map)
{
	int btns = getbtns();
	if (btns != 0)
	{
		int sw = getsw();
		if (btns & 0b1) // btn2
		{
			move_player(player_pos, *player_angle, map);
		}
		// if (btns & 0b10) // btn3
		// {
		// 	move_player(player_pos, *player_angle + PI, map);
		// }
		if (btns & 0b100) // btn4
		{
			rotate_player(player_angle);
		}
	}
}

spawn_enemies(uint8_t map2d[8][16])
{
    srand(time(0));
    int randomNumberx = (rand() & 16);
    int randomNumbery = (rand() & 8);
    if (map2d[randomNumbery][randomNumberx] == 0)
    {
        map2d[randomNumbery][randomNumberx] = 2;
    }
    else
    {
        while (map2d[randomNumbery][randomNumberx] == 1)
        {
            int randomNumberx = (rand() & 16);
            int randomNumbery = (rand() & 8);
        }
        map2d[randomNumbery][randomNumberx] = 2;
    }
}

// game loop
void game(uint32_t *map)
{
	draw_player(player_pos, player_angle, map, map2d);
	player_inputs(&player_pos, &player_angle, map);
	conv_2d_to_map(map2d, map);
}