#include <stdint.h>

void gen_map(uint32_t *map)
{
	int i;
	uint32_t edge = 0;
	edge = 1 << 31;
	edge |= 1;
	edge = ~edge;
	uint32_t normal = 0;
	normal = 1 << 30;
	normal |= 2;

	for (i = 0; i < 128; i++)
	{
		if (i == 1 || i == 126)
		{
			map[i] = edge;
		}
		else if (i == 0 || i == 127)
		{
			map[i] = 0x0;
		}
		else
		{
			map[i] = normal;
		}
	}
}

void init_game(uint32_t *map) {
    gen_map(map);
}
// game loop
void game(uint32_t *map) {
	
}