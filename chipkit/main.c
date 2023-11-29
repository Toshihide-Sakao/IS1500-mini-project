#include <pic32mx.h>
#include <stdint.h>
#include "chipkit_funcs.h"
#include "game.h"

char textbuffer[4][16];
uint32_t map[128];

uint8_t game_state = 0;
uint8_t selected = 1;

uint32_t frame = 0;

int main(void)
{
	/* Set up peripheral bus clock */
	OSCCON &= ~0x180000;
	OSCCON |= 0x080000;

	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;

	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;

	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);

	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 15; // 8MHz I changed

	/* Clear SPIROV*/
	SPI2STATCLR &= ~0x40;
	/* Set CKP = 1, MSTEN = 1; */
	SPI2CON |= 0x60;

	/* Turn on SPI */
	SPI2CONSET = 0x8000;

	display_init();
	display_reset();

	while (1)
	{
		if (game_state == 0)
		{
			main_screen();
		}
		else if (game_state == 1)
		{
			display_string(0, "HELL");
			display_update_text(96, 4, map);

			game(map, &frame);
		}
		else if (game_state == 2) // leader board
		{
		}
		else if (game_state == 3) // place to put in name (3 chars)
		{
		}

		display_update(map);
	}

	return 0;
}

void main_screen()
{
	reset_map();

	display_string(0, "Game of wtf");
	display_string(1, "Start");
	display_string(2, "Leaderboard");
	display_string(3, "B4  B2");
	display_update_text(10, 11, selected, map);

	main_scr_input();

	delay(500000);
}

void main_scr_input()
{
	int btns = getbtns();
	if (btns != 0)
	{
		int sw = getsw();
		if (btns & 0b1) // btn2
		{
			if (selected == 1)
			{
				reset_textbuffer();
				reset_map();
				display_reset();
				init_game(map);

				selected = 5; // so nothing selected
				game_state = 1;

				frame = 0;
				
				delay(1000);
			}
			else if (selected == 2)
			{
				game_state = 2;
			}
		}
		if (btns & 0b100) // btn4
		{
			selected++;
			if (selected > 2)
			{
				selected = 1;
			}
		}
	}
}

void reset_map()
{
	int i;
	for (i = 0; i < 128; i++)
	{
		map[i] = 0;
	}
}

void reset_textbuffer()
{
	int i;
	for (i = 0; i < 4; i++)
	{
		int j;
		for (j = 0; j < 16; j++)
		{
			textbuffer[i][j] = 0;
		}
	}
}