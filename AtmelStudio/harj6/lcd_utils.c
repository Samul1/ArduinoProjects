/*********************************************************

	Author: Ville Tiikkainen
	Group: TTV16S1
	Organisation: Kajaani University of Applied Sciences
	Date: 22.03.2017
	Note: -

*********************************************************/

/*

How the LCD Display works

    TOP OF THE LCD DISPLAY
*****************************
*MSB       LSB*MSB       LSB*
*   Column    *   Column    *
*             *             *
*****************************
*             *             *
*     Page    *    Page     *
*             *             *
*             *             *
*             *             *
*             *             ****
*             *             ****
*             *             ****	Connector to the card
*             *             ****
*             *             ****
*             *             *
*****************************
*/

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lcd_utils.h"

#define WIDTH 64
#define HEIGHT 128
#define PIXELWIDTH 8

#define CMD  0
#define DATA 1

char FLAGS = 0x00;

/*
	CHARACTERS ARE 5 x 8 's
*/

char CHAR_A[5] = { 0xFE, 0x11, 0x11, 0x11, 0xFE }; // A
char CHAR_a[5] = { 0x70, 0x88, 0x88, 0xF8, 0x00 }; // a
char CHAR_B[5] = { 0xFF, 0x89, 0x89, 0x89, 0x76 }; // B
char CHAR_b[5] = { 0xFF, 0x50, 0x88, 0x88, 0x70 }; // b
char CHAR_C[5] = { 0x7E, 0x81, 0x81, 0x81, 0x66 }; // C
char CHAR_c[5] = { 0x70, 0x88, 0x88, 0x50, 0x00 }; // c
char CHAR_D[5] = { 0xFF, 0x81, 0x81, 0x82, 0x7C }; // D
char CHAR_d[5] = { 0x70, 0x88, 0x88, 0x48, 0xFF }; // d
char CHAR_E[5] = { 0xFF, 0x89, 0x89, 0x81, 0x81 }; // E
char CHAR_e[5] = { 0x70, 0xA8, 0xA8, 0xB0, 0x00 }; // e
char CHAR_F[5] = { 0xFF, 0x09, 0x09, 0x01, 0x01 }; // F
char CHAR_f[5] = { 0x04, 0xFE, 0x09, 0x01, 0x06 }; // f
char CHAR_G[5] = { 0x7E, 0x81, 0x81, 0x91, 0x76 }; // G
char CHAR_g[5] = { 0x00, 0x4C, 0x92, 0x92, 0x7E }; // g
char CHAR_H[5] = { 0xFF, 0x08, 0x08, 0x08, 0xFF }; // H
char CHAR_h[5] = { 0xFF, 0x10, 0x08, 0x08, 0xF0 }; // h
char CHAR_I[5] = { 0x81, 0xFF, 0x81, 0x00, 0x00 }; // I
char CHAR_i[5] = { 0x80, 0xF4, 0x80, 0x00, 0x00 }; // i
char CHAR_J[5] = { 0x70, 0x80, 0x80, 0x80, 0x7F }; // J
char CHAR_j[5] = { 0x40, 0x80, 0x80, 0x76, 0x00 }; // j
char CHAR_K[5] = { 0xFE, 0x10, 0x28, 0x44, 0x82 }; // K
char CHAR_k[5] = { 0xFF, 0x20, 0x50, 0x88, 0x00 }; // k
char CHAR_L[5] = { 0xFF, 0x80, 0x80, 0x80, 0x80 }; // L
char CHAR_l[5] = { 0xFF, 0x00, 0x00, 0x00, 0x00 }; // l
char CHAR_M[5] = { 0xFF, 0x02, 0x0C, 0x02, 0xFF }; // M
char CHAR_m[5] = { 0xF8, 0x10, 0xE0, 0x10, 0xE0 }; // m
char CHAR_N[5] = { 0xFF, 0x06, 0x18, 0x60, 0xFF }; // N
char CHAR_n[5] = { 0xF8, 0x10, 0x08, 0x08, 0xF0 }; // n
char CHAR_O[5] = { 0x7E, 0x81, 0x81, 0x81, 0x7E }; // O
char CHAR_o[5] = { 0x70, 0x88, 0x88, 0x88, 0x70 }; // o
char CHAR_P[5] = { 0xFF, 0x11, 0x11, 0x11, 0x0E }; // P
char CHAR_p[5] = { 0xFF, 0x12, 0x11, 0x11, 0x0E }; // p
char CHAR_Q[5] = { 0x7E, 0x81, 0xA1, 0x41, 0xBE }; // Q
char CHAR_q[5] = { 0x0C, 0x12, 0x12, 0xFE, 0x00 }; // q
char CHAR_R[5] = { 0xFF, 0x11, 0x31, 0x51, 0x9E }; // R
char CHAR_r[5] = { 0xFC, 0x08, 0x08, 0x10, 0x00 }; // r
char CHAR_S[5] = { 0x46, 0x89, 0x89, 0x89, 0x72 }; // S
char CHAR_s[5] = { 0x48, 0x94, 0xA4, 0x48, 0x00 }; // s
char CHAR_T[5] = { 0x01, 0x01, 0xFF, 0x01, 0x01 }; // T
char CHAR_t[5] = { 0x08, 0xFC, 0x88, 0x00, 0x00 }; // t
char CHAR_U[5] = { 0x7F, 0x80, 0x80, 0x40, 0xFF }; // U
char CHAR_u[5] = { 0x78, 0x80, 0x80, 0x40, 0xF8 }; // u
char CHAR_V[5] = { 0x3F, 0x40, 0x80, 0x40, 0x3F }; // V
char CHAR_v[5] = { 0x38, 0x40, 0x80, 0x78, 0x00 }; // v
char CHAR_W[5] = { 0x7F, 0x80, 0x70, 0x80, 0x7F }; // W
char CHAR_w[5] = { 0x70, 0x80, 0x40, 0x80, 0x70 }; // w
char CHAR_X[5] = { 0xC6, 0x28, 0x10, 0x28, 0xC6 }; // X
char CHAR_x[5] = { 0xD8, 0x20, 0xD8, 0x00, 0x00 }; // x
char CHAR_Y[5] = { 0x07, 0x08, 0xF0, 0x08, 0x07 }; // Y
char CHAR_y[5] = { 0x4C, 0x90, 0x90, 0x7C, 0x00 }; // y
char CHAR_Z[5] = { 0xC2, 0xA2, 0x92, 0x8A, 0x86 }; // Z
char CHAR_z[5] = { 0xC4, 0xA4, 0x94, 0x8C, 0x00 }; // z

