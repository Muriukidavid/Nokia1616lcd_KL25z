/*
 * 1616_lcd.h
 *
 *  Created on: Jul 24, 2014
 *      Author: karibe
 */

#ifndef NOKIA_1616_LCD_H_
#define NOKIA_1616_LCD_H_
#include "PE_Types.h"
//get defined font lookup and colors
#include "font.h"
#include "colors.h"

//colormode and background color
enum colormodes {twelve=0,sixteen,eighteen};
#define COLORMODE sixteen //16 bit color-mode, 1 pixel 2 bytes
#define Background White // a white background
#define Foreground Black // a black foreground

#define width 128 //width of LCD in pixels
#define height 160 //height of LCD in pixels
#define rows 15 //height of display in in characters
#define cols 22 //width of display in characters

uint8_t blanks[cols] = "                     "; //blank row for clearing buffer
uint8_t buffer[rows][cols]={}; //a buffer for display
uint8_t LCDBuffer[20]; // a buffer for itoa
int buffer_ptr=0; //a buffer fro display pointer
int selected=0; //the currently selected line

//function prototypes
void itoa(int n);
void lcd_init(Color clr);
void lcd_reset(void);
void lcd_clear(Color clr);
void lcd_invert(char on_off);
void lcd_scroll(void);
void lcd_sendchar(char c, char X, char Y, Color foreground, Color background);
void lcd_printstr(uint8_t *str, char X, char Y, Color foreground, Color background);
void lcd_printint(int value, char x, char y, Color foreground, Color background);
void lcd_sendcmd(char cmd);
void lcd_senddata(char data);
void lcd_send(char byte, char comdata);
void lcd_setwin(char x, char y, char w, char h);
void lcd_sendpixel(Color color);
void lcd_delayms(unsigned int ms);
void add2display(unsigned char *str, int offset);
void display(void);
void resetBuffer(void);
void select();

//functions implementations
void select(int y){
	lcd_setwin(0,y,width,8);
	int i;
	for(i=0; i<width*8; i++){
		lcd_sendpixel(Foreground);
	}
}

void add2display(unsigned char *str, int offset){
	UTIL1_strcpy(buffer[buffer_ptr+offset],cols,str);
	buffer_ptr++;
	if(buffer_ptr>15){
			buffer_ptr=0;
		}
}

void display(void){
	int x=0;
	int y=1;
	while(x<rows){
		if((selected*8+1)==y){
			lcd_printstr(buffer[x], 1, y, Background,Foreground);
		}else{
			lcd_printstr(buffer[x], 1, y, Foreground,Background);
		}
		y+=8;
		x++;
	}
}

void resetBuffer(void){
	int x= 0;
	while(x<rows){
		buffer[x][0]='\0';
		x++;
	}
	buffer_ptr = 0;
}

/*
** ===================================================================
**     Method      :  itoa(int n)
**     Description :
**         Format the output into a buffer (16bit data type).
**     Parameters  :
**         NAME            - DESCRIPTION
**         n          	   - The number to be output to screen.
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
**     Method      :  itoa(int n)
**     Description :
**         initialize the lcd.
**     Parameters  :
**         NAME            - DESCRIPTION
**         none            -
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

//reset the display and init gpio
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

void lcd_delayms(unsigned int ms){
	WAIT1_Waitms(ms);
}

void lcd_clear(Color clr){
	lcd_setwin(0,0,width,height);
	int i;
	for(i=0; i<width*height; i++){
		lcd_sendpixel(clr);
	}
}

/*
 * To set the window, you need to pass two points that
 * define the rectangle: (x1, y1), (x1, y2)
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

	lcd_sendcmd(0x2C);  		//RAMWR (2Ch): Memory Write
}

//invert the background/forground
void lcd_invert(char on_off){
	if(!on_off){
		lcd_sendcmd(0x20); //INVOFF (20h): Display Inversion Off
		lcd_sendcmd(0x13); //NORON (13h): Normal Display Mode On
	}else{
		lcd_sendcmd(0x21); //INVON (21h): Display Inversion On
	}
}

//TODO: implement LCD scrolling. No immediate use
void lcd_scroll(void){
	//lcd_sendcmd(0x37);  //VSCSAD (37h): Vertical Scroll Start Address of RAM
	//lcd_senddata(0x00); //SSA = 0, 1, 2, ..., 161
}

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


void lcd_sendchar(char c, char X, char Y, Color foreground, Color background){
	unsigned char i,j;
	lcd_setwin(X, Y, 6, 8);			//define the box to paint pixels in
	for(j=0; j<8; j++)				// for al the rows
	{
		for(i=0; i<5; i++)			//for all the columns
			if( font_lookup[c-32][i] & (1 << j) )	// pixel is on,
				lcd_sendpixel(foreground);					// fill it with foreground
			else                              		// pixel is off
				lcd_sendpixel(background);			// pad with a background
		lcd_sendpixel(background);					// pad with a space(Background)
	}
}

void lcd_printstr(uint8_t *str, char X, char Y, Color foreground, Color background){
	unsigned char i = 0;
	while( str[i] != 0){
		lcd_sendchar( (unsigned char)str[i] , X+i*6, Y, foreground, background);
		i++;
	}
}

void lcd_printint(int value, char x, char y, Color foreground, Color background){
  itoa(value);
  lcd_printstr(LCDBuffer, x, y, foreground ,background);
}

void lcd_sendcmd(char cmd){
	lcd_send(cmd,0);
}

void lcd_senddata(char data){
	lcd_send(data,0x01);
}

// 3-wire 9 bit bit-banging SPI write
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

void lcd_delay(char ms){
	WAIT1_Waitms(ms);
}

#endif /* 1616_LCD_H_ */
