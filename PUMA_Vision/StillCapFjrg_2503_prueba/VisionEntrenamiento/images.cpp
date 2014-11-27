/************************************************
*												*
*	IMAGES.CCP									*
*												*
*	Basic images processing routines.			*
*												*
*			FI. UNAM							*
*			11-27-96							*
*			9-20-05								*
*			Jesus Savage						*
*												*
*************************************************/

#include <stdio.h>
#include <math.h>
//#include "images.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "archivos.h"
#include "colores.h"
/*****************************************************************************
*  Define structure for vertex, lines and polygons.                          *
*****************************************************************************/
typedef struct Image_ {
	char **red;
	char **green;
	char **blue;
//	char **alfa;
} Image;



/* It allocates memory space */

int *alloc_vector_int(int len)
{
    return (int *) malloc((unsigned) len*sizeof(int));
}

char **alloc_matrix_char(int row,int col)
{
    int i;
    char **m;

    m = (char **) malloc((unsigned) row*sizeof(char *));
    for (i=0; i<row; i++)
        m[i] = (char *) malloc((unsigned) col*sizeof(char));
    return m;
}

Matr **alloc_Mem2d(int x, int y)
{
	Matr **m;

	if((m=(Matr **)malloc((unsigned) y *sizeof(Matr *)))==NULL)
	{
		printf("\nError al asignar memoria en 2D **-*");
		exit(1);
	}
	
	for(int j=0; j<y; j++)
	{
		if((m[j]=(Matr *)malloc((unsigned) x * sizeof(Matr)))==NULL)
			{
				printf("\nError al asignar memoria en 3D *-");
				exit(1);
			}
	}
	return m;
}

/* It frees memory space */
void free_matrix_char(char **m,int row)
{
    int i;

    for (i=0; i<row; i++)
        free((char *) m[i]);
    free((char *) m);
}
void libera_matriz(Matr **m,int row)
{
    int i;

    for (i=0; i<row; i++)
        free((Matr *) m[i]);
    free((Matr *) m);
}

/* It sums the pixel values of a window */
float sum_window(int Pixel_i,int Pixel_j,Image x, int num_lines_mask, int num_rows_mask)
{

    int i,j,ii,jj;
    int start_i,start_j,end_i,end_j;
    float sum;
    float divide,result;

    end_i=num_lines_mask/2;
    end_j=num_rows_mask/2;
    start_i= Pixel_i - end_i;
    start_j= Pixel_j - end_j;
    end_i= Pixel_i + end_i;
    end_j= Pixel_j + end_j;
    divide=(float) (num_lines_mask*num_rows_mask);
    
    if(start_i< 0)start_i=0;
    if(start_j< 0)start_j=0;
    if(end_i>HEIGHT_DEFAULT)end_i=HEIGHT_DEFAULT;
    if(end_j>WIDTH_DEFAULT)end_j=WIDTH_DEFAULT;

    sum=0;

    for(i=start_i,ii=1; i <= end_i ; i++,ii++){
        for(j=start_j,jj=1; j <= end_j; j++,jj++){

				sum+= x.red[i][j];
				sum+= x.green[i][j];
				sum+= x.blue[i][j];
		}
    }

    result=(float)(sum/divide);

    return(result);
}



