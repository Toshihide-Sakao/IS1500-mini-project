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

char textbuffer[4][16];

static const uint8_t const font[] = {
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	94,
	0,
	0,
	0,
	0,
	0,
	0,
	4,
	3,
	4,
	3,
	0,
	0,
	0,
	36,
	126,
	36,
	36,
	126,
	36,
	0,
	0,
	36,
	74,
	255,
	82,
	36,
	0,
	0,
	0,
	70,
	38,
	16,
	8,
	100,
	98,
	0,
	0,
	52,
	74,
	74,
	52,
	32,
	80,
	0,
	0,
	0,
	0,
	4,
	3,
	0,
	0,
	0,
	0,
	0,
	0,
	126,
	129,
	0,
	0,
	0,
	0,
	0,
	0,
	129,
	126,
	0,
	0,
	0,
	0,
	42,
	28,
	62,
	28,
	42,
	0,
	0,
	0,
	8,
	8,
	62,
	8,
	8,
	0,
	0,
	0,
	0,
	0,
	128,
	96,
	0,
	0,
	0,
	0,
	8,
	8,
	8,
	8,
	8,
	0,
	0,
	0,
	0,
	0,
	0,
	96,
	0,
	0,
	0,
	0,
	64,
	32,
	16,
	8,
	4,
	2,
	0,
	0,
	62,
	65,
	73,
	65,
	62,
	0,
	0,
	0,
	0,
	66,
	127,
	64,
	0,
	0,
	0,
	0,
	0,
	98,
	81,
	73,
	70,
	0,
	0,
	0,
	0,
	34,
	73,
	73,
	54,
	0,
	0,
	0,
	0,
	14,
	8,
	127,
	8,
	0,
	0,
	0,
	0,
	35,
	69,
	69,
	57,
	0,
	0,
	0,
	0,
	62,
	73,
	73,
	50,
	0,
	0,
	0,
	0,
	1,
	97,
	25,
	7,
	0,
	0,
	0,
	0,
	54,
	73,
	73,
	54,
	0,
	0,
	0,
	0,
	6,
	9,
	9,
	126,
	0,
	0,
	0,
	0,
	0,
	102,
	0,
	0,
	0,
	0,
	0,
	0,
	128,
	102,
	0,
	0,
	0,
	0,
	0,
	0,
	8,
	20,
	34,
	65,
	0,
	0,
	0,
	0,
	20,
	20,
	20,
	20,
	0,
	0,
	0,
	0,
	65,
	34,
	20,
	8,
	0,
	0,
	0,
	2,
	1,
	81,
	9,
	6,
	0,
	0,
	0,
	28,
	34,
	89,
	89,
	82,
	12,
	0,
	0,
	0,
	126,
	9,
	9,
	126,
	0,
	0,
	0,
	0,
	127,
	73,
	73,
	54,
	0,
	0,
	0,
	0,
	62,
	65,
	65,
	34,
	0,
	0,
	0,
	0,
	127,
	65,
	65,
	62,
	0,
	0,
	0,
	0,
	127,
	73,
	73,
	65,
	0,
	0,
	0,
	0,
	127,
	9,
	9,
	1,
	0,
	0,
	0,
	0,
	62,
	65,
	81,
	50,
	0,
	0,
	0,
	0,
	127,
	8,
	8,
	127,
	0,
	0,
	0,
	0,
	65,
	127,
	65,
	0,
	0,
	0,
	0,
	0,
	32,
	64,
	64,
	63,
	0,
	0,
	0,
	0,
	127,
	8,
	20,
	99,
	0,
	0,
	0,
	0,
	127,
	64,
	64,
	64,
	0,
	0,
	0,
	127,
	2,
	4,
	2,
	127,
	0,
	0,
	0,
	127,
	6,
	8,
	48,
	127,
	0,
	0,
	0,
	0,
	62,
	65,
	65,
	62,
	0,
	0,
	0,
	0,
	127,
	9,
	9,
	6,
	0,
	0,
	0,
	0,
	62,
	65,
	97,
	126,
	64,
	0,
	0,
	0,
	127,
	9,
	9,
	118,
	0,
	0,
	0,
	0,
	38,
	73,
	73,
	50,
	0,
	0,
	0,
	1,
	1,
	127,
	1,
	1,
	0,
	0,
	0,
	0,
	63,
	64,
	64,
	63,
	0,
	0,
	0,
	31,
	32,
	64,
	32,
	31,
	0,
	0,
	0,
	63,
	64,
	48,
	64,
	63,
	0,
	0,
	0,
	0,
	119,
	8,
	8,
	119,
	0,
	0,
	0,
	3,
	4,
	120,
	4,
	3,
	0,
	0,
	0,
	0,
	113,
	73,
	73,
	71,
	0,
	0,
	0,
	0,
	127,
	65,
	65,
	0,
	0,
	0,
	0,
	2,
	4,
	8,
	16,
	32,
	64,
	0,
	0,
	0,
	0,
	65,
	65,
	127,
	0,
	0,
	0,
	4,
	2,
	1,
	2,
	4,
	0,
	0,
	0,
	64,
	64,
	64,
	64,
	64,
	64,
	0,
	0,
	0,
	1,
	2,
	4,
	0,
	0,
	0,
	0,
	0,
	48,
	72,
	40,
	120,
	0,
	0,
	0,
	0,
	127,
	72,
	72,
	48,
	0,
	0,
	0,
	0,
	48,
	72,
	72,
	0,
	0,
	0,
	0,
	0,
	48,
	72,
	72,
	127,
	0,
	0,
	0,
	0,
	48,
	88,
	88,
	16,
	0,
	0,
	0,
	0,
	126,
	9,
	1,
	2,
	0,
	0,
	0,
	0,
	80,
	152,
	152,
	112,
	0,
	0,
	0,
	0,
	127,
	8,
	8,
	112,
	0,
	0,
	0,
	0,
	0,
	122,
	0,
	0,
	0,
	0,
	0,
	0,
	64,
	128,
	128,
	122,
	0,
	0,
	0,
	0,
	127,
	16,
	40,
	72,
	0,
	0,
	0,
	0,
	0,
	127,
	0,
	0,
	0,
	0,
	0,
	120,
	8,
	16,
	8,
	112,
	0,
	0,
	0,
	0,
	120,
	8,
	8,
	112,
	0,
	0,
	0,
	0,
	48,
	72,
	72,
	48,
	0,
	0,
	0,
	0,
	248,
	40,
	40,
	16,
	0,
	0,
	0,
	0,
	16,
	40,
	40,
	248,
	0,
	0,
	0,
	0,
	112,
	8,
	8,
	16,
	0,
	0,
	0,
	0,
	72,
	84,
	84,
	36,
	0,
	0,
	0,
	0,
	8,
	60,
	72,
	32,
	0,
	0,
	0,
	0,
	56,
	64,
	32,
	120,
	0,
	0,
	0,
	0,
	56,
	64,
	56,
	0,
	0,
	0,
	0,
	56,
	64,
	32,
	64,
	56,
	0,
	0,
	0,
	0,
	72,
	48,
	48,
	72,
	0,
	0,
	0,
	0,
	24,
	160,
	160,
	120,
	0,
	0,
	0,
	0,
	100,
	84,
	84,
	76,
	0,
	0,
	0,
	0,
	8,
	28,
	34,
	65,
	0,
	0,
	0,
	0,
	0,
	126,
	0,
	0,
	0,
	0,
	0,
	0,
	65,
	34,
	28,
	8,
	0,
	0,
	0,
	0,
	4,
	2,
	4,
	2,
	0,
	0,
	0,
	120,
	68,
	66,
	68,
	120,
	0,
	0,
};