char CHAR_0[5] = { 0x7E, 0xA1, 0x91, 0x89, 0x7E }; // 0
char CHAR_1[5] = { 0x82, 0xFF, 0x80, 0x00, 0x00 }; // 1
char CHAR_2[5] = { 0xC2, 0xA1, 0x91, 0x89, 0x86 }; // 2
char CHAR_3[5] = { 0x42, 0x81, 0x89, 0x89, 0x76 }; // 3
char CHAR_4[5] = { 0x18, 0x14, 0x12, 0x11, 0xFF }; // 4
char CHAR_5[5] = { 0x4F, 0x89, 0x89, 0x89, 0x71 }; // 5
char CHAR_6[5] = { 0x7E, 0x89, 0x89, 0x89, 0x72 }; // 6
char CHAR_7[5] = { 0x01, 0x01, 0xC1, 0x31, 0x0F }; // 7
char CHAR_8[5] = { 0x76, 0x89, 0x89, 0x89, 0x76 }; // 8
char CHAR_9[5] = { 0x4E, 0x91, 0x91, 0x91, 0x7E }; // 9

char CHAR_DOT[5] =         { 0xC0, 0xC0, 0x00, 0x00, 0x00 }; // .
char CHAR_COMMA[5] =       { 0x80, 0x60, 0x00, 0x00, 0x00 }; // ,
char CHAR_DASH[5] =        { 0x10, 0x10, 0x10, 0x00, 0x00 }; // -
char CHAR_SPACE[5] =       { 0x00, 0x00, 0x00, 0x00, 0x00 }; // ' '
char CHAR_CITATION[5] =    { 0x06, 0x00, 0x06, 0x00, 0x00 }; // "
char CHAR_APOSTHROPE[5] =  { 0x06, 0x00, 0x00, 0x00, 0x00 }; // '
char CHAR_EXCLAMATION[5] = { 0xBF, 0x00, 0x00, 0x00, 0x00 }; // !
char CHAR_QUESTION[5] =    { 0x06, 0xB2, 0x0E, 0x00, 0x00 }; // ?
char CHAR_COLON[5] =       { 0x28, 0x00, 0x00, 0x00, 0x00 }; // :

