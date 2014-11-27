#define WIDTH_DEFAULT   320
#define HEIGHT_DEFAULT  240
#define FRAME 30
#define TIME_WINDOW 3
#define MAX_VALUE 255
#define MAX_VALUE_ANGLE 360
#define OUT_VALUE 100
#define THRESHOLD 10*2
#define THRESHOLD_GRADIENT 650
#define THRESHOLD_GRADIENT_AGAIN 800
#define THRESHOLD_WIND 15.0
#define THRESHOLD_SUM 170
#define THRESHOLD_WIND_SUM 35.0
#define NUM_LINES_MASK 3*3  //para el cuadrito parecen ser pixeles
#define NUM_ROWS_MASK 3*3  //para el cuadrito
#define NUM_LINES_WINDOW 32*1 //para el marquito 32*1
#define NUM_ROWS_WINDOW 24*1  //para el marquito 24*1
#define NUM_LINES_SCREEN 125
#define NUM_ROWS_SCREEN 125
/*#define NUM_LINES_SCREEN 90
#define NUM_ROWS_SCREEN 90*/
#define MASK_VALUE 1
#define NUM_FRAMES_SUM 15
#define SIZE_ANGLES 10
#define CHAR_SIZE 200
#define NUM_SECCIONS 1
#define ORDER MAX_VALUE_ANGLE/SIZE_ANGLES 
#define VQ_SIZE_SECC 8
#define EPS .0005  
#define NUM_MAX_ITERATIONS 30 
#define SHOW_DISTORTION 0       /* Show the distortion values */ 
#define NUM_MAX_OBS     600     /* Number maximum of observations */  
#define NUM_FRAMES_REC  18     /* Number of frames to do the recognition */  

#include "distorsion.h"
typedef struct Matr_ {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
//	unsigned char alfa;
} Matr;
process_imagej(int PixelsPerLine,int Lines,unsigned char *address);
Matr **alloc_Mem2d(int x, int y);
void libera_matriz(Matr **m,int row);
Matr ** vector_a_matriz(int Lines, int PixelsPerLine,unsigned char *vector, Matr **matrix);
unsigned char * matriz_a_vector(int Lines, int PixelsPerLine,unsigned char *vector, Matr **matrix);
Matr ** unos_to_matrix(int Lines, int PixelsPerLine,unsigned char *vector, Matr **matrix);
unsigned char *process_imagevj(int x, int y,unsigned char * pBuffer);
Matrixyi ** process_imagedvj(int x, int y,unsigned char * pBuffer);
unsigned char *imag_distj(int x, int y,unsigned char * pBuffer, Matrixyi ** coord);
Matrixyi ** process_imagedvj1(int x, int y, int centrox, int centroy, unsigned char * pBuffer, float a, float b, float c);
void process_imageavj(int x, int y,unsigned char * pBuffer);