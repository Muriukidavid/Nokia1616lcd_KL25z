/*
 * lcd.c
 *
 *  Created on: Jun 22, 2015
 *      Author: karibe
 */
#include <math.h>
#include "PE_Types.h"
#include "PE_LDD.h"
#include "1616_lcd.h"
#include "font.h"
//static Color clr;

uint8_t blanks[cols] = "                     "; //blank row for clearing buffer
uint8_t buffer[rows][cols]={}; //a buffer for display
int buffer_ptr = 0;
RGB clr;

RGB batt_icon[252]={
{255,255,255},{255,255,255},{255,255,255},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},
{255,255,255},{255,255,255},{255,255,255},{255,0,0},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,0,0},
{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},
{255,0,0},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},
{255,0,0},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},
{255,0,0},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},
{255,0,0},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},
{255,0,0},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},
{255,0,0},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},
{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},{255,0,0},{255,255,255},{255,0,0},
{255,255,255},{255,255,255},{255,255,255},{255,0,0},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,0,0},
{255,255,255},{255,255,255},{255,255,255},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0}
};
uint16_t batt16[252]={
65535,65535,65535,63488,63488,63488,63488,63488,63488,63488,63488,63488,63488,63488,63488,63488,63488,63488,63488,63488,63488,
65535,65535,65535,63488,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,63488,
63488,63488,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,
63488,65535,65535,65535,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,
63488,65535,65535,65535,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,
63488,65535,65535,65535,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,
63488,65535,65535,65535,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,
63488,65535,65535,65535,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,
63488,65535,65535,65535,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,
63488,63488,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,63488,65535,63488,
65535,65535,65535,63488,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,63488,
65535,65535,65535,63488,63488,63488,63488,63488,63488,63488,63488,63488
};

int abs(int x); //a stab for abs math function


//drawing
void lcd_drawpixel(int x, int y, Color clr){
	lcd_setwin(x,y,x,y);
	lcd_sendpixel(clr);
}

/*
 void lcd_drawline(int x0, int y0, int x1, int y1, Color clr) {
	int dx, dy;
	if(x1>x0)
		dx=x1-x0;
	else
		dx = x0-x1;
	int sx = x0<x1 ? 1 : -1;
	//int dx = abs(x1-x0),
	if(y1>y0)
		dy = y1-y0;
	else
		dy=y0-y1;
	int sy = y0<y1 ? 1 : -1;
	//int dy = abs(y1-y0),
	int err = (dx>dy ? dx : -dy)/2, e2;


	for(;;){
		lcd_drawpixel(x0,y0,clr);
		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}*/


