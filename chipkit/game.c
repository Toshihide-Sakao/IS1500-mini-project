#include <stdint.h>
// #include <stdlib.h>
#include <math.h>
#include "chipkit_funcs.h"

#include "vector.h"

#define PI 3.14159265535

unsigned long int frame = 0; // frame counter
static unsigned long int next = 1; // for rand()
int diff = 250; // start with spawning oen enemy per approx 6sec

vec2 player_pos = {50, 9};
double player_angle = PI * (7.0 / 4.0);

uint8_t pistol_num = 0; // which pistol frame to draw
uint8_t shooting = 0; // if the pistol is should run the animation
uint8_t shot = 0; // if the pistol is shot (for game logic)

uint8_t enemy_anim_frame = 0; // which enemy frame to draw
uint8_t enemy_poses[20][2];	 // enemy positions (max enemy number is 20)
uint8_t amount_enemies = 0; // amount of enemies

// x: 96, y: 32
// topdown view of map (1 = wall, 0 = empty, 2 = enemy)
// one pixel is 4x4 pixels in game
// one pixel is 2x2 in the map view at topright
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

// turns on the pixel at x, y
void set_pos(int x, int y, uint32_t *map)
{
	uint32_t mask = 0;
	mask = 1 << (y);
	map[x] |= mask;
}

// turns off the pixel at x, y
void clr_pos(int x, int y, uint32_t *map)
{
	uint32_t mask = 0;
	mask = 1 << (y);
	map[x] &= ~mask;
}

// returns the value of the pixel at x, y
uint8_t get_pos(int x, int y, uint32_t *map)
{
	uint32_t mask = 0;
	mask = 1 << (y);
	return (map[x] & mask) >> y;
}

// set the column at x to val (or adding the val to the column by or)
void set_column(int x, uint32_t val, uint32_t *map)
{
	map[x] |= val;
}

// draw rectangles from (startX, startY) to (endX, endY)
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

// clear rectangles from (startX, startY) to (endX, endY)
void clr_rects(int startX, int startY, int endX, int endY, uint32_t *map)
{
	int i;
	uint32_t tmp = ~0;
	tmp = tmp >> (31 - (endY - startY));
	tmp = tmp << startY;
	for (i = startX; i < endX; i++)
	{
		map[i] &= ~tmp;
	}
}

// draw the pistol
void draw_pistol(uint32_t *map)
{
	int i;
	for (i = 0; i < 16; i++)
	{
		map[38 + i] &= ~pistol_border[i];
		set_column(38 + i, pistol[i + 15 * (pistol_num % 4)], map); // pistol_num % 4 is for choosing the right animation
	}
}

// draw the whole enemy at x in full size(test func)
void draw_enemy(int x, uint32_t *map)
{
	int i;
	for (i = 0; i < 30; i++)
	{
		map[x + i] &= ~enemy_border[i];
		set_column(x + i, enemy[i + 30 * (enemy_anim_frame % 4)], map);
	}
}

// draw the eneny at x in scalable size (only the x value)
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
	int tmp_border = (tmp0 | tmp1 | tmp2) << (amount_rem);

	int enem_col = col + 30 * (enemy_anim_frame % 4);
	tmp0 = enemy[enem_col] & 0b111;
	tmp1 = (enemy[enem_col] & middle_mask) >> amount_rem;
	tmp2 = (enemy[enem_col] & 0b11111110000000000000000000000000) >> (amount_rem * 2);
	int tmp_enemy = (tmp0 | tmp1 | tmp2) << (amount_rem);

	map[x] &= ~tmp_border;
	set_column(x, tmp_enemy, map);
}

// draw enemy only at the x value (used for when it is super near)
void draw_enemy_x(int x, int col, uint32_t *map)
{
	map[x] &= ~enemy_border[col];
	set_column(x, enemy[col + 30 * (enemy_anim_frame % 4)], map);
}

// draw the 2dmap on the topright of the screen.
void conv_2d_to_map(uint32_t *map)
{
	int i, j;
	for (i = 0; i < 8 * 2; i++)
	{
		for (j = 0; j < 16 * 2; j++)
		{
			int tmp = map2d[i / 2][j / 2];
			if (tmp == 1)
			{
				set_pos(j + 96, i, map);
			}
		}
	}
}

// reset all enemy position
void reset_enem_poses()
{
	int i;
	for (i = 0; i < 20; i++)
	{
		enemy_poses[i][0] = 0;
		enemy_poses[i][1] = 0;
	}
}

// reset whole map2d by removing all enemies(2)
void reset_map2d()
{
	int i, j;
	for (i = 1; i < 7; i++)
	{
		for (j = 1; j < 15; j++)
		{
			if (map2d[i][j] == 2)
			{
				map2d[i][j] = 0;
			}
		}
	}
}

// initializes the game
void init_game(uint32_t *map)
{
	frame = 0;

	// reseting stuff
	amount_enemies = 0;
	reset_enem_poses();
	reset_map2d();
	reset_last_pos();

	player_pos.x = 50;
	player_pos.y = 9;
	player_angle = PI;

	conv_2d_to_map(map);
}

// gets the player inputs and moves/rotates the player
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
			// conv_2d_to_map(map2d, map);
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
			// conv_2d_to_map(map2d, map);
		}
	}
	else
	{
		shooting = 0;
	}
}

// if enemy is by the player -1 hp and remove the enemy (it pops :))
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

