/*
 * 1616_lcd.h
 *
 *  Created on: Jul 24, 2014
 *      Author: karibe
 */

#ifndef NOKIA_1616_LCD_H_
#define NOKIA_1616_LCD_H_
//get defined font lookup and colors
#include "font.h"
#include "colors.h"

//colormode and background color
enum colormodes {twelve=0,sixteen,eighteen};
#define COLORMODE sixteen //16 bit color-mode, 1 pixel 2 bytes
#define Background White

// a buffer for itoa
char LCDBuffer[20];

//function prototypes
void itoa(int n);
void lcd_init(Color clr);
void lcd_reset(void);
void lcd_clear(Color clr);
void lcd_invert(char on_off);
void lcd_scroll(void);
void lcd_sendchar(char ch, char X, char Y, Color clr);
void lcd_printstr(char *str, char X, char Y, Color clr);
void lcd_printint(int Value, char X, char Y, Color clr);
void lcd_sendcmd(char cmd);
void lcd_senddata(char data);
void lcd_send(char byte, char comdata);
void lcd_setwin(char x, char y, char w, char h);
void lcd_sendpixel(Color color);
void lcd_delayms(unsigned int ms);
//functions implementations

/*
** ===================================================================
**     Method      :  itoa(int n)
**     Description :
**         Format an integer to a string in a buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**         n          	   - The integer to be converted.
**     Returns     : Nothing
** ===================================================================
*/
void itoa(int n){
	int i=0, neg = 0;
	if (n < 0) {
		 n = -1 * n;
		 LCDBuffer[0] = '-';
		 neg = i = 1;
	}

	do {
		LCDBuffer[i++]= n%10 + '0';
		n -= n%10;
	} while ((n /= 10) > 0);

	char c; int p, j;
	for (p = neg, j = i-1; p<j; p++, j--)
	{
	   c = LCDBuffer[p];
	   LCDBuffer[p] = LCDBuffer[j];
	   LCDBuffer[j] = c;
	}
	//null termination
	int k;
	for(k=i; k<6; k++)
	  LCDBuffer[k] = ' ';
	LCDBuffer[k] = 0;
}


/*
** ===================================================================
**     Method      :  lcd_init(Color clr)
**     Description :
**         initialize the lcd.
**     Parameters  :
**         NAME            - DESCRIPTION
**         clr               A color to paint the background in at the end of initialization
**     Returns     : Nothing
** ===================================================================
*/
void lcd_init(Color clr){
	//Hardware reset the display
	lcd_reset();
	//SWRESET (01h): Software Reset
	lcd_sendcmd(0x01);
	lcd_delayms(200);

	//Set LCD color mode - colormodes {twelve=0,sixteen=1,eighteen=2};
	lcd_sendcmd(0x3A);  // COLMOD (3Ah): Interface Pixel Format
	//lcd_senddata(0x05); // Write data for 16-bits/pixel (RGB 5-6-5-bits input)
	if(COLORMODE == sixteen)
		lcd_senddata(0x05); // Write data for 16-bits/pixel (RGB 5-6-5-bits input), 65K-colours
	else if(COLORMODE == eighteen)
		lcd_senddata(0x06); // Write data for 18-bits/pixel (RGB 6-6-6-bits input), 262K-colours
	else if(COLORMODE == twelve)
		lcd_senddata(0x03); // Write data for 12-bits/pixel (RGB 4-4-4-bits input), 4K-colours

	//switching on
	lcd_sendcmd(0x11);  //SLPOUT (11h): Sleep Out
	lcd_sendcmd(0x29); //DISPON - Display on

	//paint the background
	lcd_clear(clr);
}

/*
** ===================================================================
**     Method      :  lcd_reset(void)
**     Description :
**         Hard reset the LCD and initialize GPIO pins
**     Parameters  :
**         NAME            - DESCRIPTION
**         none            -
**     Returns     : Nothing
** ===================================================================
*/
void lcd_reset(void){
	CS_ClrVal();
	RESET_ClrVal();
	lcd_delayms(100);
	CS_SetVal();
	RESET_SetVal();
	lcd_delayms(100);
	MOSI_ClrVal();
	SCK_SetVal();
}

