#ifndef __color__
#define __color__
	
typedef struct HSI_
{
	int h;
	int s;
	int i;
}HSI;
#endif //__color__
HSI **alloc_Memh(int x, int y);
void libera_matrizh(HSI **m,int row);
HSI Rgb2Hsi( Matr rgb );