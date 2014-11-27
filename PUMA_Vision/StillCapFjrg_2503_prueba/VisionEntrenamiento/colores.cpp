#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "images.h"
#include "colores.h"

HSI **alloc_Memh(int x, int y)
{
	HSI **m;

	if((m=(HSI **)malloc((unsigned) y *sizeof(HSI *)))==NULL)
	{
		printf("\nError al asignar memoria en 2D **-*");
		exit(1);
	}
	
	for(int j=0; j<y; j++)
	{
		if((m[j]=(HSI *)malloc((unsigned) x * sizeof(HSI)))==NULL)
			{
				printf("\nError al asignar memoria en 3D *-");
				exit(1);
			}
	}
	return m;
}
void libera_matrizh(HSI **m,int row)
{
    int i;

    for (i=0; i<row; i++)
        free((HSI *) m[i]);
    free((HSI *) m);
}

HSI Rgb2Hsi( Matr rgb )
{
	HSI hsi;
//	int theta = (byte)( acos( (2*rgb.red-rgb.green-rgb.blue) / (2*sqrt((float)((rgb.red-rgb.green)*(rgb.red-rgb.green)+(rgb.red-rgb.blue)*(rgb.green-rgb.blue))) )) * 57.29);
	int theta = (int)( acos( (2*rgb.red-rgb.green-rgb.blue) / (2*sqrt((float)((rgb.red-rgb.green)*(rgb.red-rgb.green)+(rgb.red-rgb.blue)*(rgb.green-rgb.blue))) )) * 57.29);		
	hsi.h = rgb.blue <= rgb.green ? theta : (360-theta);

	int m = rgb.red <= rgb.green ? rgb.red : rgb.green;
	m = m <= rgb.blue ? m : rgb.blue;
	m = m <= 0 ? 1 : m;

	float temp = (765 / (float)(rgb.red+rgb.green+rgb.blue) );
	temp = temp * (float)m;
	hsi.s = 255- (int)temp;
	
	hsi.i = (int)( (rgb.red+rgb.green+rgb.blue) / 3 );
	
	return hsi;
}
