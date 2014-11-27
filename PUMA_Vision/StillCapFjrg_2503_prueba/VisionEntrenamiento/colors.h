#include "math.h"

#ifndef __colors__
#define __colors__
	
typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} RGB;

typedef struct 
{
	int h;
	int s;
	int i;
}HSI;



typedef enum 
{
	orange,
	blue,
	yellow,
	green,
	pink,
	cyan
}HSICOLOR;


typedef struct 
{
	int min;
	int max;
	int win;
}HSITHRESH;

typedef struct 
{
	HSITHRESH h;
	HSITHRESH s;
	HSITHRESH i;
}HSITH;


typedef struct 
{
	HSITHRESH r;
	HSITHRESH g;
	HSITHRESH b;
}RGBTH;	

typedef struct
{
	int bright;
	int contrast;
	int saturation;
} PREPROCESSPARAM;

	
//extern HSITH hsiCalib;

extern HSITH hsi[6];				// Here is the complete info about the HSI color ranges
extern HSI myhsi;
extern HSITH hnew, hold;

extern HSI hm;

extern RGB *rgb;
extern RGB PickedColor;

HSI RGB2HSI(RGB);

#endif /* __colors__ */