/*
	Pixels on screen, research the dimension
*/
char pixels[128 * 8]; // Screen pixels, what is currently on the screen
char buffer[128 * 8]; // "Backbuffer", what will be rendered to the screen

/*
	pixels[0] = TOP LEFT
	pixels[1] = TOP LEFT + 8  width pixels
	pixels[2] = TOP LEFT + 16 width pixels etc.

	pixels[0 + 8 ] = TOP LEFT + 1 height pixel
	pixels[0 + 16] = TOP LEFT + 2 height pixels etc.
*/

/*
	sendData(uchar CD, uchar DATA)
	CD = Command/Data (0 = Command, 1 = Data)
	DATA = 8 bits of data
*/
void sendData(unsigned char CDWR, unsigned char data);

/*
	getChar(char c)
	Returns the data for printing characters
*/
char * getChar(char c);

/*
	void flip(uchar* c1, uchar* c2)
	Flips the values of the two arguments
*/
void flip(unsigned char* c1, unsigned char* c2);

void LCD_SetFlags(char flags)
{
	FLAGS = flags;
}

void LCD_Init()
{
	LCD_DDR |= LCD_RESET_PIN | LCD_DATA_PIN | LCD_CD_PIN | LCD_CLK_PIN | LCD_SCE_PIN;

	/*
		RESET THE DISPLAY
	*/
	_delay_ms(30);
	LCD_PORT &= ~LCD_RESET_PIN; //Reset
	_delay_ms(30);
	LCD_PORT |= LCD_RESET_PIN;  //Disable Reset
	_delay_ms(30);

	sendData(0xEB, CMD); // LCD-Bias
	sendData(0x23, CMD); // 64 rows
	sendData(0x81, CMD); // Potentiometer
	sendData(0x64, CMD); // Contrast
	sendData(0xAF, CMD); // Enable display
	sendData(0xCC, CMD); // RAM Mapping

	if(FLAGS & FLAG_INVERTED) sendData(0xA7, CMD); // Inversion

	LCD_Clear(); // Clear the buffer
	LCD_Display(); // Send the clear buffer to display
}

void LCD_Init_With_Flags(char flags)
{
	LCD_SetFlags(flags);
	LCD_Init();
}

/*
	sendData(uchar CD, uchar DATA)
	CD = Command/Data (0 = Command, 1 = Data)
	DATA = 8 bits of data
*/
void sendData(unsigned char data, unsigned char CD)
{
	int j;

	// Enable display controller (LOW active)
	LCD_PORT &= ~LCD_SCE_PIN;

	if(CD == 1) LCD_PORT |=  LCD_CD_PIN; // Data
	else        LCD_PORT &= ~LCD_CD_PIN; // Command

	for(int i = 0;i < 8;i++) // Send all the bits to driver
	{
		if((data >> (7 - i)) & 1)     LCD_PORT |=  LCD_DATA_PIN; // If the data is 1
		else                      LCD_PORT &= ~LCD_DATA_PIN; // If the data is 0
		// Toggle clock pin
		LCD_PORT |= LCD_CLK_PIN; 
		for(j = 0;j < 4;j++);
		LCD_PORT &= ~LCD_CLK_PIN;
	}

	// Disable display controller (LOW active)
	LCD_PORT |= LCD_SCE_PIN;
}

void LCD_Test()
{
	sendData(0xA5, CMD);
}

void LCD_Invert()
{
	if(FLAGS & FLAG_INVERTED) FLAGS &= ~(1 << FLAG_INVERTED);
	else FLAGS |= FLAG_INVERTED;
	sendData(0xA6 + ((FLAGS & FLAG_INVERTED) > 0 ? 1 : 0), CMD);
}

