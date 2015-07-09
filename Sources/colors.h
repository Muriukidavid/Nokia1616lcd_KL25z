/*
 * colors.h
 *
 *  Created on: Jul 28, 2014
 *      Author: karibe
 */

#ifndef COLORS_H_
#define COLORS_H_

//colormode and background color
enum colormodes {twelve=0,sixteen,eighteen};
#define COLORMODE sixteen //16 bit color-mode, 1 pixel 2 bytes

typedef struct{
  byte r;
  byte g;
  byte b;
} RGB;
typedef uint16_t Color;
//declare some RGB colors
static RGB
White = {255,255,255},
Black = {0,0,0},
Red = {255,0,0},
Green = {0,255,0},
Blue = {0,0,255},
Brown = {0x1b,0x3c,0},
Gray = {192,192,192},
Yellow = {255,255,0},
Crimson = {80,0,0},
Purple = {153,0,255};
//convert rgb to 16-bit color
#define packColor(rgb)		(((Color)rgb.r << 11) & 0xf800) | (((Color)rgb.g << 5) & 0x07c0) | (((Color)rgb.b) & 0x003f)
#define unpackColor(color)	(RGB)color // not really used

#define white packColor(White)
#define black packColor(Black)
#define red packColor(Red)
#define green packColor(Green)
#define blue packColor(Blue)
#define brown packColor(Brown)
#define gray packColor(Gray)
#define yellow packColor(Yellow)
#define crimson packColor(Crimson)
#define purple packColor(Purple)

#define Background white // a white background
#define Foreground blue // a black foreground


#endif /* COLORS_H_ */
