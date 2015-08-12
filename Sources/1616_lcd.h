/*
 * 1616_lcd.h
 *
 *  Created on: Jul 24, 2014
 *      Author: karibe
 */

#ifndef NOKIA_1616_LCD_H_
#define NOKIA_1616_LCD_H_

#include "colors.h"
//get defined font lookup and colors


//Display parameters
//display inversion Control: whether to invert line(=0) or frame(=1)
#define NVCTR	0XB4
#define NVCTR_NLA		(1 << 2)//display inversion in normal full color mode: 0=line 1=frame
#define NVCTR_NLB		(1 << 1)//in idle mode
#define NVCTR_NLC		(1 << 0)//in full colours partial mode

//MADCTR: Memory AdDress ConTrolleR
#define MADCTR 		0x36		// MADCTR command code
#define MADCTR_RGB 	(1 << 3) 	// RGB-BGR order bit: ‘0’ =RGB color filter panel, ‘1’ =BGR color filter panel)
#define MADCTR_ML 	(1 << 4) 	// Vertical refresh order:LCD vertical refresh direction control,
								// ‘0’ = LCD vertical refresh Top to Bottom,
								// ‘1’ = LCD vertical refresh Bottom to Top
								//					----
#define MADCTR_MV 	(1 << 5)	// Row/Column Exchange	|
#define MADCTR_MX 	(1 << 6)	// Column Address Order	|--> Controls MCU to memory write/read direction.
#define MADCTR_MY 	(1 << 7)	// Row Address Order	|		Used to change screen orientation
								//					----

#define width 128 //width of LCD in pixels
#define height 160 //height of LCD in pixels
#define rows 15 //height of display in in characters
#define cols 22 //width of display in characters

enum orientations {_0 = 0, _90, _180, _270};
extern uint8_t blanks[cols] ; //blank row for clearing buffer
extern uint8_t buffer[rows][cols]; //a buffer for display
extern uint8_t LCDBuffer[20]; // a buffer for itoa
extern int buffer_ptr; //a buffer for display pointer
extern int selected; //the currently selected line
extern char orientation;
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
void lcd_sendcmd(uint16_t cmd);
void lcd_senddata(uint16_t data);
void lcd_send(uint16_t byte);
void lcd_setwin(char x, char y, char w, char h);
void lcd_sendpixel(Color color);
void lcd_delayms(unsigned int ms);
void add2display(unsigned char *str, int offset);
void display(void);
void resetBuffer(void);
void select();
void lcd_setOrientation(uint8_t or);
char getWidth(void);
char getHeight(void);
//drawing
void lcd_drawpixel(int x, int y, Color clr);
void lcd_drawline(int x0, int y0, int x1, int y1, Color clr);
void lcd_drawHline(int x0, int x1, int y, Color clr);
void lcd_drawVline(int x, int y0, int y1, Color clr);
void lcd_drawrect(int x, int y, int w, int h, Color clr);
void lcd_fillrect(int x0, int y0, int w, int h, Color clr);
void lcd_drawcircle(int x0, int y0, int r, Color color);
void lcd_fillcircle(int x0, int y0, int r, Color clr);
void lcd_fillcircleold(int x0, int y0, int r, Color clr);
void lcd_fillcirclehelper(int x0, int y0, int r, uint8_t cornername,  int16_t delta, Color color);
void lcd_showImage(uint8_t x, uint8_t y, uint8_t w, uint8_t h, RGB imgData[]);
#endif /* 1616_LCD_H_ */