const uint8_t const dog_icon[] = {
	255, 255, 255, 255, 255, 255, 127, 187,
	68, 95, 170, 93, 163, 215, 175, 95,
	175, 95, 175, 95, 223, 111, 175, 247,
	59, 237, 242, 254, 171, 254, 1, 255,
	255, 255, 15, 211, 109, 58, 253, 8,
	178, 77, 58, 199, 122, 197, 242, 173,
	242, 237, 186, 215, 40, 215, 41, 214,
	35, 175, 91, 212, 63, 234, 149, 111,
	171, 84, 253, 252, 254, 253, 126, 184,
	195, 52, 201, 22, 225, 27, 196, 19,
	165, 74, 36, 146, 72, 162, 85, 8,
	226, 25, 166, 80, 167, 216, 167, 88,
	106, 149, 161, 95, 135, 91, 175, 87,
	142, 123, 134, 127, 134, 121, 134, 121,
	132, 59, 192, 27, 164, 74, 177, 70,
	184, 69, 186, 69, 254, 80, 175, 217};

const uint32_t const pistol[15 * 4] = {
	// start first
	0, 0, 0, 0,
	0b11111111000000000000000000000000,
	0b00000000111000000000000000000000,
	0b11010101000100000000000000000000,
	0b10100001100100000000000000000000,
	0b00001100111000000000000000000000,
	0b10001111000000000000000000000000,
	0b00001000000000000000000000000000,
	0b00001000000000000000000000000000,
	0, 0, 0,
	// second
	0, 0, 0, 0,
	0b1111111100000000000000000000000,
	0b0000000011100000000000000000000,
	0b1101010100010000000000000000000,
	0b1010000110010000000000000000000,
	0b0001100011100000000000000000000,
	0b1001011100000000000000000000000,
	0b0001000000000000000000000000000,
	0b0001000000000000000000000000000,
	0, 0, 0,
	// third
	0, 0, 0, 0,
	0b1111111100000000000000000000000,
	0b0000000011100000000000000000000,
	0b1101010100011000000000000000000,
	0b1010000110011000000000000000000,
	0b0011000011100000000000000000000,
	0b1011011100000000000000000000000,
	0b0010000000000000000000000000000,
	0b0010000000000000000000000000000,
	0, 0, 0,
	// fourth
	0, 0, 0, 0,
	0b1111111100000000000000000000000,
	0b0000000011100000000000000000000,
	0b1101010100010000000000000000000,
	0b1010000110010110000000000000000,
	0b0110000011100000000000000000000,
	0b1100011100000000000000000000000,
	0b01000000000000000000000000000000,
	0b01010000000000000000000000000000,
	0b00100000000000000000000000000000,
	0, 0};

