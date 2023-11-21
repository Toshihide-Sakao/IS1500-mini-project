#include <stdint.h>
#include "chipkit_funcs.h"

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
	mask = 1 << (x);
	map[y] |= mask;
}

void clear_pos(int x, int y, uint32_t *map)
{
	uint32_t mask = 0;
	mask = 1 << (x);
	map[y] &= ~mask;
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
				set_pos(i, j, map);
			}
		}
	}
}

void init_game(uint32_t *map) {
    // gen_2d_map();
	conv_2d_to_map(map2d, map);
}
// game loop
void game(uint32_t *map) {
	
}