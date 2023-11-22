#include <stdint.h>
#include <math.h>
#include "chipkit_funcs.h"

#include "vector.h"

#define PI 3.14159265

vec2 player_pos = {10, 10};
double player_angle = PI * (4.0 / 4.0);

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

void draw_line(vec2 p1, vec2 p2, uint32_t *map)
{
	int i;
	double dx = p2.x - p1.x;
	double dy = p2.y - p1.y;
	double step = 0.1;
	double x = p1.x;
	double y = p1.y;

	for (i = 0; i < 10; i++)
	{
		set_pos((int)x, (int)y, map);
		x += dx * step;
		y += dy * step;
	}
}

void conv_2d_to_map(uint8_t map2d[8][16], uint32_t *map)
{
	int i, j;
	for (i = 0; i < 8 * 4; i++)
	{
		for (j = 0; j < 16 * 4; j++)
		{
			if (map2d[i / 4][j / 4] == 1)
			{
				set_pos(j, i, map);
			}
		}
	}
}

void init_game(uint32_t *map)
{
	// gen_2d_map();
	conv_2d_to_map(map2d, map);
	draw_player(player_pos, player_angle, map);
}

void player_inputs(vec2 *player_pos, double player_angle, uint32_t *map)
{
	int btns = getbtns();
	if (btns != 0)
	{
		int sw = getsw();
		if (btns & 0b1) // btn2
		{
			move_player(player_pos, player_angle, map);
		}
		if (btns & 0b10) // btn3
		{
			move_player(player_pos, player_angle + PI, map);
		}
		if (btns & 0b100) // btn4
		{
			rotate_player(player_angle);
		}
	}
}

// game loop
void game(uint32_t *map)
{
	
	player_inputs(&player_pos, player_angle, map);
	draw_player(player_pos, player_angle, map);
	conv_2d_to_map(map2d, map);
}