const uint32_t const pistol_border[15] = {
	0, 0, 0, 0,
	0b11111111000000000000000000000000,
	0b11111111111000000000000000000000,
	0b11111111111100000000000000000000,
	0b11111111111100000000000000000000,
	0b11111111111000000000000000000000,
	0b11111111000000000000000000000000,
	0b00001000000000000000000000000000,
	0b00001000000000000000000000000000,
	0, 0, 0
};

const uint32_t const enemy[30 * 4] = {
	// first
	0, 0, 
	0b00000111111110000000000000000000,
	0b00001000000001100000000000000000,
	0b00010000000000010000000000000000,
	0b00100000000000001000000000000000,
	0b01000000000000001000000000000000,
	0b01000000000000000100000000000000,
	0b01000000000000001110000000000000,
	0b01000000000000001010000000000000,
	0b01000000000000110001100011100000,
	0b01000000000000100000100010100000,
	0b01000000000110110001101100011000,
	0b01000000000110001010001000001000,
	0b01000000000000001110001100011000,
	0b01000000100000000001000010100000,
	0b01000000100000000001000011100000,
	0b01000000100000000001110000000000,
	0b01000000100000000001010000000000,
	0b01000000100000000110001100000000,
	0b01000000100000000100000100000000,
	0b01000000000000000110001100000000,
	0b01000000000110001101010000000000,
	0b01000000000110001001110000000000,
	0b00100000000000001000000000000000,
	0b00010000000000010000000000000000,
	0b00001000000011100000000000000000,
	0b00000111111100000000000000000000,
	0, 0,
	// second
	0, 0, 
	0b00000111111111000000000000000000,
	0b00001000000000110000000000000000,
	0b00010000000000001000000000000000,
	0b00100000000000000100000000000000,
	0b01000000000000000100000000000000,
	0b01000000000000000010000000000000,
	0b01000000000000000111000000000000,
	0b01000000000000000101000000000000,
	0b01000000000000011000110011100000,
	0b01000000000000010000010010100000,
	0b01000000000011011000111100011000,
	0b01000000000011000101001000001000,
	0b01000000000000000111001100011000,
	0b01000000010000000000100010100000,
	0b01000000010000000000100011100000,
	0b01000000010000000000111000000000,
	0b01000000010000000000101000000000,
	0b01000000010000000011000110000000,
	0b01000000010000000010000010000000,
	0b01000000000000000011000110000000,
	0b01000000000011000110101000000000,
	0b01000000000011000100111000000000,
	0b00100000000000000100000000000000,
	0b00010000000000001000000000000000,
	0b00001000000001110000000000000000,
	0b00000111111110000000000000000000,
	0, 0,
	// third
	0, 0, 
	0b00000111111111100000000000000000,
	0b00001000000000011000000000000000,
	0b00010000000000000100000000000000,
	0b00100000000000000010000000000000,
	0b01000000000000000010000000000000,
	0b01000000000000000001000000000000,
	0b01000000000000000011100000000000,
	0b01000000000000000010100000000000,
	0b01000000000000001100011001110000,
	0b01000000000000001000001001010000,
	0b01000000000001101100011110001100,
	0b01000000000001100010100100000100,
	0b01000000000000000011100110001100,
	0b01000000001000000000010001010000,
	0b01000000001000000000010001110000,
	0b01000000001000000000011100000000,
	0b01000000001000000000010100000000,
	0b01000000001000000001100011000000,
	0b01000000001000000001000001000000,
	0b01000000000000000001100011000000,
	0b01000000000001100011010100000000,
	0b01000000000001100010011100000000,
	0b00100000000000000010000000000000,
	0b00010000000000000100000000000000,
	0b00001000000000111000000000000000,
	0b00000111111111000000000000000000,
	0, 0,
	// fourth
	0, 0, 
	0b00000111111111000000000000000000,
	0b00001000000000110000000000000000,
	0b00010000000000001000000000000000,
	0b00100000000000000100000000000000,
	0b01000000000000000100000000000000,
	0b01000000000000000010000000000000,
	0b01000000000000000111000000000000,
	0b01000000000000000101000000000000,
	0b01000000000000011000110001110000,
	0b01000000000000010000010001010000,
	0b01000000000011011000110110001100,
	0b01000000000011000101000100000100,
	0b01000000000000000111000110001100,
	0b01000000010000000000100001010000,
	0b01000000010000000000100001110000,
	0b01000000010000000000111000000000,
	0b01000000010000000000101000000000,
	0b01000000010000000011000110000000,
	0b01000000010000000010000010000000,
	0b01000000000000000011000110000000,
	0b01000000000011000110101000000000,
	0b01000000000011000100111000000000,
	0b00100000000000000100000000000000,
	0b00010000000000001000000000000000,
	0b00001000000001110000000000000000,
	0b00000111111110000000000000000000,
	0, 0,
};

