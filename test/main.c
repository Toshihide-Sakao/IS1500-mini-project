#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "vector.h"
#include "player.h"

#define PI 3.14159265

uint32_t map[128];
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

vec2 player_pos = { 10, 10 };
double player_angle = PI * (4.0/4.0);

void draw_map(uint32_t *map)
{
	// printf("draw map (rotation is wrong)\n");
	for (int i = 0; i < 96; i++)
	{
		print_binary(map[i], 31);
	}
	// printf("----------------------------------\n");
}

void print_binary(uint32_t x, int n)
{
	int i;
	for (i = n; i >= 0; i--)
	{
		printf("%d", (x >> i) & 1);
	}
	printf("\n");
}

void gen_map(uint32_t *map)
{
	int i;
	uint32_t edge = 0;
	edge = ~0;
	uint32_t normal = 0;
	normal = 1 << 31;
	normal |= 1;

	for (i = 0; i < 96; i++)
	{
		if (i == 0 || i == 95)
		{
			map[i] = edge;
		}
		else
		{
			map[i] = normal;
		}
	}
}

void print_2d_map()
{
	int i, j;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 16; j++)
		{
			printf("%d", map2d[i][j]);
		}
		printf("\n");
	}
	printf("----------------------------------\n");
}

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

int main()
{

	// print_2d_map();
	conv_2d_to_map(map2d, map);
	draw_player(player_pos, player_angle, map);
	printf("player_pos: %f, %f\n", player_pos.x, player_pos.y);

	draw_map(map);

	// while (1)
	// {
	// 	system("clear");
	// 	conv_2d_to_map(map2d, map);
	// 	draw_map(map);
	// }

	return 0;
}