// some rand funcs
int rand(void) // RAND_MAX assumed to be 32767
{
	next = next * 1103515245 + 12345;
	return (unsigned int)(next / 65536) % 32768;
}

void srand(unsigned int seed)
{
	next = seed;
}

// spawn enemies
void spawn_enemies(uint32_t *map)
{
	srand(frame);
	int randomNumberx = (rand() % 16);
	int randomNumbery = (rand() % 8);

	if (map2d[randomNumbery][randomNumberx] == 0)
	{
		map2d[randomNumbery][randomNumberx] = 2;
		enemy_poses[amount_enemies][0] = randomNumberx;
		enemy_poses[amount_enemies][1] = randomNumbery;
		amount_enemies++;
	}
	// else
	// {
	// 	int tmp = map2d[randomNumbery][randomNumberx];
	// 	while (tmp == 1)
	// 	{
	// 		int randomNumberx = (rand() % 16);
	// 		int randomNumbery = (rand() % 8);

	// 		tmp = map2d[randomNumberx][randomNumbery];
	// 	}
	// 	map2d[randomNumbery][randomNumberx] = 2;
	// 	enemy_poses[amount_enemies][0] = randomNumberx;
	// 	enemy_poses[amount_enemies][1] = randomNumbery;
	// 	amount_enemies++;
	// 	// set_pos(randomNumberx*4 + 96, (int)randomNumbery*4, map);
	// }
}

// update enemy positions
// so the enemies will move towards the player
void update_enemy_poses()
{
	int mpx = (int)(player_pos.x / 4.0);
	int mpy = (int)(player_pos.y / 4.0);

	int i;
	for (i = 0; i < amount_enemies; i++)
	{
		int mx = enemy_poses[i][0];
		int my = enemy_poses[i][1];

		if (mx < mpx)
		{
			map2d[(int)enemy_poses[i][1]][(int)enemy_poses[i][0]] = 0;
			enemy_poses[i][0] += 1;
			map2d[(int)enemy_poses[i][1]][(int)enemy_poses[i][0]] = 2;
		}
		else if (mx > mpx)
		{
			map2d[(int)enemy_poses[i][1]][(int)enemy_poses[i][0]] = 0;
			enemy_poses[i][0] -= 1;
			map2d[(int)enemy_poses[i][1]][(int)enemy_poses[i][0]] = 2;
		}
		if (my < mpy)
		{
			map2d[(int)enemy_poses[i][1]][(int)enemy_poses[i][0]] = 0;
			enemy_poses[i][1] += 1;
			map2d[(int)enemy_poses[i][1]][(int)enemy_poses[i][0]] = 2;
		}
		else if (my > mpy)
		{
			map2d[(int)enemy_poses[i][1]][(int)enemy_poses[i][0]] = 0;
			enemy_poses[i][1] -= 1;
			map2d[(int)enemy_poses[i][1]][(int)enemy_poses[i][0]] = 2;
		}
	}
}

// gen the life string
char *gen_life_str(short *life)
{
	char tmp[5];
	char *str2 = (char *)itoaconv((int)*life);
	int str_len = sizeof(str2) / sizeof(str2[0]);
	tmp[0] = (char)124;
	int i;
	for (i = 0; i < str_len; i++)
	{
		tmp[i + 1] = str2[i];
	}
	tmp[1 + str_len] = '\0';

	// tmp[1] = str2[0];
	// tmp[2] = '\0';

	return tmp;
}

// gen the score string
char *gen_scr_str(int *score)
{
	char tmp[5];
	char *str2 = (char *)itoaconv((int)*score);
	int str_len = sizeof(str2) / sizeof(str2[0]);
	tmp[0] = (char)125;
	int i;
	for (i = 0; i < str_len; i++)
	{
		tmp[i + 1] = str2[i];
	}
	tmp[i + str_len] = '\0';

	return tmp;
}

// game loop
game(uint32_t *map, short *player_life, int *player_score)
{
	conv_2d_to_map(map); // redraws map every frame
	draw_player(player_pos, player_angle, shot, player_score, map, map2d); // draws the player on map2d
	draw_rays_3d(player_pos, player_angle, shot, player_score, map, map2d); // draw the 3d screen
	draw_pistol(map); // draw the pistol

	player_inputs(&player_pos, &player_angle, map); // get player inputs and move/rotate the player

	// display hp and score
	display_string(2, gen_scr_str(player_score));
	display_update_text_row(96, 4, 5, 2, map);
	display_string(3, gen_life_str(player_life));
	display_update_text_row(96, 4, 5, 3, map);

	// every 6th frame run the pistol animation
	if ((int)frame % 6 == 0 && shooting)
	{
		pistol_num++;
		if (pistol_num % 4 == 3) // every 4th(in the 6th) frame set shot to 1
		{
			shot = 1;
		}
		else // dont shoot
		{
			shot = 0;
		}
	}

	// every 4th frame run the enemy animation
	if ((int)frame % 4 == 0)
	{
		enemy_anim_frame++;
		enemy_attack_check(player_life);
	}

	// every 250th frame spawn an enemy
	if ((int)frame % diff == 0 && amount_enemies < 20)
	{
		spawn_enemies(map);
		diff = (int)(diff / 1.03);
	}

	// every 60th frame update enemy positions
	if ((int)frame % 60 == 0)
	{
		update_enemy_poses();
	}

	frame++;
}