/*
** ===================================================================
**     Method      :  lcd_delayms(unsigned int ms)
**     Description :
**         Wait for the LCD to perform a task
**     Parameters  :
**         NAME            - DESCRIPTION
**         ms              - Number of milliseconds to wait
**     Returns     : Nothing
** ===================================================================
*/
void lcd_delayms(unsigned int ms){
	WAIT1_Waitms(ms);
}

/*
** ===================================================================
**     Method      :  lcd_clear(Color clr)
**     Description :
**         Paint the whole LCD with a background color
**     Parameters  :
**         NAME            - DESCRIPTION
**         clr             - A background color to be used to paint the background
**     Returns     : Nothing
** ===================================================================
*/
void lcd_clear(Color clr){
	lcd_setwin(0,0,128,160);
	int i;
	for(i=0; i<128*160; i++){
		lcd_sendpixel(clr);
	}
}


/*
** ===================================================================
**     Method      :  lcd_setwin(char x, char y, char w, char h)
**     Description :
**         Set bounds of a window to write inside.
**     Parameters  :
**         NAME            - DESCRIPTION
**         x               - left corner x is (2+x)
**         y			   - left corner y is (1+y)
**         w			   - right corner x is (2+x+w-1)
**         h               - right corner y is (1+y+h)
**     Returns     : Nothing
** ===================================================================
*/
void lcd_setwin(char x, char y, char w, char h){
	//choosing a range of columns
	lcd_sendcmd(0x2A);         	// CASET (2Ah): Column Address Set.
	lcd_senddata(0);           	// the second byte is always zero, because sending 2 bytes
	lcd_senddata(2+x);         	// left corner - x
	lcd_senddata(0);
	lcd_senddata(2+x+w-1);     	// right angle - x

	lcd_sendcmd(0x2B);        	// RASET (2Bh): Row Address Set
	lcd_senddata(0);
	lcd_senddata(1+y);		  	// Left corner - y
	lcd_senddata(0);
	lcd_senddata(1+y+h);		// Right angle - y
	//lcd_sendcmd(0x20);		// RAMWR command ?? where from?
	lcd_sendcmd(0x2C);  		//RAMWR (2Ch): Memory Write
}

/*
** ===================================================================
**     Method      :  lcd_invert(char on_off)
**     Description :
**         Invert the LCD background color bright/dark
**     Parameters  :
**         NAME            - DESCRIPTION
**         on_off          - Inversion status, whether the inversion to be on or off
**     Returns     : Nothing
** ===================================================================
*/
void lcd_invert(char on_off){
	if(!on_off){
		lcd_sendcmd(0x20); //INVOFF (20h): Display Inversion Off
		lcd_sendcmd(0x13); //NORON (13h): Normal Display Mode On
	}else{
		lcd_sendcmd(0x21); //INVON (21h): Display Inversion On
	}
}

//to be implemented
void lcd_scroll(void){
	//lcd_sendcmd(0x37);  //VSCSAD (37h): Vertical Scroll Start Address of RAM
	//lcd_senddata(0x00); //SSA = 0, 1, 2, ..., 161
}

/*
** ===================================================================
**     Method      :  lcd_sendpixel(Color color)
**     Description :
**         Transmit color data for a single pixel
**     Parameters  :
**         NAME            - DESCRIPTION
**         color           - The foreground color
**     Returns     : Nothing
** ===================================================================
*/
void lcd_sendpixel(Color color){
	if(COLORMODE==sixteen){
		lcd_senddata((color.r << 3) | (color.g >> 5));
		lcd_senddata((color.g << 5) | (color.b << 3));
	}else if(COLORMODE==eighteen){
		lcd_senddata(color.r<<2);
		lcd_senddata(color.g<<2);
		lcd_senddata(color.b<<2);
	}else if(COLORMODE==twelve){
		;// to be implemented
	}
}