void LCD_DrawPixel(unsigned char x, unsigned char y)
{
	if((FLAGS & FLAG_LANDSCAPE) == 0)
	{
		if(x >= WIDTH || y >= HEIGHT) return;
		buffer[(x / 8) * 128 + y] |= (1 << (7 - x % 8));
	}
	else
	{
		unsigned char tempX = x;
		unsigned char tempY = y;
		x = 63 - tempY;
		y = tempX;
		buffer[(x / 8) * 128 + y] |= (1 << (7 - x % 8));
	}
}

void LCD_ClearPixel(unsigned char x, unsigned char y)
{
	if((FLAGS & FLAG_LANDSCAPE) == 0)
	{
		if(x >= WIDTH || y >= HEIGHT) return;
		buffer[(x / 8) * 128 + y] &= ~(1 << (7 - x % 8));
	}
	else
	{
		unsigned char tempX = x;
		unsigned char tempY = y;
		x = 63 - tempY;
		y = tempX;
		buffer[(x / 8) * 128 + y] &= ~(1 << (7 - x % 8));
	}
}

void LCD_Clear()
{
	memset(buffer, 0, sizeof(buffer));
}

void LCD_ClearArea(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1)
{
	for(char y = y0;y < y1;y++)
	{
		for(char x = x0;x < x1;x++)
		{
			LCD_ClearPixel(x, y);
		}
	}
}

void LCD_DrawRect(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1)
{
	for(unsigned char x = x0;x <= x1;x++)
	{
		LCD_DrawPixel(x, y0);
		LCD_DrawPixel(x, y1);
	}
	for(unsigned char y = y0;y <= y1;y++)
	{
		LCD_DrawPixel(x0, y);
		LCD_DrawPixel(x1, y);
	}
}

void LCD_FillRect(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1)
{
	if(x0 > x1)flip(&x0, &x1);
	if(y0 > y1)flip(&y0, &y1);
	for(unsigned char x = x0;x <= x1;x++)
	{
		for(unsigned char y = y0;y <= y1;y++)
		{
			LCD_DrawPixel(x, y);
		}
	}
}

void LCD_DrawLine(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1)
{
	int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
	int err = (dx>dy ? dx : -dy)/2, e2;

  	for(;;)
	{
   		LCD_DrawPixel(x0,y0);
   		if (x0==x1 && y0==y1) break;
   		e2 = err;
   		if (e2 >-dx) { err -= dy; x0 += sx; }
   		if (e2 < dy) { err += dx; y0 += sy; }
  	}
}

int LCD_PrintChar(const char c, unsigned char x0, unsigned char y0)
{
	char whitespace = 0;
	char * CHAR = getChar(c);
	for(unsigned char x = 0;x < 5;x++)
	{
		if(FLAGS & FLAG_IGNORE_WHITESPACE && CHAR[x] == 0) whitespace++;
		if(CHAR[x] == 0x00)continue;
		for(unsigned char y = 0;y < 8;y++)
		{
			if(CHAR[x] & 1 << y)
				LCD_DrawPixel(x + x0, y + y0);
		}
	}
	return whitespace;
}

void LCD_PrintString(const char * str, unsigned char x0, unsigned char y0)
{
	unsigned char xOff = 0;
	unsigned char yOff = 0;
	while(*str != '\0')
	{
		if(*str == ' ') xOff += 3;
		else if(*str == '\n') 
		{
			xOff = 0;
			yOff += 9;
		}
		else xOff += 6 - LCD_PrintChar(*str, x0 + xOff, y0 + yOff);
		str++;
	}
}

int LCD_StringWidth(const char * str)
{
	int length = 0;
	const char * original = str;
	while(*str != '\0')
	{
		length++;
		str++;
	}
	str = original;
	if(FLAGS & FLAG_IGNORE_WHITESPACE)
	{
		length = 0;
		char * CHAR;
		int off;
		while(*str != '\0')
		{
			CHAR = getChar(*str);
			for(off = 4;off >= 0;off--)
			{
				if(CHAR[off] != 0x00) break;
			}
			length += off + 2;
			str++;
		}
		str = original;
		return length;
	}
	else return 5 * length;
}

