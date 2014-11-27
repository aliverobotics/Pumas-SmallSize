
#ifndef POINT_H
#define POINT_H


typedef struct
{
	int x;
	int y;
} Point;

typedef struct 
{
	Point corner0;
	Point corner1;
}FIELD;

typedef struct 
{
	float x;
	float y;
} FPOINT;

struct FFIELD
{
	FPOINT corner0;
	FPOINT corner1;
};

#endif // POINT_H