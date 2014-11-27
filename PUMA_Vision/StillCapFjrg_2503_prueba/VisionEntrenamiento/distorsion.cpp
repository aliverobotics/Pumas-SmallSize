//Para la distorsión de lente focal (Barril o Cojin)(Barrel o Pincushion)
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "distorsion.h"

Matri **alloc_Mem2df(int x, int y)
{
	Matri **m;
	if((m=(Matri **)malloc((unsigned) y *sizeof(Matri *)))==NULL)
	{
		printf("\nError al asignar memoria en 2Di **-*");
		exit(1);
	}
	for(int j=0; j<y; j++)
	{
		if((m[j]=(Matri *)malloc((unsigned) x * sizeof(Matri)))==NULL)
			{
				printf("\nError al asignar memoria en 3Di *-");
				exit(1);
			}
	}
	return m;
}
Matrixyf **alloc_Mem2xyf(int x, int y)
{
	Matrixyf **m;
	if((m=(Matrixyf **)malloc((unsigned) y *sizeof(Matrixyf *)))==NULL)
	{
		printf("\nError al asignar memoria en 2Di **-*");
		exit(1);
	}
	for(int j=0; j<y; j++)
	{
		if((m[j]=(Matrixyf *)malloc((unsigned) x * sizeof(Matrixyf)))==NULL)
			{
				printf("\nError al asignar memoria en 3Di *-");
				exit(1);
			}
	}
	return m;
}
Matrixyi **alloc_Mem2xyi(int x, int y)
{
	Matrixyi **m;
	if((m=(Matrixyi **)malloc((unsigned) y *sizeof(Matrixyi *)))==NULL)
	{
		printf("\nError al asignar memoria en 2Di **-*");
		exit(1);
	}
	for(int j=0; j<y; j++)
	{
		if((m[j]=(Matrixyi *)malloc((unsigned) x * sizeof(Matrixyi)))==NULL)
			{
				printf("\nError al asignar memoria en 3Di *-");
				exit(1);
			}
	}
	return m;
}

void libera_matrizd(Matri **m,int row)
{
    int i;
    for (i=0; i<row; i++)
        free((Matri *) m[i]);
    free((Matri *) m);
}
void libera_matrizxyf(Matrixyf **m,int row)
{
    int i;
    for (i=0; i<row; i++)
        free((Matrixyf *) m[i]);
    free((Matrixyf *) m);
}
void libera_matrizxyi(Matrixyi **m,int row)
{
    int i;
    for (i=0; i<row; i++)
        free((Matrixyi *) m[i]);
    free((Matrixyi *) m);
}

int puntomedioentero(int num)
{
	float pm=0;
	pm=(float)(num/2);//saco el punto medio del punto
	if(num%2 != 0)pm=(float)(pm+0.5);
	num=(int)pm;
	return(num);
}
//redondear número entero
int redondeofi(float cord)
{
  float redond=0, dif=0, uno;
  int resultado=0;
  uno=(float)(1.0);
  redond=fmod(cord,uno);//modulo de 1 para sacar las decimas
  if(redond>0.5)
    {
      dif=1-redond;
      resultado=(int)(dif+cord);
    }
  else resultado=(int)(cord-redond);
  return (resultado);
}
//debe recibir la matriz, el tamaño de ella (checar si no hay error)
Matri ** distancia_centro(Matri ** dista,int x, int y, int centrox, int centroy)
{
	float dist=0;
	for(int i=0; i<y; i++)
	{
		for(int j=0; j<x; j++)
		{
            dist=sqrt((float)(((j-centrox)*(j-centrox))+((i-centroy)*(i-centroy))) );//distancia desde el centro
			dista[i][j].d =dist;
		}
	}
	return (dista);
}

