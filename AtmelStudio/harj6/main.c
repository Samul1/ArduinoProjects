/********************************************************
** Kajaani Amk
** Markku Karppinen 25.5. 2012
** Muokattu: 4.10.12
** Muokattu: 12.8.-14 uudelle bartron näytölle (COG_2)
** Huomaa korjata Datalinja näytön liittimellä.
** Muutettu Näytön alustus Init Fugntio!
**
** Markku Karppinen 26.10.17
** Testaus uudet näyttödriveri.
**
** ** Samuli Juutinen TTV23SA 20.02.2024
** **   Neliön tulostus.
** **   Ympyrän tulostus.
** **   Suun tulostus
** **   Silmien tulostus
**
** Näyttö Batron  Example Program
** Näyttö kytketään Opetuskortti II 
*	PC0: Reset
*	PC1: Sce 
*	PC2: D / C
*	PC3: SDin (Data)
*	PC4: SCLK (Kello)
**
** Alusta:
** 	Opetukortti ATMEGA 128:: AVR-GCC
**	Fuse Bits:0xFF, 0xB9, 0xE4
**
*************!!!!!!  Power 3.3c !!!!!*********************
**
**********************************************************/
#define F_CPU 8000000UL		
#include <util/delay.h>

#define	true	1
#define false	0
#define	NB_SHOTS	5
#define STEP_MAX	1

#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "lcd_utils.h"
#include "stdlib.h"
#include "string.h"


// Funktiot:
void suu(int num);
void silma(int randomNum1, double x, double y, int randomNum2);

// Globaalit muuttujaiset
int x, y;
/********************************
 main 

 *******************************/
int main(void)
{
int i,j;
char Buffer[6];
	
	//Käynnistys viive
	for(i=0;i<100;i++)
		for(j=0;j<100;j++);

	LCD_Init_With_Flags(FLAG_LANDSCAPE | FLAG_IGNORE_WHITESPACE);
	/*
	LCD_DrawLine(0, 11, 128, 11);
	LCD_DrawLine(0, 13, 128, 13);

	LCD_PrintChar('S',20,2);
	LCD_PrintChar('A',30,2);
	LCD_PrintChar('M',40,2);
	LCD_PrintChar('U',50,2);
	LCD_PrintChar('L',60,2);
	LCD_PrintChar('I',70,2);
	LCD_Display();
	
	LCD_PrintChar('T',10,15);
	LCD_PrintChar('T',15,15);
	LCD_PrintChar('V',20,15);
	LCD_PrintChar('2',25,15);
	LCD_PrintChar('3',30,15);
	LCD_PrintChar('S',35,15);
	LCD_PrintChar('A',40,15);

	LCD_PrintChar('H',10,25);
	LCD_PrintChar('U',15,25);
	LCD_PrintChar('O',20,25);
	LCD_PrintChar('M',25,25);
	LCD_PrintChar('E',30,25);
	LCD_PrintChar('N',35,25);
	LCD_PrintChar('T',40,25);
	LCD_PrintChar('A',45,25);
	
	_delay_ms(300);

	sprintf(Buffer, "ÄJ Tulostuu!");

	while(1)
	{

		LCD_PrintString(Buffer, 10, 40);
		LCD_Display();

		_delay_ms(10);

	}
	*/

	
	//LCD_PrintChar('S',30,25);
	// Nelikulmio:
	LCD_DrawRect(5, 5, 123, 59);

	
		
	

	
	double r1;
	double r2;
	int r3;
	
	for(;;)
	{
		// Randomisoinnit:
		r1 = rand() % 15 + 8;
		r2 = rand() % 15 + 8;
		r3 = rand() % 5 + 1;

		// Vasen silmä ja suu:
		silma(r1, 34, 32, r3);
		suu(r3);
		
		// Uusi randomisointi oikealle silmälle:
		r3 = rand() % 5 + 1;
		// Oikean silmän tulostus:
		silma(r2, 90, 32, r3);
		

		LCD_Display();
		_delay_ms(1000);
		LCD_ClearArea(6, 6, 122, 58);
	}
	

}

