#include <stdio.h>
#include <stdlib.h>
//#include <malloc.h>
//#include <string.h>
//#include <process.h>
#include "archivos.h"

Archivo crear_archivo(int color)
{
	Archivo nuevo;
	switch (color)
	{
	case 1:		if((nuevo.archvec=fopen("naranja.col","w"))==NULL)
					{
						printf("\nNo puedo abrir el archivo naranja");
						nuevo.activo=0;
					}
				else nuevo.activo=1;
				break;
	case 2:		if((nuevo.archvec=fopen("azul.col","w"))==NULL)
					{
						printf("\nNo puedo abrir el archivo azul");
						nuevo.activo=0;
					}
				else nuevo.activo=1;
				break;
	case 3:		if((nuevo.archvec=fopen("verde.col","w"))==NULL)
					{
						printf("\nNo puedo abrir el archivo verde");
						nuevo.activo=0;
					}
				else nuevo.activo=1;
				break;
	case 4:		if((nuevo.archvec=fopen("amarillo.col","w"))==NULL)
					{
						printf("\nNo puedo abrir el archivo amarillo");
						nuevo.activo=0;
					}
				else nuevo.activo=1;
				break;
	case 5:		if((nuevo.archvec=fopen("rosa.col","w"))==NULL)
					{
						printf("\nNo puedo abrir el archivo rosa");
						nuevo.activo=0;
					}
				else nuevo.activo=1;
				break;
	case 6:		if((nuevo.archvec=fopen("cyan.col","w"))==NULL)
					{
						printf("\nNo puedo abrir el archivo cyan");
						nuevo.activo=0;
					}
				else nuevo.activo=1;
				break;
	}
	return(nuevo);
}
Archivo crear_archivoh(int color)
{
	Archivo nuevo;
	switch (color)
	{
	case 1:		if((nuevo.archvec=fopen("naranjah.col","w"))==NULL)
					{
						printf("\nNo puedo abrir el archivo naranja");
						nuevo.activo=0;
					}
				else nuevo.activo=1;
				break;
	case 2:		if((nuevo.archvec=fopen("azulh.col","w"))==NULL)
					{
						printf("\nNo puedo abrir el archivo azul");
						nuevo.activo=0;
					}
				else nuevo.activo=1;
				break;
	case 3:		if((nuevo.archvec=fopen("verdeh.col","w"))==NULL)
					{
						printf("\nNo puedo abrir el archivo verde");
						nuevo.activo=0;
					}
				else nuevo.activo=1;
				break;
	case 4:		if((nuevo.archvec=fopen("amarilloh.col","w"))==NULL)
					{
						printf("\nNo puedo abrir el archivo amarillo");
						nuevo.activo=0;
					}
				else nuevo.activo=1;
				break;
	case 5:		if((nuevo.archvec=fopen("rosah.col","w"))==NULL)
					{
						printf("\nNo puedo abrir el archivo rosa");
						nuevo.activo=0;
					}
				else nuevo.activo=1;
				break;
	case 6:		if((nuevo.archvec=fopen("cyanh.col","w"))==NULL)
					{
						printf("\nNo puedo abrir el archivo cyan");
						nuevo.activo=0;
					}
				else nuevo.activo=1;
				break;
	}
	return(nuevo);
}

Archivo inicio_archivo(Archivo nuevo)
{
	int resp;
    resp=fseek(nuevo.archvec, 0L, SEEK_SET );
    if(resp==0) nuevo.activo=1;
	else nuevo.activo=0;
	return (nuevo);
}

Archivo escribemat_archivo(Archivo nuevo, Matr ** in, int x, int y)
{
	for(int i=0; i<y; i++)
	{
        for(int j=0; j<x; j++)
		{
			for(int k=0; k<3; k++)//tres por ser 3 componentes de color BGR
			{
				switch(k)
				{
				case 0: fprintf(nuevo.archvec, "%i,", in[i][j].blue);
					break;
				case 1: fprintf(nuevo.archvec, "%i,", in[i][j].green);
					break;
				case 2: fprintf(nuevo.archvec, "%i", in[i][j].red);
					break;
				}
			}
            fwrite(" ",1,1,nuevo.archvec);
		}
		fwrite("\n",1,1,nuevo.archvec);
	}
	return (nuevo);
}
Archivo escribemath_archivo(Archivo nuevo, HSI ** in, int x, int y)
{
	for(int i=0; i<y; i++)
	{
        for(int j=0; j<x; j++)
		{
			for(int k=0; k<3; k++)//tres por ser 3 componentes de color BGR
			{
				switch(k)
				{
				case 0: fprintf(nuevo.archvec, "%i,", in[i][j].h);
					break;
				case 1: fprintf(nuevo.archvec, "%i,", in[i][j].s);
					break;
				case 2: fprintf(nuevo.archvec, "%i", in[i][j].i);
					break;
				}
			}
            fwrite(" ",1,1,nuevo.archvec);
		}
		fwrite("\n",1,1,nuevo.archvec);
	}
	return (nuevo);
}

void cerrar_archivo(Archivo nuevo)
{
	fclose(nuevo.archvec);
}
void guardar_matriz(Matr ** in, int x, int y, int color)
{
	Archivo nuevo;
	nuevo=crear_archivo(color);
	nuevo=inicio_archivo(nuevo);
	nuevo=escribemat_archivo(nuevo,in,x,y);
	cerrar_archivo(nuevo);
}
void guardar_matrizh(HSI ** in, int x, int y, int color)
{
	Archivo nuevo;
	nuevo=crear_archivoh(color);
	nuevo=inicio_archivo(nuevo);
	nuevo=escribemath_archivo(nuevo,in,x,y);
	cerrar_archivo(nuevo);
}

/*
    
  		fwrite("P2\n",3,1,archivo);
		fprintf(archivo, "%i %i\n%i\n", di[k].x, di[k].y, di[k].nivgris);		

		  fwrite("\n",1,1,archivo);
		}
	    fclose(archivo);


    if((archivo=fopen(strcat(itoa (k,s,10),".pgm"),"w"))==NULL)
        {
            printf("\nNo puedo abrir el archivo");
			exit(1);
		}

*/