/* It marks a section of the image */
mark_section(int Pixel_i,int Pixel_j,Image out,int num_lines_mask,int num_rows_mask,int send)
{

    int i,j;
    int start_i,start_j,end_i,end_j;

    end_i=num_lines_mask/2;
    end_j=num_rows_mask/2;
    start_i= Pixel_i - end_i;
    start_j= Pixel_j - end_j;
    end_i= Pixel_i + end_i;
    end_j= Pixel_j + end_j;

    if(start_i< 0)start_i=0;
    if(start_j< 0)start_j=0;
    if(end_i>HEIGHT_DEFAULT)end_i=HEIGHT_DEFAULT;
    if(end_j>WIDTH_DEFAULT)end_j=WIDTH_DEFAULT;

	for(i=start_i; i <= end_i; i++){
		
/*                out.red[i][end_j]=(char) MAX_VALUE;
                out.red[i][end_j-1]=(char)MAX_VALUE;
                out.red[i][start_j]=(char)MAX_VALUE;
                out.red[i][start_j+1]=(char)MAX_VALUE;
*/
		        out.red[i][end_j]=(char) 0;
                out.red[i][end_j-1]=(char)0;
                out.red[i][start_j]=(char)0;
                out.red[i][start_j+1]=(char)0;

				out.green[i][end_j]=(char) 0;
                out.green[i][end_j-1]=(char)0;
                out.green[i][start_j]=(char)0;
                out.green[i][start_j+1]=(char)0;

/*				out.blue[i][end_j]=(char) 0;
                out.blue[i][end_j-1]=(char)0;
                out.blue[i][start_j]=(char)0;
                out.blue[i][start_j+1]=(char)0;
*/        
				out.blue[i][end_j]=(char) MAX_VALUE;
                out.blue[i][end_j-1]=(char)MAX_VALUE;
                out.blue[i][start_j]=(char)MAX_VALUE;
                out.blue[i][start_j+1]=(char)MAX_VALUE;
    }

    for(j=start_j; j <= end_j  ; j++){
                out.red[start_i][j]=(char)MAX_VALUE;
                out.red[start_i+1][j]=(char)MAX_VALUE;
                out.red[end_i][j]=(char)MAX_VALUE;
                out.red[end_i-1][j]=(char)MAX_VALUE;

				out.green[start_i][j]=(char)0;
                out.green[start_i+1][j]=(char)0;
                out.green[end_i][j]=(char)0;
                out.green[end_i-1][j]=(char)0;

				out.blue[start_i][j]=(char)0;
                out.blue[start_i+1][j]=(char)0;
                out.blue[end_i][j]=(char)0;
                out.blue[end_i-1][j]=(char)0;
    }
	return(1);
}       



/* Llena el cuadrito interno del marco It fills a region of the image with a value */
void fill_window(int Pixel_i,int Pixel_j,Image x,int num_lines_mask,
					int num_rows_mask, int value)
{

     int i,j,start_i,start_j,end_i,end_j;
    
    end_i=num_lines_mask/2;
    end_j=num_rows_mask/2;
    start_i= Pixel_i - end_i;
    start_j= Pixel_j - end_j;
    end_i= Pixel_i + end_i;
    end_j= Pixel_j + end_j;
    if(start_i< 0)start_i=0;
    if(start_j< 0)start_j=0;
    if(end_i>HEIGHT_DEFAULT)end_i=HEIGHT_DEFAULT;
    if(end_j>WIDTH_DEFAULT)end_j=WIDTH_DEFAULT;

    for(i=start_i; i <= end_i ; i++){
        for(j=start_j; j <= end_j; j++){
                x.red[i][j]=value;
				x.green[i][j]=value;
				x.blue[i][j]=value;

        }
    }

}


/* It modifies the colors of the pixels */
/*process_matrixn(int Lines,int PixelsPerLines,Image image_in,Image image_process)
{
	int i,j;

	for(i=0; i < Lines ; i++){
    	for(j=0; j < PixelsPerLines; j++){
                image_process.red[i][j]=(char)(image_in.red[i][j]*0.0);
				image_process.green[i][j]=(char)(image_in.green[i][j]*0.0);
				image_process.blue[i][j]=(char)(image_in.blue[i][j]*1.0);
				image_process.alfa[i][j]=(char) 255;
		}
    }

	return(1);
}
*/

/* It transforms the vector data into a matrix */
void vector_to_matrix(int Lines, int PixelsPerLine,unsigned char *vector, Image matrix){

	int i,j,k;

	k=0;
    for(i=0; i <= Lines ; i++){
    	for(j=0; j < PixelsPerLine; j++){
                matrix.blue[i][j]=vector[k];
				matrix.green[i][j]=vector[k + 1];
				matrix.red[i][j]=vector[k + 2];
//				matrix.alfa[i][j]=vector[k + 3];
//				k=k+4;
				k=k+3;
		}
    }
}
Matr ** vector_a_matriz(int Lines, int PixelsPerLine,unsigned char *vector, Matr **matrix){

	int i,j,k;

	k=0;
    for(i=0; i < Lines ; i++){
    	for(j=0; j < PixelsPerLine; j++){
				matrix[i][j].blue=vector[k];
				matrix[i][j].green=vector[k + 1];
				matrix[i][j].red=vector[k + 2];
//				matrix.alfa[i][j]=vector[k + 3];
//				k=k+4;
				k=k+3;
		}
    }
	return (matrix);
}

