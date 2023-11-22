#include <pic32mx.h>
#include <stdint.h>
#include "chipkit_funcs.h"

#define DISPLAY_VDD PORTFbits.RF6
#define DISPLAY_VBATT PORTFbits.RF5
#define DISPLAY_COMMAND_DATA PORTFbits.RF4
#define DISPLAY_RESET PORTGbits.RG9

#define DISPLAY_VDD_PORT PORTF
#define DISPLAY_VDD_MASK 0x40
#define DISPLAY_VBATT_PORT PORTF
#define DISPLAY_VBATT_MASK 0x20
#define DISPLAY_COMMAND_DATA_PORT PORTF
#define DISPLAY_COMMAND_DATA_MASK 0x10
#define DISPLAY_RESET_PORT PORTG
#define DISPLAY_RESET_MASK 0x200

#define cbOledDispMax 512 // max number of bytes in display buffer
#define ccolOledMax 128	  // number of display columns
#define crowOledMax 32	  // number of display rows
#define cpagOledMax 4	  // number of display memory pages

void delay(int cyc)
{
	int i;
	for (i = cyc; i > 0; i--)
		;
}

uint8_t spi_send_recv(uint8_t data)
{
	while (!(SPI2STAT & 0x08))
		;
	SPI2BUF = data;
	while (!(SPI2STAT & 0x01))
		;
	return SPI2BUF;
}

void display_init()
{
	DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
	delay(10);
	DISPLAY_VDD_PORT &= ~DISPLAY_VDD_MASK;
	delay(1000000);

	spi_send_recv(0xAE);
	DISPLAY_RESET_PORT &= ~DISPLAY_RESET_MASK;
	delay(10);
	DISPLAY_RESET_PORT |= DISPLAY_RESET_MASK;
	delay(10);

	spi_send_recv(0x8D);
	spi_send_recv(0x14);

	spi_send_recv(0xD9);
	spi_send_recv(0xF1);

	DISPLAY_VBATT_PORT &= ~DISPLAY_VBATT_MASK;
	delay(10000000);

	spi_send_recv(0xA1);
	spi_send_recv(0xC8);

	spi_send_recv(0xDA);
	spi_send_recv(0x20);

	spi_send_recv(0xAF);
}

void display_string(int line, char *s)
{
	int i;
	if (line < 0 || line >= 4)
		return;
	if (!s)
		return;

	for (i = 0; i < 16; i++)
		if (*s)
		{
			textbuffer[line][i] = *s;
			s++;
		}
		else
			textbuffer[line][i] = ' ';
}

void display_update(const uint32_t *map)
{
	int i, y, x;

	for (i = 0; i < 4; i++) // y is divided into 4 parts
	{
		DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(0x0);
		spi_send_recv(0x10);

		DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;

		for (x = 0; x < 128; x++)
		{
			spi_send_recv((map[x] >> 8 * i) & 0xff);
		}
	}
}

void display_reset()
{
	int i, y, x;

	for (i = 0; i < 4; i++) // y is divided into 4 parts
	{
		DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(0x0);
		spi_send_recv(0x10);

		DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;

		for (x = 0; x < 128; x++)
		{
			spi_send_recv(0);
		}
	}
}

void display_update_text()
{
	int i, j, k;
	int c;
	for (i = 0; i < 4; i++)
	{
		DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(0x0);
		spi_send_recv(0x10);

		DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;

		for (j = 0; j < 16; j++)
		{
			c = textbuffer[i][j];
			if (c & 0x80)
				continue;

			for (k = 0; k < 8; k++)
			{
				spi_send_recv(font[c * 8 + k]);
			}
		}
	}
}

// convert a uint32_t[128] to a uint8_t[512]
// void conv_normal_to_disp(uint32_t *map, uint8_t *disp)
// {
// 	int i, j;
// 	for (i = 0; i < 128; i++)
// 	{
// 		for (j = 0; j < 4; j++)
// 		{
// 			disp[128 * j + i] = (map[i] >> 8 * j) & 0xff;
// 		}
// 	}
// }