float distancia_mayor(Matri ** dista,int x, int y)
{
	float distmay=0;
	for(int i=0; i<y; i++)
		for(int j=0; j<x; j++)
            if(distmay<dista[i][j].d)distmay=dista[i][j].d;
    return (distmay);
}
Matri ** distancia_normalizada(Matri ** dista, Matri ** distnorm,int x, int y, float distmay)
{
	for(int i=0; i<y; i++)
		for(int j=0; j<x; j++)
            distnorm[i][j].d=(1/distmay)*dista[i][j].d;
	return (distnorm);
}
Matrixyi ** quitar_distorsion(Matri ** dista, Matri ** distnorm, Matrixyi ** coordi, int x, int y, int centrox, int centroy, float a, float b, float c, float distmay)
{
	float angulo=0, sindist=0, nuevadist=0, coordx=0, coordy=0;
	float d=0;
	d=1-(a+b+c);//para no afectar la escala
    for(int i=0; i<y; i++)
	{
        for(int j=0; j<x; j++)
		{
			if(dista[i][j].d==0)angulo=0;
			else angulo=acos((j-centrox)/dista[i][j].d);
			sindist=((d*distnorm[i][j].d)+
					  (c*(distnorm[i][j].d*distnorm[i][j].d))+
				      (b*(distnorm[i][j].d*distnorm[i][j].d*distnorm[i][j].d))+
					  (a*(distnorm[i][j].d*distnorm[i][j].d*distnorm[i][j].d*distnorm[i][j].d)));
			nuevadist=sindist*distmay;
			coordx=nuevadist*cos(angulo)+centrox;
			if(i<=centroy)
			{
				coordy=nuevadist*sin(angulo)+centroy;
                coordy=y-coordy;
			}
			else  coordy=nuevadist*sin(angulo)+centroy;  
			coordi[i][j].x=redondeofi(coordx);
			coordi[i][j].y=redondeofi(coordy);
		}
		angulo=0;
		sindist=0;
		nuevadist=0;
		coordx=0;
		coordy=0;
	}
	return (coordi);
}
Matrixyi ** medio_coord(Matrixyi ** coordi, int x, int y)
{
	float coordx=0, coordy=0;
    for(int i=1; i<y-1; i++)
	{
        for(int j=1; j<x-1; j++)
		{
            if(coordi[i][j+1].x == coordi[i][j].x && coordi[i][j-1].x == coordi[i][j].x)
			{}
			else coordx=(float)((coordi[i][j-1].x + coordi[i][j+1].x)/2);
			if(coordi[i+1][j].y == coordi[i][j].y && coordi[i-1][j].y == coordi[i][j].y)
			{}
			else coordy=(float)((coordi[i-1][j].y + coordi[i+1][j].y)/2);
			coordi[i][j].x=redondeofi(coordx);
			coordi[i][j].y=redondeofi(coordy);
		}
		coordx=0;
		coordy=0;
	}
	return (coordi);
}
Matrixyi ** distorsionj(int x, int y)//debe recibir el centro de la imagen donde la distorsión es menor
{
	int centrox=0, centroy=0;
	float distmay=0;
	float a,b,c;
    Matri ** dista;
	Matri ** distnorm;
	Matrixyi ** coordi;

    dista=alloc_Mem2df(x, y);
    distnorm=alloc_Mem2df(x, y);
    coordi=alloc_Mem2xyi(x, y);
	a=-0.1;//circ arriba y abajo
	b=0.25;//circ en los lados
	c=0;//0.05
	centrox=puntomedioentero(x);
	centroy=puntomedioentero(y);
    dista=distancia_centro(dista, x, y, centrox, centroy);
	distmay=distancia_mayor(dista, x, y);
	distnorm=distancia_normalizada(dista, distnorm, x, y, distmay);
	coordi=quitar_distorsion(dista, distnorm, coordi, x, y, centrox, centroy, a, b, c, distmay);
	coordi=medio_coord(coordi, x, y);
    
	libera_matrizd(dista,y);
	libera_matrizd(distnorm,y);
    return (coordi);
}

Matrixyi ** distorsionj1(int x, int y, int centrox, int centroy, float a, float b, float c)//debe recibir el centro de la imagen donde la distorsión es menor
{
//	int centrox=0, centroy=0;
	float distmay=0;
    Matri ** dista;
	Matri ** distnorm;
	Matrixyi ** coordi;

    dista=alloc_Mem2df(x, y);
    distnorm=alloc_Mem2df(x, y);
    coordi=alloc_Mem2xyi(x, y);
//	a=-0.1;//circ arriba y abajo
//	b=0.25;//circ en los lados
//	c=0;//0.05
    dista=distancia_centro(dista, x, y, centrox, centroy);
	distmay=distancia_mayor(dista, x, y);
	distnorm=distancia_normalizada(dista, distnorm, x, y, distmay);
	coordi=quitar_distorsion(dista, distnorm, coordi, x, y, centrox, centroy, a, b, c, distmay);
	coordi=medio_coord(coordi, x, y);
    
	libera_matrizd(dista,y);
	libera_matrizd(distnorm,y);
    return (coordi);
}