/* It transfers the processed image to display vector */
void matrix_to_vector(int Lines, int PixelsPerLine,unsigned char *vector, Image matrix){

	int i,j,k;

    k=0;
    for(i=0; i <= Lines ; i++){
    	for(j=0; j < PixelsPerLine; j=j+1){
                vector[k]=matrix.blue[i][j];
				vector[k+1]=matrix.green[i][j];
				vector[k+2]=matrix.red[i][j];
//				vector[k+3]=matrix.alfa[i][j];
				k=k+3;
//				k=k+4;
		}
    }

}
unsigned char * matriz_a_vector(int Lines, int PixelsPerLine,unsigned char *vector, Matr **matrix){

	int i,j,k;

    k=0;
    for(i=0; i < Lines ; i++){
    	for(j=0; j < PixelsPerLine; j=j+1){
                vector[k]=matrix[i][j].blue;
				vector[k+1]=matrix[i][j].green;
				vector[k+2]=matrix[i][j].red;
//				vector[k+3]=matrix.alfa[i][j];
				k=k+3;
//				k=k+4;
		}
    }
 return (vector);
}


/* It allocates processing Images */
Image allocate_Images(int Lines, int PixelsPerLine){

	Image Image;


    if((Image.red =( char **) alloc_matrix_char(Lines+1,PixelsPerLine+1)) == 0){
        printf("\n Memory allocation error ");
        exit(0);
    }

	if((Image.green =( char **) alloc_matrix_char(Lines+1,PixelsPerLine+1)) == 0){
        printf("\n Memory allocation error ");
        exit(0);
    }

	if((Image.blue =( char **) alloc_matrix_char(Lines+1,PixelsPerLine+1)) == 0){
        printf("\n Memory allocation error ");
        exit(0);
    }

//	if((Image.alfa =( char **) alloc_matrix_char(Lines+1,PixelsPerLine+1)) == 0){
//        printf("\n Memory allocation error ");
//        exit(0);
//    }

	return(Image);
}

// It frees image memory space
void free_images(int Lines, Image Image){

     free_matrix_char(Image.red,Lines+1);
	 free_matrix_char(Image.green,Lines+1);
	 free_matrix_char(Image.blue,Lines+1);
//	 free_matrix_char(Image.alfa,Lines+1);

}



/* Llena la imagen con un valor It fills a image with a value */
void fill_image(int num_lines,int num_rows,Image mask, char value)
{
 int i,j;

 for(i=0;i<num_lines;i++)
	for(j=0;j<num_rows;j++)
		mask.red[i][j]=value;
		mask.green[i][j]=value;
		mask.blue[i][j]=value;

}

//pon ceros
Matr ** unos_to_matrix(int Lines, int PixelsPerLine,unsigned char *vector, Matr **matrix)
{
	int i,j,k;
	k=0;
    for(i=0; i < (int)Lines ; i++)
	{
    	for(j=0; j < (int)PixelsPerLine; j++)
		{
            matrix[i][j].blue=255;//vector[k];
			matrix[i][j].green=0;//vector[k + 1];
	  		matrix[i][j].red=0;//vector[k + 2];
				k=k+3;
		}
    }
	return (matrix);
}
Matr ** matrix_gris(int Lines, int PixelsPerLine,unsigned char *vector, Matr **matrix)
{
	int i,j,k;
	k=0;
    for(i=0; i < (int)Lines ; i++)
	{
    	for(j=0; j < (int)PixelsPerLine; j++)
		{
                matrix[i][j].blue=vector[k];//pone en escala de gris todos iguales
				matrix[i][j].green=vector[k];//pone en escala de gris todos iguales
				matrix[i][j].red=vector[k];//pone en escala de gris todos iguales
				k=k+3;
		}
    }
	return (matrix);
}