const uint32_t const enemy_border[30] = {
	0, 0, 
	0b00000111111111100000000000000000,
	0b00001111111111111000000000000000,
	0b00011111111111111100000000000000,
	0b00111111111111111110000000000000,
	0b01111111111111111110000000000000,
	0b01111111111111111111000000000000,
	0b01111111111111111111100000000000,
	0b01111111111111111111100000000000,
	0b01111111111111111111111011110000,
	0b01111111111111111111111011110000,
	0b01111111111111111111111111111100,
	0b01111111111111111111101111111100,
	0b01111111111111111111101111111100,
	0b01111111111111111111110011110000,
	0b01111111111111111111110011110000,
	0b01111111111111111111111100000000,
	0b01111111111111111111111100000000,
	0b01111111111111111111111111000000,
	0b01111111111111111111111111000000,
	0b01111111111111111111111111000000,
	0b01111111111111111111111100000000,
	0b01111111111111111111111100000000,
	0b00111111111111111110000000000000,
	0b00011111111111111100000000000000,
	0b00001111111111111000000000000000,
	0b00000111111111000000000000000000,
	0, 0
};


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

void display_update_text(int x, int amount_of_chars, int selected_row, uint32_t *map)
{
	int i, j, k;
	int c;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < amount_of_chars + 1; j++)
		{
			c = textbuffer[i][j];
			if (c != 32)
			{
			}

			if (c & 0x80)
				continue;

			for (k = 0; k < 8; k++)
			{
				int startX = x + j * 8 + k;
				// map[startX] &= ~(255 << (i * 8));
				if (i == selected_row)
				{
					set_8s_in_32(map, ~font[c * 8 + k], i, startX);
				}
				else
				{
					set_8s_in_32(map, font[c * 8 + k], i, startX);
				}
			}
		}
	}
}