void LCD_PrintSpecial(const char * SPECIAL, unsigned char x0, unsigned char y0, unsigned char width, unsigned char height)
{
	for(unsigned char x = 0;x < width;x++)
	{
		for(unsigned char y = 0;y < height;y++)
		{
			if(SPECIAL[x] & (1 << y))
			{
				LCD_DrawPixel(x + x0, y + y0);
			}
		}
	}
}

void LCD_Display()
{
	int page, column;
	for(page = 7;page >= 0;page--)
	{
		sendData(0xB0 | (7 - page), CMD); // Page address
		sendData(0x10, CMD); // Column addres MSB (0)
		sendData(0x04, CMD); // Column address LSB (4)
		for(column = 0;column < 128;column++)
		{
			sendData(buffer[page * 128 + column], DATA); // Data
		}
	}
	memcpy(pixels, buffer, sizeof(pixels));
}

int LCD_GetWidth()
{
	return ((FLAGS & FLAG_LANDSCAPE) > 0 ? 128 : 64);
}

int LCD_GetHeight()
{
	return ((FLAGS & FLAG_LANDSCAPE) > 0 ? 64 : 128);
}

char * getChar(char c)
{
	switch(c)
	{
		case 'A': return CHAR_A;
		case 'a': return CHAR_a;
		case 'B': return CHAR_B;
		case 'b': return CHAR_b;
		case 'C': return CHAR_C;
		case 'c': return CHAR_c;
		case 'D': return CHAR_D;
		case 'd': return CHAR_d;
		case 'E': return CHAR_E;
		case 'e': return CHAR_e;
		case 'F': return CHAR_F;
		case 'f': return CHAR_f;
		case 'G': return CHAR_G;
		case 'g': return CHAR_g;
		case 'H': return CHAR_H;
		case 'h': return CHAR_h;
		case 'I': return CHAR_I;
		case 'i': return CHAR_i;
		case 'J': return CHAR_J;
		case 'j': return CHAR_j;
		case 'K': return CHAR_K;
		case 'k': return CHAR_k;
		case 'L': return CHAR_L;
		case 'l': return CHAR_l;
		case 'M': return CHAR_M;
		case 'm': return CHAR_m;
		case 'N': return CHAR_N;
		case 'n': return CHAR_n;
		case 'O': return CHAR_O;
		case 'o': return CHAR_o;
		case 'P': return CHAR_P;
		case 'p': return CHAR_p;
		case 'Q': return CHAR_Q;
		case 'q': return CHAR_q;
		case 'R': return CHAR_R;
		case 'r': return CHAR_r;
		case 'S': return CHAR_S;
		case 's': return CHAR_s;
		case 'T': return CHAR_T;
		case 't': return CHAR_t;
		case 'U': return CHAR_U;
		case 'u': return CHAR_u;
		case 'V': return CHAR_V;
		case 'v': return CHAR_v;
		case 'W': return CHAR_W;
		case 'w': return CHAR_w;
		case 'X': return CHAR_X;
		case 'x': return CHAR_x;
		case 'Y': return CHAR_Y;
		case 'y': return CHAR_y;
		case 'Z': return CHAR_Z;
		case 'z': return CHAR_z;

		case '0': return CHAR_0;
		case '1': return CHAR_1;
		case '2': return CHAR_2;
		case '3': return CHAR_3;
		case '4': return CHAR_4;
		case '5': return CHAR_5;
		case '6': return CHAR_6;
		case '7': return CHAR_7;
		case '8': return CHAR_8;
		case '9': return CHAR_9;

		case '.': return CHAR_DOT;
		case ',': return CHAR_COMMA;
		case '-': return CHAR_DASH;
		case '"': return CHAR_CITATION;
		case '!': return CHAR_EXCLAMATION;
		case '?': return CHAR_QUESTION;
		case ':': return CHAR_COLON;
		default: return CHAR_SPACE;
	}
}

void flip(unsigned char* c1, unsigned char* c2)
{
	unsigned char temp = *c1;
	*c1 = *c2;
	*c2 = temp;
	temp = 0;
}