void lcd_drawline(int x0, int y0, int x1, int y1, Color clr) {
	int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
	int err = (dx>dy ? dx : -dy)/2, e2;

	for(;;){
		lcd_drawpixel(x0,y0,clr);
		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

void lcd_drawHline(int x0, int x1, int y, Color clr){
	lcd_drawline(x0,y,x1,y,clr);
}

void lcd_drawVline(int x, int y0, int y1, Color clr){
	lcd_drawline(x,y0,x,y1, clr);
}

void lcd_drawrect(int x0, int y0, int x1, int y1, Color clr){
	lcd_drawHline(x0,x1,y0,clr);
	lcd_drawVline(x0,y0,y1,clr);
	lcd_drawVline(x1,y0,y1,clr);
	lcd_drawHline(x0,x1,y1,clr);
}

void lcd_fillrect(int x0,int y0, int x1, int y1, Color clr){
	int i=0;
	for(i=x0;i<=x1;i++){
		lcd_drawVline(i,y0,y1,clr);
	}
}



void lcd_drawcircle(int x0, int y0, int r, Color color){
	  int16_t f = 1 - r;
	  int16_t ddF_x = 1;
	  int16_t ddF_y = -2 * r;
	  int16_t x = 0;
	  int16_t y = r;

	  lcd_drawpixel(x0  , y0+r, color);
	  lcd_drawpixel(x0  , y0-r, color);
	  lcd_drawpixel(x0+r, y0  , color);
	  lcd_drawpixel(x0-r, y0  , color);

	  while (x<y) {
	    if (f >= 0) {
	      y--;
	      ddF_y += 2;
	      f += ddF_y;
	    }
	    x++;
	    ddF_x += 2;
	    f += ddF_x;

	    lcd_drawpixel(x0 + x, y0 + y, color);
	    lcd_drawpixel(x0 - x, y0 + y, color);
	    //lcd_drawHline(x0 + x,x0 - x, y0 + y, color);
	    lcd_drawpixel(x0 + x, y0 - y, color);
	    lcd_drawpixel(x0 - x, y0 - y, color);
	   // lcd_drawHline(x0 + x, x0 - x, y0 - y, color);
	    lcd_drawpixel(x0 + y, y0 + x, color);
	    lcd_drawpixel(x0 - y, y0 + x, color);
	    //lcd_drawHline(x0 + y, x0 - y, y0 + x, color);
	    lcd_drawpixel(x0 + y, y0 - x, color);
	    lcd_drawpixel(x0 - y, y0 - x, color);
	    //lcd_drawHline(x0 + y, x0 - y, y0 - x, color);
	    //lcd_drawHline(x0 + r, x0 - r, y0, color);
	  }
}
void lcd_fillcircle(int x0, int y0, int r, Color color){
	  int16_t f = 1 - r;
	  int16_t ddF_x = 1;
	  int16_t ddF_y = -2 * r;
	  int16_t x = 0;
	  int16_t y = r;

	  lcd_drawpixel(x0  , y0+r, color);
	  lcd_drawpixel(x0  , y0-r, color);
	  lcd_drawpixel(x0+r, y0  , color);
	  lcd_drawpixel(x0-r, y0  , color);

	  while (x<y) {
	    if (f >= 0) {
	      y--;
	      ddF_y += 2;
	      f += ddF_y;
	    }
	    x++;
	    ddF_x += 2;
	    f += ddF_x;

	    lcd_drawpixel(x0 + x, y0 + y, color);
	    lcd_drawpixel(x0 - x, y0 + y, color);
	    lcd_drawHline(x0 + x,x0 - x, y0 + y, color);
	    lcd_drawpixel(x0 + x, y0 - y, color);
	    lcd_drawpixel(x0 - x, y0 - y, color);
	    lcd_drawHline(x0 + x, x0 - x, y0 - y, color);
	    lcd_drawpixel(x0 + y, y0 + x, color);
	    lcd_drawpixel(x0 - y, y0 + x, color);
	    lcd_drawHline(x0 + y, x0 - y, y0 + x, color);
	    lcd_drawpixel(x0 + y, y0 - x, color);
	    lcd_drawpixel(x0 - y, y0 - x, color);
	    lcd_drawHline(x0 + y, x0 - y, y0 - x, color);
	    lcd_drawHline(x0 + r, x0 - r, y0, color);
	  }
}

// used to do circles and roundrects!
void lcd_fillcirclehelper(int x0, int y0, int r, uint8_t cornername,  int16_t delta, Color color){
	int16_t f     = 1 - r;
	  int16_t ddF_x = 1;
	  int16_t ddF_y = -2 * r;
	  int16_t x     = 0;
	  int16_t y     = r;

	  while (x<y) {
	    if (f >= 0) {
	      y--;
	      ddF_y += 2;
	      f     += ddF_y;
	    }
	    x++;
	    ddF_x += 2;
	    f     += ddF_x;

	    if (cornername & 0x1) {
	      lcd_drawVline(x0+x, y0-y, 2*y+1+delta, color);
	      lcd_drawVline(x0+y, y0-x, 2*x+1+delta, color);
	    }
	    if (cornername & 0x2) {
	      lcd_drawVline(x0-x, y0-y, 2*y+1+delta, color);
	      lcd_drawVline(x0-y, y0-x, 2*x+1+delta, color);
	    }
	  }
}

void lcd_fillcircleold(int x0, int y0, int r, Color color){
	lcd_drawVline(x0, y0-r, 2*r+1, color);
	lcd_fillcirclehelper(x0, y0, r, 3, 2, color);
}

void lcd_drawcirclehelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, Color color){
	int16_t f     = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x     = 0;
	int16_t y     = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		if (cornername & 0x4) {
			lcd_drawpixel(x0 + x, y0 + y, color);
			lcd_drawpixel(x0 + y, y0 + x, color);
		}
		if (cornername & 0x2) {
			lcd_drawpixel(x0 + x, y0 - y, color);
			lcd_drawpixel(x0 + y, y0 - x, color);
		}
		if (cornername & 0x8) {
			lcd_drawpixel(x0 - y, y0 + x, color);
			lcd_drawpixel(x0 - x, y0 + y, color);
		}
		if (cornername & 0x1) {
			lcd_drawpixel(x0 - y, y0 - x, color);
			lcd_drawpixel(x0 - x, y0 - y, color);
		}
	}
}

