#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

const uint32_t map[128];
const uint8_t disp[512];

void draw_map(uint32_t *map)
{
	printf("draw map (rotation is wrong)\n");
	for (int i = 0; i < 128; i++)
	{
		print_binary(map[i], 31);
	}
	printf("----------------------------------\n");
}

void draw_disp(uint8_t *disp)
{
	printf("draw disp\n");
	

	printf("----------------------------------\n");
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

void conv_normal_to_disp(uint32_t *map, uint8_t *disp)
{
	int i, j;
	for (i = 0; i < 128; i++)
	{
		int k = 3;
		for (j = 0; j < 4; j++)
		{
			//dumb
			uint8_t tmp;
			tmp = (map[i] >> 8 * j) & 0xff;
			print_binary(tmp, 7);
			disp[4 * i + k] = tmp;
			k--;
		}
		printf("-------\n");
	}
}

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

int main()
{
	gen_map(map);
	draw_map(map);

	conv_normal_to_disp(map, disp); // not working
	// printf("testing\n");
	// print_binary(disp[0]);
	// print_binary(disp[1]);
	// draw_disp(disp);

	return 0;
}