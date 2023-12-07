#include <pic32mx.h>
#include <stdint.h>
#include <math.h>
#include "chipkit_funcs.h"
#include "game.h"

// array of 128(y) 32bit(x) ints to represent the screen.
uint32_t map[128];

// int for gamestate (0 = main screen, 1 = game, 2 = leaderboard, 3 = dead screen, 4 = name input)
uint8_t game_state = 0;
uint8_t selected = 1; // which row is seleted (bits inversed)

char l_name[4][3]; // leaderboard names (has 4 so it can sort it without showing it)
int l_score[4];	   // leaderboard scores (has 4 so it can sort it without showing it)

// leaderboard string for row 1, 2 and 3
char l1[14];
char l2[14];
char l3[14];

// the string for the current name input
char curr_name[3] = "AAA";
int char_selected = 65; // 65 = A, 66 = B, 67 = C, etc. (so starting at A)
int curr_char = 0;		// which char is selected (0 = first, 1 = second, 2 = third)

short player_life = 10; // player life
int player_score = 0;	// player score

// generate a empty leaderboard
void gen_fake_leaderboard()
{
	// testing ---------------------
	l_name[0][0] = ' ';
	l_name[0][1] = ' ';
	l_name[0][2] = ' ';
	// l_score[0] = 0;

	l_name[1][0] = ' ';
	l_name[1][1] = ' ';
	l_name[1][2] = ' ';
	// l_score[1] = 8;

	l_name[2][0] = ' ';
	l_name[2][1] = ' ';
	l_name[2][2] = ' ';

	l_name[3][0] = ' ';
	l_name[3][1] = ' ';
	l_name[3][2] = ' ';

	// -----------------------------
}

// generate a leaderboard string for a row
char *gen_l_str(int i)
{
	char l_str[14];
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
	char *score_str = (char *)itoaconv(l_score[i]);
	int str_len = sizeof(score_str) / sizeof(score_str[0]);
	for (j = 0; j < str_len; j++)
	{
		l_str[j + 8] = score_str[j];
	}
	l_str[8 + str_len] = '\0';

	return l_str;
}

// sort the leaderboard
void sort_l_board()
{
	int i;
	for (i = 0; i < 4; i++)
	{
		int j;
		for (j = 0; j < 4; j++)
		{
			if (l_score[i] > l_score[j])
			{
				int temp = l_score[i];
				l_score[i] = l_score[j];
				l_score[j] = temp;

				char temp2[3];
				int w;
				for (w = 0; w < 3; w++)
				{
					temp2[w] = l_name[i][w];
					l_name[i][w] = l_name[j][w];
					l_name[j][w] = temp2[w];
				}
			}
		}
	}
}

// reset the map array
void reset_map()
{
	int i;
	for (i = 0; i < 128; i++)
	{
		map[i] = 0;
	}
}

// reset the textbuffer
void reset_textbuffer()
{
	int i;
	for (i = 0; i < 4; i++)
	{
		int j;
		for (j = 0; j < 16; j++)
		{
			textbuffer[i][j] = ' ';
		}
	}
}

// cheking for main screen inputs
void main_scr_input()
{
	// pong easter egg
	int btns = getbtns();
	if (getbtn1())
	{
		reset_textbuffer();
		reset_map();

		// ----------inits pong -----------
		pong_init(map);
		game_state = 5; // go to pong!!!!
		// --------------------------------

		delay(100000);
	}

	btns = getbtns(); // getting btns
	if (btns != 0)		  // if any btns are pressed
	{
		if (btns & 0b1) // btn2
		{
			if (selected == 1) // if start is selected
			{
				// reseting all shit that can make any bug to happen
				reset_textbuffer();
				reset_map();

				// inits life and score for player
				player_life = 10;
				player_score = 0;

				selected = 5; // so nothing selected
				delay(1000);

				// ----------inits real game -----------
				init_game(map);
				// -------------------------------------

				game_state = 1; // go to game!!!!
				display_reset();
				delay(1000);
			}
			else if (selected == 2) // if leaderboard is selected
			{
				// resets map array and textbuffer
				reset_textbuffer();
				reset_map();
				sort_l_board(); // sorts leaderboard

				selected = 5;	// set so nothing is selected
				game_state = 2; // go to leaderboard

				// testing ------------
				// curr_char = 0;
				// game_state = 3;
				// --------------------

				delay(1000);
			}
		}
		// if player want to switch between start and leaderboard
		if (btns & 0b100) // btn4
		{
			// DUMBEST SOLUTION EVER BUT I LIKE IT
			selected++;
			if (selected > 2)
			{
				selected = 1;
			}
		}
	}
}

