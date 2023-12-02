#include <stdint.h>
#include <math.h>
#include "chipkit_funcs.h"

#include "vector.h"

#define PI 3.14159265535

uint32_t frame = 0;

vec2 player_pos = {50, 9};
double player_angle = PI * (7.0 / 4.0);

uint8_t pistol_num = 0;
uint8_t shooting = 0;
uint8_t shot = 0;

uint8_t enemy_num = 0;

// x: 96, y: 32
uint8_t map2d[8][16] =
	{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 1, 1, 0, 0, 0, 0, 1, 2, 0, 2, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 2, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 1, 0, 0, 2, 0, 1, 0, 1, 1, 1},
		{1, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 1, 0, 0, 0, 1},
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

void draw_enemy(int x, uint32_t *map)
{
	int i;
	for (i = 0; i < 30; i++)
	{
		map[x + i] &= ~enemy_border[i];
		set_column(x + i, enemy[i + 30 * (enemy_num % 4)], map);
	}
}

void draw_enemy_scalable(int x, int amount_rem, int col, uint32_t *map)
{
	// we are always keeping 7 pixels from the top and 3 from the bottom
	// amount rem is the amount of pixels we are removing from the top and bottom
	// so if we are removing 2 pixels from the top and 2 from the bottom, we are removing 4 pixels in total
	// then amount_rem is 2.

	// eg. rem = 3, ((1 << (32 - (3 + 7 + 3*2))) -1) << (3 + rem) =
	// ((1 << (32 - (3 + 7 + rem*2))) - 1) == 2^(32 - (3 + 7 + rem*2)) - 1 == 0b111...111 (32 - (3 + 7 + rem*2)) 1's
	// 0b111...111 << (3 + rem) == 0b111. ...1 1100 0000

	int middle_mask = ((1 << (32 - (3 + 7 + amount_rem * 2))) - 1) << (3 + amount_rem);
	// printf("middle mask: %x, rem: %d\n", middle_mask, amount_rem);

	int tmp0 = enemy_border[col] & 0b111;
	int tmp1 = (enemy_border[col] & middle_mask) >> amount_rem;
	int tmp2 = (enemy_border[col] & 0b11111110000000000000000000000000) >> (amount_rem * 2);
	// int tmp2 = enemy_border[col] & 0b1111 1110 0000 0000 0000 0000 0000 0000 >> (amount_rem*2);
	int tmp_border = (tmp0 | tmp1 | tmp2) << (amount_rem);
	// printf("b: tmp2 = %x & %x = %x then shift\n", enemy_border[col], 0b11111110000000000000000000000000, (enemy_border[col] & 0b11111110000000000000000000000000));
	// printf("b: real_b; %x, tmp0: %x, tmp1: %x, tmp2: %x, tmp_border: %x\n", enemy_border[col], tmp0, tmp1, tmp2, tmp_border);

	int enem_col = col + 30 * (enemy_num % 4);
	tmp0 = enemy[enem_col] & 0b111;
	tmp1 = (enemy[enem_col] & middle_mask) >> amount_rem;
	tmp2 = (enemy[enem_col] & 0b11111110000000000000000000000000) >> (amount_rem * 2);
	int tmp_enemy = (tmp0 | tmp1 | tmp2) << (amount_rem);
	// printf("e: real_e: %x, tmp0: %x, tmp1: %x, tmp2: %x, tmp_enemy: %x\n", enemy[enem_col], tmp0, tmp1, tmp2, tmp_enemy);

	map[x] &= ~tmp_border;
	set_column(x, tmp_enemy, map);
}

void draw_enemy_x(int x, int col, uint32_t *map)
{
	map[x] &= ~enemy_border[col];
	set_column(x, enemy[col + 30 * (enemy_num % 4)], map);
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
			if (*player_angle < 0.0)
			{
				*player_angle += 2.0 * PI;
			}
			else if (*player_angle >= 2.0 * PI)
			{
				*player_angle -= 2.0 * PI;
			}
			rotate_player(player_angle, 1);
			conv_2d_to_map(map2d, map);
		}
		if (sw & 0b1000) // sw4
		{
			if (*player_angle < 0.0)
			{
				*player_angle += 2.0 * PI;
			}
			else if (*player_angle >= 2.0 * PI)
			{
				*player_angle -= 2.0 * PI;
			}
			rotate_player(player_angle, -1);
			conv_2d_to_map(map2d, map);
		}
	}
	else
	{
		shooting = 0;
	}
}

void enemy_attack_check(short *player_life)
{
	int mx = (int)(player_pos.x) / 4;
	int my = (int)(player_pos.y) / 4;

	if (map2d[my][mx] == 2)
	{
		map2d[my][mx] = 0;
		*player_life -= 1;
	}
}

// game loop
game(uint32_t *map, short *player_life, int *player_score)
{
	draw_player(player_pos, player_angle, shot, player_score, map, map2d);
	player_inputs(&player_pos, &player_angle, map);

	// draw_enemy(map);
	draw_pistol(map);

	display_string(0, (char *)itoaconv((int)*player_life));
	display_update_text_row(96, 4, 5, 0, map);
	display_string(3, (char *)itoaconv((int)*player_score));
	display_update_text_row(96, 4, 5, 3, map);

	if ((int)frame % 6 == 0 && shooting)
	{
		pistol_num++;
		if (pistol_num % 4 == 3)
		{
			shot = 1;
		}
		else
		{
			shot = 0;
		}
	}
	if ((int)frame % 4 == 0)
	{
		enemy_num++;
		enemy_attack_check(player_life);
	}

	frame++;
}