/* Encuentra la región de mas movimiento It finds the region of more movement  */
process_matrix(int Lines,int PixelsPerLines,Image x,Image out)
{

    int i,j;
    int lines = Lines;
    float threshold=THRESHOLD_SUM;
    float sum,max=0;
    int i_upper=Lines,i_lower=0;
    int j_left=PixelsPerLines,j_right=0;
    static int num_images_patt;
    static int send=0;
	static Image Image_old, Image_sum;
	static int global=0;
	int flag_global=0;
	static int global_j=NUM_ROWS_SCREEN,global_i=NUM_ROWS_WINDOW;
	
	if(global == 0){
		// Allocates static memory space
		Image_old=allocate_Images(Lines,PixelsPerLines);
		Image_sum=allocate_Images(Lines,PixelsPerLines);
		
		global=1;
		fill_image(Lines,PixelsPerLines,Image_old,0);//iniciliza la imagen con cero
		fill_image(Lines,PixelsPerLines,Image_sum,0);//iniciliza la imagen con cero
	}


	/* it calculates the difference between 2 consecutives frames */
    for(i=0; i < lines ; i++){
        for(j=0; j < PixelsPerLines; j++){
				Image_sum.red[i][j]+=abs(x.red[i][j]-Image_old.red[i][j]);
				Image_sum.green[i][j]+=abs(x.green[i][j]-Image_old.green[i][j]);
				Image_sum.blue[i][j]+=abs(x.blue[i][j]-Image_old.blue[i][j]);

				/* Este genera la imagen de salida It generates the output*/
				out.red[i][j]=x.red[i][j];
				out.blue[i][j]=x.blue[i][j];
				out.green[i][j]=x.green[i][j];

				/* Guarda la imagen presente (actual)it stores the present image */
				Image_old.red[i][j]=x.red[i][j];
				Image_old.blue[i][j]=x.blue[i][j];
				Image_old.green[i][j]=x.green[i][j];
        }
    }


	/* Every NUM_FRAMES_SUM it selects the window with more movement */
   if((global > NUM_FRAMES_SUM)==0){
	
		max=0;
		global=1;
		
		for(i=0; i < Lines; i+=NUM_LINES_WINDOW){
			for(j=0;j < PixelsPerLines; j+=NUM_ROWS_WINDOW){
				sum=sum_window(i,j,Image_sum,NUM_LINES_WINDOW,NUM_ROWS_WINDOW);
	   
				if(sum>max){
					max= sum;
					global_i=i;
					global_j=j;
				}
			}
		}
        
		/* It initialices image_sum with the last diference */
		for(i=0; i < lines; i++){
			for(j=0;j < PixelsPerLines ; j++){
				Image_sum.red[i][j]=abs(x.red[i][j]-Image_old.red[i][j]);
				Image_sum.green[i][j]=abs(x.green[i][j]-Image_old.green[i][j]);
				Image_sum.blue[i][j]=abs(x.blue[i][j]-Image_old.blue[i][j]);	
			}
		}
   }
  
   global++;

   /* it marks the region with more movement */
   if(max > THRESHOLD){
		fill_window(global_i,global_j,out,NUM_LINES_MASK,NUM_ROWS_MASK,MAX_VALUE);
 	    mark_section(global_i,global_j,out,NUM_LINES_WINDOW*3,NUM_ROWS_WINDOW*3,send);//tamaño del marquito rojo
	}

    return(1);

}