// main screen
void main_screen()
{
	reset_map();

	// strings
	display_string(0, " BLOB SHOOT");
	display_string(1, "   Start");
	display_string(2, "Leaderboard");
	char *tmp = "~B4  {  zB2"; // ~ is downarrow and z is select, { is |
	display_string(3, tmp);
	display_update_text(10, 11, selected, map);

	delay(500000);
	main_scr_input(); // checking for inputs
}

void leader_screen()
{
	// sort
	sort_l_board();

	// strings
	display_string(0, "Leaderboard");
	display_string(1, gen_l_str(0));
	display_string(2, gen_l_str(1));
	display_string(3, gen_l_str(2));
	display_update_text(0, 13, selected, map);

	delay(100000);
	int btns = getbtns(); // getting btns
	if (btns != 0)		  // if any btns are pressed
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

// dead screen
void dead_screen()
{
	curr_char = 0;
	reset_map();
	reset_textbuffer();

	char score_str[14];
	char *tmp1 = " Score: ";

	// converting score(int) to string
	char *score = itoaconv((int)player_score);

	// start score_str with "Score: "
	int i;
	for (i = 0; i < 8; i++)
	{
		score_str[i] = tmp1[i];
	}

	// add score(int) to score_str
	int str_len = sizeof(score) / sizeof(score[0]);
	for (i = 0; i < str_len; i++)
	{
		score_str[i + 8] = score[i];
	}
	score_str[8 + str_len] = '\0'; // end string

	// display it
	display_string(0, "   YOU DIED");
	display_string(1, score_str);
	display_string(2, "     zB2");

	selected = 2; // so nothing is selected
	display_update_text(10, 13, selected, map);

	delay(100000);
	int btns = getbtns(); // getting btns
	if (btns != 0)		  // if any btns are pressed
	{
		// delay(100000);
		if (btns & 0b1) // btn2
		{
			// resets shit
			reset_map();
			char_selected = 65;
			selected = 5;
			curr_char = 0;
			reset_textbuffer();

			game_state = 4;

			delay(1000000);
		}
	}
}

// name input screen
void name_input()
{
	selected = 1; // selecting the row with name
	display_string(0, "Enter name:");
	display_string(1, curr_name);
	display_string(2, "~B4     zB2");
	display_update_text(10, 13, selected, map);

	delay(100000);
	int btns = getbtns();
	if (btns != 0)
	{
		reset_textbuffer();
		reset_map();

		// if btn4 is pressed the char goes to next char so A become B
		if (btns & 0b100) // btn4
		{
			curr_name[curr_char] = (char)char_selected;

			char_selected++;
			if (char_selected > 90) // Sets limit to Z so after Z it is A again
			{
				char_selected = 65;
			}
			delay(100000);
		}

		// confirms the char if not done. if done it confirms the name and goes to leaderboard.
		if (btns & 0b1) // btn2
		{
			if (curr_char >= 3)
			{
				// save name
				l_name[3][0] = curr_name[0];
				l_name[3][1] = curr_name[1];
				l_name[3][2] = curr_name[2];
				l_score[3] = player_score;

				selected = 2;
				game_state = 0; // go to main screen with leaderboard selected

				delay(10000);
				sort_l_board();
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

	// init display
	display_init();

	// reset display
	display_reset();

	// generating leaderboard (right now it generates a empty leaderboard)
	gen_fake_leaderboard();

	// the game loop
	while (1)
	{
		if (game_state == 0) // main screen
		{
			delay(10000);
			main_screen();
		}
		else if (game_state == 1) // game
		{
			if (player_life <= 0) // dead
			{
				curr_name[0] = 'A';
				curr_name[1] = 'A';
				curr_name[2] = 'A';

				delay(100000);
				game_state = 3;
			}
			else // alive
			{
				// ------- for real game --------
				game(map, &player_life, &player_score);
				// -----------------------------------
			}
		}
		else if (game_state == 2) // leader board
		{
			delay(10000);
			leader_screen();
		}
		else if (game_state == 3) // dead screen
		{
			delay(10000);
			dead_screen();
		}
		else if (game_state == 4) // place to put in name (3 chars)
		{
			delay(10000);
			name_input();
		}
		else if (game_state == 5) // for pong easter egg
		{
			//------- for pong -------
			if(pong_game(map))
			{
				delay(10000);
				selected = 2;
				game_state = 0;
			}
			delay(50000);
			// ------------------------
		}

		display_update(map); // update display
	}

	return 0;
}