void display_update_text_row(int x, int amount_of_chars, int selected_row, int row_to_change, uint32_t *map)
{
	int j, k;
	int c;
	for (j = 0; j < amount_of_chars + 1; j++)
	{
		c = textbuffer[row_to_change][j];

		if (c & 0x80)
			continue;

		for (k = 0; k < 8; k++)
		{
			int startX = x + j * 8 + k;
			map[startX] &= ~(255 << (row_to_change * 8));
			if (row_to_change == selected_row)
			{
				set_8s_in_32(map, ~font[c * 8 + k], row_to_change, startX);
			}
			else
			{
				set_8s_in_32(map, font[c * 8 + k], row_to_change, startX);
			}
		}
	}
}

void set_8s_in_32(uint32_t *map, uint8_t num, int line, int startX)
{
	uint32_t mask = 0;
	mask = num << (line * 8);
	map[startX] |= mask;
}

#define ITOA_BUFSIZ (24)
char *itoaconv(int num)
{
	register int i, sign;
	static char itoa_buffer[ITOA_BUFSIZ];
	static const char maxneg[] = "-2147483648";

	itoa_buffer[ITOA_BUFSIZ - 1] = 0; /* Insert the end-of-string marker. */
	sign = num;						  /* Save sign. */
	if (num < 0 && num - 1 > 0)		  /* Check for most negative integer */
	{
		for (i = 0; i < sizeof(maxneg); i += 1)
			itoa_buffer[i + 1] = maxneg[i];
		i = 0;
	}
	else
	{
		if (num < 0)
			num = -num;		 /* Make number positive. */
		i = ITOA_BUFSIZ - 2; /* Location for first ASCII digit. */
		do
		{
			itoa_buffer[i] = num % 10 + '0'; /* Insert next digit. */
			num = num / 10;					 /* Remove digit from number. */
			i -= 1;							 /* Move index to next empty position. */
		} while (num > 0);
		if (sign < 0)
		{
			itoa_buffer[i] = '-';
			i -= 1;
		}
	}
	/* Since the loop always sets the index i to the next empty position,
	 * we must add 1 in order to return a pointer to the first occupied position. */
	return (&itoa_buffer[i + 1]);
}