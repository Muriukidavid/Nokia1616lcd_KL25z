/*
 * colors.h
 *
 *  Created on: Jul 28, 2014
 *      Author: karibe
 */

#ifndef COLORS_H_
#define COLORS_H_

typedef struct{
  byte r;
  byte g;
  byte b;
} Color;

//static Color clr;
Color White = {255,255,255};
Color Black = {0,0,0};
Color Red = {255,0,0};
Color Green = {0,255,0};
Color Blue = {0,0,255};
Color Brown = {0x1b,0x3c,0};
Color Gray = {192,192,192};
Color Yellow = {255,255,0};
Color Crimson = {80,0,0};
Color Purple = {153,0,255};

#endif /* COLORS_H_ */
