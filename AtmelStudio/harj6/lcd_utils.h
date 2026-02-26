/*

	Author: Ville Tiikkainen
	Group: TTV16S1
	Organisation: Kajaani University of Applied Sciences
	Date: 22.03.2017
	Note: -

*/
/*
	INCLUDE GUARD
*/
#ifndef _LCD_UTILS_H_
#define _LCD_UTILS_H_

/*
	Definitions for connections to LCD controller
*/
#define LCD_DDR       DDRC
#define LCD_PORT      PORTC
#define LCD_RESET_PIN (1 << PC0)
#define LCD_SCE_PIN   (1 << PC1)
#define LCD_CD_PIN    (1 << PC2)
#define LCD_DATA_PIN  (1 << PC3)
#define LCD_CLK_PIN   (1 << PC4)

/*
	FLAGS	
*/
#define FLAG_LANDSCAPE         0x01
#define FLAG_INVERTED          0x02
#define FLAG_IGNORE_WHITESPACE 0x04

/*
	void LCD_SetFlags(char FLAGS)
	Sets the flags that are used for rendering
	Possible flags are:
		INIT ONLY:
			FLAG_LANDSCAPE - Enables landscape mode
			FLAG_INVERTED - Enables inversion mode
		RUNTIME OR INIT:
			FLAG_IGNORE_WHITESPACE - Sets the string printing to ignore whitespace, default false
*/
void LCD_SetFlags(char FLAGS);

/*
	void LCD_Init(uchar orientation)
	Initializes the LCD display for use
	Uses flags FLAG_INVERTED and FLAG_LANDSCAPE on init, other flags are runtime only
*/
void LCD_Init();

/*
	void LCD_Init_With_Flags(char FLAGS)
	Wrapper for LCD_SetFlags() and LCD_Init()
*/
void LCD_Init_With_Flags(char FLAGS);

/*
	void LCD_Test()
	Used to test if the LCD display is working correctly
	Puts the LCD to ALL-ON mode
*/
void LCD_Test();

/*
	void LCD_Invert()
	Toggles the inversion of the display
*/
void LCD_Invert();

/*
	LCD_DrawPixel(uchar x, uchar y)
	Draws a single pixel to buffer
*/
void LCD_DrawPixel(unsigned char x, unsigned char y);

/*
	LCD_ClearPixel(uchar x, uchar y)
	Clears a single pixel from buffer
*/
void LCD_ClearPixel(unsigned char x, unsigned char y);

/*
	LCD_Clear()
	Clears the whole buffer
*/
void LCD_Clear();

/*
	LCD_ClearArea(uchar x0, uchar y0, uchar x1, uchar y1)
	Clears an area from x0,y0 to x1,y1 from buffer
*/
void LCD_ClearArea(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1);

/*
	LCD_DrawRect(uchar x0, uchar y0, uchar x1, uchar y1)
	Draws the outlines of a rectangle from x0,y0 to x1,y1
	The position is the top-left of the rectangle
*/
void LCD_DrawRect(unsigned char x0,  unsigned char y0,  unsigned char x1, unsigned char y1);

/*
	LCD_FillRect(uchar x, uchar y, uchar width, uchar height)
	Fills a rectangle from x0,y0, to x1,y1
	The position is the top-left of the rectangle
*/
void LCD_FillRect(unsigned char x0,  unsigned char y0,  unsigned char width, unsigned char height);

/*
	void LCD_DrawLine(uchar x0, uchar y0, uchar x1, uchar y1)
	Draws a line using Bresenham's algorithm from x0, y0 to x1, y1
*/
void LCD_DrawLine(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1);

/*
	LCD_PrintChar(char c, uchar x, uchar y)
	Prints a character to specified position
	The position is top left of the character
	Returns the whitespace count
*/
int LCD_PrintChar(const char c, unsigned char x, unsigned char y);

/*
	LCD_PrintString(const char * str, uchar x, uchar y)
	Prints a string to specified position
	The position is top left of the first character of the string
	Auto-wrap is disabled by default
*/
void LCD_PrintString(const char * str, unsigned char x, unsigned char y);

/*
	int LCD_StringWidth(const char * str)
	Returns the width of string str if printed on screen
	Depends on FLAG_IGNORE_WHITESPACE
		If FLAG_IGNORE_WHITESPACE is not set, all char widths are 5
		If FLAG_IGNORE_WHITESPACE is set, char widths are variable
	Does NOT handle newlines '\n', undefined behaviour
	Also can have errors on some characters
*/
int LCD_StringWidth(const char * str);

/*
	LCD_PrintSpecial(const chat * spec, uchar x, uchar y, uchar width, uchar height)
	Prints a custom character to the specified position
	The position is the top-left of the character
	The character must be of size width * height
	Maximum height of a character is 8
	Width can be higher
*/
void LCD_PrintSpecial(const char * spec, unsigned char x, unsigned char y, unsigned char width, unsigned char height);

/*
	void Display()
	Displays drawn buffer to the LCD display
*/
void LCD_Display();

/*
	LCD_GetWidth()
	Returns the width of the display, depending on the orientation of the screen
	Possible returns: PORTRAIT (64) LANDSCAPE(128)
*/
int LCD_GetWidth();

/*
	LCD_GetHeight()
	Returns the height of the display, depending on the orientation of the screen
	Possible returns: PORTRAIT(128) LANDSCAPE(64)
*/
int LCD_GetHeight();

#endif