void lcd_drawtriangle(uint16_t x0, uint16_t y0, uint16_t x1,uint16_t y1, uint16_t x2, uint16_t y2, Color color){

}

char getWidth(void){
	char w;
	switch(orientation){
		case _0:
		case _180:
			w = width;
		break;
		case 90:
		case _270:
			w=height;
		break;
		default:w=width;
		break;
	}
	return w;
}

char getHeight(void){
	char h;
	switch(orientation){
		case _0:
		case _180:
			h=height;
		break;
		case _90:
		case _270:
			h=width;
		break;
		default:
			h=height;
		break;
	}
	return h;
}

void lcd_setOrientation(uint8_t or){
	uint8_t setting = 0;
	switch(or){
		case _90: //90 degrees
			setting = MADCTR_MV | MADCTR_MX;
		break;
		case _0: // 0 degrees: normal
			setting = 0;
		break;
		case _180: //180 degrees
			setting = MADCTR_MY | MADCTR_MX;
		break;
		case _270: //270 degrees rotation
			setting = MADCTR_MV | MADCTR_MY;
		break;
	}
	lcd_sendcmd(MADCTR);
	lcd_senddata(setting);
	orientation = or;
}

void select(int y){
	char w = getWidth();
	lcd_setwin(0,y,w,8);
	int i;
	for(i=0; i<w*8; i++){
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
	lcd_setwin(0,0,getWidth(),getHeight());
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

	lcd_sendcmd(0x2C);  		//RAMWR (2Ch): Memory Write unlock
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

//TODO: implement LCD scrolling
void lcd_scroll(void){
	//lcd_sendcmd(0x37);  //VSCSAD (37h): Vertical Scroll Start Address of RAM
	//lcd_senddata(0x00); //SSA = 0, 1, 2, ..., 161
}

void lcd_sendpixel(Color color){
	if(COLORMODE==sixteen){
		lcd_senddata(color >> 8);
		lcd_senddata(color & 0xFF);
	}else if(COLORMODE==eighteen){
		lcd_senddata(color >> 24  & 0xFF);
		lcd_senddata((color >>16) & 0xFF);
		lcd_senddata(color & 0xFF);
	}else if(COLORMODE==twelve){
		;//TODO: implement 12 bit color mode sending
	}
}


void lcd_sendchar(char c, char X, char Y, Color foreground, Color background){
	unsigned char i,j;
	lcd_setwin(X, Y, 6, 8);			//define the box to paint pixels in
	for(j=0; j<8; j++)				// for all the rows
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

void lcd_sendcmd(uint16_t cmd){
	lcd_send(cmd & 0xFF); //clear
}

void lcd_senddata(uint16_t data){
	lcd_send(data | 0x100);
}

// 3-wire 9 bit bit-banging SPI write first bit command/data, next 8bits the details
void lcd_send(uint16_t byte){
	uint16_t i;
	//enable write
	CS_ClrVal();
	//send the 9 bits
	for(i=0;i<9;i++){
		if(byte & (256 >> i))
			MOSI_SetVal();
		else
			MOSI_ClrVal();
		//toggle the clock to transmit 1 bit
		SCK_ClrVal();
		SCK_SetVal();
	}
	//disable write
	CS_SetVal();
}

void lcd_delay(char ms){
	WAIT1_Waitms(ms);
}

void lcd_showImage(uint8_t x, uint8_t y, uint8_t w, uint8_t h, RGB imgData[]){
	uint16_t i=0;

	lcd_setwin(x,y,w,h);//select the window to display image
	while(i<(w*h)){
		clr = imgData[i];
		uint16_t cl = packColor(clr);
		lcd_sendpixel(cl);
		i+=1;
	}
}

void lcd_showImage2(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t imgData[]){
	uint16_t i=0;
	uint16_t cl;
	lcd_setwin(x,y,w,h);//select the window to display image
	while(i<(w*h)){
		cl = imgData[i];
		lcd_sendpixel(cl);
		i+=1;
	}
}



