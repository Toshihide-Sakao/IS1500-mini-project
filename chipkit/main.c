#include <pic32mx.h>
#include <stdint.h>

#include <math.h>
#include "chipkit_funcs.h"
#include "game.h"

uint32_t map[128];

uint8_t game_state = 0;
uint8_t selected = 1;

char *l_name[4][3];
int l_score[4];

char curr_name[3] = "AAA";
int char_selected = 65;
int curr_char = 0;

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
			game(map);
		}
		else if (game_state == 2) // leader board
		{
			leader_screen();
		}
		else if (game_state == 3) // dead screen
		{
			dead_screen();
		}
		else if (game_state == 4) // place to put in name (3 chars)
		{
			name_input();
		}

		display_update(map);
	}

	return 0;
}

void main_screen()
{
	reset_map();

	display_string(0, "Game of wtf");
	display_string(1, "   Start");
	display_string(2, "Leaderboard");
	display_string(3, "B4       B2");
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
				init_game(map);

				selected = 5; // so nothing selected
				game_state = 1;

				delay(1000);
			}
			else if (selected == 2)
			{
				reset_textbuffer();
				reset_map();

				selected = 5;
				game_state = 2;

				// testing ------------
				// game_state = 3;
				// --------------------

				delay(1000);
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

void leader_screen()
{
	// testing ---------------------
	l_name[0][0] = 'A';
	l_name[0][1] = 'A';
	l_name[0][2] = 'A';
	l_score[0] = 100;

	l_name[1][0] = 'A';
	l_name[1][1] = 'B';
	l_name[1][2] = 'A';
	l_score[1] = 90;

	// -----------------------------
	char l1[14];
	gen_l_str(l1, 0);
	char l2[14];
	gen_l_str(l2, 1);
	char l3[14];
	gen_l_str(l3, 2);

	display_string(0, "Leaderboard");
	display_string(1, l1);
	display_string(2, l2);
	display_string(3, l3);
	display_update_text(8, 13, selected, map);

	int btns = getbtns();
	if (btns != 0)
	{
		if (btns & 0b100) // btn4
		{
			reset_map();
			selected = 1;
			reset_textbuffer();

			game_state = 0;

			delay(1000);
		}
	}

	delay(500000);
}

void gen_l_str(char *l_str, int i)
{
	l_str[0] = (char)((i + 1) + 48);
	l_str[1] = '.';
	l_str[2] = ' ';

	int j;
	for (j = 0; j < 3; j++)
	{
		l_str[j + 3] = l_name[i][j];
	}
	l_str[6] = ' ';
	l_str[7] = ' ';
	l_str[8] = ' ';
	l_str[9] = (char)(((int)floor(l_score[i] / 100) % 100) + 48);
	l_str[10] = (char)(((int)floor(l_score[i] / 10) % 10) + 48);
	l_str[11] = (char)(((int)floor(l_score[i] / 1) % 1) + 48);
}

void dead_screen()
{
	display_string(0, "   YOU DIED");
	display_string(1, "  Score: 100");
	display_string(2, "B2 to continue");

	selected = 2;
	display_update_text(10, 13, selected, map);

	int btns = getbtns();
	if (btns != 0)
	{
		if (btns & 0b1) // btn2
		{
			reset_map();
			char_selected = 65;
			selected = 5;
			curr_char = 0;
			reset_textbuffer();

			game_state = 4;

			delay(1000);
		}
	}
}

void name_input()
{
	selected = 1;
	display_string(0, "Enter name:");
	display_string(1, curr_name);
	display_update_text(10, 13, selected, map);

	delay(100000000);
	int btns = getbtns();
	if (btns != 0)
	{
		reset_textbuffer();
		reset_map();

		if (btns & 0b100) // btn4
		{
			curr_name[curr_char] = (char)char_selected;

			char_selected++;
			if (char_selected > 90)
			{
				char_selected = 65;
			}
			delay(100000);
		}
		if (btns & 0b1) // btn2
		{
			if (curr_char >= 3)
			{
				// save name
				selected = 2;
				game_state = 0;

				delay(1000000);
			}
			else
			{
				curr_name[curr_char] = (char)char_selected;
				curr_char++;

				char_selected = 65;
				delay(100000);
			}
		}
	}
}

void sort_l_board() { // TODO:
	int i;
	for (i = 0; i < 4; i++) {
		int j;
		for (j = 0; j < 4; j++) {
			if (l_score[i] > l_score[j]) {
				int temp = l_score[i];
				l_score[i] = l_score[j];
				l_score[j] = temp;

				char *temp2 = l_name[i];
				int w;
				for (w = 0; w < 3; w++)
				{
					l_name[i][w] = l_name[j][w];
					l_name[j][w] = temp2[w];
				}
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