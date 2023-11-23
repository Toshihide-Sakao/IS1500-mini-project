#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include <time.h>

#include "vector.h"
#include "player.h"

#define PI 3.14159265535

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

vec2 player_pos = {16, 9};
double player_angle = PI * (0.0 / 4.0);

void draw_map(uint32_t *map)
{
	// printf("draw map (rotation is wrong)\n");
	for (int i = 0; i < 128; i++)
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

uint8_t get_pos(int x, int y, uint32_t *map)
{
	uint32_t mask = 0;
	mask = 1 << (y);
	return (map[x] & mask) >> y;
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
				set_pos(j + 96, i + 1, map);
			}
		}
	}
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
		if (x < 0 || x > 95 || y < 0 || y > 31)
		{
			break;
		} // not needed if we are not dumb

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
		map[i] |= tmp;
	}
}

int main()
{
	conv_2d_to_map(map2d, map);

	// print_2d_map();
	// conv_2d_to_map(map2d, map);
	draw_player(player_pos, player_angle, map, map2d);
	// printf("player_pos: %f, %f\n", player_pos.x, player_pos.y);
	conv_2d_to_map(map2d, map);

	draw_map(map);

	while (1)
	{
		int redraw = 0;
		char c = getchar();
		if (c == 'w')
		{
			move_player(&player_pos, player_angle, map2d);
			redraw = 1;
		}
		if (c == 'a')
		{
			rotate_player(&player_angle);
			redraw = 1;
		}

		if (redraw)
		{
			system("clear");
			conv_2d_to_map(map2d, map);
			draw_map(map);
		}

		delay(100);
	}

	return 0;
}

void delay(int milliseconds)
{
	// Storing start time
	clock_t start_time = clock();

	// looping till required time is not achieved
	while (clock() < start_time + milliseconds)
		;
}