#include <stdint.h>
#include <math.h>
#include "chipkit_funcs.h"

#include "vector.h"

#define PI 3.14159265535

uint32_t frame = 0;

vec2 player_pos = {9, 17};
double player_angle = PI * (0 / 4.0);
uint8_t pistol_num = 0;
uint8_t shooting = 0;

uint8_t enemy_num = 0;

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

void set_column(int x, uint32_t val, uint32_t *map)
{
	map[x] |= val;
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
		map[i] = tmp;
	}
}

void draw_pistol(uint32_t *map)
{
	int i;
	for (i = 0; i < 16; i++)
	{
		map[38 + i] &= ~pistol_border[i];
		set_column(38 + i, pistol[i + 15 * (pistol_num % 4)], map);
	}
}

void draw_enemy(uint32_t *map)
{
	int i;
	for (i = 0; i < 30; i++)
	{
		map[20 + i] &= ~enemy_border[i];
		set_column(20 + i, enemy[i + 30 * (enemy_num % 4)], map);
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
				set_pos(j + 96, i + 8, map);
			}
		}
	}
}

void init_game(uint32_t *map)
{
	frame = 0;
	conv_2d_to_map(map2d, map);
}

void player_inputs(vec2 *player_pos, double *player_angle, uint32_t *map)
{
	int btns = getbtns();
	if (getbtn1())
	{
		move_player(player_pos, *player_angle + (PI / 2.0), map, map2d);
	}

	if (btns != 0)
	{
		if (btns & 0b1) // btn2
		{
			move_player(player_pos, *player_angle, map, map2d);
		}
		if (btns & 0b10) // btn3
		{
			move_player(player_pos, *player_angle + PI, map, map2d);
		}
		if (btns & 0b100) // btn4
		{
			move_player(player_pos, *player_angle + (PI * 3.0 / 2.0), map, map2d);
		}
	}
	int sw = getsw();

	if (sw != 0)
	{
		if (sw & 0b1) // sw1
		{
			shooting = 1;
		} 
		else
		{
			shooting = 0;
		}
		if (sw & 0b10) // sw2
		{
		}
		if (sw & 0b100) // sw3
		{
			rotate_player(player_angle, 1);
		}
		if (sw & 0b1000) // sw4
		{
			rotate_player(player_angle, -1);
		}
	}
	else
	{
		shooting = 0;
	}
	
}

// game loop
void game(uint32_t *map)
{
	draw_player(player_pos, player_angle, map, map2d);
	player_inputs(&player_pos, &player_angle, map);
	conv_2d_to_map(map2d, map);

	draw_enemy(map);
	draw_pistol(map);

	display_string(0, itoaconv((int)frame));
	display_update_text_row(96, 4, 5, 0, map);

	if ((int)frame % 2 == 0 && shooting)
	{
		pistol_num++;
		enemy_num++;
	}
	frame++;
}