void suu(int num)
{
	// Muuttujien alustus
	double ii = 0.0;
	double x0 = 0.0;
	double y0 = 0.0;
	double piste = 0.0;
	double cosix = 0.0;
	double siniy = 0.0;
	unsigned char xx;
	unsigned char yy;
	double r = 7;
	int i;

	if(num == 1)
	{
		// Suu1 hymy
		x0 = 62;
		y0 = 46;
		for(i=1;i<=180;i++)
		{
			ii = i;
			piste=i*((2.0*3.14)/360.0);
			cosix = cos(piste);
			siniy = sin(piste);
			xx = x0 + r*cosix;
			yy = y0 + r*siniy;
		
			LCD_DrawPixel(xx,yy);		
		}
	}

	else if(num == 2)
	{
		// Suu2: viiva
		LCD_DrawLine(48, 46, 76, 46);
	}

	else if(num == 3)
	{
		// Suu3 suru
		x0 = 62;
		y0 = 46;
		for(i=180;i<=360;i++)
		{
			ii = i;
			piste=i*((2.0*3.14)/360.0);
			cosix = cos(piste);
			siniy = sin(piste);
			xx = x0 + r*cosix;
			yy = y0 + r*siniy;
		
			LCD_DrawPixel(xx,yy);		
		}
	}

	else if(num == 4)
	{
	double r1 = 5.0;
	double r2 = 4.0;
		// Suu3 uwu
		x0 = 57;
		y0 = 46;
		for(i=0;i<=180;i++)
		{
			ii = i;
			piste=i*((2.0*3.14)/360.0);
			cosix = cos(piste);
			siniy = sin(piste);
			xx = x0 + r1*cosix;
			yy = y0 + r1*siniy;
		
			LCD_DrawPixel(xx,yy);		
		}
		x0 = 66;
		y0 = 46;
		for(i=0;i<=180;i++)
		{
			ii = i;
			piste=i*((2.0*3.14)/360.0);
			cosix = cos(piste);
			siniy = sin(piste);
			xx = x0 + r2*cosix;
			yy = y0 + r2*siniy;
		
			LCD_DrawPixel(xx,yy);		
		}
	}

	else if(num == 5)
	{
	double r1 = 6.0;
		// Suu5: :P
		// Viiva:
		LCD_DrawLine(48, 46, 76, 46);

		// Kieli:
		x0 = 72;
		y0 = 46;
		for(i=0;i<=180;i++)
		{
			ii = i;
			piste=i*((2.0*3.14)/360.0);
			cosix = cos(piste);
			siniy = sin(piste);
			xx = x0 + r1*cosix;
			yy = y0 + r1*siniy;
		
			LCD_DrawPixel(xx,yy);		
		}
	}
}

void silma(int randomNum1, double x, double y, int randomNum2)
{
	// Muuttujien alustus.
	double ii = 0.0;
	double x0 = x;
	double y0 = y;
	double piste = 0.0;
	double cosix = 0.0;
	double siniy = 0.0;
	unsigned char xx;
	unsigned char yy;
	double r = randomNum1;
	int i;

	// Pyöreä silmä
	if(randomNum2 == 1 || randomNum2 == 2)
	{
		// Ympyrä 1
		for(i=1;i<=360;i++)
		{
			ii = i;
			piste=i*((2.0*3.14)/360.0);
			cosix = cos(piste);
			siniy = sin(piste);
			xx = x0 + r*cosix;
			yy = y0 + r*siniy;
		
			LCD_DrawPixel(xx,yy);		
		}
	}
	// Risti silmä
	else if(randomNum2 == 3 || randomNum2 == 4)
	{
		for(i=1;i<=360;i++)
		{
			ii = i;
			piste=i*((2.0*3.14)/360.0);
			cosix = cos(piste);
			siniy = sin(piste);
			xx = x0 + r*cosix;
			yy = y0 + r*siniy;
		
			LCD_DrawPixel(xx,yy);		
		}
		LCD_DrawLine(x-r+8, y, x+r-8, y);
		LCD_DrawLine(x, y-r+8, x, y+r-8);
	}
	// x-silmä
	else if(randomNum2 == 5)	
	{
		for(i=1;i<=360;i++)
		{
			ii = i;
			piste=i*((2.0*3.14)/360.0);
			cosix = cos(piste);
			siniy = sin(piste);
			xx = x0 + r*cosix;
			yy = y0 + r*siniy;
		
			LCD_DrawPixel(xx,yy);		
		}
		LCD_DrawLine(x-r+8, y-r+8, x+r-8, y+r-8);
		LCD_DrawLine(x+r-8, y-r+8, x-r+8, y+r-8);
	}
	

}