/* It process a image frame */
/*process_image(int PixelsPerLine,int Lines,char * address)
{

    Image Image_in, Image_processed;

	// Esto pide espacio de memoria para las imagenes It allocate images' memory spaces
	Image_in=allocate_Images(Lines,PixelsPerLine);
	Image_processed=allocate_Images(Lines,PixelsPerLine);
	//yh

	// Este transforma los datos lineales (vector) en una matrizIt transform the linear data into a matrix 
	vector_to_matrix(Lines,PixelsPerLine,address,Image_in);//void(int Lines, int PixelsPerLine,char *vector, Image matrix)
//modifico imagen_in y supongo que al hacerlo en memoria no es necesario ser tan explicito en devolver valores
	// Esto procesa la matriz de la imagen It process the matrix's image 
    process_matrix(Lines,PixelsPerLine,Image_in,Image_processed);

	// Transfiere la imagen procesada al vector de despliegue it transfers the processed image to the display vector 
	matrix_to_vector(Lines,PixelsPerLine,address,Image_processed);

	// Esto libera el espacio de memoria It frees images' memory space
	free_images(Lines,Image_in);
	free_images(Lines,Image_processed);

	return(1);

}
*/
process_imagej(int PixelsPerLine,int Lines,unsigned char * address)
{

    Matr **Image_in, **Image_processed;

	// Esto pide espacio de memoria para las imagenes It allocate images' memory spaces
	Image_in=alloc_Mem2d(Lines,PixelsPerLine);//allocate_Images(Lines,PixelsPerLine);
	Image_processed=alloc_Mem2d(Lines,PixelsPerLine);//allocate_Images(Lines,PixelsPerLine);
	//yh

	// Este transforma los datos lineales (vector) en una matrizIt transform the linear data into a matrix 
	Image_in=vector_a_matriz(Lines,PixelsPerLine,address,Image_in);//void(int Lines, int PixelsPerLine,char *vector, Image matrix)
//modifico imagen_in y supongo que al hacerlo en memoria no es necesario ser tan explicito en devolver valores
	// Esto procesa la matriz de la imagen It process the matrix's image 
//    process_matrix(Lines,PixelsPerLine,Image_in,Image_processed);
	Image_in=unos_to_matrix(Lines,PixelsPerLine,address,Image_in);
	// Transfiere la imagen procesada al vector de despliegue it transfers the processed image to the display vector 
	address=matriz_a_vector(Lines,PixelsPerLine,address,Image_in);

	// Esto libera el espacio de memoria It frees images' memory space
	libera_matriz(Image_in, Lines);
	libera_matriz(Image_processed, Lines);
//	free_images(Lines,Image_in);
//	free_images(Lines,Image_processed);

	return(1);

}
unsigned char *imag_distj(int x, int y,unsigned char * pBuffer, Matrixyi ** coord)
{
	Matr ** in;
	Matr ** aux;
	in=alloc_Mem2d(x,y);
	aux=alloc_Mem2d(x,y);
	in=vector_a_matriz(y, x, pBuffer, in);
	aux=vector_a_matriz(y, x, pBuffer, aux);
	for(int i=0; i<y; i++)
	{
        for(int j=0; j<x; j++)
		{
            if(coord[i][j].y>0 && coord[i][j].x>0 && coord[i][j].y<y && coord[i][j].x<x)
			{
                in[coord[i][j].y][coord[i][j].x].blue=aux[i][j].blue;
				in[coord[i][j].y][coord[i][j].x].green=aux[i][j].green;
				in[coord[i][j].y][coord[i][j].x].red=aux[i][j].red;
			}
		}
	}
	pBuffer=matriz_a_vector(y, x, pBuffer, in);
	libera_matriz(in,y);
	libera_matriz(aux,y);
    return (pBuffer);
}

HSI ** imag_hsij(int x, int y, Matr ** in)
{
	HSI ** hs;
	hs=alloc_Memh(x, y);


	for(int i=0; i<y; i++)
	{
        for(int j=0; j<x; j++)
		{
            hs[i][j]=Rgb2Hsi( in[i][j] );
		}
	}
//void libera_matrizh(HSI **m,int row);
    return (hs);
}

unsigned char *process_imagevj(int x, int y,unsigned char * pBuffer)
{
	Matr ** in;
	in=alloc_Mem2d(x,y);
	in=vector_a_matriz(y, x, pBuffer, in);
	in=matrix_gris(y, x, pBuffer, in);
	pBuffer=matriz_a_vector(y, x, pBuffer, in);
	libera_matriz(in,y);
    return (pBuffer);
}
Matrixyi ** process_imagedvj(int x, int y,unsigned char * pBuffer)
{
	Matr ** in;
	Matrixyi ** coord;
	in=alloc_Mem2d(x,y);
	in=vector_a_matriz(y, x, pBuffer, in);
//	in=matrix_gris(y, x, pBuffer, in);
	coord=distorsionj(x,y);
	pBuffer=matriz_a_vector(y, x, pBuffer, in);
	libera_matriz(in,y);
    return (coord);
}
Matrixyi ** process_imagedvj1(int x, int y, int centrox, int centroy, unsigned char * pBuffer, float a, float b, float c)
{
	Matr ** in;
	Matrixyi ** coord;
	in=alloc_Mem2d(x,y);
	in=vector_a_matriz(y, x, pBuffer, in);
//	in=matrix_gris(y, x, pBuffer, in);
	coord=distorsionj1(x,y,centrox,centroy,a,b,c);
	pBuffer=matriz_a_vector(y, x, pBuffer, in);
	libera_matriz(in,y);
    return (coord);
}

void process_imageavj(int x, int y,unsigned char * pBuffer)
{
	Matr ** in;
	HSI ** hs;
	in=alloc_Mem2d(x,y);
	in=vector_a_matriz(y, x, pBuffer, in);
	guardar_matriz(in,x,y,1);
    hs=imag_hsij(x, y, in);
	guardar_matrizh(hs,x,y,1);
	pBuffer=matriz_a_vector(y, x, pBuffer, in);
	libera_matriz(in,y);
	libera_matrizh(hs,y);
}

//void guardar_matriz(Matr ** in, int x, int y)