/*
** ===================================================================
**     Method      :  lcd_sendchar(char c, char X, char Y, Color clr)
**     Description :
**         Transmit color data for a single character, pixel by pixel
**     Parameters  :
**         NAME            - DESCRIPTION
**         c               - The character
**         X               - start column
**         Y               - start row
**         clr             - The foreground color
**     Returns     : Nothing
** ===================================================================
*/
void lcd_sendchar(char c, char X, char Y, Color clr){
	unsigned char i,j;
	lcd_setwin(X, Y, 6, 8);			//define the box to paint pixels in
	for(j=0; j<8; j++)				// for al the rows
	{
		for(i=0; i<5; i++)			//for all the columns
			if( font_lookup[c-32][i] & (1 << j) )	// pixel is on,
				lcd_sendpixel(clr);					// fill it with foreground
			else                              		// pixel is off
				lcd_sendpixel(Background);			// pad with a background
		lcd_sendpixel(Background);					// pad with a space(Background)
	}
}

/*
** ===================================================================
**     Method      :  lcd_printstr(char* str, char X, char Y, Color clr)
**     Description :
**         Print a string, character by character
**     Parameters  :
**         NAME            - DESCRIPTION
**         str             - The pointer to the start of the string
**         X               - start column
**         Y               - start row
**         clr             - The foreground color
**     Returns     : Nothing
** ===================================================================
*/
void lcd_printstr(char* str, char X, char Y, Color clr){
	unsigned char i = 0;
	while( str[i] != 0){
		lcd_sendchar( (unsigned char)str[i] , X+i*6, Y, clr);
		i++;
	}
}

/*
** ===================================================================
**     Method      :  lcd_printstr(char* str, char X, char Y, Color clr)
**     Description :
**         Print a string, character by character
**     Parameters  :
**         NAME            - DESCRIPTION
**         value           - The integer to be printed
**         x               - start column
**         y               - start row
**         clr             - The foreground color
**     Returns     : Nothing
** ===================================================================
*/
void lcd_printint(int value, char x, char y, Color clr){
  itoa(value);
  lcd_printstr(LCDBuffer, x, y, clr);
}

/*
** ===================================================================
**     Method      :  lcd_sendcmd(char cmd)
**     Description :
**         Send a command to the LCD controller
**     Parameters  :
**         NAME            - DESCRIPTION
**         cmd         	   - The command to send.
**     Returns     : Nothing
** ===================================================================
*/
void lcd_sendcmd(char cmd){
	lcd_send(cmd,0);
}

/*
** ===================================================================
**     Method      :  lcd_senddata(char data)
**     Description :
**         Send a byte of data to the LCD controller
**     Parameters  :
**         NAME            - DESCRIPTION
**         cmd         	   - The byte of data to send.
**     Returns     : Nothing
** ===================================================================
*/
void lcd_senddata(char data){
	lcd_send(data,0x01);
}

/*
** ===================================================================
**     Method      :  lcd_send(char byte, char comdata)
**     Description :
**         Send a command or data byte to the LCD controller using 3-wire 9 bit bit-banging SPI
**     Parameters  :
**         NAME            - DESCRIPTION
**         comdata         - The command or data byte to send.
**     Returns     : Nothing
** ===================================================================
*/
void lcd_send(char byte, char comdata){
	char i;
	//Hold CS low to enable write
	CS_ClrVal();
	//is it command or data?
	if(comdata)
		MOSI_SetVal();
	else
		MOSI_ClrVal();
	//toggle the clock to transmit 1 bit
	SCK_ClrVal();
	SCK_SetVal();

	//send the byte
	for(i=0;i<8;i++){
		if(byte&0x80)
			MOSI_SetVal();
		else
			MOSI_ClrVal();
		//toggle the clock to transmit 1 bit
		SCK_ClrVal();
		SCK_SetVal();
		//shift to the next bit in byte
		byte<<=1;
	}
	//disable write
	CS_SetVal();
}

#endif /* 1616_LCD_H_ */
