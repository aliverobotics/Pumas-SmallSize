// 11-08-04
// This release is being building from the RLE approach

// 05-08-04
// This release uses the kmeans algorithm implementation written by Wolf
// in his linux program namely grabview.cpp. It's necessary to point out that the original
// code has been left untouched for future revitions
//
// NOTE: Once either implementation works well the remainder should be erased in order to
// keep as clean as possible this software

//------------------------------------------------------------------------------
// File: StillCapDlg.cpp
//
// Desc: DirectShow sample code - implementation of callback and dialog
//       objects for StillCap application.
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------

//#define RGB


#define MOVE_POTENTIALS		0.5



#include <stdafx.h>
#include "StillCap.h"
#include "StillCapDlg.h"
#include "..\..\..\common\dshowutil.cpp" 
#include "detectcolors.h"
#include "detectrobots.h"
#include "linklistblob.h"
#include "colors.h"


#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include <process.h>

#include "SaveAndRestoreParameters.h"
#include "Persistence.h"

/**************************/
//Jair para la distorsion
#include "images.h"
/**************************/
#define WIN_LENGHT	5 //5
#define RATIO		10
#define DRAW_BLOB_SIZE 6	// It's the wondows size 



#include <winsock.h>
//#include "C:\DXSDK\Samples\C++\DirectShow\Editing\StillCap\socklib.h"
#include "socklib.h"

//#include "sockets.h"

#include "myudpsockets.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// An application can advertise the existence of its filter graph
// by registering the graph with a global Running Object Table (ROT).
// The GraphEdit application can detect and remotely view the running
// filter graph, allowing you to 'spy' on the graph with GraphEdit.
//
// To enable registration in this sample, define REGISTER_FILTERGRAPH.
//
#ifdef DEBUG
#define REGISTER_FILTERGRAPH
#endif

// Constants
#define WM_CAPTURE_BITMAP   WM_APP + 1

// Global data
BOOL g_bOneShot=FALSE;
DWORD g_dwGraphRegister=0;  // For running object table
HWND g_hwnd=0;

// Structures
typedef struct _callbackinfo 
{
    double dblSampleTime;
    long lBufferSize;
    BYTE *pBuffer;
    BITMAPINFOHEADER bih;

} CALLBACKINFO;

CALLBACKINFO cb={0};


/*========================== Selects a cam to work with ==========================*/

/*	Undefine for using a second cam connected to the system. For default this software
	uses the very first it founds
*/
//#define SECONDCAM 1//sALTA PARA TOMAR LA SEGUNDA CAMARA
//#define THIRDCAM 1

/*================================================================================*/


/*========================== Toggles a view between RLS's and centroids ==========*/
#define PRINT_CENTR 1

/*================================================================================*/

RGB *rgb;



void RLEDrawBlob( int x0, int y0, int x1, int y1, unsigned char *b, RGB rgb );
void RLEDrawBlob( Point p, unsigned char *b, RGB rgb );


BOOL MybFileWritten=FALSE;

BOOL m_timer=FALSE;
int bright=0;
float contrast=1;
float saturation = 0.0;
BOOL m_process=FALSE;
BOOL km_ok = FALSE;


/*========================== Selects the cam's resolution ========================*/
//#define MAX_RES_X			352
//#define MAX_RES_Y			288
//#define MAX_RES_X			1280
//#define MAX_RES_Y			1024

//#define MAX_RES_X			320
//#define MAX_RES_Y			240

//#define MAX_RES_X			720
//#define MAX_RES_Y			480
int MAX_RES_X, MAX_RES_Y;

#define RATIO_X				MAX_RES_X/300
#define RATIO_Y				MAX_RES_Y/243


/*================================================================================*/

#define MAX_IMAGE_SIZE		MAX_RES_X*MAX_RES_Y	//640*480

#define SQ					15 //25

/*========================== Selects the defult colors to show ===================*/
#define RGB_ORANGE(x)	x.r=255;	x.g=0;		x.b=0;
#define RGB_BLUE(x)		x.r=0;		x.g=0;		x.b=255;
#define RGB_YELLOW(x)	x.r=255;	x.g=255;	x.b=0;
#define RGB_GREEN(x)	x.r=0;		x.g=255;	x.b=0;
#define RGB_PINK(x)		x.r=255;	x.g=0;		x.b=255;
#define RGB_CYAN(x)		x.r=0;		x.g=255;	x.b=255;

#define RGB_BLACK(x)	x.r=0;		x.g=0;		x.b=0;
#define RGB_FIELD(x)	x.r=255;	x.g=255;	x.b=255;
#define RGB_TEST(x)		x.r=0;	x.g=255;	x.b=255;

/*================================================================================*/


#define MAXREDAREA			10

int MinBlobButt;
int MinBlobArea, MinRLE, MinBlobX, MinBlobY;
int OptArea;

int FrameRate;		


FIELD FieldCoords, WinCoords, FieldCoordsDown;


/* Field limits in meters */
FFIELD FieldLim;



/*========================== Sockets stuff =======================================*/
#define PROTOPORT       5193            
/* default protocol port number */


char localhost[] =   "localhost";    
int sd, sd2, n;         /* socket descriptors                  */
char SckBuf[256];       /* buffer for string the server sends  */
BOOL SocketAlreadyOpen;
char IPLocal[24] = "132.248.52.188", IPRemote[24] = "192.168.1.107";
int IPPort = 8011;
UDPSockets mysox;

/*================================================================================*/






int TeamColor = 0;
int Color = 0;
BOOL CalibrationInProgress = FALSE;

BOOL CalibrationCancha = FALSE;





#define hORANGE			13
#define sORANGE			127
#define iORANGE			127

#define hBLUE			92
#define sBLUE			127
#define IBLUE			127		// The 'I' uppercase is to avoid conflicts with previous
								// definitions made for DX9
#define hYELLOW			26
#define sYELLOW			127
#define iYELLOW			127

#define hGREEN			40
#define sGREEN			127
#define IGREEN			127		// See above

#define hPINK			145
#define sPINK			127
#define iPINK			127

#define hCYAN			80
#define sCYAN			127
#define iCYAN			127

HSITH hsi[6];				// Here is the complete info about the HSI color ranges
HSI myhsi;
HSITH hnew, hold;

HSI hm;


HSITH hsiCalib;
int MaxDist;

int hue[6] = { hORANGE, hBLUE, hYELLOW, hGREEN, hPINK, hCYAN };
int sat[6] = { sORANGE, sBLUE, sYELLOW, sGREEN, sPINK, sCYAN };
int ins[6] = { iORANGE, IBLUE, iYELLOW, IGREEN, iPINK, iCYAN };

RGB PickedColor;

int times = 0;
BYTE hist[3][256];


float acost[2000];

typedef struct
{
	int		times; 
	BYTE	hist[3][360];
	float	expected;
	float	sigma;
} CALIB;

CALIB calib[3];


#define PUNTO_ANCHO 1




typedef struct{
	unsigned char *r;
	unsigned char *g;
	unsigned char *b;
} RGB_BUFFER;


int VideoP;

#define SIGMA_FACTOR 2.0

/************************************************************/
                 //Jair para la distorsion
BOOL distor=FALSE;//yo para manejar el boton de distorsión
BOOL bandist=TRUE;
Matrixyi ** coord;
int centrox;
int centroy;
float disto_a;
float disto_b;
float disto_c;
/************************************************************/

int Linear2Matrix( unsigned char *pb, RGB_BUFFER *b, int maxX, int maxY ){
	int s, addr, contx, conty;
	for( conty = 0; conty < maxY; conty++ ){
		for( contx = 0; contx < maxX; contx++ ){
			*(b->b+contx) = *pb++;
			*(b->g+contx) = *pb++;
			*(b->r+contx) = *pb++;
		}
	}
	return 0;
}



void RLEDrawBlob( int x0, int y0, int x1, int y1, unsigned char *b, RGB rgb )
{
	int addr,s;

	for( int col = y0; col < y1; col++ )
	{
		s = col*MAX_RES_X*3;
		for( int row = x0; row < x1; row++ )
		{
			addr = row*3 + s;
			*(b+addr) = rgb.b;
			*(b+addr+1) = rgb.g;
			*(b+addr+2) = rgb.r;
		}
	}
}

void RLEDrawBlob( Point p, unsigned char *b, RGB rgb )
{
	int addr,s, x0, x1, y0, y1;

	y0 = p.y-DRAW_BLOB_SIZE/2;
	y1 = p.y+DRAW_BLOB_SIZE/2;
	if( y0 < 0 ) y0 = 0;
	if( y1 > MAX_RES_Y ) y1 = MAX_RES_Y;

	x0 = p.x-DRAW_BLOB_SIZE/2;
	x1 = p.x+DRAW_BLOB_SIZE/2;
	if( x0 < 0 ) x0 = 0;
	if( x1 > MAX_RES_X ) x1 = MAX_RES_X;

	for( int col = y0; col < y1; col++ )
	{


		s = col*MAX_RES_X*3;
		for( int row = x0; row < x1; row++ )
		{


			addr = row*3 + s;
			*(b+addr) = rgb.b;
			*(b+addr+1) = rgb.g;
			*(b+addr+2) = rgb.r;
		}
	}

}

void RLEDrawBlob( int x0, int y0, int x1, int y1, unsigned char *b, int color)
{
	int addr,s;
	for( int col = y0; col < y1; col++ )
	{
		s = col*MAX_RES_X*3;
		for( int row = x0; row < x1; row++ )
		{
			addr = row*3 + s;
			if (color == orange)
			{
				*(b+addr) = 0;
				*(b+addr+1) = 0;
				*(b+addr+2) = 255;
			}
			else if (color == yellow)
			{
				*(b+addr) = 0;
				*(b+addr+1) = 230;
				*(b+addr+2) = 255;
			}
			else if (color == blue)
			{
				*(b+addr) = 255;
				*(b+addr+1) = 0;
				*(b+addr+2) = 0;
			}
			else if (color == green)
			{
				*(b+addr) = 100;
				*(b+addr+1) = 255;
				*(b+addr+2) = 60;
			}
			else if (color == pink)
			{
				*(b+addr) = 244;
				*(b+addr+1) = 4;
				*(b+addr+2) = 202;
			}
			else if (color == cyan)
			{
				*(b+addr) = 255;
				*(b+addr+1) = 255;
				*(b+addr+2) = 70;
			}
			else if (color == 10)
			{
				*(b+addr) = 57;
				*(b+addr+1) = 172;
				*(b+addr+2) = 210;
			}
			else if (color == 11)
			{
				*(b+addr) = 160;
				*(b+addr+1) = 160;
				*(b+addr+2) = 160;
			}
		}
	}
}


void drawLine( int x0, int y0, int d, float angle, unsigned char *b, int color){
	int newX, newY;
	float ang = angle * 3.14159 / 180;
	newX = (int)(d * cos((float)ang) + x0);
	newY = (int)(d * sin((float)ang) + y0);
	int x = x0;
	int y = y0;
	float m;
	while (x < newX){  
		m = tan((float)ang);
		y = m * (x-x0) + y0;
		if ( x-PUNTO_ANCHO >= 0 && x-PUNTO_ANCHO <= MAX_RES_X &&
			y-PUNTO_ANCHO >= 0 && y-PUNTO_ANCHO <= MAX_RES_Y &&
			x+PUNTO_ANCHO >= 0 && x+PUNTO_ANCHO <= MAX_RES_X &&
			y+PUNTO_ANCHO >= 0 && y+PUNTO_ANCHO <= MAX_RES_Y )
	//	if ( x-PUNTO_ANCHO >= 0 && y-PUNTO_ANCHO>= 0 && x+PUNTO_ANCHO <= MAX_RES_X && y+PUNTO_ANCHO <= MAX_RES_Y)
			RLEDrawBlob(x-PUNTO_ANCHO, y-PUNTO_ANCHO,x+PUNTO_ANCHO,y+PUNTO_ANCHO,b,color);
		x++;
	}
	x=x0;
	while (x > newX){
		m = tan((float)ang);
		y = m * (x-x0) + y0;
		if ( x-PUNTO_ANCHO >= 0 && x-PUNTO_ANCHO <= MAX_RES_X &&
			y-PUNTO_ANCHO >= 0 && y-PUNTO_ANCHO <= MAX_RES_Y &&
			x+PUNTO_ANCHO >= 0 && x+PUNTO_ANCHO <= MAX_RES_X &&
			y+PUNTO_ANCHO >= 0 && y+PUNTO_ANCHO <= MAX_RES_Y )
	//	if ( x-PUNTO_ANCHO >= 0 && y-PUNTO_ANCHO>= 0 && x+PUNTO_ANCHO <= MAX_RES_X && y+PUNTO_ANCHO <= MAX_RES_Y)
			RLEDrawBlob(x-PUNTO_ANCHO, y-PUNTO_ANCHO,x+PUNTO_ANCHO,y+PUNTO_ANCHO,b,color);
		x--;
	}
	while (y < newY){  
		m = tan((float)ang);
		x = (y-y0) / m + x0;
		if ( x-PUNTO_ANCHO >= 0 && x-PUNTO_ANCHO <= MAX_RES_X &&
			y-PUNTO_ANCHO >= 0 && y-PUNTO_ANCHO <= MAX_RES_Y &&
			x+PUNTO_ANCHO >= 0 && x+PUNTO_ANCHO <= MAX_RES_X &&
			y+PUNTO_ANCHO >= 0 && y+PUNTO_ANCHO <= MAX_RES_Y )
	//	if ( x-PUNTO_ANCHO >= 0 && y-PUNTO_ANCHO>= 0 && x+PUNTO_ANCHO <= MAX_RES_X && y+PUNTO_ANCHO <= MAX_RES_Y)
			RLEDrawBlob(x-PUNTO_ANCHO, y-PUNTO_ANCHO,x+PUNTO_ANCHO,y+PUNTO_ANCHO,b,color);
		y++;
	}
	y=y0;
	while (y > newY){  
		m = tan((float)ang);
		x = (y-y0) / m + x0;
		if ( x-PUNTO_ANCHO >= 0 && x-PUNTO_ANCHO <= MAX_RES_X &&
			y-PUNTO_ANCHO >= 0 && y-PUNTO_ANCHO <= MAX_RES_Y &&
			x+PUNTO_ANCHO >= 0 && x+PUNTO_ANCHO <= MAX_RES_X &&
			y+PUNTO_ANCHO >= 0 && y+PUNTO_ANCHO <= MAX_RES_Y )
	//	if ( x-PUNTO_ANCHO >= 0 && y-PUNTO_ANCHO>= 0 && x+PUNTO_ANCHO <= MAX_RES_X && y+PUNTO_ANCHO <= MAX_RES_Y)
			RLEDrawBlob(x-PUNTO_ANCHO, y-PUNTO_ANCHO,x+PUNTO_ANCHO,y+PUNTO_ANCHO,b,color);
		y--;
	}
}

void drawCircle( int x0, int y0, int r, unsigned char *b, int color)
{
	int x = x0, x1, x2;
	int y = y0, y1, y2;
	int rCuad = pow(r,2);
	while ( pow(x-x0,2) <= rCuad )
	{
		y1 = sqrt( rCuad - pow(x-x0,2) ) + y0;
		y2 = -1*sqrt( rCuad - pow(x-x0,2) ) + y0;
		if (x-PUNTO_ANCHO >= 0 && x-PUNTO_ANCHO <= MAX_RES_X &&
			y1-PUNTO_ANCHO >= 0 && y1-PUNTO_ANCHO <= MAX_RES_Y &&
			x+PUNTO_ANCHO >= 0 && x+PUNTO_ANCHO <= MAX_RES_X &&
			y1+PUNTO_ANCHO >= 0 && y1+PUNTO_ANCHO <= MAX_RES_Y &&
			y2-PUNTO_ANCHO >= 0 && y2-PUNTO_ANCHO <= MAX_RES_Y &&
			y2+PUNTO_ANCHO >= 0 && y2+PUNTO_ANCHO <= MAX_RES_Y )
		{
//		if ( x-PUNTO_ANCHO >= 0 && y1-PUNTO_ANCHO>= 0 && x+PUNTO_ANCHO <= MAX_RES_X && y1+PUNTO_ANCHO <= MAX_RES_Y && y2+PUNTO_ANCHO <= MAX_RES_Y){
			RLEDrawBlob(x-PUNTO_ANCHO, y1-PUNTO_ANCHO,x+PUNTO_ANCHO,y1+PUNTO_ANCHO,b,color);
			RLEDrawBlob(x-PUNTO_ANCHO, y2-PUNTO_ANCHO,x+PUNTO_ANCHO,y2+PUNTO_ANCHO,b,color);
		}
		x++;
	}
	x = x0;
	while ( pow(x-x0,2) <= rCuad )
	{
		y1 = sqrt( rCuad - pow(x-x0,2) ) + y0;
		y2 = -1*sqrt( rCuad - pow(x-x0,2) ) + y0;
		if (x-PUNTO_ANCHO >= 0 && x-PUNTO_ANCHO <= MAX_RES_X &&
			y1-PUNTO_ANCHO >= 0 && y1-PUNTO_ANCHO <= MAX_RES_Y &&
			x+PUNTO_ANCHO >= 0 && x+PUNTO_ANCHO <= MAX_RES_X &&
			y1+PUNTO_ANCHO >= 0 && y1+PUNTO_ANCHO <= MAX_RES_Y &&
			y2-PUNTO_ANCHO >= 0 && y2-PUNTO_ANCHO <= MAX_RES_Y &&
			y2+PUNTO_ANCHO >= 0 && y2+PUNTO_ANCHO <= MAX_RES_Y )
		{
//		if ( x-PUNTO_ANCHO >= 0 && y1-PUNTO_ANCHO>= 0 && x+PUNTO_ANCHO <= MAX_RES_X && y1+PUNTO_ANCHO <= MAX_RES_Y && y2+PUNTO_ANCHO <= MAX_RES_Y){
			RLEDrawBlob(x-PUNTO_ANCHO, y1-PUNTO_ANCHO,x+PUNTO_ANCHO,y1+PUNTO_ANCHO,b,color);
			RLEDrawBlob(x-PUNTO_ANCHO, y2-PUNTO_ANCHO,x+PUNTO_ANCHO,y2+PUNTO_ANCHO,b,color);
		}
		x--;
	}
// respecto a y
	y = y0;
	while ( pow(y-y0,2) <= rCuad )
	{
		x1 = sqrt( rCuad - pow(y-y0,2) ) + x0;
		x2 = -1*sqrt( rCuad - pow(y-y0,2) ) + x0;
		if (y-PUNTO_ANCHO >= 0 && y-PUNTO_ANCHO <= MAX_RES_Y &&
			x1-PUNTO_ANCHO >= 0 && x1-PUNTO_ANCHO <= MAX_RES_X &&
			y+PUNTO_ANCHO >= 0 && y+PUNTO_ANCHO <= MAX_RES_Y &&
			x1+PUNTO_ANCHO >= 0 && x1+PUNTO_ANCHO <= MAX_RES_X &&
			x2-PUNTO_ANCHO >= 0 && x2-PUNTO_ANCHO <= MAX_RES_X &&
			x2+PUNTO_ANCHO >= 0 && x2+PUNTO_ANCHO <= MAX_RES_X )
		{
//		if ( x-PUNTO_ANCHO >= 0 && y1-PUNTO_ANCHO>= 0 && x+PUNTO_ANCHO <= MAX_RES_X && y1+PUNTO_ANCHO <= MAX_RES_Y && y2+PUNTO_ANCHO <= MAX_RES_Y){
			RLEDrawBlob(x1-PUNTO_ANCHO, y-PUNTO_ANCHO,x1+PUNTO_ANCHO,y+PUNTO_ANCHO,b,color);
			RLEDrawBlob(x2-PUNTO_ANCHO, y-PUNTO_ANCHO,x2+PUNTO_ANCHO,y+PUNTO_ANCHO,b,color);
		}
		y++;
	}
	y = y0;
	y = y0;
	while ( pow(y-y0,2) <= rCuad )
	{
		x1 = sqrt( rCuad - pow(y-y0,2) ) + x0;
		x2 = -1*sqrt( rCuad - pow(y-y0,2) ) + x0;
		if (y-PUNTO_ANCHO >= 0 && y-PUNTO_ANCHO <= MAX_RES_Y &&
			x1-PUNTO_ANCHO >= 0 && x1-PUNTO_ANCHO <= MAX_RES_X &&
			y+PUNTO_ANCHO >= 0 && y+PUNTO_ANCHO <= MAX_RES_Y &&
			x1+PUNTO_ANCHO >= 0 && x1+PUNTO_ANCHO <= MAX_RES_X &&
			x2-PUNTO_ANCHO >= 0 && x2-PUNTO_ANCHO <= MAX_RES_X &&
			x2+PUNTO_ANCHO >= 0 && x2+PUNTO_ANCHO <= MAX_RES_X )
		{
//		if ( x-PUNTO_ANCHO >= 0 && y1-PUNTO_ANCHO>= 0 && x+PUNTO_ANCHO <= MAX_RES_X && y1+PUNTO_ANCHO <= MAX_RES_Y && y2+PUNTO_ANCHO <= MAX_RES_Y){
			RLEDrawBlob(x1-PUNTO_ANCHO, y-PUNTO_ANCHO,x1+PUNTO_ANCHO,y+PUNTO_ANCHO,b,color);
			RLEDrawBlob(x2-PUNTO_ANCHO, y-PUNTO_ANCHO,x2+PUNTO_ANCHO,y+PUNTO_ANCHO,b,color);
		}
		y--;
	}
}






/*==================================================================================
*						Histogram()
==================================================================================*/
void Histogram( char *namefile, BYTE hist[], int scale, int lim )
{
	int cont;
	FILE *fp;
	fp = fopen( namefile, "w" );

	for( cont = 0; cont < lim; cont++ )
	{
		fprintf( fp, "%3d ", cont );
		
		for( int cnt = 0; cnt < (int)(hist[cont]/scale); cnt++ )
			fprintf( fp, "*" );
		

		fprintf( fp, " %d\n", hist[cont] );
	}

	fclose( fp );
}


/*==================================================================================
*						GetExpected
==================================================================================*/
/** Gets the expectation
 *
 */
float GetExpected( CALIB cal, int lim )
{
	int cont;
	unsigned sum;
	float prob[360], expected;

	for( cont = 0; cont < lim; cont++ )
		cal.hist[0][cont] += cal.hist[1][cont] + cal.hist[2][cont];
	
	for( cont = 0, sum = 0; cont < lim; cont++ )
		sum += cal.hist[0][cont];

	for( cont = 0; cont < lim; cont++ )
		prob[cont] = (float)(cal.hist[0][cont]) / (float)sum;

	for( cont = 0, expected = 0.0; cont < lim; cont++ )
		expected += (float)(cont) * prob[cont];

	return expected;
}



/*==================================================================================
*						GetSigma
==================================================================================*/
/** Gets sigma */
float GetSigma( CALIB cal, int lim )
{
	float sigma=0;
	int cont;
	int sum;
	float prob[360], expected, temp;

	// obtiene el histograma globlal
	for( cont = 0; cont < lim; cont++ )
		cal.hist[0][cont] += cal.hist[1][cont] + cal.hist[2][cont];

	// obtiene el número de muestras
	for( cont = 0, sum = 0; cont < lim; cont++ )
		sum += cal.hist[0][cont];

	// obtiene la PDF
	for( cont = 0; cont < lim; cont++ )
		prob[cont] = (float)(cal.hist[0][cont]) / (float)sum;

	// obtiene el valor esperado E{(X)^2}
	for( cont = 0, expected = 0.0; cont < lim; cont++ )
		expected += (float)(cont) * prob[cont];
	

	for( cont = 0; cont < lim; cont++ ){
		temp = (float)cont - expected;
		temp *= temp;

		sigma += temp * prob[cont];
	}

	sigma = sqrt( sigma );

	return sigma;
}

int hsiGetMiddle( int max, int min, int valm )
{
	int grad;

	if( min <= max )
		grad = (max+min) >> 1;	
	else
	{
		grad = ( (valm-min+max)>>1 ) + min;
		if( grad > valm )
			grad -= valm;
	}

	return grad;
}





void rgbBright( unsigned char *pBuffer, int bright, int countmax)
{
	for( int cont = 0, contb=0; cont < countmax; cont += 3 )
	{
		for( int cont2 = 0; cont2 < 3; cont2++ )
		{
			if( (*(pBuffer+cont+cont2)+bright) > 255 )
				*(pBuffer+cont+cont2) = 255;
			else if( (*(pBuffer+cont+cont2)+bright) < 0 )
				*(pBuffer+cont+cont2) = 0;
			else
				*(pBuffer+cont+cont2)+=bright;
		}
	}
}

void rgbContrast( unsigned char *pBuffer, float contrast, int countmax)
{
	for( int cont = 0, contb=0; cont < countmax; cont += 3 )
	{
		float db = *(pBuffer+cont) - 127.0;
		float dg = *(pBuffer+cont+1) - 127.0;
		float dr = *(pBuffer+cont+2) - 127.0;

		db *= contrast;
		dg *= contrast;
		dr *= contrast;
			
		db += 127.0;
		dg += 127.0;
		dr += 127.0;
			
		if( db > 255 )
			*(pBuffer+cont) = 255;
		else if( db < 0 )
			*(pBuffer+cont) = 0;
		else
			*(pBuffer+cont) = (int)db;

		if( dg > 255 )
			*(pBuffer+cont+1) = 255;
		else if( dg < 0 )
			*(pBuffer+cont+1) = 0;
		else
			*(pBuffer+cont+1) = (int)dg;

		if( dr > 255 )
			*(pBuffer+cont+2) = 255;
		else if( dr < 0 )
			*(pBuffer+cont+2) = 0;
		else
			*(pBuffer+cont+2) = (int)dr;
	}
}


void rgbSaturation( unsigned char *pBuffer, float saturation, int countmax)
{
	float gray;
	float db, dg, dr;
	
	for( int cont = 0, contb=0; cont < countmax; cont += 3 )
	{
		dr = (float)*(pBuffer+cont+2);
		dg = (float)*(pBuffer+cont+1);
		db = (float)*(pBuffer+cont);

		gray = 0.212671*dr + 0.71516*dg + 0.072169*db;

		dr = dr + saturation * (dr - gray);
		dg = dg + saturation * (dg - gray);
		db = db + saturation * (db - gray);

		*(pBuffer+cont+2) = (int)dr;
		if( dr > 255.0 )
			*(pBuffer+cont+2) = 255;
		else if( dr < 0.0 )
			*(pBuffer+cont+2) = 0;

		*(pBuffer+cont+1) = (int)dg;
		if( dg > 255.0 )
			*(pBuffer+cont+1) = 255;
		else if( dg < 0.0 )
			*(pBuffer+cont+1) = 0;

		*(pBuffer+cont) = (int)db;
		if( db > 255.0 )
			*(pBuffer+cont) = 255;
		else if( db < 0.0 )
			*(pBuffer+cont) = 0;
	}
}



// Note: this object is a SEMI-COM object, and can only be created statically.
// We use this little semi-com object to handle the sample-grab-callback,
// since the callback must provide a COM interface. We could have had an interface
// where you provided a function-call callback, but that's really messy, so we
// did it this way. You can put anything you want into this C++ object, even
// a pointer to a CDialog. Be aware of multi-thread issues though.
//
class CSampleGrabberCB : public ISampleGrabberCB 
{
public:
    // these will get set by the main thread below. We need to
    // know this in order to write out the bmp
    long lWidth;
    long lHeight;
    CStillCapDlg * pOwner;
    TCHAR m_szCapDir[MAX_PATH]; // the directory we want to capture to
    TCHAR m_szSnappedName[MAX_PATH];
    BOOL bFileWritten;

    CSampleGrabberCB( )
    {
        pOwner = NULL;
        ZeroMemory(m_szCapDir, sizeof(m_szCapDir));
        ZeroMemory(m_szSnappedName, sizeof(m_szSnappedName));
        bFileWritten = FALSE;
    }   


    // fake out any COM ref counting
    //
    STDMETHODIMP_(ULONG) AddRef() { return 2; }
    STDMETHODIMP_(ULONG) Release() { return 1; }

    // fake out any COM QI'ing
    //
    STDMETHODIMP QueryInterface(REFIID riid, void ** ppv)
    {
        if( riid == IID_ISampleGrabberCB || riid == IID_IUnknown ) 
        {
            *ppv = (void *) static_cast<ISampleGrabberCB*> ( this );
            return NOERROR;
        }    
        return E_NOINTERFACE;
    }

    // we don't implement this interface for this example
    //
    STDMETHODIMP SampleCB( double SampleTime, IMediaSample * pSample )
    {
        return 0;
    }

    // The sample grabber is calling us back on its deliver thread.
    // This is NOT the main app thread!
    //
    //           !!!!! WARNING WARNING WARNING !!!!!
    //
    // On Windows 9x systems, you are not allowed to call most of the 
    // Windows API functions in this callback.  Why not?  Because the
    // video renderer might hold the global Win16 lock so that the video
    // surface can be locked while you copy its data.  This is not an
    // issue on Windows 2000, but is a limitation on Win95,98,98SE, and ME.
    // Calling a 16-bit legacy function could lock the system, because 
    // it would wait forever for the Win16 lock, which would be forever
    // held by the video renderer.
    //
    // As a workaround, copy the bitmap data during the callback,
    // post a message to our app, and write the data later.
    //
    STDMETHODIMP BufferCB( double dblSampleTime, BYTE * pBuffer, long lBufferSize )
    {
        // this flag will get set to true in order to take a picture
        //
        if( !g_bOneShot )
            return 0;

        if (!pBuffer)
            return E_POINTER;

        if( cb.lBufferSize < lBufferSize )
        {
            delete [] cb.pBuffer;
            cb.pBuffer = NULL;
            cb.lBufferSize = 0;
        }

        // Since we can't access Windows API functions in this callback, just
        // copy the bitmap data to a global structure for later reference.
        cb.dblSampleTime = dblSampleTime;

        // If we haven't yet allocated the data buffer, do it now.
        // Just allocate what we need to store the new bitmap.
        if (!cb.pBuffer)
        {
            cb.pBuffer = new BYTE[lBufferSize];
            cb.lBufferSize = lBufferSize;
        }

        if( !cb.pBuffer )
        {
            cb.lBufferSize = 0;
            return E_OUTOFMEMORY;
        }

        // Copy the bitmap data into our global buffer
        memcpy(cb.pBuffer, pBuffer, lBufferSize);

        // Post a message to our application, telling it to come back
        // and write the saved data to a bitmap file on the user's disk.
        PostMessage(g_hwnd, WM_CAPTURE_BITMAP, 0, 0L);
        return 0;
    }

    // This function will be called whenever a captured still needs to be
    // displayed in the preview window.  It is called initially within
    // CopyBitmap() to display the captured still, but it is also called
    // whenever the main dialog needs to repaint and when we transition
    // from video capture mode back into still capture mode.
    //
    BOOL DisplayCapturedBits(BYTE *pBuffer, BITMAPINFOHEADER *pbih)
    {
        // If we haven't yet snapped a still, return
        if (!bFileWritten || !pOwner || !pBuffer)
            return FALSE;


////////////////////////////////////////////////
///
        if (!MybFileWritten)
            return FALSE;

		MybFileWritten = FALSE;
///
////////////////////////////////////////////////        
		
		// put bits into the preview window with StretchDIBits
        //
        HWND hwndStill = NULL;
        pOwner->GetDlgItem( IDC_STILL, &hwndStill );

        RECT rc;
        ::GetWindowRect( hwndStill, &rc );
        long lStillWidth = rc.right - rc.left;
        long lStillHeight = rc.bottom - rc.top;
        
        HDC hdcStill = GetDC( hwndStill );
        PAINTSTRUCT ps;
        BeginPaint(hwndStill, &ps);

        SetStretchBltMode(hdcStill, COLORONCOLOR);
        StretchDIBits( 
                    hdcStill, 0, 0, 
                    lStillWidth, lStillHeight, 
                    0, 0, lWidth, lHeight, 
                    pBuffer, 
                    (BITMAPINFO*) pbih, 
                    DIB_RGB_COLORS, 
                    SRCCOPY );

        EndPaint(hwndStill, &ps);
        ReleaseDC( hwndStill, hdcStill );  
		

        return TRUE;
    }



	


	// This is the implementation function that writes the captured video
    // data onto a bitmap on the user's disk.
    //
    BOOL CopyBitmap( double dblSampleTime, BYTE * pBuffer, long lBufferSize )
    {
		if( !g_bOneShot || !pBuffer )
            return 0;

        // we only take one at a time
        //
        g_bOneShot = FALSE;
		//g_bOneShot = 2;

        // figure out where to capture to
        //
        TCHAR m_ShortName[MAX_PATH];

        wsprintf( m_szSnappedName, TEXT("%sStillCap%4.4ld.bmp\0"), 
                  m_szCapDir, pOwner->m_nCapTimes );

        wsprintf( m_ShortName, TEXT("StillCap%4.4ld.bmp\0"), 
                  pOwner->m_nCapTimes );

        // increment bitmap number if user requested it
        // otherwise, we'll reuse the filename next time
//        if( pOwner->IsDlgButtonChecked( IDC_AUTOBUMP ) )
//            pOwner->m_nCapTimes++;


        // write out the file header
        //
        BITMAPFILEHEADER bfh;
        memset( &bfh, 0, sizeof( bfh ) );
        bfh.bfType = 'MB';
        bfh.bfSize = sizeof( bfh ) + lBufferSize + sizeof( BITMAPINFOHEADER );
        bfh.bfOffBits = sizeof( BITMAPINFOHEADER ) + sizeof( BITMAPFILEHEADER );


        // and the bitmap format
        //
        BITMAPINFOHEADER bih;
        memset( &bih, 0, sizeof( bih ) );
        bih.biSize = sizeof( bih );
        bih.biWidth = lWidth;
        bih.biHeight = lHeight;
        bih.biPlanes = 1;
        bih.biBitCount = 24;
		//bih.biBitCount = 16;

		//int MAX_RES_X, MAX_RES_Y;

		MAX_RES_X = lWidth;
		MAX_RES_Y = lHeight;

//========================= Image processing ===========================================
//							
//
//
//======================================================================================


/***********************************************************/
		//Jair 18 de Agosto 2006 para agregar distorsion
		if(distor)//para la distorsión de la imágen
		{
//			disto_a=-0.1;
//			disto_b=0.25;
//			disto_c=0.0;
			if(bandist)coord=process_imagedvj1(bih.biWidth,bih.biHeight,centrox,centroy, pBuffer, disto_a, disto_b, disto_c);
			pBuffer=imag_distj(bih.biWidth,bih.biHeight, pBuffer,coord);
			bandist=FALSE;
		}
		else if(bandist==FALSE)
            {
                libera_matrizxyi(coord,bih.biHeight);		
                bandist=TRUE;
			}

/***********************************************************/
	#ifdef comment
		RGB_BUFFER rgbBuffer;

		rgbBuffer.r = new unsigned char[bih.biWidth*bih.biHeight];
		rgbBuffer.g = new unsigned char [bih.biWidth*bih.biHeight];
		rgbBuffer.b = new unsigned char [bih.biWidth*bih.biHeight];

		Linear2Matrix( pBuffer, &rgbBuffer, bih.biWidth, bih.biHeight );

/*
		int s, addr, contx, conty;
		for( conty = 0; conty < bih.biHeight; conty++ ){
			for( contx = 0; contx < bih.biWidth; contx++ ){
				*(rgbBuffer.b+contx) = *pBuffer++;
				*(rgbBuffer.g+contx) = *pBuffer++;
				*(rgbBuffer.r+contx) = *pBuffer++;
			}
		}
*/
		delete rgbBuffer.b;
		delete rgbBuffer.g;
		delete rgbBuffer.r;
	#endif


// Color array is the form BGR

		int fieldleft = 206-FieldCoords.corner0.x;
		int fieldright = bih.biHeight-271;
		
//		rgbBright( pBuffer, bright, countmax );
//		rgbContrast( pBuffer, contrast, countmax );
//		rgbSaturation( pBuffer, saturation, countmax );
		
	#ifdef comment
		RGB frgb;
		FieldCoords.corner0.x = 20;
		FieldCoords.corner0.y = 20;
		RGB_TEST(frgb);
		RLEDrawBlob( FieldCoords.corner0, pBuffer, frgb );
	#endif

		if( CalibrationCancha  ){
			RGB frgb;
			RGB_TEST(frgb);

			RLEDrawBlob( FieldCoordsDown.corner0, pBuffer, frgb );
			RLEDrawBlob( FieldCoordsDown.corner0, pBuffer, frgb );	
			RLEDrawBlob( FieldCoordsDown.corner1, pBuffer, frgb );
			RLEDrawBlob( FieldCoordsDown.corner1, pBuffer, frgb );	
		}

		LinkListBLOB listOrange, listBlue, listYellow, listGreen, listPink, listCyan, listOthers, listPumas, listAmerica, *pLb, *pLo;
		
		if( m_process )
		{
			initBlob(&listOrange);
			initBlob(&listBlue);
			initBlob(&listYellow);
			initBlob(&listGreen);
			initBlob(&listPink);
			initBlob(&listCyan);
			initBlob(&listOthers);

			detectColors(pBuffer,&listOrange,&listBlue,&listYellow,&listGreen,&listPink,&listCyan, bih.biWidth, bih.biHeight, FieldCoordsDown.corner0, FieldCoordsDown.corner1 );
			
			firstNNodes(&listOrange,100,MinBlobArea);
			firstNNodes(&listBlue,100,MinBlobArea);
			firstNNodes(&listYellow,100,MinBlobArea);
			firstNNodes(&listGreen,100,MinBlobArea);
			firstNNodes(&listPink,100,MinBlobArea);
			firstNNodes(&listCyan,100,MinBlobArea);

			listCatBlob(&listOthers, &listGreen);
			listCatBlob(&listOthers, &listPink);
			listCatBlob(&listOthers, &listCyan);
		
/*==================================================================================*/
			GROUPOBJECTS win[5];
//			float dir;
			int value;
			ROBOT robot, myRobots[10],orobot, ball;
			CString str;
			int rcont;

			for(int iR=0; iR<10; iR++){
				myRobots[iR].team = -10;
				myRobots[iR].pos.x = -10;
				myRobots[iR].pos.y = -10;
				myRobots[iR].angle = -10;
				myRobots[iR].vel = -10;
				myRobots[iR].robot = -10;
				myRobots[iR].radius = -10;
			}

			pLb = &listBlue;
			pLo = &listOthers;

			if( TeamColor+1 == blue )
			{
				listPumas = listBlue;
				listAmerica = listYellow;
			}
			else
			{
				listPumas = listYellow;
				listAmerica = listBlue;
			}

			pLb = &listPumas;

			/*************************************************************************
			*					Extracts all info about our team
			*************************************************************************/

			float fx, fy;

			iR=0;
			if( GroupsObjects( pLb, pLo, win, MaxDist ) )
			{
				for( rcont = 0; rcont < 5; rcont++ )
				{
					if( win[rcont].cont ==  3 ) /* 'cont' is the number of patches found */
					{
						DetectsRobotOrientation( win[rcont], &robot );
						//DetectsRobotOrientation( win[rcont], &robot[rcont] );
						//DetectsRobotVelocity( win[rcont], &robot );
						robot.vel = 0;
						DecodesColorCode( win[rcont], &value );

						if( IdentifiesRobot( win[rcont], value, &robot ) )
						{	
							myRobots[iR] = robot;
							iR++;
							/*if( SocketAlreadyOpen )
							{
								FillsRobotInfoString( SckBuf, robot, TeamColor+1 );
//								WriteSocket( sd, SckBuf );
								mysox.UDPSocketsWrite( SckBuf );
							}*/
						}
					}
				}
			}

			/*************************************************************************
			*					Extracts the x,y pos of the other team
			*************************************************************************/
			rcont = 0;
			while( listAmerica != NULL && rcont < 5 )
			{
				orobot.pos = listAmerica->info.ctr;

				orobot.angle = 0;
				orobot.robot = rcont;
				orobot.vel = 0;
				orobot.team = listAmerica->info.color;
				myRobots[iR]=orobot;
				iR++;

				/*if( SocketAlreadyOpen )
				{
					FillsRobotInfoString( SckBuf, orobot, TeamColor+1 );
//					WriteSocket( sd, SckBuf );
					mysox.UDPSocketsWrite( SckBuf );
				}*/

				rcont++;
				listAmerica = listAmerica->next;
			}

			ball.pos.x = -10;
			ball.pos.y = -10;
			/*************************************************************************
			*					Extracts the x,y pos of the ball
			*************************************************************************/
			char mensaje[512];
			for( int c=0; c<512;c++)
				mensaje[c]= NULL;

			if( listOrange != NULL )
			{
				ball.pos = listOrange->info.ctr;
				ball.vel = 0;


				fx = (float)(FieldLim.corner1.x-FieldLim.corner0.x);
				fx *= (ball.pos.x-FieldCoordsDown.corner0.x);
				fx /= (float)(FieldCoordsDown.corner1.x-FieldCoordsDown.corner0.x);

				fy = (float)(FieldLim.corner1.y-FieldLim.corner0.y);
				fy *= (ball.pos.y-FieldCoordsDown.corner0.y);
				fy /= (float)(FieldCoordsDown.corner1.y-FieldCoordsDown.corner0.y);

				ball.realPos.x = fx*10.0;
				ball.realPos.y = fy*10.0;


//				if( SocketAlreadyOpen )
//				{
					FillsBallInfoString( SckBuf, ball );
//					WriteSocket( sd, SckBuf );
//					mysox.UDPSocketsWrite( SckBuf );
					sprintf(mensaje,"%s, ",SckBuf);		
			
//				}
			}

/***********************************/
/***********************************/
			LinkListBLOB aux;
			BLB *auxB;
			duplicateBlob(&aux,&listBlue);
			while(!isEmptyBlob(&aux)){
				auxB=queryTopBlob(&aux);
				if ( auxB->tag != 0 )
					RLEDrawBlob(auxB->p0.x,auxB->p0.y,auxB->p1.x,auxB->p1.y,pBuffer,blue);
				nextBlob(&aux);
			}
			
			duplicateBlob(&aux,&listOrange);
			while(!isEmptyBlob(&aux)){
				auxB=queryTopBlob(&aux);
				if ( auxB->tag != 0 )
					RLEDrawBlob(auxB->p0.x,auxB->p0.y,auxB->p1.x,auxB->p1.y,pBuffer,orange);
				nextBlob(&aux);
			}

			duplicateBlob(&aux,&listYellow);
			while(!isEmptyBlob(&aux)){
				auxB=queryTopBlob(&aux);
				if ( auxB->tag != 0 )
					RLEDrawBlob(auxB->p0.x,auxB->p0.y,auxB->p1.x,auxB->p1.y,pBuffer,yellow);
				nextBlob(&aux);
			}
			
			duplicateBlob(&aux,&listOthers);
			while(!isEmptyBlob(&aux)){
				auxB=queryTopBlob(&aux);
				if ( auxB->tag != 0 )
					RLEDrawBlob(auxB->p0.x,auxB->p0.y,auxB->p1.x,auxB->p1.y,pBuffer,auxB->color);
				nextBlob(&aux);
			}
/***********************************/
/***********************************/				

			destroyBlob(&listBlue);
			destroyBlob(&listYellow);
			destroyBlob(&listOrange);
			destroyBlob(&listOthers);
/***********Jair aqui va lo de solo una coordenada*****************************************************/			
			iR = 0;
			int potRobot = -10;
			while (myRobots[iR].team != -10 && iR < 10)
			{
				
				fx = (float)(FieldLim.corner1.x-FieldLim.corner0.x);
				fx *= (myRobots[iR].pos.x-FieldCoordsDown.corner0.x);
				fx /= (float)(FieldCoordsDown.corner1.x-FieldCoordsDown.corner0.x);

				fy = (float)(FieldLim.corner1.y-FieldLim.corner0.y);
				fy *= (myRobots[iR].pos.y-FieldCoordsDown.corner0.y);
				fy /= (float)(FieldCoordsDown.corner1.y-FieldCoordsDown.corner0.y);
							
				myRobots[iR].realPos.x = fx*10.0;
				myRobots[iR].realPos.y = fy*10.0;
				
				if ( myRobots[iR].team == TeamColor + 1 ){
					drawCircle( myRobots[iR].pos.x, myRobots[iR].pos.y, /*sqrt((float)win[0].elem->info.area)*2.0*/MaxDist, pBuffer, 10);
					drawLine( myRobots[iR].pos.x, myRobots[iR].pos.y, /*sqrt((float)win[0].elem->info.area)*2.0*/MaxDist, myRobots[iR].angle, pBuffer, blue);
				}
				else drawCircle( myRobots[iR].pos.x, myRobots[iR].pos.y, /*sqrt((float)win[0].elem->info.area)*2.0*/MaxDist, pBuffer, 11);
 				myRobots[iR].angle = myRobots[iR].angle * 3.14159 / 180;

				
				//if( SocketAlreadyOpen )
				//{
					FillsRobotInfoString( SckBuf, myRobots[iR], myRobots[iR].team);//TeamColor+1 );
					//WriteSocket( sd, SckBuf );
				//	mysox.UDPSocketsWrite( SckBuf );
					strcat(mensaje,SckBuf);
					strcat(mensaje,", ");
				//}

				iR++;
			}

#ifdef comment
			if ( potRobot!=-10 && ball.pos.x!=-10) 
			{
				float mvDist, mvAngle;
				potentialFields(myRobots, potRobot, ball.realPos, &mvDist, &mvAngle);

				//if( SocketAlreadyOpen )
				//{
					sprintf( SckBuf, "INFO mv %f %fEND%c", 2.0/*mvDist*/, mvAngle, '\0');
					//sprintf( SckBuf, "INFOmv %.2f %.2f -> %.2f\tEN", /*mvDist*/2.0, mvAngle, mvAngle*((float)180/(float)3.14159));
					//WriteSocket( sd, SckBuf );
					//mysox.UDPSocketsWrite( SckBuf );
					strcat(mensaje,SckBuf);
					strcat(mensaje,", ");
					
					
					float ddd = sqrt( (myRobots[potRobot].pos.x - ball.pos.x)*(myRobots[potRobot].pos.x - ball.pos.x) + (myRobots[potRobot].pos.y - ball.pos.y)*(myRobots[potRobot].pos.y - ball.pos.y) );
					if ( ddd <= 60 )
						sprintf( SckBuf, "INFOshootonEND");
					else 
						sprintf( SckBuf, "INFOshootoffEND");
					//WriteSocket( sd, SckBuf );
				//	mysox.UDPSocketsWrite( SckBuf );
					strcat(mensaje,SckBuf);
					strcat(mensaje,", ");
				//}
			}
#endif

			//if( SocketAlreadyOpen )
			//{
				sprintf( SckBuf, "FRAMEEND");
			//	mysox.UDPSocketsWrite( SckBuf );
				strcat(mensaje,SckBuf);
			//}
			if( SocketAlreadyOpen )
			{
				mysox.UDPSocketsWrite( mensaje );
			}

		}


		
		//bFileWritten = FALSE;

        // Save bitmap header for later use when repainting the window
        //memcpy(&(cb.bih), &bih, sizeof(bih)); 

        // Display the bitmap bits on the dialog's preview window
/**********Jair le quite comentarioa a if*****************/        
		if( VideoP > 0 )DisplayCapturedBits(pBuffer, &bih);
		


        // Save bitmap header for later use when repainting the window
        //memcpy(&(cb.bih), &bih, sizeof(bih)); 
		
		bFileWritten = TRUE;
		MybFileWritten = TRUE;

        // show where it captured
        //
//        pOwner->SetDlgItemText( IDC_SNAPNAME, m_ShortName );

        // Enable the 'View Still' button
        HWND hwndButton = NULL;
//        pOwner->GetDlgItem( IDC_BUTTON_VIEWSTILL, &hwndButton );
        ::EnableWindow(hwndButton, TRUE);


		return 0;
    }


    void PlaySnapSound(void)
    {
        TCHAR szSound[MAX_PATH];
        const TCHAR szFileToPlay[] = { TEXT("\\media\\click.wav\0") };
        int nSpaceAllowed = MAX_PATH - NUMELMS(szFileToPlay);
        int nSpaceUsed=0;
        
        nSpaceUsed = GetWindowsDirectory(szSound, nSpaceAllowed);
        if (nSpaceUsed && nSpaceUsed <= nSpaceAllowed)
        {
            _tcscat( szSound, szFileToPlay );
            sndPlaySound(szSound, SND_ASYNC);
        }           
    }
};


//////////////////////////////////////////////////////////////////
//
// This semi-COM object will receive sample callbacks for us
//
//////////////////////////////////////////////////////////////////

CSampleGrabberCB mCB;


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
        // No message handlers
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CStillCapDlg dialog

CStillCapDlg::CStillCapDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CStillCapDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CStillCapDlg)
	m_EdBright = _T("");
	m_EdContrast = _T("");
	m_EdSat = _T("");
	m_EdBG = _T("");
	m_EdBR = _T("");
	m_EdOB = _T("");
	m_EdOG = _T("");
	m_EdOR = _T("");
	m_EdYB = _T("");
	m_EdYG = _T("");
	m_EdYR = _T("");
	m_MinBlobButt = _T("");
	m_rad = -1;
	m_EdXBlob = _T("");
	m_EdYBlob = _T("");
	m_IPPort = _T("");
	m_TeamColor = -1;
	m_Color = -1;
	m_CanchaX0 = 0.0f;
	m_CanchaX1 = 2.0f;
	m_CanchaY0 = 0.0f;
	m_CanchaY1 = 2.0f;
	m_DistMaxBl = 0;
	m_VideoP = FALSE;
	m_centro = FALSE;
	m_distorsion = FALSE;
	//}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStillCapDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CStillCapDlg)
	DDX_Control(pDX, IDC_LISTX, m_liscentrox);
	DDX_Control(pDX, IDC_LIST_DISTC, m_lisdistc);
	DDX_Control(pDX, IDC_LIST_DISTB, m_lisdistb);
	DDX_Control(pDX, IDC_LIST_DISTA, m_lisdista);
	DDX_Control(pDX, IDC_BUTTON_DISTOR, m_Distor);
	DDX_Control(pDX, IDC_BUTTON_PROCESS, m_ProcessButon);
	DDX_Control(pDX, IDC_SLIDER_DISTMAXBL, m_DistMaxBlSl);
	DDX_Control(pDX, IDC_BUTTON_CALIBCANCHA, m_CalibCancha);
	DDX_Control(pDX, IDC_BUTTON_CALCOLOR, m_CalColor);
	DDX_Control(pDX, IDC_STATIC_SP, m_Static_SP);
	DDX_Control(pDX, IDC_STATIC_IP, m_Static_IP);
	DDX_Control(pDX, IDC_STATIC_HP, m_Static_HP);
	DDX_Control(pDX, IDC_SLIDER_SP, m_SliderSP);
	DDX_Control(pDX, IDC_SLIDER_IP, m_SliderIP);
	DDX_Control(pDX, IDC_SLIDER_HP, m_SliderHP);
	DDX_Control(pDX, IDC_SLIDER_OI, m_SliderOI);
	DDX_Control(pDX, IDC_STATIC_OS, m_Static_OS);
	DDX_Control(pDX, IDC_SLIDER_OS, m_SliderOS);
	DDX_Control(pDX, IDC_STATIC_OI, m_Static_OI);
	DDX_Control(pDX, IDC_SLIDER_OH, m_SliderOH);
	DDX_Control(pDX, IDC_STATIC_OH, m_Static_OH);
	DDX_Control(pDX, IDC_BUTTON_COMM, m_CommButton);
	DDX_Control(pDX, IDC_IPADDRESS_DEST, m_IPRemote);
	DDX_Control(pDX, IDC_IPADDRESS_IP_LOCAL, m_IPLocal);
	DDX_Control(pDX, IDC_LIST_REFRES, m_FrameRate);
	DDX_Control(pDX, IDC_SLIDER_YBLOB, m_YBlob);
	DDX_Control(pDX, IDC_SLIDER_XBLOB, m_XBlob);
	DDX_Control(pDX, IDC_SLIDER_BLOB, m_MinBlob);
	DDX_Control(pDX, IDC_SLIDER_SAT, m_Saturation);
	DDX_Control(pDX, IDC_SLIDER1_CONTRAST, m_Contrast);
	DDX_Control(pDX, IDC_SLIDER_BRIGHT, m_Bright);
    DDX_Control(pDX, IDC_STATUS, m_StrStatus);
    DDX_Control(pDX, IDC_STILL, m_StillScreen);
    DDX_Control(pDX, IDC_PREVIEW, m_PreviewScreen);
	DDX_Text(pDX, IDC_EDIT_BRIGHT, m_EdBright);
	DDX_Text(pDX, IDC_EDIT_CONTRAST, m_EdContrast);
	DDX_Text(pDX, IDC_EDIT_SAT, m_EdSat);
	DDX_Text(pDX, IDC_EDIT_BLOB, m_MinBlobButt);
	DDX_Radio(pDX, IDC_RADIO_AREA, m_rad);
	DDX_Text(pDX, IDC_EDIT_XBLOB, m_EdXBlob);
	DDX_Text(pDX, IDC_EDIT_YBLOB, m_EdYBlob);
	DDX_Text(pDX, IDC_EDIT_IP_PORT, m_IPPort);
	DDX_Radio(pDX, IDC_RADIO_TEAMCOLOR, m_TeamColor);
	DDX_Radio(pDX, IDC_RADIO_COLOR, m_Color);
	DDX_Text(pDX, IDC_EDIT_CANCHAX1, m_CanchaX0);
	DDV_MinMaxFloat(pDX, m_CanchaX0, 0.f, 100.f);
	DDX_Text(pDX, IDC_EDIT_CANCHAX2, m_CanchaX1);
	DDV_MinMaxFloat(pDX, m_CanchaX1, 0.f, 100.f);
	DDX_Text(pDX, IDC_EDIT_CANCHAY1, m_CanchaY0);
	DDV_MinMaxFloat(pDX, m_CanchaY0, 0.f, 100.f);
	DDX_Text(pDX, IDC_EDIT_CANCHAY2, m_CanchaY1);
	DDV_MinMaxFloat(pDX, m_CanchaY1, 0.f, 100.f);
	DDX_Text(pDX, IDC_EDIT_DISTMAXBL, m_DistMaxBl);
	DDV_MinMaxInt(pDX, m_DistMaxBl, 0, 1000);
	DDX_Check(pDX, IDC_CHECK_VIDEOP, m_VideoP);
	DDX_Check(pDX, IDC_CHECK_CENTRO, m_centro);
	DDX_Check(pDX, IDC_CHECK_DISTOR, m_distorsion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStillCapDlg, CDialog)
    //{{AFX_MSG_MAP(CStillCapDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_SNAP, OnSnap)
    ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_PROCESS, OnButtonProcess)
	ON_BN_CLICKED(IDC_BUTTON_RESET_PREPROC, OnButtonResetPreproc)
	ON_BN_CLICKED(IDC_BUTTON_REGI_RESET, OnButtonRegiReset)
	ON_BN_CLICKED(IDC_RADIO_DIM, OnRadioDim)
	ON_LBN_SELCHANGE(IDC_LIST_REFRES, OnSelchangeListRefres)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_IP_LOCAL, OnFieldchangedIpaddressIpLocal)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_DEST, OnFieldchangedIpaddressDest)
	ON_EN_KILLFOCUS(IDC_EDIT_IP_PORT, OnKillfocusEditIpPort)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_COMM, OnButtonComm)
	ON_BN_CLICKED(IDC_RADIO_TEAMCOLOR, OnRadioTeamcolor)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, OnButtonColor)
	ON_BN_CLICKED(IDC_BUTTON_VERCOLOR, OnButtonVercolor)
	ON_BN_CLICKED(IDC_BUTTON_COLORPICKER, OnButtonColorpicker)
	ON_BN_CLICKED(IDC_BUTTON_CALCOLOR, OnButtonCalcolor)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_HISTOGRAM, OnButtonHistogram)
	ON_BN_CLICKED(IDC_BUTTON_CALIBCANCHA, OnButtonCalibcancha)
	ON_BN_CLICKED(IDC_BUTTON_CANCHADAT, OnButtonCanchadat)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_COMM_BURRON, OnCommBurron)
	ON_BN_CLICKED(IDC_CHECK_VIDEOP, OnCheckVideop)
	ON_BN_CLICKED(IDC_BUTTON_DISTOR, OnButtonDistor)
	ON_BN_CLICKED(IDC_CHECK_CENTRO, OnCheckCentro)
	ON_BN_CLICKED(IDC_CHECK_DISTOR, OnCheckDistor)
	ON_LBN_SELCHANGE(IDC_LIST_DISTA, OnSelchangeListDista)
	ON_LBN_SELCHANGE(IDC_LIST_DISTB, OnSelchangeListDistb)
	ON_LBN_SELCHANGE(IDC_LISTX, OnSelchangeListx)
	ON_LBN_SELCHANGE(IDC_LIST_DISTC, OnSelchangeListDistc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStillCapDlg message handlers

void CStillCapDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CStillCapDlg::OnPaint() 
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();

//////////////////////////////////////////////////////////
///
		// Update the bitmap preview window, if we have
        // already captured bitmap data
		if( MybFileWritten )
			mCB.DisplayCapturedBits(cb.pBuffer, &(cb.bih));
///
//////////////////////////////////////////////////////////		
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CStillCapDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

BOOL CStillCapDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon
    
    
	
	
	//
    // StillCap-specific initialization
    //
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    // Default to this capture directory
    //
//    SetDlgItemText( IDC_CAPDIR, TEXT("c:\\\0") );

    // Default to capturing stills
    //
//    CheckDlgButton( IDC_CAPSTILLS, 1 );
    m_bCapStills = true;
    m_nCapState = 0;
    m_nCapTimes = 0;
    g_hwnd = GetSafeHwnd();

    
	
	// start up the still image capture graph
    //
    HRESULT hr = InitStillGraph( );
    // Don't display an error message box on failure, because InitStillGraph()
    // will already have displayed a specific error message

    // Modify the window style of the capture and still windows
    // to prevent excessive repainting
    m_PreviewScreen.ModifyStyle(0, WS_CLIPCHILDREN);
    m_StillScreen.ModifyStyle(0, WS_CLIPCHILDREN);

	
	/////////////////////////////////////////////////////////////////////////
	//						User initialization
	//
	//
	//
	/////////////////////////////////////////////////////////////////////////

	m_Bright.SetRange(-200,200,TRUE);
	m_Bright.SetTicFreq(20);

	m_Contrast.SetRange(-100,100,TRUE);
	m_Contrast.SetTicFreq(10);
	//m_Bright.SetPageSize(10);

	m_Saturation.SetRange(-100,100,TRUE);
	m_Saturation.SetTicFreq(10);
	
	m_Saturation.SetPos(0);
	m_Contrast.SetPos(1);
	m_Bright.SetPos(0);

	////////////////////////////////
	//		HSI sliders
	////////////////////////////////

	m_SliderOH.SetRange(0,360,TRUE);
	m_SliderOH.SetPos(hORANGE);

	m_SliderOS.SetRange(0,255,TRUE);
	m_SliderOS.SetPos(sORANGE);	

	m_SliderOI.SetRange(0,255,TRUE);
	m_SliderOI.SetPos(iORANGE);	


	m_SliderHP.SetRange(0,180, TRUE);
	m_SliderHP.SetPos(5); /* 18 */

	m_SliderSP.SetRange(0,127, TRUE);
	m_SliderSP.SetPos(10);

	m_SliderIP.SetRange(0,127, TRUE);
	m_SliderIP.SetPos(10);

	for( int z = orange; z <= cyan; z++ )
	{
		hsi[z].h.win = m_SliderHP.GetPos();
		hsi[z].h.max = hue[z] + hsi[z].h.win;
		if( hsi[z].h.max > 360 ) 
			hsi[z].h.max -= 360;

		hsi[z].h.min = hue[z] - hsi[z].h.win;
		if( hsi[z].h.min < 0 ) 
			hsi[z].h.min += 360;

		
		hsi[z].s.win = m_SliderSP.GetPos();
		hsi[z].s.max = sat[z] + hsi[z].s.win;
		if( hsi[z].s.max > 255 ) 
			hsi[z].s.max = 255;

		hsi[z].s.min = sat[z] - hsi[z].s.win;
		if( hsi[z].s.min < 0 ) 
			hsi[z].s.min = 0;
		
		
		hsi[z].i.win = m_SliderIP.GetPos();
		hsi[z].i.max = ins[z] + hsi[z].i.win;
		if( hsi[z].i.max > 255 ) 
			hsi[z].i.max = 255;

		hsi[z].i.min = ins[z] - hsi[z].i.win;
		if( hsi[z].i.min < 0 ) 
			hsi[z].i.min = 0;
	}

	
	//.SetRange(0, 100, TRUE ); m_MinRLE.SetPos( 2 );
	m_MinBlob.SetRange(0, 500, TRUE ); m_MinBlob.SetPos( MAXREDAREA );
	MinBlobArea = 10;
	MinRLE = 2;

	m_XBlob.SetRange(0, 100, TRUE ); m_XBlob.SetPos( 2 );
	m_YBlob.SetRange(0, 100, TRUE ); m_YBlob.SetPos( 2 );
	MinBlobX = 2;
	MinBlobY = 2;

	m_DistMaxBlSl.SetRange( 0, 100, TRUE ); 
	m_DistMaxBlSl.SetPos( 10 );
	
	MaxDist = m_DistMaxBlSl.GetPos();
	m_DistMaxBl = MaxDist;
	


	
	char cad[80];	
	sprintf( cad, "%d", m_Bright.GetPos() ); m_EdBright = cad;
	sprintf( cad, "%d", m_Contrast.GetPos() ); m_EdContrast = cad;
	sprintf( cad, "%d", m_Saturation.GetPos() ); m_EdSat = cad;

	sprintf( cad, "%d", m_SliderOH.GetPos() ); m_Static_OH.SetWindowText( cad );
	sprintf( cad, "%d", m_SliderOS.GetPos() ); m_Static_OS.SetWindowText( cad );
	sprintf( cad, "%d", m_SliderOI.GetPos() ); m_Static_OI.SetWindowText( cad );
	
	sprintf( cad, "%d", m_SliderHP.GetPos() ); m_Static_HP.SetWindowText( cad );
	sprintf( cad, "%d", m_SliderSP.GetPos() ); m_Static_SP.SetWindowText( cad );
	sprintf( cad, "%d", m_SliderIP.GetPos() ); m_Static_IP.SetWindowText( cad );




	sprintf( cad, "%d", m_MinBlob.GetPos() ); m_MinBlobButt = cad;
//	sprintf( cad, "%d", m_MinRLE.GetPos() ); m_MinRLEButt = cad;

	sprintf( cad, "%d", MinBlobX = m_XBlob.GetPos() ); 
	m_EdXBlob = cad;
	sprintf( cad, "%d", MinBlobY = m_YBlob.GetPos() ); 
	m_EdYBlob = cad;
	
	
	m_FrameRate.AddString( "25" );
	m_FrameRate.AddString( "20" );
	m_FrameRate.AddString( "15" );
	m_FrameRate.AddString( "10" );
	m_FrameRate.AddString( "5" );
	m_FrameRate.AddString( "2" );
	m_FrameRate.AddString( "1" );
	m_FrameRate.AddString( "0.5" );
	m_FrameRate.SetCurSel( 5 );
	FrameRate = 500;		// ms

/*****Jair****************************/
	m_lisdista.AddString("0.99");//0
	m_lisdista.AddString("0.98");//1
	m_lisdista.AddString("0.97");//2
	m_lisdista.AddString("0.96");//3
	m_lisdista.AddString("0.95");//4
	m_lisdista.AddString("0.94");//5
	m_lisdista.AddString("0.93");//6
	m_lisdista.AddString("0.92");//7
	m_lisdista.AddString("0.91");//8
	m_lisdista.AddString("0.90");//9
	m_lisdista.AddString("0.89");//10
	m_lisdista.AddString("0.88");
	m_lisdista.AddString("0.87");
	m_lisdista.AddString("0.86");
	m_lisdista.AddString("0.85");
	m_lisdista.AddString("0.84");
	m_lisdista.AddString("0.83");
	m_lisdista.AddString("0.82");
	m_lisdista.AddString("0.81");
	m_lisdista.AddString("0.80");
	m_lisdista.AddString("0.79");//20
	m_lisdista.AddString("0.78");
	m_lisdista.AddString("0.77");
	m_lisdista.AddString("0.76");
	m_lisdista.AddString("0.75");
	m_lisdista.AddString("0.74");
	m_lisdista.AddString("0.73");
	m_lisdista.AddString("0.72");
	m_lisdista.AddString("0.71");
	m_lisdista.AddString("0.70");
	m_lisdista.AddString("0.69");//30
	m_lisdista.AddString("0.68");
	m_lisdista.AddString("0.67");
	m_lisdista.AddString("0.66");
	m_lisdista.AddString("0.65");
	m_lisdista.AddString("0.64");
	m_lisdista.AddString("0.63");
	m_lisdista.AddString("0.62");
	m_lisdista.AddString("0.61");
	m_lisdista.AddString("0.60");
	m_lisdista.AddString("0.59");//40
	m_lisdista.AddString("0.58");
	m_lisdista.AddString("0.57");
	m_lisdista.AddString("0.56");
	m_lisdista.AddString("0.55");
	m_lisdista.AddString("0.54");
	m_lisdista.AddString("0.53");
	m_lisdista.AddString("0.52");
	m_lisdista.AddString("0.51");
	m_lisdista.AddString("0.50");
	m_lisdista.AddString("0.49");//50
	m_lisdista.AddString("0.48");
	m_lisdista.AddString("0.47");
	m_lisdista.AddString("0.46");
	m_lisdista.AddString("0.45");
	m_lisdista.AddString("0.44");
	m_lisdista.AddString("0.43");
	m_lisdista.AddString("0.42");
	m_lisdista.AddString("0.41");
	m_lisdista.AddString("0.40");
	m_lisdista.AddString("0.39");//60
	m_lisdista.AddString("0.38");
	m_lisdista.AddString("0.37");
	m_lisdista.AddString("0.36");
	m_lisdista.AddString("0.35");
	m_lisdista.AddString("0.34");
	m_lisdista.AddString("0.33");
	m_lisdista.AddString("0.32");
	m_lisdista.AddString("0.31");
	m_lisdista.AddString("0.30");
	m_lisdista.AddString("0.29");//70
	m_lisdista.AddString("0.28");
	m_lisdista.AddString("0.27");
	m_lisdista.AddString("0.26");
	m_lisdista.AddString("0.25");
	m_lisdista.AddString("0.24");
	m_lisdista.AddString("0.23");
	m_lisdista.AddString("0.22");
	m_lisdista.AddString("0.21");
	m_lisdista.AddString("0.20");
	m_lisdista.AddString("0.19");//80
	m_lisdista.AddString("0.18");
	m_lisdista.AddString("0.17");
	m_lisdista.AddString("0.16");
	m_lisdista.AddString("0.15");
	m_lisdista.AddString("0.14");
	m_lisdista.AddString("0.13");
	m_lisdista.AddString("0.12");
	m_lisdista.AddString("0.11");
	m_lisdista.AddString("0.10");
	m_lisdista.AddString("0.09");//90
	m_lisdista.AddString("0.08");
	m_lisdista.AddString("0.07");
	m_lisdista.AddString("0.06");
	m_lisdista.AddString("0.05");
	m_lisdista.AddString("0.04");
	m_lisdista.AddString("0.03");
	m_lisdista.AddString("0.02");
	m_lisdista.AddString("0.01");
	m_lisdista.AddString("0.00");
	m_lisdista.AddString("-0.01");//100
	m_lisdista.AddString("-0.02");
	m_lisdista.AddString("-0.03");
	m_lisdista.AddString("-0.04");
	m_lisdista.AddString("-0.05");
	m_lisdista.AddString("-0.06");
	m_lisdista.AddString("-0.07");
	m_lisdista.AddString("-0.08");
	m_lisdista.AddString("-0.09");
	m_lisdista.AddString("-0.10");
	m_lisdista.AddString("-0.11");//110
	m_lisdista.AddString("-0.12");
	m_lisdista.AddString("-0.13");
	m_lisdista.AddString("-0.14");
	m_lisdista.AddString("-0.15");
	m_lisdista.AddString("-0.16");
	m_lisdista.AddString("-0.17");
	m_lisdista.AddString("-0.18");
	m_lisdista.AddString("-0.19");
	m_lisdista.AddString("-0.20");
	m_lisdista.AddString("-0.21");//120
	m_lisdista.AddString("-0.22");
	m_lisdista.AddString("-0.23");
	m_lisdista.AddString("-0.24");
	m_lisdista.AddString("-0.25");
	m_lisdista.AddString("-0.26");
	m_lisdista.AddString("-0.27");
	m_lisdista.AddString("-0.28");
	m_lisdista.AddString("-0.29");
	m_lisdista.AddString("-0.30");
	m_lisdista.AddString("-0.31");//130
	m_lisdista.AddString("-0.32");
	m_lisdista.AddString("-0.33");
	m_lisdista.AddString("-0.34");
	m_lisdista.AddString("-0.35");
	m_lisdista.AddString("-0.36");
	m_lisdista.AddString("-0.37");
	m_lisdista.AddString("-0.38");
	m_lisdista.AddString("-0.39");
	m_lisdista.AddString("-0.40");
	m_lisdista.AddString("-0.41");//140
	m_lisdista.AddString("-0.42");
	m_lisdista.AddString("-0.43");
	m_lisdista.AddString("-0.44");
	m_lisdista.AddString("-0.45");
	m_lisdista.AddString("-0.46");
	m_lisdista.AddString("-0.47");
	m_lisdista.AddString("-0.48");
	m_lisdista.AddString("-0.49");
	m_lisdista.AddString("-0.50");
	m_lisdista.AddString("-0.51");//150
	m_lisdista.AddString("-0.52");
	m_lisdista.AddString("-0.53");
	m_lisdista.AddString("-0.54");
	m_lisdista.AddString("-0.55");
	m_lisdista.AddString("-0.56");
	m_lisdista.AddString("-0.57");
	m_lisdista.AddString("-0.58");
	m_lisdista.AddString("-0.59");
	m_lisdista.AddString("-0.60");
	m_lisdista.AddString("-0.61");//160
	m_lisdista.AddString("-0.62");
	m_lisdista.AddString("-0.63");
	m_lisdista.AddString("-0.64");
	m_lisdista.AddString("-0.65");
	m_lisdista.AddString("-0.66");
	m_lisdista.AddString("-0.67");
	m_lisdista.AddString("-0.68");
	m_lisdista.AddString("-0.69");
	m_lisdista.AddString("-0.70");
	m_lisdista.AddString("-0.71");//170
	m_lisdista.AddString("-0.72");
	m_lisdista.AddString("-0.73");
	m_lisdista.AddString("-0.74");
	m_lisdista.AddString("-0.75");
	m_lisdista.AddString("-0.76");
	m_lisdista.AddString("-0.77");
	m_lisdista.AddString("-0.78");
	m_lisdista.AddString("-0.79");
	m_lisdista.AddString("-0.80");
	m_lisdista.AddString("-0.81");//180
	m_lisdista.AddString("-0.82");
	m_lisdista.AddString("-0.83");
	m_lisdista.AddString("-0.84");
	m_lisdista.AddString("-0.85");
	m_lisdista.AddString("-0.86");
	m_lisdista.AddString("-0.87");
	m_lisdista.AddString("-0.88");
	m_lisdista.AddString("-0.89");
	m_lisdista.AddString("-0.90");
	m_lisdista.AddString("-0.91");//190
	m_lisdista.AddString("-0.92");
	m_lisdista.AddString("-0.93");
	m_lisdista.AddString("-0.94");
	m_lisdista.AddString("-0.95");
	m_lisdista.AddString("-0.96");
	m_lisdista.AddString("-0.97");
	m_lisdista.AddString("-0.98");
	m_lisdista.AddString("-0.99");//198
	m_lisdista.SetCurSel( 109 );

	m_lisdistb.AddString("0.99");//0
	m_lisdistb.AddString("0.98");//1
	m_lisdistb.AddString("0.97");//2
	m_lisdistb.AddString("0.96");//3
	m_lisdistb.AddString("0.95");//4
	m_lisdistb.AddString("0.94");//5
	m_lisdistb.AddString("0.93");//6
	m_lisdistb.AddString("0.92");//7
	m_lisdistb.AddString("0.91");//8
	m_lisdistb.AddString("0.90");//9
	m_lisdistb.AddString("0.89");//10
	m_lisdistb.AddString("0.88");
	m_lisdistb.AddString("0.87");
	m_lisdistb.AddString("0.86");
	m_lisdistb.AddString("0.85");
	m_lisdistb.AddString("0.84");
	m_lisdistb.AddString("0.83");
	m_lisdistb.AddString("0.82");
	m_lisdistb.AddString("0.81");
	m_lisdistb.AddString("0.80");
	m_lisdistb.AddString("0.79");//20
	m_lisdistb.AddString("0.78");
	m_lisdistb.AddString("0.77");
	m_lisdistb.AddString("0.76");
	m_lisdistb.AddString("0.75");
	m_lisdistb.AddString("0.74");
	m_lisdistb.AddString("0.73");
	m_lisdistb.AddString("0.72");
	m_lisdistb.AddString("0.71");
	m_lisdistb.AddString("0.70");
	m_lisdistb.AddString("0.69");//30
	m_lisdistb.AddString("0.68");
	m_lisdistb.AddString("0.67");
	m_lisdistb.AddString("0.66");
	m_lisdistb.AddString("0.65");
	m_lisdistb.AddString("0.64");
	m_lisdistb.AddString("0.63");
	m_lisdistb.AddString("0.62");
	m_lisdistb.AddString("0.61");
	m_lisdistb.AddString("0.60");
	m_lisdistb.AddString("0.59");//40
	m_lisdistb.AddString("0.58");
	m_lisdistb.AddString("0.57");
	m_lisdistb.AddString("0.56");
	m_lisdistb.AddString("0.55");
	m_lisdistb.AddString("0.54");
	m_lisdistb.AddString("0.53");
	m_lisdistb.AddString("0.52");
	m_lisdistb.AddString("0.51");
	m_lisdistb.AddString("0.50");
	m_lisdistb.AddString("0.49");//50
	m_lisdistb.AddString("0.48");
	m_lisdistb.AddString("0.47");
	m_lisdistb.AddString("0.46");
	m_lisdistb.AddString("0.45");
	m_lisdistb.AddString("0.44");
	m_lisdistb.AddString("0.43");
	m_lisdistb.AddString("0.42");
	m_lisdistb.AddString("0.41");
	m_lisdistb.AddString("0.40");
	m_lisdistb.AddString("0.39");//60
	m_lisdistb.AddString("0.38");
	m_lisdistb.AddString("0.37");
	m_lisdistb.AddString("0.36");
	m_lisdistb.AddString("0.35");
	m_lisdistb.AddString("0.34");
	m_lisdistb.AddString("0.33");
	m_lisdistb.AddString("0.32");
	m_lisdistb.AddString("0.31");
	m_lisdistb.AddString("0.30");
	m_lisdistb.AddString("0.29");//70
	m_lisdistb.AddString("0.28");
	m_lisdistb.AddString("0.27");
	m_lisdistb.AddString("0.26");
	m_lisdistb.AddString("0.25");
	m_lisdistb.AddString("0.24");
	m_lisdistb.AddString("0.23");
	m_lisdistb.AddString("0.22");
	m_lisdistb.AddString("0.21");
	m_lisdistb.AddString("0.20");
	m_lisdistb.AddString("0.19");//80
	m_lisdistb.AddString("0.18");
	m_lisdistb.AddString("0.17");
	m_lisdistb.AddString("0.16");
	m_lisdistb.AddString("0.15");
	m_lisdistb.AddString("0.14");
	m_lisdistb.AddString("0.13");
	m_lisdistb.AddString("0.12");
	m_lisdistb.AddString("0.11");
	m_lisdistb.AddString("0.10");
	m_lisdistb.AddString("0.09");//90
	m_lisdistb.AddString("0.08");
	m_lisdistb.AddString("0.07");
	m_lisdistb.AddString("0.06");
	m_lisdistb.AddString("0.05");
	m_lisdistb.AddString("0.04");
	m_lisdistb.AddString("0.03");
	m_lisdistb.AddString("0.02");
	m_lisdistb.AddString("0.01");
	m_lisdistb.AddString("0.00");
	m_lisdistb.AddString("-0.01");//100
	m_lisdistb.AddString("-0.02");
	m_lisdistb.AddString("-0.03");
	m_lisdistb.AddString("-0.04");
	m_lisdistb.AddString("-0.05");
	m_lisdistb.AddString("-0.06");
	m_lisdistb.AddString("-0.07");
	m_lisdistb.AddString("-0.08");
	m_lisdistb.AddString("-0.09");
	m_lisdistb.AddString("-0.10");
	m_lisdistb.AddString("-0.11");//110
	m_lisdistb.AddString("-0.12");
	m_lisdistb.AddString("-0.13");
	m_lisdistb.AddString("-0.14");
	m_lisdistb.AddString("-0.15");
	m_lisdistb.AddString("-0.16");
	m_lisdistb.AddString("-0.17");
	m_lisdistb.AddString("-0.18");
	m_lisdistb.AddString("-0.19");
	m_lisdistb.AddString("-0.20");
	m_lisdistb.AddString("-0.21");//120
	m_lisdistb.AddString("-0.22");
	m_lisdistb.AddString("-0.23");
	m_lisdistb.AddString("-0.24");
	m_lisdistb.AddString("-0.25");
	m_lisdistb.AddString("-0.26");
	m_lisdistb.AddString("-0.27");
	m_lisdistb.AddString("-0.28");
	m_lisdistb.AddString("-0.29");
	m_lisdistb.AddString("-0.30");
	m_lisdistb.AddString("-0.31");//130
	m_lisdistb.AddString("-0.32");
	m_lisdistb.AddString("-0.33");
	m_lisdistb.AddString("-0.34");
	m_lisdistb.AddString("-0.35");
	m_lisdistb.AddString("-0.36");
	m_lisdistb.AddString("-0.37");
	m_lisdistb.AddString("-0.38");
	m_lisdistb.AddString("-0.39");
	m_lisdistb.AddString("-0.40");
	m_lisdistb.AddString("-0.41");//140
	m_lisdistb.AddString("-0.42");
	m_lisdistb.AddString("-0.43");
	m_lisdistb.AddString("-0.44");
	m_lisdistb.AddString("-0.45");
	m_lisdistb.AddString("-0.46");
	m_lisdistb.AddString("-0.47");
	m_lisdistb.AddString("-0.48");
	m_lisdistb.AddString("-0.49");
	m_lisdistb.AddString("-0.50");
	m_lisdistb.AddString("-0.51");//150
	m_lisdistb.AddString("-0.52");
	m_lisdistb.AddString("-0.53");
	m_lisdistb.AddString("-0.54");
	m_lisdistb.AddString("-0.55");
	m_lisdistb.AddString("-0.56");
	m_lisdistb.AddString("-0.57");
	m_lisdistb.AddString("-0.58");
	m_lisdistb.AddString("-0.59");
	m_lisdistb.AddString("-0.60");
	m_lisdistb.AddString("-0.61");//160
	m_lisdistb.AddString("-0.62");
	m_lisdistb.AddString("-0.63");
	m_lisdistb.AddString("-0.64");
	m_lisdistb.AddString("-0.65");
	m_lisdistb.AddString("-0.66");
	m_lisdistb.AddString("-0.67");
	m_lisdistb.AddString("-0.68");
	m_lisdistb.AddString("-0.69");
	m_lisdistb.AddString("-0.70");
	m_lisdistb.AddString("-0.71");//170
	m_lisdistb.AddString("-0.72");
	m_lisdistb.AddString("-0.73");
	m_lisdistb.AddString("-0.74");
	m_lisdistb.AddString("-0.75");
	m_lisdistb.AddString("-0.76");
	m_lisdistb.AddString("-0.77");
	m_lisdistb.AddString("-0.78");
	m_lisdistb.AddString("-0.79");
	m_lisdistb.AddString("-0.80");
	m_lisdistb.AddString("-0.81");//180
	m_lisdistb.AddString("-0.82");
	m_lisdistb.AddString("-0.83");
	m_lisdistb.AddString("-0.84");
	m_lisdistb.AddString("-0.85");
	m_lisdistb.AddString("-0.86");
	m_lisdistb.AddString("-0.87");
	m_lisdistb.AddString("-0.88");
	m_lisdistb.AddString("-0.89");
	m_lisdistb.AddString("-0.90");
	m_lisdistb.AddString("-0.91");//190
	m_lisdistb.AddString("-0.92");
	m_lisdistb.AddString("-0.93");
	m_lisdistb.AddString("-0.94");
	m_lisdistb.AddString("-0.95");
	m_lisdistb.AddString("-0.96");
	m_lisdistb.AddString("-0.97");
	m_lisdistb.AddString("-0.98");
	m_lisdistb.AddString("-0.99");//198
	m_lisdistb.SetCurSel( 74 );

	m_lisdistc.AddString("0.99");//0
	m_lisdistc.AddString("0.98");//1
	m_lisdistc.AddString("0.97");//2
	m_lisdistc.AddString("0.96");//3
	m_lisdistc.AddString("0.95");//4
	m_lisdistc.AddString("0.94");//5
	m_lisdistc.AddString("0.93");//6
	m_lisdistc.AddString("0.92");//7
	m_lisdistc.AddString("0.91");//8
	m_lisdistc.AddString("0.90");//9
	m_lisdistc.AddString("0.89");//10
	m_lisdistc.AddString("0.88");
	m_lisdistc.AddString("0.87");
	m_lisdistc.AddString("0.86");
	m_lisdistc.AddString("0.85");
	m_lisdistc.AddString("0.84");
	m_lisdistc.AddString("0.83");
	m_lisdistc.AddString("0.82");
	m_lisdistc.AddString("0.81");
	m_lisdistc.AddString("0.80");
	m_lisdistc.AddString("0.79");//20
	m_lisdistc.AddString("0.78");
	m_lisdistc.AddString("0.77");
	m_lisdistc.AddString("0.76");
	m_lisdistc.AddString("0.75");
	m_lisdistc.AddString("0.74");
	m_lisdistc.AddString("0.73");
	m_lisdistc.AddString("0.72");
	m_lisdistc.AddString("0.71");
	m_lisdistc.AddString("0.70");
	m_lisdistc.AddString("0.69");//30
	m_lisdistc.AddString("0.68");
	m_lisdistc.AddString("0.67");
	m_lisdistc.AddString("0.66");
	m_lisdistc.AddString("0.65");
	m_lisdistc.AddString("0.64");
	m_lisdistc.AddString("0.63");
	m_lisdistc.AddString("0.62");
	m_lisdistc.AddString("0.61");
	m_lisdistc.AddString("0.60");
	m_lisdistc.AddString("0.59");//40
	m_lisdistc.AddString("0.58");
	m_lisdistc.AddString("0.57");
	m_lisdistc.AddString("0.56");
	m_lisdistc.AddString("0.55");
	m_lisdistc.AddString("0.54");
	m_lisdistc.AddString("0.53");
	m_lisdistc.AddString("0.52");
	m_lisdistc.AddString("0.51");
	m_lisdistc.AddString("0.50");
	m_lisdistc.AddString("0.49");//50
	m_lisdistc.AddString("0.48");
	m_lisdistc.AddString("0.47");
	m_lisdistc.AddString("0.46");
	m_lisdistc.AddString("0.45");
	m_lisdistc.AddString("0.44");
	m_lisdistc.AddString("0.43");
	m_lisdistc.AddString("0.42");
	m_lisdistc.AddString("0.41");
	m_lisdistc.AddString("0.40");
	m_lisdistc.AddString("0.39");//60
	m_lisdistc.AddString("0.38");
	m_lisdistc.AddString("0.37");
	m_lisdistc.AddString("0.36");
	m_lisdistc.AddString("0.35");
	m_lisdistc.AddString("0.34");
	m_lisdistc.AddString("0.33");
	m_lisdistc.AddString("0.32");
	m_lisdistc.AddString("0.31");
	m_lisdistc.AddString("0.30");
	m_lisdistc.AddString("0.29");//70
	m_lisdistc.AddString("0.28");
	m_lisdistc.AddString("0.27");
	m_lisdistc.AddString("0.26");
	m_lisdistc.AddString("0.25");
	m_lisdistc.AddString("0.24");
	m_lisdistc.AddString("0.23");
	m_lisdistc.AddString("0.22");
	m_lisdistc.AddString("0.21");
	m_lisdistc.AddString("0.20");
	m_lisdistc.AddString("0.19");//80
	m_lisdistc.AddString("0.18");
	m_lisdistc.AddString("0.17");
	m_lisdistc.AddString("0.16");
	m_lisdistc.AddString("0.15");
	m_lisdistc.AddString("0.14");
	m_lisdistc.AddString("0.13");
	m_lisdistc.AddString("0.12");
	m_lisdistc.AddString("0.11");
	m_lisdistc.AddString("0.10");
	m_lisdistc.AddString("0.09");//90
	m_lisdistc.AddString("0.08");
	m_lisdistc.AddString("0.07");
	m_lisdistc.AddString("0.06");
	m_lisdistc.AddString("0.05");
	m_lisdistc.AddString("0.04");
	m_lisdistc.AddString("0.03");
	m_lisdistc.AddString("0.02");
	m_lisdistc.AddString("0.01");
	m_lisdistc.AddString("0.00");
	m_lisdistc.AddString("-0.01");//100
	m_lisdistc.AddString("-0.02");
	m_lisdistc.AddString("-0.03");
	m_lisdistc.AddString("-0.04");
	m_lisdistc.AddString("-0.05");
	m_lisdistc.AddString("-0.06");
	m_lisdistc.AddString("-0.07");
	m_lisdistc.AddString("-0.08");
	m_lisdistc.AddString("-0.09");
	m_lisdistc.AddString("-0.10");
	m_lisdistc.AddString("-0.11");//110
	m_lisdistc.AddString("-0.12");
	m_lisdistc.AddString("-0.13");
	m_lisdistc.AddString("-0.14");
	m_lisdistc.AddString("-0.15");
	m_lisdistc.AddString("-0.16");
	m_lisdistc.AddString("-0.17");
	m_lisdistc.AddString("-0.18");
	m_lisdistc.AddString("-0.19");
	m_lisdistc.AddString("-0.20");
	m_lisdistc.AddString("-0.21");//120
	m_lisdistc.AddString("-0.22");
	m_lisdistc.AddString("-0.23");
	m_lisdistc.AddString("-0.24");
	m_lisdistc.AddString("-0.25");
	m_lisdistc.AddString("-0.26");
	m_lisdistc.AddString("-0.27");
	m_lisdistc.AddString("-0.28");
	m_lisdistc.AddString("-0.29");
	m_lisdistc.AddString("-0.30");
	m_lisdistc.AddString("-0.31");//130
	m_lisdistc.AddString("-0.32");
	m_lisdistc.AddString("-0.33");
	m_lisdistc.AddString("-0.34");
	m_lisdistc.AddString("-0.35");
	m_lisdistc.AddString("-0.36");
	m_lisdistc.AddString("-0.37");
	m_lisdistc.AddString("-0.38");
	m_lisdistc.AddString("-0.39");
	m_lisdistc.AddString("-0.40");
	m_lisdistc.AddString("-0.41");//140
	m_lisdistc.AddString("-0.42");
	m_lisdistc.AddString("-0.43");
	m_lisdistc.AddString("-0.44");
	m_lisdistc.AddString("-0.45");
	m_lisdistc.AddString("-0.46");
	m_lisdistc.AddString("-0.47");
	m_lisdistc.AddString("-0.48");
	m_lisdistc.AddString("-0.49");
	m_lisdistc.AddString("-0.50");
	m_lisdistc.AddString("-0.51");//150
	m_lisdistc.AddString("-0.52");
	m_lisdistc.AddString("-0.53");
	m_lisdistc.AddString("-0.54");
	m_lisdistc.AddString("-0.55");
	m_lisdistc.AddString("-0.56");
	m_lisdistc.AddString("-0.57");
	m_lisdistc.AddString("-0.58");
	m_lisdistc.AddString("-0.59");
	m_lisdistc.AddString("-0.60");
	m_lisdistc.AddString("-0.61");//160
	m_lisdistc.AddString("-0.62");
	m_lisdistc.AddString("-0.63");
	m_lisdistc.AddString("-0.64");
	m_lisdistc.AddString("-0.65");
	m_lisdistc.AddString("-0.66");
	m_lisdistc.AddString("-0.67");
	m_lisdistc.AddString("-0.68");
	m_lisdistc.AddString("-0.69");
	m_lisdistc.AddString("-0.70");
	m_lisdistc.AddString("-0.71");//170
	m_lisdistc.AddString("-0.72");
	m_lisdistc.AddString("-0.73");
	m_lisdistc.AddString("-0.74");
	m_lisdistc.AddString("-0.75");
	m_lisdistc.AddString("-0.76");
	m_lisdistc.AddString("-0.77");
	m_lisdistc.AddString("-0.78");
	m_lisdistc.AddString("-0.79");
	m_lisdistc.AddString("-0.80");
	m_lisdistc.AddString("-0.81");//180
	m_lisdistc.AddString("-0.82");
	m_lisdistc.AddString("-0.83");
	m_lisdistc.AddString("-0.84");
	m_lisdistc.AddString("-0.85");
	m_lisdistc.AddString("-0.86");
	m_lisdistc.AddString("-0.87");
	m_lisdistc.AddString("-0.88");
	m_lisdistc.AddString("-0.89");
	m_lisdistc.AddString("-0.90");
	m_lisdistc.AddString("-0.91");//190
	m_lisdistc.AddString("-0.92");
	m_lisdistc.AddString("-0.93");
	m_lisdistc.AddString("-0.94");
	m_lisdistc.AddString("-0.95");
	m_lisdistc.AddString("-0.96");
	m_lisdistc.AddString("-0.97");
	m_lisdistc.AddString("-0.98");
	m_lisdistc.AddString("-0.99");//198
	m_lisdistc.SetCurSel( 99 );

	m_liscentrox.AddString("%100");//0
	m_liscentrox.AddString("%99");//0
	m_liscentrox.AddString("%98");//1
	m_liscentrox.AddString("%97");//2
	m_liscentrox.AddString("%96");//3
	m_liscentrox.AddString("%95");//4
	m_liscentrox.AddString("%94");//5
	m_liscentrox.AddString("%93");//6
	m_liscentrox.AddString("%92");//7
	m_liscentrox.AddString("%91");//8
	m_liscentrox.AddString("%90");//10
	m_liscentrox.AddString("%89");
	m_liscentrox.AddString("%88");
	m_liscentrox.AddString("%87");
	m_liscentrox.AddString("%86");
	m_liscentrox.AddString("%85");
	m_liscentrox.AddString("%84");
	m_liscentrox.AddString("%83");
	m_liscentrox.AddString("%82");
	m_liscentrox.AddString("%81");
	m_liscentrox.AddString("%80");//20
	m_liscentrox.AddString("%79");
	m_liscentrox.AddString("%78");
	m_liscentrox.AddString("%77");
	m_liscentrox.AddString("%76");
	m_liscentrox.AddString("%75");
	m_liscentrox.AddString("%74");
	m_liscentrox.AddString("%73");
	m_liscentrox.AddString("%72");
	m_liscentrox.AddString("%71");
	m_liscentrox.AddString("%70");//30
	m_liscentrox.AddString("%69");
	m_liscentrox.AddString("%68");
	m_liscentrox.AddString("%67");
	m_liscentrox.AddString("%66");
	m_liscentrox.AddString("%65");
	m_liscentrox.AddString("%64");
	m_liscentrox.AddString("%63");
	m_liscentrox.AddString("%62");
	m_liscentrox.AddString("%61");
	m_liscentrox.AddString("%60");//40
	m_liscentrox.AddString("%59");
	m_liscentrox.AddString("%58");
	m_liscentrox.AddString("%57");
	m_liscentrox.AddString("%56");
	m_liscentrox.AddString("%55");
	m_liscentrox.AddString("%54");
	m_liscentrox.AddString("%53");
	m_liscentrox.AddString("%52");
	m_liscentrox.AddString("%51");
	m_liscentrox.AddString("%50");//50
	m_liscentrox.AddString("%49");
	m_liscentrox.AddString("%48");
	m_liscentrox.AddString("%47");
	m_liscentrox.AddString("%46");
	m_liscentrox.AddString("%45");
	m_liscentrox.AddString("%44");
	m_liscentrox.AddString("%43");
	m_liscentrox.AddString("%42");
	m_liscentrox.AddString("%41");
	m_liscentrox.AddString("%40");//60
	m_liscentrox.AddString("%39");
	m_liscentrox.AddString("%38");
	m_liscentrox.AddString("%37");
	m_liscentrox.AddString("%36");
	m_liscentrox.AddString("%35");
	m_liscentrox.AddString("%34");
	m_liscentrox.AddString("%33");
	m_liscentrox.AddString("%32");
	m_liscentrox.AddString("%31");
	m_liscentrox.AddString("%30");//70
	m_liscentrox.AddString("%29");
	m_liscentrox.AddString("%28");
	m_liscentrox.AddString("%27");
	m_liscentrox.AddString("%26");
	m_liscentrox.AddString("%25");
	m_liscentrox.AddString("%24");
	m_liscentrox.AddString("%23");
	m_liscentrox.AddString("%22");
	m_liscentrox.AddString("%21");
	m_liscentrox.AddString("%20");//80
	m_liscentrox.AddString("%19");
	m_liscentrox.AddString("%18");
	m_liscentrox.AddString("%17");
	m_liscentrox.AddString("%16");
	m_liscentrox.AddString("%15");
	m_liscentrox.AddString("%14");
	m_liscentrox.AddString("%13");
	m_liscentrox.AddString("%12");
	m_liscentrox.AddString("%11");
	m_liscentrox.AddString("%10");//90
	m_liscentrox.AddString("%09");
	m_liscentrox.AddString("%08");
	m_liscentrox.AddString("%07");
	m_liscentrox.AddString("%06");
	m_liscentrox.AddString("%05");
	m_liscentrox.AddString("%04");
	m_liscentrox.AddString("%03");
	m_liscentrox.AddString("%02");
	m_liscentrox.AddString("%01");
	m_liscentrox.AddString("%00");//100
	m_liscentrox.AddString("-%01");
	m_liscentrox.AddString("-%02");
	m_liscentrox.AddString("-%03");
	m_liscentrox.AddString("-%04");
	m_liscentrox.AddString("-%05");
	m_liscentrox.AddString("-%06");
	m_liscentrox.AddString("-%07");
	m_liscentrox.AddString("-%08");
	m_liscentrox.AddString("-%09");
	m_liscentrox.AddString("-%10");//110
	m_liscentrox.AddString("-%11");
	m_liscentrox.AddString("-%12");
	m_liscentrox.AddString("-%13");
	m_liscentrox.AddString("-%14");
	m_liscentrox.AddString("-%15");
	m_liscentrox.AddString("-%16");
	m_liscentrox.AddString("-%17");
	m_liscentrox.AddString("-%18");
	m_liscentrox.AddString("-%19");
	m_liscentrox.AddString("-%20");//120
	m_liscentrox.AddString("-%21");
	m_liscentrox.AddString("-%22");
	m_liscentrox.AddString("-%23");
	m_liscentrox.AddString("-%24");
	m_liscentrox.AddString("-%25");
	m_liscentrox.AddString("-%26");
	m_liscentrox.AddString("-%27");
	m_liscentrox.AddString("-%28");
	m_liscentrox.AddString("-%29");
	m_liscentrox.AddString("-%30");//130
	m_liscentrox.AddString("-%31");
	m_liscentrox.AddString("-%32");
	m_liscentrox.AddString("-%33");
	m_liscentrox.AddString("-%34");
	m_liscentrox.AddString("-%35");
	m_liscentrox.AddString("-%36");
	m_liscentrox.AddString("-%37");
	m_liscentrox.AddString("-%38");
	m_liscentrox.AddString("-%39");
	m_liscentrox.AddString("-%40");//140
	m_liscentrox.AddString("-%41");
	m_liscentrox.AddString("-%42");
	m_liscentrox.AddString("-%43");
	m_liscentrox.AddString("-%44");
	m_liscentrox.AddString("-%45");
	m_liscentrox.AddString("-%46");
	m_liscentrox.AddString("-%47");
	m_liscentrox.AddString("-%48");
	m_liscentrox.AddString("-%49");
	m_liscentrox.AddString("-%50");//150
	m_liscentrox.AddString("-%51");
	m_liscentrox.AddString("-%52");
	m_liscentrox.AddString("-%53");
	m_liscentrox.AddString("-%54");
	m_liscentrox.AddString("-%55");
	m_liscentrox.AddString("-%56");
	m_liscentrox.AddString("-%57");
	m_liscentrox.AddString("-%58");
	m_liscentrox.AddString("-%59");
	m_liscentrox.AddString("-%60");//160
	m_liscentrox.AddString("-%61");
	m_liscentrox.AddString("-%62");
	m_liscentrox.AddString("-%63");
	m_liscentrox.AddString("-%64");
	m_liscentrox.AddString("-%65");
	m_liscentrox.AddString("-%66");
	m_liscentrox.AddString("-%67");
	m_liscentrox.AddString("-%68");
	m_liscentrox.AddString("-%69");
	m_liscentrox.AddString("-%70");//170
	m_liscentrox.AddString("-%71");
	m_liscentrox.AddString("-%72");
	m_liscentrox.AddString("-%73");
	m_liscentrox.AddString("-%74");
	m_liscentrox.AddString("-%75");
	m_liscentrox.AddString("-%76");
	m_liscentrox.AddString("-%77");
	m_liscentrox.AddString("-%78");
	m_liscentrox.AddString("-%79");
	m_liscentrox.AddString("-%80");//180
	m_liscentrox.AddString("-%81");
	m_liscentrox.AddString("-%82");
	m_liscentrox.AddString("-%83");
	m_liscentrox.AddString("-%84");
	m_liscentrox.AddString("-%85");
	m_liscentrox.AddString("-%86");
	m_liscentrox.AddString("-%87");
	m_liscentrox.AddString("-%88");
	m_liscentrox.AddString("-%89");
	m_liscentrox.AddString("-%90");//190
	m_liscentrox.AddString("-%91");
	m_liscentrox.AddString("-%92");
	m_liscentrox.AddString("-%93");
	m_liscentrox.AddString("-%94");
	m_liscentrox.AddString("-%95");
	m_liscentrox.AddString("-%96");
	m_liscentrox.AddString("-%97");
	m_liscentrox.AddString("-%98");
	m_liscentrox.AddString("-%99");
	m_liscentrox.AddString("-%100");//200
	m_liscentrox.SetCurSel( 100 );


/********************************/


	m_IPLocal.SetWindowText( IPLocal );	
	m_IPRemote.SetWindowText( IPRemote );
	
	
	sprintf( cad, "%d", IPPort ); 
	m_IPPort = cad;
	//m_IPPort.GetBuffer( 2 ) );

	m_rad = 1;
	m_TeamColor = 0;
	m_Color = 0;

	VideoP = 1;
	m_VideoP = VideoP;
		
	UpdateData(FALSE);	
	
	/////////////////////////////////////////////////////////////////////////
	//							OpenSocket
	//sd=OpenSocketClient( "localhost" , 2000 );
	//sd=OpenSocketClient( "132.248.52.200" , 8002 );
	
	#ifdef comment
	sd=OpenSocketClient( "132.248.52.198" , 8002 );
	sprintf( SckBuf, "%s", "OPEN&fjrg76&END\n" );
	WriteSocket( sd, SckBuf );
	CloseSocket( sd );	
	#endif

	SocketAlreadyOpen = FALSE;
	
	// Get coord's from the process window
//	HRESULT hr;
    HWND hwndStill = NULL;
    GetDlgItem( IDC_STILL, &hwndStill );
    RECT rc;
    ::GetWindowRect( hwndStill, &rc );

	WinCoords.corner0.x = rc.left;
	WinCoords.corner0.y = rc.top;
	WinCoords.corner1.x = rc.right;
	WinCoords.corner1.y = rc.bottom;

	FieldCoords.corner0.x = 0;
	FieldCoords.corner0.y = 0;
	
	FieldCoords.corner1.x = WinCoords.corner1.x;
	FieldCoords.corner1.y = WinCoords.corner1.y;

	FieldCoordsDown.corner0.x = 0;
	FieldCoordsDown.corner0.y = 0;

	FieldLim.corner0.x = 0.0;
	FieldLim.corner0.y = 0.0;
	FieldLim.corner1.x = 2.0;
	FieldLim.corner1.y = 2.0;

	//***************** Gets the acos table *********************
	for( int cont = 0; cont < 2000; cont++ ){
		
		
		acost[cont] = acos( (float)(cont-1000)/1000.0 );
	}
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CStillCapDlg::ClearGraphs( )
{
    // Destroy capture graph
    if( m_pGraph )
    {
        // have to wait for the graphs to stop first
        //
        CComQIPtr< IMediaControl, &IID_IMediaControl > pControl = m_pGraph;
        if( pControl ) 
            pControl->Stop( );

        // make the window go away before we release graph
        // or we'll leak memory/resources
        // 
        CComQIPtr< IVideoWindow, &IID_IVideoWindow > pWindow = m_pGraph;
        if( pWindow )
        {
            pWindow->put_Visible( OAFALSE );
            pWindow->put_Owner( NULL );
        }

#ifdef REGISTER_FILTERGRAPH
        // Remove filter graph from the running object table   
        if (g_dwGraphRegister)
            RemoveGraphFromRot(g_dwGraphRegister);
#endif

        m_pGraph.Release( );
        m_pGrabber.Release( );
    }

    // Destroy playback graph, if it exists
    if( m_pPlayGraph )
    {
        CComQIPtr< IMediaControl, &IID_IMediaControl > pControl = m_pPlayGraph;
        if( pControl ) 
            pControl->Stop( );

        CComQIPtr< IVideoWindow, &IID_IVideoWindow > pWindow = m_pPlayGraph;
        if( pWindow )
        {
            pWindow->put_Visible( OAFALSE );
            pWindow->put_Owner( NULL );
        }

        m_pPlayGraph.Release( );
    }
}


HRESULT CStillCapDlg::InitStillGraph( )
{
    HRESULT hr;

    // create a filter graph
    //
    hr = m_pGraph.CoCreateInstance( CLSID_FilterGraph );
    if( !m_pGraph )
    {
        Error( TEXT("Could not create filter graph") );
        return E_FAIL;
    }

    // get whatever capture device exists
    //
    CComPtr< IBaseFilter > pCap;
    GetDefaultCapDevice( &pCap );
    if( !pCap )
    {
        Error( TEXT("No video capture device was detected on your system.\r\n\r\n")
               TEXT("This sample requires a functional video capture device, such\r\n")
               TEXT("as a USB web camera.  Video capture will be disabled.") );
        return E_FAIL;
    }

    // add the capture filter to the graph
    //
    hr = m_pGraph->AddFilter( pCap, L"Cap" );
    if( FAILED( hr ) )
    {
        Error( TEXT("No se encontró ningún dispositivo válido de video"));
        return E_FAIL;
    }

    // create a sample grabber
    //
    hr = m_pGrabber.CoCreateInstance( CLSID_SampleGrabber );
    if( !m_pGrabber )
    {
        Error( TEXT("Could not create SampleGrabber (is qedit.dll registered?)"));
        return hr;
    }
    CComQIPtr< IBaseFilter, &IID_IBaseFilter > pGrabBase( m_pGrabber );

    // force it to connect to video, 24 bit
    //
    CMediaType VideoType;
    VideoType.SetType( &MEDIATYPE_Video );
    VideoType.SetSubtype( &MEDIASUBTYPE_RGB24 );
	hr = m_pGrabber->SetMediaType( &VideoType ); 
	
	// shouldn't fail
    if( FAILED( hr ) )
    {
        Error( TEXT("Could not set media type"));
        return hr;
    }

    // add the grabber to the graph
    //
    hr = m_pGraph->AddFilter( pGrabBase, L"Grabber" );
    if( FAILED( hr ) )
    {
        Error( TEXT("Could not put sample grabber in graph"));
        return hr;
    }

    // build the graph
    CComPtr<ICaptureGraphBuilder2> pCGB2;
    hr = pCGB2.CoCreateInstance (CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC);
    if (FAILED( hr ))
    {
        Error(TEXT ("Can't get a ICaptureGraphBuilder2 reference"));
        return hr;
    }

    hr = pCGB2->SetFiltergraph( m_pGraph );
    if (FAILED( hr ))
    {
        Error(TEXT("SetGraph failed"));
        return hr;
    }


    // If there is a VP pin present on the video device, then put the 
    // renderer on CLSID_NullRenderer
    CComPtr<IPin> pVPPin;
    hr = pCGB2->FindPin(
                        pCap, 
                        PINDIR_OUTPUT, 
                        &PIN_CATEGORY_VIDEOPORT, 
                        NULL, 
                        FALSE, 
                        0, 
                        &pVPPin);


    // If there is a VP pin, put the renderer on NULL Renderer
    CComPtr<IBaseFilter> pRenderer;
    if (S_OK == hr)
    {   
        hr = pRenderer.CoCreateInstance(CLSID_NullRenderer);    
        if (S_OK != hr)
        {
            Error(TEXT("Unable to make a NULL renderer"));
            return S_OK;
        }
        hr = m_pGraph->AddFilter(pRenderer, L"NULL renderer");
        if (FAILED (hr))
        {
            Error(TEXT("Can't add the filter to graph"));
            return hr;
        }
    }

    hr = pCGB2->RenderStream(
                            &PIN_CATEGORY_PREVIEW,
                            &MEDIATYPE_Interleaved, 
                            pCap, 
                            pGrabBase, 
                            pRenderer);
    if (FAILED (hr))
    {
        // try to render preview pin
        hr = pCGB2->RenderStream( 
                                &PIN_CATEGORY_PREVIEW, 
                                &MEDIATYPE_Video,
                                pCap, 
                                pGrabBase, 
                                pRenderer);

        // try to render capture pin
        if( FAILED( hr ) )
        {
            hr = pCGB2->RenderStream( 
                                    &PIN_CATEGORY_CAPTURE, 
                                    &MEDIATYPE_Video,
                                    pCap, 
                                    pGrabBase, 
                                    pRenderer);
        }
    }

    if( FAILED( hr ) )
    {
        Error( TEXT("Can't build the graph") );
        return hr;
    }

    // ask for the connection media type so we know how big
    // it is, so we can write out bitmaps
    //
    AM_MEDIA_TYPE mt;
    hr = m_pGrabber->GetConnectedMediaType( &mt );
    if ( FAILED( hr) )
    {
        Error( TEXT("Could not read the connected media type"));
        return hr;
    }
    
    VIDEOINFOHEADER * vih = (VIDEOINFOHEADER*) mt.pbFormat;
    mCB.pOwner = this;
    mCB.lWidth  = vih->bmiHeader.biWidth;
    mCB.lHeight = vih->bmiHeader.biHeight;
    FreeMediaType( mt );

    // don't buffer the samples as they pass through
    //
    hr = m_pGrabber->SetBufferSamples( FALSE );
	

    // only grab one at a time, stop stream after
    // grabbing one sample
    //
    hr = m_pGrabber->SetOneShot( FALSE );
	
	// set the callback, so we can grab the one sample
    //
    hr = m_pGrabber->SetCallback( &mCB, 1 );

    // find the video window and stuff it in our window
    //
    CComQIPtr< IVideoWindow, &IID_IVideoWindow > pWindow = m_pGraph;
    if( !pWindow )
    {
        Error( TEXT("Could not get video window interface"));
        return E_FAIL;
    }

    // set up the preview window to be in our dialog
    // instead of floating popup
    //
    HWND hwndPreview = NULL;
    GetDlgItem( IDC_PREVIEW, &hwndPreview );
    RECT rc;
    ::GetWindowRect( hwndPreview, &rc );

    hr = pWindow->put_Owner( (OAHWND) hwndPreview );
    hr = pWindow->put_Left( 0 );
    hr = pWindow->put_Top( 0 );
    hr = pWindow->put_Width( rc.right - rc.left );
    hr = pWindow->put_Height( rc.bottom - rc.top );
    hr = pWindow->put_WindowStyle( WS_CHILD | WS_CLIPSIBLINGS );
    hr = pWindow->put_Visible( OATRUE );
    
    // Add our graph to the running object table, which will allow
    // the GraphEdit application to "spy" on our graph
#ifdef REGISTER_FILTERGRAPH
    hr = AddGraphToRot(m_pGraph, &g_dwGraphRegister);
    if (FAILED(hr))
    {
        Error(TEXT("Failed to register filter graph with ROT!"));
        g_dwGraphRegister = 0;
    }
#endif

    // run the graph
    //
    CComQIPtr< IMediaControl, &IID_IMediaControl > pControl = m_pGraph;
    hr = pControl->Run( );
    if( FAILED( hr ) )
    {
        Error( TEXT("Could not run graph"));
        return hr;
    }

    UpdateStatus(_T("Previewing Live Video"));
    return 0;
}


HRESULT CStillCapDlg::InitCaptureGraph( TCHAR * pFilename )
{
    HRESULT hr;

    // make a filter graph
    //
    m_pGraph.CoCreateInstance( CLSID_FilterGraph );
    if( !m_pGraph )
    {
        Error(TEXT("Could not create filter graph"));
        return E_FAIL;
    }

    // get whatever capture device exists
    //
    CComPtr< IBaseFilter > pCap;
    GetDefaultCapDevice( &pCap );
    if( !pCap )
    {
        Error( TEXT("No video capture device was detected on your system.\r\n\r\n")
               TEXT("This sample requires a functional video capture device, such\r\n")
               TEXT("as a USB web camera.  Video capture will be disabled.") );
        return E_FAIL;
    }

    // add the capture filter to the graph
    //
    hr = m_pGraph->AddFilter( pCap, L"Cap" );
    if( FAILED( hr ) )
    {
        Error( TEXT("blabla Could not put capture device in graph"));
        return hr;
    }

    // make a capture builder graph (for connecting help)
    //
    CComPtr< ICaptureGraphBuilder2 > pBuilder;
    hr = pBuilder.CoCreateInstance( CLSID_CaptureGraphBuilder2 );
    if( !pBuilder )
    {
        Error( TEXT("Could not create capture graph builder2"));
        return hr;
    }

    hr = pBuilder->SetFiltergraph( m_pGraph );
    if( FAILED( hr ) )
    {
        Error( TEXT("Could not set filtergraph on capture graph builder2"));
        return hr;
    }

    CComPtr< IBaseFilter > pMux;
    CComPtr< IFileSinkFilter > pSink;
    USES_CONVERSION;

    hr = pBuilder->SetOutputFileName( &MEDIASUBTYPE_Avi,  T2W( pFilename ), 
                                      &pMux, &pSink );
    if( FAILED( hr ) )
    {
        Error( TEXT("Could not create/hookup mux and writer"));
        return hr;
    }

    hr = pBuilder->RenderStream( &PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video,
                                 pCap, NULL,pMux );
    if( FAILED( hr ) )
    {
        Error( TEXT("Could not connect capture pin"));
        return hr;
    }

    hr = pBuilder->RenderStream( &PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video,
                                 pCap, NULL, NULL );
    if( FAILED( hr ) )
    {
        Error( TEXT("Could not render capture pin"));
        return hr;
    }
    if( hr == VFW_S_NOPREVIEWPIN )
    {
        // preview was faked up using the capture pin, so we can't
        // turn capture on and off at will.
        hr = 0;
    }

    // find the video window and stuff it in our window
    //
    CComQIPtr< IVideoWindow, &IID_IVideoWindow > pWindow = m_pGraph;
    if( !pWindow )
    {
        Error( TEXT("Could not get video window interface"));
        return hr;
    }

    // set up the preview window to be in our dialog
    // instead of floating popup
    //
    HWND hwndPreview = NULL;
    GetDlgItem( IDC_PREVIEW, &hwndPreview );
    RECT rc;
    ::GetWindowRect( hwndPreview, &rc );

    hr = pWindow->put_Owner( (OAHWND) hwndPreview );
    hr = pWindow->put_Left( 0 );
    hr = pWindow->put_Top( 0 );
    hr = pWindow->put_Width( rc.right - rc.left );
    hr = pWindow->put_Height( rc.bottom - rc.top );
    hr = pWindow->put_Visible( OATRUE );
    hr = pWindow->put_WindowStyle( WS_CHILD | WS_CLIPSIBLINGS );
    
    // run the graph
    //
    CComQIPtr< IMediaControl, &IID_IMediaControl > pControl = m_pGraph;
    hr = pControl->Run( );
    if( FAILED( hr ) )
    {
        Error( TEXT("Could not run graph"));
        return hr;
    }

    UpdateStatus(_T("Capturing Video To Disk"));
    return 0;
}


HRESULT CStillCapDlg::InitPlaybackGraph( TCHAR * pFilename )
{
    USES_CONVERSION;

    m_pPlayGraph.CoCreateInstance( CLSID_FilterGraph );

    HRESULT hr = m_pPlayGraph->RenderFile( T2W( pFilename ), NULL );
    if (FAILED(hr))
        return hr;

    // find the video window and stuff it in our window
    //
    CComQIPtr< IVideoWindow, &IID_IVideoWindow > pWindow = m_pPlayGraph;
    if( !pWindow )
    {
        Error( TEXT("Could not get video window interface"));
        return E_FAIL;
    }

    // set up the preview window to be in our dialog
    // instead of floating popup
    //
    HWND hwndPreview = NULL;
    GetDlgItem( IDC_STILL, &hwndPreview );
    RECT rc;
    ::GetWindowRect( hwndPreview, &rc );

    hr = pWindow->put_Owner( (OAHWND) hwndPreview );
    hr = pWindow->put_Left( 0 );
    hr = pWindow->put_Top( 0 );
    hr = pWindow->put_Width( rc.right - rc.left );
    hr = pWindow->put_Height( rc.bottom - rc.top );
    hr = pWindow->put_Visible( OATRUE );
    hr = pWindow->put_WindowStyle( WS_CHILD | WS_CLIPSIBLINGS );

    CComQIPtr< IMediaControl, &IID_IMediaControl > pControl;
    pControl = m_pPlayGraph;

    // Play back the recorded video
    hr = pControl->Run( );
    UpdateStatus(_T("Playing Back Recorded Video"));
    return hr;
}


void CStillCapDlg::GetDefaultCapDevice( IBaseFilter ** ppCap )
{
    HRESULT hr;

    ASSERT(ppCap);
    if (!ppCap)
        return;

    *ppCap = NULL;

    // create an enumerator
    //
    CComPtr< ICreateDevEnum > pCreateDevEnum;
    pCreateDevEnum.CoCreateInstance( CLSID_SystemDeviceEnum );

    ASSERT(pCreateDevEnum);
    if( !pCreateDevEnum )
        return;

    // enumerate video capture devices
    //
    CComPtr< IEnumMoniker > pEm;
    pCreateDevEnum->CreateClassEnumerator( CLSID_VideoInputDeviceCategory, &pEm, 0 );

    ASSERT(pEm);
    if( !pEm )
        return;

    pEm->Reset( );

	
 
    // go through and find first video capture device
    //
    while( 1 )
    {
        ULONG ulFetched = 0;
        CComPtr< IMoniker > pM;

	#ifdef SECONDCAM
		hr = pEm->Skip ( 1 );
	#endif
        hr = pEm->Next( 1, &pM, &ulFetched );
		if( hr != S_OK )
            break;

        // get the property bag interface from the moniker
        //
        CComPtr< IPropertyBag > pBag;
        hr = pM->BindToStorage( 0, 0, IID_IPropertyBag, (void**) &pBag );
        if( hr != S_OK )
            continue;

        // ask for the english-readable name
        //
        CComVariant var;
        var.vt = VT_BSTR;
        hr = pBag->Read( L"FriendlyName", &var, NULL );
        if( hr != S_OK )
            continue;

        // set it in our UI
        //
        USES_CONVERSION;
        SetDlgItemText( IDC_CAPOBJ, W2T( var.bstrVal ) );

        // ask for the actual filter
        //
        hr = pM->BindToObject( 0, 0, IID_IBaseFilter, (void**) ppCap );
        if( *ppCap )
            break;
    }

    return;
}


void CStillCapDlg::OnSnap() 
{
    CString CapDir;
//    GetDlgItemText( IDC_CAPDIR, CapDir );

    // Determine AVI filename
    TCHAR szFilename[MAX_PATH], szFile[MAX_PATH];
    sprintf( szFilename, TEXT("%sStillCap%04d.avi\0"), (LPCTSTR) CapDir, m_nCapTimes );
    sprintf( szFile, TEXT("StillCap%04d.avi\0"), m_nCapTimes );

    // Snap a still picture?
    if( m_bCapStills )
    {
        _tcsncpy( mCB.m_szCapDir, CapDir, NUMELMS(mCB.m_szCapDir) );
        g_bOneShot = TRUE;
    
///////////////////////////////////////
////		
		// Sets the frame rate
		if( !m_timer )
		{
			SetTimer( 1, FrameRate, NULL );
			m_timer = TRUE;
		}
	
	
	}

    // Start capturing video
    else
    {
        if( m_nCapState == 0 )
        {
//            if( IsDlgButtonChecked( IDC_AUTOBUMP ) )
 //               m_nCapTimes++;
        }

        // start capturing, show playing button
        //
        if( m_nCapState == 0 )
        {
            ClearGraphs( );
            InitCaptureGraph( szFilename );
            SetDlgItemText( IDC_SNAP, TEXT("&Start Playback"));
            m_nCapState = 1;
        }
        else if( m_nCapState == 1 )
        {
            // show us where it captured to
            //
//            SetDlgItemText( IDC_SNAPNAME, szFile );

            ClearGraphs( );
            InitPlaybackGraph( szFilename );
            SetDlgItemText( IDC_SNAP, TEXT("&Start Capture"));
            m_nCapState = 0;
        }
    }
}

BOOL CStillCapDlg::DestroyWindow() 
{
    
	ClearGraphs( );

    return CDialog::DestroyWindow();
}


void CStillCapDlg::Error( TCHAR * pText )
{
    GetDlgItem( IDC_SNAP )->EnableWindow( FALSE );
    ::MessageBox( NULL, pText, TEXT("Error!"), MB_OK | MB_TASKMODAL | MB_SETFOREGROUND );
}


#ifdef comment
void CStillCapDlg::OnCapstills() 
{
    if( m_bCapStills )
        return;

    SetDlgItemText( IDC_SNAP, TEXT("&Snap Still"));
    m_bCapStills = true;

    ClearGraphs( );
    InitStillGraph( );

    // Update the bitmap preview window, if we have
    // already captured bitmap data
    mCB.DisplayCapturedBits(cb.pBuffer, &(cb.bih));
}



void CStillCapDlg::OnCapvid() 
{
    if( !m_bCapStills )
        return;

    ClearGraphs( );
    m_bCapStills = false;
    m_nCapState = 0;

    // use OnSnap to set the UI state and the graphs
    //
    OnSnap( );
}


void CStillCapDlg::OnButtonReset() 
{
    // Reset bitmap counter to reset at zero
    m_nCapTimes = 0;
}


void CStillCapDlg::OnButtonViewstill() 
{
    // Open the bitmap with the system-default application
    ShellExecute(this->GetSafeHwnd(), TEXT("open\0"), mCB.m_szSnappedName, 
                 NULL, NULL, SW_SHOWNORMAL);
}
#endif

void CStillCapDlg::UpdateStatus(TCHAR *szStatus)
{
    m_StrStatus.SetWindowText(szStatus);
}


LRESULT CStillCapDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
    // Field the message posted by our SampleGrabber callback function.
    if (message == WM_CAPTURE_BITMAP)
        mCB.CopyBitmap(cb.dblSampleTime, cb.pBuffer, cb.lBufferSize);        
    
    return CDialog::WindowProc(message, wParam, lParam);
}


void CStillCapDlg::OnClose() 
{
    // Free the memory allocated for our bitmap data buffer
    if (cb.pBuffer != 0)
    {
        delete [] cb.pBuffer;
        cb.pBuffer = 0;
    }
        
////////////////////////////////////////////////////////////////////
//						CloseSockets
	//CloseSocket( sd );	

    CDialog::OnClose();
}





void CStillCapDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( !CalibrationInProgress )
		g_bOneShot = TRUE;
		
	CDialog::OnTimer(nIDEvent);
}


void CStillCapDlg::OnButtonProcess() 
{
	UpdateData( TRUE );
	TeamColor = m_TeamColor;

	if( m_process ){ 
		m_process = FALSE;
		m_ProcessButon.SetWindowText( "Procesar" );
	}
	else{
		m_process = TRUE;
		m_ProcessButon.SetWindowText( "Procesando" );
	}
}

void CStillCapDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{


	UpdateData(TRUE);


////////////////////////////////////////////////////////////////////////////
//								Preprocessing controls

	char cad[80];
	int position = m_Bright.GetPos();
	sprintf( cad, "%d", m_Bright.GetPos() ); m_EdBright = cad;
	bright = position;

	int cposition = m_Contrast.GetPos();
	


	sprintf( cad, "%d", m_Contrast.GetPos() ); m_EdContrast = cad;

	if( cposition >= 0 ) 
	{
		contrast = (float)(cposition*7.0);
		contrast /= 100.0;
		contrast = pow( (float)2, contrast );
		contrast = 1+contrast/16;
	}
	else
	{
		contrast = abs(cposition);
		contrast /= 100;
		contrast = 1 - contrast;
	}


	int sposition = m_Saturation.GetPos();

	if( sposition >= 0 )
		saturation = 4.75*sposition/100.0;
	else
		saturation = sposition/100.0;

	sprintf( cad, "%d", m_Saturation.GetPos() ); m_EdSat = cad;

////////////////////////////////////////////////////////////////////////////
//								Segmentation controls
	
	// Prints the value that each one of the controls takes, but doesn't do anything with
	// them right now
#ifndef comment
	sprintf( cad, "%d", m_SliderOH.GetPos() ); m_Static_OH.SetWindowText( cad );
	sprintf( cad, "%d", m_SliderOS.GetPos() ); m_Static_OS.SetWindowText( cad );
	sprintf( cad, "%d", m_SliderOI.GetPos() ); m_Static_OI.SetWindowText( cad );

	sprintf( cad, "%d", m_SliderHP.GetPos() ); m_Static_HP.SetWindowText( cad );
	sprintf( cad, "%d", m_SliderSP.GetPos() ); m_Static_SP.SetWindowText( cad );
	sprintf( cad, "%d", m_SliderIP.GetPos() ); m_Static_IP.SetWindowText( cad );

	hue[m_Color] = m_SliderOH.GetPos(); 
	hsi[m_Color].h.win = m_SliderHP.GetPos();
	hsi[m_Color].h.max = hue[m_Color] + hsi[m_Color].h.win;
	if( hsi[m_Color].h.max > 360 ) hsi[m_Color].h.max -= 360; /* <-- 360 */

	hsi[m_Color].h.min = hue[m_Color] - hsi[m_Color].h.win;
	if( hsi[m_Color].h.min < 0 ) hsi[m_Color].h.min += 360; /* <-- 360 */

	sat[m_Color] = m_SliderOS.GetPos();
	hsi[m_Color].s.win = m_SliderSP.GetPos();
	hsi[m_Color].s.max = sat[m_Color] + hsi[m_Color].s.win;
	if( hsi[m_Color].s.max > 255 ) hsi[m_Color].s.max = 255;

	hsi[m_Color].s.min = sat[m_Color] - hsi[m_Color].s.win;
	if( hsi[m_Color].s.min < 0 ) hsi[m_Color].s.min = 0;
	
	
	ins[m_Color] = m_SliderOI.GetPos();
	hsi[m_Color].i.win = m_SliderIP.GetPos();
	hsi[m_Color].i.max = ins[m_Color] + hsi[m_Color].i.win;
	if( hsi[m_Color].i.max > 255 ) hsi[m_Color].i.max = 255;

	hsi[m_Color].i.min = ins[m_Color] - hsi[m_Color].i.win;
	if( hsi[m_Color].i.min < 0 ) hsi[m_Color].i.min = 0;

#endif


/**************************************************************************
*							Distance from blob to blob
**************************************************************************/
	MaxDist = m_DistMaxBlSl.GetPos();
	m_DistMaxBl = MaxDist;
	
	
////////////////////////////////////////////////////////////////////////////
//							Regions controls
	
	sprintf( cad, "%d", MinBlobArea = m_MinBlob.GetPos() ); m_MinBlobButt = cad;
//	sprintf( cad, "%d", MinRLE = m_MinRLE.GetPos() ); m_MinRLEButt = cad;
	sprintf( cad, "%d", MinBlobX = m_XBlob.GetPos() ); m_EdXBlob = cad;
	sprintf( cad, "%d", MinBlobY = m_YBlob.GetPos() ); m_EdYBlob = cad;


	UpdateData(FALSE);

	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CStillCapDlg::OnButtonResetPreproc() 
{
	// TODO: Add your control notification handler code here

	bright = 0;
	contrast = 1.0;
	saturation = 0.0;

	m_Saturation.SetPos(0);
	m_Contrast.SetPos(1);
	m_Bright.SetPos(0);
}

void CStillCapDlg::OnButtonRegiReset() 
{
	char cad[80];

	// TODO: Add your control notification handler code here
	m_MinBlob.SetPos(MAXREDAREA);
	//m_MinRLE.SetPos( 20 );
	m_XBlob.SetPos( 10 );
	m_YBlob.SetPos( 10 );

	sprintf( cad, "%d", MinBlobArea = m_MinBlob.GetPos() ); m_MinBlobButt = cad;
//	sprintf( cad, "%d", MinRLE = m_MinRLE.GetPos() ); m_MinRLEButt = cad;

	sprintf( cad, "%d", MinBlobX = m_XBlob.GetPos() ); m_EdXBlob = cad;
	sprintf( cad, "%d", MinBlobY = m_YBlob.GetPos() ); m_EdYBlob = cad;


	UpdateData(FALSE);

	
}

void CStillCapDlg::OnRadioDim() 
{
	m_rad;
}

void CStillCapDlg::OnSelchangeListRefres() 
{

	switch( m_FrameRate.GetCurSel() )
	{
		case 0: FrameRate = 40; break; 
		case 1: FrameRate = 50; break;
		case 2: FrameRate = 66; break;
		case 3: FrameRate = 100; break;
		case 4: FrameRate = 200; break;
		case 5: FrameRate = 500; break;
		case 6: FrameRate = 1000; break;
		case 7: FrameRate = 2000; break;
		default: FrameRate = 500;
	}

	SetTimer( 1, FrameRate, NULL );
}

void CStillCapDlg::OnFieldchangedIpaddressIpLocal(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	m_IPLocal.GetWindowText( IPLocal, 80 );	
	
	*pResult = 0;
}

void CStillCapDlg::OnFieldchangedIpaddressDest(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_IPRemote.GetWindowText( IPRemote, 80 );	
		
	*pResult = 0;
}



void CStillCapDlg::OnKillfocusEditIpPort() 
{
	IPPort = atoi( m_IPPort.GetBuffer(5) );
}


void CStillCapDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	char cad[80]; 
	int grad;
	

	COLORREF c, cmat[WIN_LENGHT][WIN_LENGHT];
	Point p;

	RGB mat[WIN_LENGHT][WIN_LENGHT];;
	//HSI hsi;
	int offset = WIN_LENGHT;

	static int ccon;

	if( CalibrationInProgress )
	{
		// Get the actual button
		UpdateData(TRUE);

		//////////////////////////////////////////////////////////////////////
		//		Gets the hsi's values for the selected color

		int r = PickedColor.r;
		int g = PickedColor.g;
		int b = PickedColor.b;
		
	
		int theta = (int)( acos( ((2*r-g-b)) / (2*sqrt( (float)((r-g)*(r-g)+(r-b)*(g-b))) )) * 57.29);
		
		int h = b <= g ? theta : (360-theta);

		int m = r <= g ? r : g;
		m = m <= b ? m : b;
		m = m <= 0 ? 1 : m;

		float temp = (765 / (float)(r+g+b) );
		temp = temp * (float)m;
		int s = 255- (int)temp;

		int i = (int)( (r+g+b) / 3 );

	//////////////////////////////////////////////////////////////////////
		//		Shows the actual hsi's values


		grad = hsiGetMiddle( hsi[m_Color].h.max, hsi[m_Color].h.min, 360 );
		sprintf( cad, "%d", grad ); m_Static_OH.SetWindowText( cad );
		m_SliderOH.SetPos( grad );
		
		//grad = hsiGetRange( hsi[m_Color].h.max, hsi[m_Color].h.min, 360 );
		grad = hsi[m_Color].h.win;
		sprintf( cad, "%d", grad ); m_Static_HP.SetWindowText( cad );
		m_SliderHP.SetPos( grad );
		
		
		grad = hsiGetMiddle( hsi[m_Color].s.max, hsi[m_Color].s.min, 255 );
		sprintf( cad, "%d", grad ); m_Static_OS.SetWindowText( cad );
		m_SliderOS.SetPos( grad );
		
		//grad = hsiGetRange( hsi[m_Color].s.max, hsi[m_Color].s.min, 255 );
		grad = hsi[m_Color].s.win;
		sprintf( cad, "%d", grad ); m_Static_SP.SetWindowText( cad );
		m_SliderSP.SetPos( grad );
		

		grad = hsiGetMiddle( hsi[m_Color].i.max, hsi[m_Color].i.min, 255 );
		sprintf( cad, "%d", grad ); m_Static_OI.SetWindowText( cad );
		m_SliderOI.SetPos( grad );
		
		//grad = hsiGetRange( hsi[m_Color].i.max, hsi[m_Color].i.min, 255 );
		grad = hsi[m_Color].i.win;
		sprintf( cad, "%d", grad ); m_Static_IP.SetWindowText( cad );
		m_SliderIP.SetPos( grad );

		UpdateData(FALSE);
	}

	if( CalibrationInProgress )
	{
		p.x = point.x - 206;
		p.y = 284 - (point.y - 24);

		if( (p.x < (0+offset)) || (p.x > (348-offset) ) || (p.y < (0+offset) ) || (p.y > (282-offset) ) )
			return;


		HWND hwndStill = NULL;
		GetDlgItem( IDC_STILL, &hwndStill );
		RECT rc;
		::GetWindowRect( hwndStill, &rc );
		
		CClientDC dc( this );

		// Hold the max and min values of the window
		int aaold=0;
												



		// Fill the RGB matrix
		for( int row = 0 - (int)(WIN_LENGHT/2); row <= (int)(WIN_LENGHT/2); row++ )
		//for( int row = -1; row < 2; row++ )
		{
			for( int col = 0 - (int)(WIN_LENGHT/2); col <= (int)(WIN_LENGHT/2); col++ )
			//for( int col = -1; col < 2; col++ )
			{
				c = GetPixel( dc, (point.x)+col, (point.y)+row );
				cmat[col+WIN_LENGHT/2][row+WIN_LENGHT/2] = c;
				
				mat[col+WIN_LENGHT/2][row+WIN_LENGHT/2].r = (char)(c & 0x000000FF);
				mat[col+WIN_LENGHT/2][row+WIN_LENGHT/2].g = (char)((c & 0x0000FF00)>>8);
				mat[col+WIN_LENGHT/2][row+WIN_LENGHT/2].b = (char)((c & 0x00FF0000)>>16);

				
			}
		}
	
		if( ++ccon == 1 )
			ccon = 0;

		int h[RATIO][RATIO] = { {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
					  };

		int zoom[RATIO*WIN_LENGHT][RATIO*WIN_LENGHT], zoom2[RATIO*WIN_LENGHT][RATIO*WIN_LENGHT];
		int x, y, i, j;
		


		// Clear the working matrix
		for( y = 0; y < RATIO*WIN_LENGHT; y++ )
		{
			for( x = 0; x < RATIO*WIN_LENGHT; x++ )
			{
				zoom[x][y] = 0;
				zoom2[x][y] = 0;
			}
		}

		int g;
		// In order to perform the convolution sum, zoom should be treated like 'x' input matrix
		// whereas zoom2 is the resulting matrix output. h is the kernel
		for( y = 0, i = 0; y < RATIO*WIN_LENGHT; y += RATIO, i++ ){
			for( x = 0, j = 0; x < RATIO*WIN_LENGHT; x += RATIO, j++ ){
				g = cmat[j][i];
				zoom[x][y] = cmat[j][i];
			}
		}


		for(  x = 0; x < RATIO*WIN_LENGHT; x++ ){
			for(  y = 0; y < RATIO*WIN_LENGHT; y++ ){
				zoom2[x][y] = 0;

				for(  i = 0; i < RATIO; i++ ){
					if( ((x-i)<0) || ((x-i)>=(RATIO*WIN_LENGHT)) ) 
						continue;

					for(  j = 0; j < RATIO; j++ ){
						if( ((y-j)<0) || ((y-j)>=(RATIO*WIN_LENGHT)) ) 
							continue;
						
						zoom2[x][y] += zoom[x-i][y-j]*h[i][j];
					}
				}
			}
		}		
	}

	if( CalibrationInProgress )
	{
		int r, g, b;
		int theta, h, s, i;
		int cont, cont2;
		BYTE row, col;
		
		for( cont2 = 0; cont2 < 3; cont2++ )
		{
			for( cont = 0; cont < 360; cont++ )
			{
				calib[0].hist[calib[0].times][cont] = 0;
				calib[1].hist[calib[1].times][cont] = 0;
				calib[2].hist[calib[2].times][cont] = 0;
			}
		}

		for( row = 0; row < WIN_LENGHT; row++ )
		{
			for( col = 0; col < WIN_LENGHT; col++ )
			{
				float j;

				r = mat[col][row].r;
				g = mat[col][row].g;
				b = mat[col][row].b;

				j = ( acos((2*r-g-b) / (2*sqrt( (float)( (r-g)*(r-g)+(r-b)*(g-b) ) ) ) ) );
				theta = j * 57.29;
				h = b <= g ? theta : (360-theta);

				int m = r <= g ? r : g;
				m = m <= b ? m : b;
				m = m <= 0 ? 1 : m;
				float temp = (765 / (float)(r+g+b) );
				temp = temp * (float)m;
				s = 255- (int)temp;
				i = (int)((r+g+b)/3);

				//hist[times][h]++;
				calib[0].hist[calib[0].times][h]++;
				calib[1].hist[calib[1].times][s]++;
				calib[2].hist[calib[2].times][i]++;
			}
		}

		int sum = 0;
		times++;

	
		calib[0].times++;
		calib[1].times++;
		calib[2].times++;
		
		if( calib[0].times == 3 )
		{
			calib[0].times = 0;
			calib[0].expected = GetExpected( calib[0], 360 );
			calib[0].sigma = GetSigma( calib[0], 360 );

			calib[1].times = 0;
			calib[1].expected = GetExpected( calib[1], 256 );
			calib[1].sigma = GetSigma( calib[1], 256 );

			calib[2].times = 0;
			calib[2].expected = GetExpected( calib[2], 256 );
			calib[2].sigma = GetSigma( calib[2], 256 );

			
			hsi[m_Color].h.win = (int)(SIGMA_FACTOR*calib[0].sigma);
			hsi[m_Color].h.max = calib[0].expected + hsi[m_Color].h.win;
			if( hsi[m_Color].h.max > 360 ) hsi[m_Color].h.max -= 360;

			hsi[m_Color].h.min = calib[0].expected - hsi[m_Color].h.win;
			if( hsi[m_Color].h.min < 0 ) hsi[m_Color].h.min += 360;		

			
			hsi[m_Color].s.win = (int)(SIGMA_FACTOR*calib[1].sigma);
			hsi[m_Color].s.max = (int)(calib[1].expected) + hsi[m_Color].s.win;
			if( hsi[m_Color].s.max > 255 ) hsi[m_Color].s.max = 255;

			hsi[m_Color].s.min = (int)(calib[1].expected) - hsi[m_Color].s.win;
			if( hsi[m_Color].s.min < 0 ) hsi[m_Color].s.min = 0;

			
			hsi[m_Color].i.win = (int)(SIGMA_FACTOR*calib[2].sigma);
			hsi[m_Color].i.max = (int)(calib[2].expected) + hsi[m_Color].i.win;
			if( hsi[m_Color].i.max > 255 ) hsi[m_Color].i.max = 255;

			hsi[m_Color].i.min = (int)(calib[2].expected) - hsi[m_Color].i.win;
			if( hsi[m_Color].i.min < 0 ) hsi[m_Color].i.min = 0;

			//////////////////////////////////////////////////////////////////////
			//		Shows the actual hsi's values

			grad = hsiGetMiddle( hsi[m_Color].h.max, hsi[m_Color].h.min, 360 );
			sprintf( cad, "%d", grad ); m_Static_OH.SetWindowText( cad );
			m_SliderOH.SetPos( grad );
			
			grad = hsi[m_Color].h.win;
			sprintf( cad, "%d", grad ); m_Static_HP.SetWindowText( cad );
			m_SliderHP.SetPos( grad );
		
			
			grad = hsiGetMiddle( hsi[m_Color].s.max, hsi[m_Color].s.min, 255 );
			sprintf( cad, "%d", grad ); m_Static_OS.SetWindowText( cad );
			m_SliderOS.SetPos( grad );
			
			grad = hsi[m_Color].s.win;
			sprintf( cad, "%d", grad ); m_Static_SP.SetWindowText( cad );
			m_SliderSP.SetPos( grad );
			
			
			grad = hsiGetMiddle( hsi[m_Color].i.max, hsi[m_Color].i.min, 255 );
			sprintf( cad, "%d", grad ); m_Static_OI.SetWindowText( cad );
			m_SliderOI.SetPos( grad );
			
			grad = hsi[m_Color].i.win;
			sprintf( cad, "%d", grad ); m_Static_IP.SetWindowText( cad );
			m_SliderIP.SetPos( grad );
			
			Beep( 4000, 250 );
		}
		UpdateData(FALSE);
	}

/*===============================================================================
*							Field calibration
================================================================================*/		

	if( CalibrationCancha  ){
		FieldCoordsDown.corner0.x = (int)((point.x-240)*RATIO_X);
		FieldCoordsDown.corner0.y = (int)((243 - point.y + 30)*RATIO_Y);
		FieldCoordsDown.corner1.x = (int)((point.x-240)*RATIO_X);
		FieldCoordsDown.corner1.y = (int)((243 - point.y + 30)*RATIO_Y);
	}


	CDialog::OnLButtonDown(nFlags, point);
}

void CStillCapDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( CalibrationCancha  ){
		FieldCoordsDown.corner1.x = (int)((point.x-240)*RATIO_X);
		FieldCoordsDown.corner1.y = (int)((243 - point.y + 30)*RATIO_Y);
	}

	CDialog::OnLButtonUp(nFlags, point);
}


void CStillCapDlg::OnButtonComm() 
{
	char cad1[80];

	// Get the contents of the EditBox'es
	UpdateData(TRUE);
	TeamColor = m_TeamColor;


	sprintf( cad1, "%s", IPRemote );
	IPPort = atoi( m_IPPort.GetBuffer(2) );
	
#ifdef comment //<-- for using with sockets TCP
	if( !SocketAlreadyOpen )
	{
		sd=OpenSocketClient( cad1, IPPort );
		if(sd>0)
		{
			SocketAlreadyOpen = TRUE;
			m_CommButton.SetWindowText( "Cerrar puerto" ); 
		}
		else
			AfxMessageBox( "No fue posible establecer una conexión" );
	}
	else
	{
		CloseSocket( sd );
		SocketAlreadyOpen = FALSE;
		m_CommButton.SetWindowText( "Abrir puerto" ); 
	}
#endif

	if( !SocketAlreadyOpen )
	{
		int error;
		 
		error = mysox.UDPSocketsCreate( cad1, IPPort);
		if(error == 0 )
		{
			SocketAlreadyOpen = TRUE;
			m_CommButton.SetWindowText( "Cerrar puerto" ); 
		}
		else
			AfxMessageBox( "No fue posible establecer una conexión" );
	}
	else
	{
		//fcloseseSocket( sd );
		SocketAlreadyOpen = FALSE;
		m_CommButton.SetWindowText( "Abrir puerto" ); 
	}
	



#ifdef comment
	if( !SocketAlreadyOpen )
	{
		Error=CreateUDPSocket(client,host,port,UDP_CLIENT);
		
		
		sd=OpenSocketClient( cad1, IPPort );
		if(sd>0)
		{
			SocketAlreadyOpen = TRUE;
			m_CommButton.SetWindowText( "Cerrar puerto" ); 
		}
		else
			AfxMessageBox( "No fue posible establecer una conexión" );
	}
	else
	{
		CloseSocket( sd );
		SocketAlreadyOpen = FALSE;
		m_CommButton.SetWindowText( "Abrir puerto" ); 
	}
#endif
}


void CStillCapDlg::OnRadioTeamcolor() 
{
}


void CStillCapDlg::OnButtonColor() 
{

	char cad[80]; 
	int grad;

	// Get the actual button
	UpdateData(TRUE);

	//////////////////////////////////////////////////////////////////////
	//		Gets the hsi's values for the selected color

		hsi[m_Color].h.win = m_SliderHP.GetPos();
		hsi[m_Color].h.max = m_SliderOH.GetPos() + hsi[m_Color].h.win;
		if( hsi[m_Color].h.max > 360 ) hsi[m_Color].h.max -= 360;

		hsi[m_Color].h.min = m_SliderOH.GetPos() - hsi[m_Color].h.win;
		if( hsi[m_Color].h.min < 0 ) hsi[m_Color].h.min += 360;

		
		hsi[m_Color].s.win = m_SliderSP.GetPos();
		hsi[m_Color].s.max = m_SliderOS.GetPos() + hsi[m_Color].s.win;
		if( hsi[m_Color].s.max > 255 ) hsi[m_Color].s.max = 255;

		hsi[m_Color].s.min = m_SliderOS.GetPos() - hsi[m_Color].s.win;
		if( hsi[m_Color].s.min < 0 ) hsi[m_Color].s.min = 0;
		
		
		hsi[m_Color].i.win = m_SliderIP.GetPos();
		hsi[m_Color].i.max = m_SliderOI.GetPos() + hsi[m_Color].i.win;
		if( hsi[m_Color].i.max > 255 ) hsi[m_Color].i.max = 255;

		hsi[m_Color].i.min = m_SliderOI.GetPos() - hsi[m_Color].i.win;
		if( hsi[m_Color].i.min < 0 ) hsi[m_Color].i.min = 0;




	//////////////////////////////////////////////////////////////////////
	//		Shows the actual hsi's values


	//grad = hsiGetMiddle( hsi[m_Color].h.max, hsi[m_Color].h.min, 255 );
	grad = (hsi[m_Color].h.max + hsi[m_Color].h.min) >> 1;
	sprintf( cad, "%d", grad ); m_Static_OH.SetWindowText( cad );
	m_SliderOH.SetPos( grad );
	
	//grad = hsiGetRange( hsi[m_Color].h.max, hsi[m_Color].h.min, 360 );
	grad = hsi[m_Color].h.win;
	sprintf( cad, "%d", grad ); m_Static_HP.SetWindowText( cad );
	m_SliderHP.SetPos( grad );
	
	
//	grad = hsiGetMiddle( hsi[m_Color].s.max, hsi[m_Color].s.min, 255 );
	grad = (hsi[m_Color].s.max + hsi[m_Color].s.min) >> 1;
	sprintf( cad, "%d", grad ); m_Static_OS.SetWindowText( cad );
	m_SliderOS.SetPos( grad );
	
	//grad = hsiGetRange( hsi[m_Color].s.max, hsi[m_Color].s.min, 255 );
	grad = hsi[m_Color].s.win;
	sprintf( cad, "%d", grad ); m_Static_SP.SetWindowText( cad );
	m_SliderSP.SetPos( grad );
	

	grad = hsiGetMiddle( hsi[m_Color].i.max, hsi[m_Color].i.min, 255 );
	sprintf( cad, "%d", grad ); m_Static_OI.SetWindowText( cad );
	m_SliderOI.SetPos( grad );
	
	//grad = hsiGetRange( hsi[m_Color].i.max, hsi[m_Color].i.min, 255 );
	grad = hsi[m_Color].i.win;
	sprintf( cad, "%d", grad ); m_Static_IP.SetWindowText( cad );
	m_SliderIP.SetPos( grad );

	UpdateData(FALSE);


}


void CStillCapDlg::OnButtonVercolor() 
{
	char cad[80]; 
	int grad;

	// Get the actual button
	UpdateData(TRUE);

	//////////////////////////////////////////////////////////////////////
	//		Show the actual hsi's values

	grad = hsiGetMiddle( hsi[m_Color].h.max, hsi[m_Color].h.min, 360 );
	sprintf( cad, "%d", grad ); m_Static_OH.SetWindowText( cad );
	m_SliderOH.SetPos( grad );
	
	//grad = hsiGetRange( hsi[m_Color].h.max, hsi[m_Color].h.min, 360 );
	grad = hsi[m_Color].h.win;
	sprintf( cad, "%d", grad ); m_Static_HP.SetWindowText( cad );
	m_SliderHP.SetPos( grad );
	
	
	grad = hsiGetMiddle( hsi[m_Color].s.max, hsi[m_Color].s.min, 255 );
	sprintf( cad, "%d", grad ); m_Static_OS.SetWindowText( cad );
	m_SliderOS.SetPos( grad );
	
	//grad = hsiGetRange( hsi[m_Color].s.max, hsi[m_Color].s.min, 255 );
	grad = hsi[m_Color].s.win;
	sprintf( cad, "%d", grad ); m_Static_SP.SetWindowText( cad );
	m_SliderSP.SetPos( grad );
	

	grad = hsiGetMiddle( hsi[m_Color].i.max, hsi[m_Color].i.min, 255 );
	sprintf( cad, "%d", grad ); m_Static_OI.SetWindowText( cad );
	m_SliderOI.SetPos( grad );
	
	//grad = hsiGetRange( hsi[m_Color].i.max, hsi[m_Color].i.min, 255 );
	grad = hsi[m_Color].i.win;
	sprintf( cad, "%d", grad ); m_Static_IP.SetWindowText( cad );
	m_SliderIP.SetPos( grad );

	UpdateData(FALSE);
	
}

void CStillCapDlg::OnButtonColorpicker() 
{
	CColorDialog cd;

	cd.DoModal();
	
}

void CStillCapDlg::OnButtonCalcolor() 
{
	CalibrationInProgress = CalibrationInProgress ? FALSE : TRUE;

	g_bOneShot = CalibrationInProgress;

	CalibrationInProgress ? m_CalColor.SetWindowText( "Terminar" ) : m_CalColor.SetWindowText( "Calibrar" );

}


HSI Rgb2Hsi( RGB rgb )
{
	HSI hsi;
	int theta = (byte)( acos( (2*rgb.r-rgb.g-rgb.b) / (2*sqrt((float)((rgb.r-rgb.g)*(rgb.r-rgb.g)+(rgb.r-rgb.b)*(rgb.g-rgb.b))) )) * 57.29);
		
	hsi.h = rgb.b <= rgb.g ? theta : (360-theta);

	int m = rgb.r <= rgb.g ? rgb.r : rgb.g;
	m = m <= rgb.b ? m : rgb.b;
	m = m <= 0 ? 1 : m;

	float temp = (765 / (float)(rgb.r+rgb.g+rgb.b) );
	temp = temp * (float)m;
	hsi.s = 255- (int)temp;
	
	hsi.i = (int)( (rgb.r+rgb.g+rgb.b) / 3 );

	return hsi;
}

void CStillCapDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( nFlags == MK_CONTROL )
	{

		COLORREF c, cmat[WIN_LENGHT][WIN_LENGHT];
		Point p;

		RGB mat[WIN_LENGHT][WIN_LENGHT];

		int offset = RATIO*WIN_LENGHT/2;


		static int h[RATIO][RATIO] = { {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
					  };
		static int zoom[RATIO*WIN_LENGHT][RATIO*WIN_LENGHT], zoom2[RATIO*WIN_LENGHT][RATIO*WIN_LENGHT];
		static int x, y, i, j;

		static int OldScreen[RATIO*WIN_LENGHT][RATIO*WIN_LENGHT];
		static Point oldp;

		static int xx, yy, row, col;

		if( CalibrationInProgress )
		{
			p.x = point.x - 206;
			p.y = 284 - (point.y - 24);

			if( (p.x < (0+offset)) || (p.x > (348-offset) ) || (p.y < (0+offset) ) || (p.y > (282-offset) ) )
				return;


			HWND hwndStill = NULL;
			GetDlgItem( IDC_STILL, &hwndStill );
			RECT rc;
			::GetWindowRect( hwndStill, &rc );
			
			CClientDC dc( this );


		#ifdef comment1
			// Restore the last screen
			for( row = 0 - (int)(RATIO*WIN_LENGHT/2), xx = 0; row < (int)(RATIO*WIN_LENGHT/2); row++, xx++ )
			{
				for( col = 0 - (int)(RATIO*WIN_LENGHT/2), yy = 0; col < (int)(RATIO*WIN_LENGHT/2); col++, yy++ )
				{
				
					SetPixel( dc, oldp.x+col, oldp.y+row, OldScreen[yy][xx] );
				
				}
			}
		#endif
			
			oldp.x = point.x;
			oldp.y = point.y;
			int row, col;

			// Hold the max and min values of the window
			int aaold=0;

			// Takes the point and gets its rgb components
			for( row = 0 - (int)(WIN_LENGHT/2); row <= (int)(WIN_LENGHT/2); row++ )
			{
				for( col = 0 - (int)(WIN_LENGHT/2); col <= (int)(WIN_LENGHT/2); col++ )
				{
					c = GetPixel( dc, point.x+col, point.y+row );

					cmat[col+WIN_LENGHT/2][row+WIN_LENGHT/2] = c;
				}
			}

		

			hnew.h.min = 360;
			hnew.s.min = 255;
			hnew.i.min = 255;
			
			hnew.h.max = 0;
			hnew.s.max = 0;
			hnew.i.max = 0;

			hnew.h.win = 0;
			hnew.s.win = 0;
			hnew.i.win = 0;
			
			for( col = 0; col < WIN_LENGHT; col++ )
			{
				for( row = 0; row < WIN_LENGHT; row++ )	
				{
					mat[row][col].r = (char)(cmat[row][col] & 0x000000FF);
					mat[row][col].g = (char)((cmat[row][col] & 0x0000FF00)>>8);
					mat[row][col].b = (char)((cmat[row][col] & 0x00FF0000)>>16);

					hm = Rgb2Hsi( mat[row][col] );
					hold.h.min = hm.h;
					hold.s.min = hm.s;
					hold.i.min = hm.i;
					
					hold.h.max = hm.h;
					hold.s.max = hm.s;
					hold.i.max = hm.i;

					hnew.h.win += hm.h;
					hnew.s.win += hm.s;
					hnew.i.win += hm.i;

					hnew.h.min = hold.h.min <= hnew.h.min ? hold.h.min : hnew.h.min;
					hnew.s.min = hold.s.min <= hnew.s.min ? hold.s.min : hnew.s.min;
					hnew.i.min = hold.i.min <= hnew.i.min ? hold.i.min : hnew.i.min;

					hnew.h.max = hold.h.max > hnew.h.max ? hold.h.max : hnew.h.max;
					hnew.s.max = hold.s.max > hnew.s.max ? hold.s.max : hnew.s.max;
					hnew.i.max = hold.i.max > hnew.i.max ? hold.i.max : hnew.i.max;
				}
			}
			
			hnew.h.win = (int)( hnew.h.win/(col*row) );
			hnew.s.win = (int)( hnew.s.win/(col*row) );
			hnew.i.win = (int)( hnew.i.win/(col*row) );

			
			// Save the pixel value for transforming it later (if necessary)
			PickedColor.r = mat[WIN_LENGHT/2][WIN_LENGHT/2].r;
			PickedColor.g = mat[WIN_LENGHT/2][WIN_LENGHT/2].g;
			PickedColor.b = mat[WIN_LENGHT/2][WIN_LENGHT/2].b;

			// Clear the matrix
			for( y = 0; y < RATIO*WIN_LENGHT; y++ )
			{
				for( x = 0; x < RATIO*WIN_LENGHT; x++ )
				{
					zoom[x][y] = 0;
					zoom2[x][y] = 0;
				}
			}

			// Fill out the matrix so it can left blanck spaces (1st step to zoom)
			for( y = 0, i = 0; y < RATIO*WIN_LENGHT; y += RATIO, i++ )
			{
				for( x = 0, j = 0; x < RATIO*WIN_LENGHT; x += RATIO, j++ )
				{
					zoom[x][y] = cmat[j][i];
				}
			}


			// 2D convolution sum between the 'zoom' matrix and the kernel
			for(  x = 0; x < RATIO*WIN_LENGHT; x++ )
			{
				for(  y = 0; y < RATIO*WIN_LENGHT; y++ )
				{
					zoom2[x][y] = 0;

					for(  i = 0; i < RATIO; i++ )
					{
						if( ((x-i)<0) || ((x-i)>=RATIO*WIN_LENGHT) ) 
							continue;

						for(  j = 0; j < RATIO; j++ )
						{
							if( ((y-j)<0) || ((y-j)>=RATIO*WIN_LENGHT) ) 
								continue;
							
							zoom2[x][y] += zoom[x-i][y-j]*h[i][j];
						}
					}
				}
			}		

		#ifdef comment2
			// Draw the 'zoom2' matrix and save the old pixels values for writing them later
			for( row = 0 - (RATIO*WIN_LENGHT/2), xx = 0; row < RATIO*WIN_LENGHT/2; row++, xx++ )
			{
				for( int col = 0 - (RATIO*WIN_LENGHT/2), yy = 0; col < RATIO*WIN_LENGHT/2; col++, yy++ )
				{
				
					OldScreen[yy][xx] = GetPixel( dc, point.x+col, point.y+row );
					SetPixel( dc, point.x+col, point.y+row, zoom2[yy][xx] );
				
				}
			}
		#endif
			
		}
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void CStillCapDlg::OnButtonHistogram() 
{
	// TODO: Add your control notification handler code here
	
}

void CStillCapDlg::OnButtonCalibcancha() 
{
	if( !CalibrationCancha  )
		CalibrationCancha  = TRUE;
	else
		CalibrationCancha  = FALSE;

	CalibrationCancha ? m_CalibCancha.SetWindowText( "Terminar" ) : m_CalibCancha.SetWindowText( "Calibrar" );
	
}

void CStillCapDlg::OnButtonCanchadat() 
{
	UpdateData( TRUE );

	FieldLim.corner0.x = m_CanchaX0;
	FieldLim.corner0.y = m_CanchaY0;
	FieldLim.corner1.x = m_CanchaX1;
	FieldLim.corner1.y = m_CanchaY1; 
}

void CStillCapDlg::OnButtonSave() 
{
	char str[255], filename[255];
	Persistence save( '#', ',' );

	

	// FALSE to save, TRUE for open
	CFileDialog filedlg( FALSE, NULL,NULL,OFN_OVERWRITEPROMPT,
						 "Archivos de configuración (*.cfg)|*.cfg|Todos(*.*)|*.*||");

	CString strFileName;
    
	int fret= filedlg.DoModal();
	
	strFileName = filedlg.GetPathName();
	if(fret == IDOK){
		sprintf( filename, "%s", strFileName );
		if( save.Open( filename, WRITE_FILE ) ){

			save.setTag( "TeamColor" );
			itoa(TeamColor, str, 10 );
			save.lastToken( str );

			save.setTag( "FrameRate" );
			itoa( FrameRate, str, 10 );
			save.lastToken( str );

			save.setTag( "Communications" );
			save.addToken( IPLocal );
			save.addToken( IPRemote );
			itoa( IPPort, str, 10 );
			save.lastToken( str );

			save.setTag( "MinBlobArea" );
			itoa( MinBlobArea, str, 10 );
			save.lastToken( str );

			save.setTag( "MaxDistance" );
			itoa ( MaxDist, str, 10 );
			save.lastToken( str );


			save.setTag( "PhysicalFieldLimits" );
			sprintf( str, "%f", FieldLim.corner0.x );
			save.addToken( str );
			sprintf( str, "%f", FieldLim.corner0.y );
			save.addToken( str );
			sprintf( str, "%f", FieldLim.corner1.x );
			save.addToken( str );						
			sprintf( str, "%f", FieldLim.corner1.y );
			save.lastToken( str );
			
			/* could be done in this way, but then it would be got married with the ','
			* delimiter, and harder to parse later
			*
			sprintf( str, "%f,%f,%f,%f", 
								FieldLim.corner0.x, FieldLim.corner0.y,
								FieldLim.corner1.x, FieldLim.corner1.y );
			save.lastToken( str );
			*/
			
			save.setTag( "LogicalFieldLimits" );
			sprintf( str, "%d", FieldCoordsDown.corner0.x );
			save.addToken( str );
			sprintf( str, "%d", FieldCoordsDown.corner0.y );
			save.addToken( str );
			sprintf( str, "%d", FieldCoordsDown.corner1.x );
			save.addToken( str );						
			sprintf( str, "%d", FieldCoordsDown.corner1.y );
			save.lastToken( str );
			
			save.setTag( "HsiParameters" );
			for( int icont = 0; icont < 5; icont++ ){
				itoa( hsi[icont].h.max, str, 10 );
				save.addToken( str );
				itoa( hsi[icont].h.min, str, 10 );
				save.addToken( str );
				itoa( hsi[icont].h.win, str, 10 );
				save.addToken( str );
				
				itoa( hsi[icont].s.max, str, 10 );
				save.addToken( str );
				itoa( hsi[icont].s.min, str, 10 );
				save.addToken( str );
				itoa( hsi[icont].s.win, str, 10 );
				save.addToken( str );

				itoa( hsi[icont].i.max, str, 10 );
				save.addToken( str );
				itoa( hsi[icont].i.min, str, 10 );
				save.addToken( str );
				itoa( hsi[icont].i.win, str, 10 );
				save.addToken( str );
			}
			itoa( hsi[icont].h.max, str, 10 );
			save.addToken( str );
			itoa( hsi[icont].h.min, str, 10 );
			save.addToken( str );
			itoa( hsi[icont].h.win, str, 10 );
			save.addToken( str );

			itoa( hsi[icont].s.max, str, 10 );
			save.addToken( str );
			itoa( hsi[icont].s.min, str, 10 );
			save.addToken( str );
			itoa( hsi[icont].s.win, str, 10 );
			save.addToken( str );

			itoa( hsi[icont].i.max, str, 10 );
			save.addToken( str );
			itoa( hsi[icont].i.min, str, 10 );
			save.addToken( str );
			itoa( hsi[icont].i.win, str, 10 );
			save.lastToken( str ); // <- lastToken

			save.Close();
			MessageBox("Operación completada!");
		}
		else
			MessageBox("Error: El archivo no pudo ser abierto");
		
	}
}

void CStillCapDlg::OnButtonLoad() 
{
	char str[255], filename[255];
	Persistence *load = new Persistence( '#', ',' );

	// FALSE to save, TRUE for open
	CFileDialog filedlg( TRUE, NULL,NULL,OFN_OVERWRITEPROMPT,
						 "Archivos de configuración (*.cfg)|*.cfg|Todos(*.*)|*.*||");

   	if( filedlg.DoModal() == IDOK ){
		CString strFileName = filedlg.GetPathName();
		sprintf( filename, "%s", strFileName );
		if( load->Open( filename, READ_FILE ) ){
/*============================================================================
							TeamColor
============================================================================*/			
			if( load->seekTag( "TeamColor" ) ){
				load->nextToken( str );
				TeamColor = atoi( str );

				m_TeamColor = TeamColor;
			}
			else
				MessageBox( "No se encontró el campo <TeamColor>" );

/*============================================================================
							FrameRate
============================================================================*/			
			if( load->seekTag( "FrameRate" ) ){
				load->nextToken( str );
				FrameRate = atoi( str );

				switch( FrameRate ){
					case 40: m_FrameRate.SetCurSel(0); break;
					case 50: m_FrameRate.SetCurSel(1); break;
					case 66: m_FrameRate.SetCurSel(2); break;
					case 100: m_FrameRate.SetCurSel(3); break;
					case 200: m_FrameRate.SetCurSel(4); break;
					case 500: m_FrameRate.SetCurSel(5); break;
					case 1000: m_FrameRate.SetCurSel(6); break;
					case 2000: m_FrameRate.SetCurSel(7); break;
				}

			}
			else
				MessageBox( "No se encontró el campo <FrameRate>" );

/*============================================================================
							Communications
============================================================================*/
			if( load->seekTag( "Communications" ) ){
				load->nextToken( str ); 
				strcpy( IPLocal, str );
				load->nextToken( str ); 
				strcpy( IPRemote, str );
				load->nextToken( str );
				IPPort = atoi( str );

				m_IPPort = str;
				m_IPLocal.SetWindowText( IPLocal );	
				m_IPRemote.SetWindowText( IPRemote );
			}
			else
				MessageBox( "No se encontró el campo <Communications>" );

/*============================================================================
							MinBlobArea
============================================================================*/			
			if( load->seekTag( "MinBlobArea" ) ){
				load->nextToken( str );
				MinBlobArea = atoi( str );
				m_MinBlob.SetPos(MinBlobArea);
				m_MinBlobButt = str;
			}
			else
				MessageBox( "No se encontró el campo <MinBlobArea>" );

/*============================================================================
							MaxDistance
============================================================================*/			
			if( load->seekTag( "MaxDistance" ) ){
				load->nextToken( str );
				MaxDist = atoi( str );
				m_DistMaxBlSl.SetPos( MaxDist );
				m_DistMaxBl = MaxDist;
			}
			else
				MessageBox( "No se encontró el campo <MaxDistance>" );

/*============================================================================
							PhysicalFieldLimits
============================================================================*/			
			if( load->seekTag( "PhysicalFieldLimits" ) ){
				load->nextToken( str );
				FieldLim.corner0.x = atof( str );
				m_CanchaX0 = FieldLim.corner0.x;
				
				load->nextToken( str );
				FieldLim.corner0.y = atof( str );
				m_CanchaY0 = FieldLim.corner0.y;

				load->nextToken( str );
				FieldLim.corner1.x = atof( str );
				m_CanchaX1 = FieldLim.corner1.x;

				load->nextToken( str );
				FieldLim.corner1.y = atof( str );
				m_CanchaY1 = FieldLim.corner1.y;
			}
			else
				MessageBox( "No se encontró el campo <PhysicalFieldLimits>" );

/*============================================================================
							LogicalFieldLimits
============================================================================*/
			if( load->seekTag( "LogicalFieldLimits" ) ){
				load->nextToken( str );
				FieldCoordsDown.corner0.x = atof( str );

				load->nextToken( str );
				FieldCoordsDown.corner0.y = atof( str );

				load->nextToken( str );
				FieldCoordsDown.corner1.x = atof( str );

				load->nextToken( str );
				FieldCoordsDown.corner1.y = atof( str );
			}
			else
				MessageBox( "No se encontró el campo <LogicalFieldLimits>" );

/*============================================================================
							HsiParameters
============================================================================*/
			if( load->seekTag( "HsiParameters" ) ){
				char cad[80]; 
				int grad;
				
				for( int cont = 0; cont < 6; cont++ ){
					load->nextToken( str );
					hsi[cont].h.max = atoi( str );
					load->nextToken( str );
					hsi[cont].h.min = atoi( str );
					load->nextToken( str );
					hsi[cont].h.win = atoi( str );

					load->nextToken( str );
					hsi[cont].s.max = atoi( str );
					load->nextToken( str );
					hsi[cont].s.min = atoi( str );
					load->nextToken( str );
					hsi[cont].s.win = atoi( str );

					load->nextToken( str );
					hsi[cont].i.max = atoi( str );
					load->nextToken( str );
					hsi[cont].i.min = atoi( str );
					load->nextToken( str );
					hsi[cont].i.win = atoi( str );
				}
				m_Color = orange;

				for( int cnt = orange; cnt < cyan; cnt++ ){
					grad = hsiGetMiddle( hsi[cnt].h.max, hsi[cnt].h.min, 360 );
					sprintf( cad, "%d", grad ); m_Static_OH.SetWindowText( cad );
					m_SliderOH.SetPos( grad );
					
					grad = hsi[cnt].h.win;
					sprintf( cad, "%d", grad ); m_Static_HP.SetWindowText( cad );
					m_SliderHP.SetPos( grad );
					
					
					grad = hsiGetMiddle( hsi[cnt].s.max, hsi[cnt].s.min, 255 );
					sprintf( cad, "%d", grad ); m_Static_OS.SetWindowText( cad );
					m_SliderOS.SetPos( grad );
					
					grad = hsi[cnt].s.win;
					sprintf( cad, "%d", grad ); m_Static_SP.SetWindowText( cad );
					m_SliderSP.SetPos( grad );
					

					grad = hsiGetMiddle( hsi[cnt].i.max, hsi[cnt].i.min, 255 );
					sprintf( cad, "%d", grad ); m_Static_OI.SetWindowText( cad );
					m_SliderOI.SetPos( grad );
					
					grad = hsi[cnt].i.win;
					sprintf( cad, "%d", grad ); m_Static_IP.SetWindowText( cad );
					m_SliderIP.SetPos( grad );
				}
			}
			else
				MessageBox( "No se encontró el campo <HsiParameters>" );
		
			
			UpdateData( FALSE );
			MessageBox( "Archivo leído" );

			
		}
		else
			MessageBox("Error: El archivo no pudo ser abierto");
	}
	delete load;
}

void CStillCapDlg::OnCommBurron() 
{
	UpdateData( TRUE );
	IPPort = atoi( m_IPPort.GetBuffer(5) );
	m_IPLocal.GetWindowText( IPLocal, 80 );	
	m_IPRemote.GetWindowText( IPRemote, 80 );	
}

void CStillCapDlg::OnCheckVideop(){
	VideoP = ~VideoP;
}

void CStillCapDlg::OnButtonDistor() 
{
    UpdateData( TRUE );
	distor=distor ? FALSE : TRUE;
	if( distor )m_Distor.SetWindowText("Con Distorsion");
	else m_Distor.SetWindowText( "Sin Distorsion" );   
	if (m_centro==0)
	{
		centrox=puntomedioentero(MAX_RES_X);//para mandarle el centro como punto medio a la distorsión
		centroy=puntomedioentero(MAX_RES_Y);//para mandarle el centro como punto medio a la distorsión
	}
    else 
	{
//		centrox=puntomedioentero(MAX_RES_X-20);//para mandarle el centro como punto medio a la distorsión
		centroy=puntomedioentero(MAX_RES_Y);//para mandarle el centro como punto medio a la distorsión
	}
	if (m_distorsion==0)
	{
		disto_a=(float)(-0.1);
		disto_b=(float)(0.25);
		disto_c=(float)(0.0);
	}
/*	else
	{
		disto_a=-0.1;
		disto_b=0.05;
		disto_c=0.50;

	}
*/	
}

void CStillCapDlg::OnCheckCentro() 
{
	m_centro = ~m_centro;	
}

void CStillCapDlg::OnCheckDistor() 
{
	m_distorsion=~m_distorsion;
}

void CStillCapDlg::OnSelchangeListDista() 
{
	switch(m_lisdista.GetCurSel())
	{
	case 0: disto_a=(float)(0.99); break;
	case 1: disto_a=(float)(0.98); break;
	case 2: disto_a=(float)(0.97); break;
	case 3: disto_a=(float)(0.96); break;
	case 4: disto_a=(float)(0.95); break;
	case 5: disto_a=(float)(0.94); break;
	case 6: disto_a=(float)(0.93); break;
	case 7: disto_a=(float)(0.92); break;
	case 8: disto_a=(float)(0.91); break;
	case 9: disto_a=(float)(0.90); break;
	case 10: disto_a=(float)(0.89); break;
	case 11: disto_a=(float)(0.88); break;
	case 12: disto_a=(float)(0.87); break;
	case 13: disto_a=(float)(0.86); break;
	case 14: disto_a=(float)(0.85); break;
	case 15: disto_a=(float)(0.84); break;
	case 16: disto_a=(float)(0.83); break;
	case 17: disto_a=(float)(0.82); break;
	case 18: disto_a=(float)(0.81); break;
	case 19: disto_a=(float)(0.80); break;
	case 20: disto_a=(float)(0.79); break;
	case 21: disto_a=(float)(0.78); break;
	case 22: disto_a=(float)(0.77); break;
	case 23: disto_a=(float)(0.76); break;
	case 24: disto_a=(float)(0.75); break;
	case 25: disto_a=(float)(0.74); break;
	case 26: disto_a=(float)(0.73); break;
	case 27: disto_a=(float)(0.72); break;
	case 28: disto_a=(float)(0.71); break;
	case 29: disto_a=(float)(0.70); break;
	case 30: disto_a=(float)(0.69); break;
	case 31: disto_a=(float)(0.68); break;
	case 32: disto_a=(float)(0.67); break;
	case 33: disto_a=(float)(0.66); break;
	case 34: disto_a=(float)(0.65); break;
	case 35: disto_a=(float)(0.64); break;
	case 36: disto_a=(float)(0.63); break;
	case 37: disto_a=(float)(0.62); break;
	case 38: disto_a=(float)(0.61); break;
	case 39: disto_a=(float)(0.60); break;
	case 40: disto_a=(float)(0.59); break;
	case 41: disto_a=(float)(0.58); break;
	case 42: disto_a=(float)(0.57); break;
	case 43: disto_a=(float)(0.56); break;
	case 44: disto_a=(float)(0.55); break;
	case 45: disto_a=(float)(0.54); break;
	case 46: disto_a=(float)(0.53); break;
	case 47: disto_a=(float)(0.52); break;
	case 48: disto_a=(float)(0.51); break;
	case 49: disto_a=(float)(0.50); break;
	case 50: disto_a=(float)(0.49); break;
	case 51: disto_a=(float)(0.48); break;
	case 52: disto_a=(float)(0.47); break;
	case 53: disto_a=(float)(0.46); break;
	case 54: disto_a=(float)(0.45); break;
	case 55: disto_a=(float)(0.44); break;
	case 56: disto_a=(float)(0.43); break;
	case 57: disto_a=(float)(0.42); break;
	case 58: disto_a=(float)(0.41); break;
	case 59: disto_a=(float)(0.40); break;
	case 60: disto_a=(float)(0.39); break;
	case 61: disto_a=(float)(0.38); break;
	case 62: disto_a=(float)(0.37); break;
	case 63: disto_a=(float)(0.36); break;
	case 64: disto_a=(float)(0.35); break;
	case 65: disto_a=(float)(0.34); break;
	case 66: disto_a=(float)(0.33); break;
	case 67: disto_a=(float)(0.32); break;
	case 68: disto_a=(float)(0.31); break;
	case 69: disto_a=(float)(0.30); break;
	case 70: disto_a=(float)(0.29); break;
	case 71: disto_a=(float)(0.28); break;
	case 72: disto_a=(float)(0.27); break;
	case 73: disto_a=(float)(0.26); break;
	case 74: disto_a=(float)(0.25); break;
	case 75: disto_a=(float)(0.24); break;
	case 76: disto_a=(float)(0.23); break;
	case 77: disto_a=(float)(0.22); break;
	case 78: disto_a=(float)(0.21); break;
	case 79: disto_a=(float)(0.20); break;
	case 80: disto_a=(float)(0.19); break;
	case 81: disto_a=(float)(0.18); break;
	case 82: disto_a=(float)(0.17); break;
	case 83: disto_a=(float)(0.16); break;
	case 84: disto_a=(float)(0.15); break;
	case 85: disto_a=(float)(0.14); break;
	case 86: disto_a=(float)(0.13); break;
	case 87: disto_a=(float)(0.12); break;
	case 88: disto_a=(float)(0.11); break;
	case 89: disto_a=(float)(0.10); break;
	case 90: disto_a=(float)(0.09); break;
	case 91: disto_a=(float)(0.08); break;
	case 92: disto_a=(float)(0.07); break;
	case 93: disto_a=(float)(0.06); break;
	case 94: disto_a=(float)(0.05); break;
	case 95: disto_a=(float)(0.04); break;
	case 96: disto_a=(float)(0.03); break;
	case 97: disto_a=(float)(0.02); break;
	case 98: disto_a=(float)(0.01); break;
	case 99: disto_a=(float)(0.00); break;
	case 100: disto_a=(float)(-0.01); break;
	case 101: disto_a=(float)(-0.02); break;
	case 102: disto_a=(float)(-0.03); break;
	case 103: disto_a=(float)(-0.04); break;
	case 104: disto_a=(float)(-0.05); break;
	case 105: disto_a=(float)(-0.06); break;
	case 106: disto_a=(float)(-0.07); break;
	case 107: disto_a=(float)(-0.08); break;
	case 108: disto_a=(float)(-0.09); break;
	case 109: disto_a=(float)(-0.10); break;
	case 110: disto_a=(float)(-0.11); break;
	case 111: disto_a=(float)(-0.12); break;
	case 112: disto_a=(float)(-0.13); break;
	case 113: disto_a=(float)(-0.14); break;
	case 114: disto_a=(float)(-0.15); break;
	case 115: disto_a=(float)(-0.16); break;
	case 116: disto_a=(float)(-0.17); break;
	case 117: disto_a=(float)(-0.18); break;
	case 118: disto_a=(float)(-0.19); break;
	case 119: disto_a=(float)(-0.20); break;
	case 120: disto_a=(float)(-0.21); break;
	case 121: disto_a=(float)(-0.22); break;
	case 122: disto_a=(float)(-0.23); break;
	case 123: disto_a=(float)(-0.24); break;
	case 124: disto_a=(float)(-0.25); break;
	case 125: disto_a=(float)(-0.26); break;
	case 126: disto_a=(float)(-0.27); break;
	case 127: disto_a=(float)(-0.28); break;
	case 128: disto_a=(float)(-0.29); break;
	case 129: disto_a=(float)(-0.30); break;
	case 130: disto_a=(float)(-0.31); break;
	case 131: disto_a=(float)(-0.32); break;
	case 132: disto_a=(float)(-0.33); break;
	case 133: disto_a=(float)(-0.34); break;
	case 134: disto_a=(float)(-0.35); break;
	case 135: disto_a=(float)(-0.36); break;
	case 136: disto_a=(float)(-0.37); break;
	case 137: disto_a=(float)(-0.38); break;
	case 138: disto_a=(float)(-0.39); break;
	case 139: disto_a=(float)(-0.40); break;
	case 140: disto_a=(float)(-0.41); break;
	case 141: disto_a=(float)(-0.42); break;
	case 142: disto_a=(float)(-0.43); break;
	case 143: disto_a=(float)(-0.44); break;
	case 144: disto_a=(float)(-0.45); break;
	case 145: disto_a=(float)(-0.46); break;
	case 146: disto_a=(float)(-0.47); break;
	case 147: disto_a=(float)(-0.48); break;
	case 148: disto_a=(float)(-0.49); break;
	case 149: disto_a=(float)(-0.50); break;
	case 150: disto_a=(float)(-0.51); break;
	case 151: disto_a=(float)(-0.52); break;
	case 152: disto_a=(float)(-0.53); break;
	case 153: disto_a=(float)(-0.54); break;
	case 154: disto_a=(float)(-0.55); break;
	case 155: disto_a=(float)(-0.56); break;
	case 156: disto_a=(float)(-0.57); break;
	case 157: disto_a=(float)(-0.58); break;
	case 158: disto_a=(float)(-0.59); break;
	case 159: disto_a=(float)(-0.60); break;
	case 160: disto_a=(float)(-0.61); break;
	case 161: disto_a=(float)(-0.62); break;
	case 162: disto_a=(float)(-0.63); break;
	case 163: disto_a=(float)(-0.64); break;
	case 164: disto_a=(float)(-0.65); break;
	case 165: disto_a=(float)(-0.66); break;
	case 166: disto_a=(float)(-0.67); break;
	case 167: disto_a=(float)(-0.68); break;
	case 168: disto_a=(float)(-0.69); break;
	case 169: disto_a=(float)(-0.70); break;
	case 170: disto_a=(float)(-0.71); break;
	case 171: disto_a=(float)(-0.72); break;
	case 172: disto_a=(float)(-0.73); break;
	case 173: disto_a=(float)(-0.74); break;
	case 174: disto_a=(float)(-0.75); break;
	case 175: disto_a=(float)(-0.76); break;
	case 176: disto_a=(float)(-0.77); break;
	case 177: disto_a=(float)(-0.78); break;
	case 178: disto_a=(float)(-0.79); break;
	case 179: disto_a=(float)(-0.80); break;
	case 180: disto_a=(float)(-0.81); break;
	case 181: disto_a=(float)(-0.82); break;
	case 182: disto_a=(float)(-0.83); break;
	case 183: disto_a=(float)(-0.84); break;
	case 184: disto_a=(float)(-0.85); break;
	case 185: disto_a=(float)(-0.86); break;
	case 186: disto_a=(float)(-0.87); break;
	case 187: disto_a=(float)(-0.88); break;
	case 188: disto_a=(float)(-0.89); break;
	case 189: disto_a=(float)(-0.90); break;
	case 190: disto_a=(float)(-0.91); break;
	case 191: disto_a=(float)(-0.92); break;
	case 192: disto_a=(float)(-0.93); break;
	case 193: disto_a=(float)(-0.94); break;
	case 194: disto_a=(float)(-0.95); break;
	case 195: disto_a=(float)(-0.96); break;
	case 196: disto_a=(float)(-0.97); break;
	case 197: disto_a=(float)(-0.98); break;
	case 198: disto_a=(float)(-0.99); break;
	default: disto_a=(float)(-0.10); 
	}
}

void CStillCapDlg::OnSelchangeListDistb() 
{
	switch(m_lisdistb.GetCurSel())
	{
	case 0: disto_b=(float)(0.99); break;
	case 1: disto_b=(float)(0.98); break;
	case 2: disto_b=(float)(0.97); break;
	case 3: disto_b=(float)(0.96); break;
	case 4: disto_b=(float)(0.95); break;
	case 5: disto_b=(float)(0.94); break;
	case 6: disto_b=(float)(0.93); break;
	case 7: disto_b=(float)(0.92); break;
	case 8: disto_b=(float)(0.91); break;
	case 9: disto_b=(float)(0.90); break;
	case 10: disto_b=(float)(0.89); break;
	case 11: disto_b=(float)(0.88); break;
	case 12: disto_b=(float)(0.87); break;
	case 13: disto_b=(float)(0.86); break;
	case 14: disto_b=(float)(0.85); break;
	case 15: disto_b=(float)(0.84); break;
	case 16: disto_b=(float)(0.83); break;
	case 17: disto_b=(float)(0.82); break;
	case 18: disto_b=(float)(0.81); break;
	case 19: disto_b=(float)(0.80); break;
	case 20: disto_b=(float)(0.79); break;
	case 21: disto_b=(float)(0.78); break;
	case 22: disto_b=(float)(0.77); break;
	case 23: disto_b=(float)(0.76); break;
	case 24: disto_b=(float)(0.75); break;
	case 25: disto_b=(float)(0.74); break;
	case 26: disto_b=(float)(0.73); break;
	case 27: disto_b=(float)(0.72); break;
	case 28: disto_b=(float)(0.71); break;
	case 29: disto_b=(float)(0.70); break;
	case 30: disto_b=(float)(0.69); break;
	case 31: disto_b=(float)(0.68); break;
	case 32: disto_b=(float)(0.67); break;
	case 33: disto_b=(float)(0.66); break;
	case 34: disto_b=(float)(0.65); break;
	case 35: disto_b=(float)(0.64); break;
	case 36: disto_b=(float)(0.63); break;
	case 37: disto_b=(float)(0.62); break;
	case 38: disto_b=(float)(0.61); break;
	case 39: disto_b=(float)(0.60); break;
	case 40: disto_b=(float)(0.59); break;
	case 41: disto_b=(float)(0.58); break;
	case 42: disto_b=(float)(0.57); break;
	case 43: disto_b=(float)(0.56); break;
	case 44: disto_b=(float)(0.55); break;
	case 45: disto_b=(float)(0.54); break;
	case 46: disto_b=(float)(0.53); break;
	case 47: disto_b=(float)(0.52); break;
	case 48: disto_b=(float)(0.51); break;
	case 49: disto_b=(float)(0.50); break;
	case 50: disto_b=(float)(0.49); break;
	case 51: disto_b=(float)(0.48); break;
	case 52: disto_b=(float)(0.47); break;
	case 53: disto_b=(float)(0.46); break;
	case 54: disto_b=(float)(0.45); break;
	case 55: disto_b=(float)(0.44); break;
	case 56: disto_b=(float)(0.43); break;
	case 57: disto_b=(float)(0.42); break;
	case 58: disto_b=(float)(0.41); break;
	case 59: disto_b=(float)(0.40); break;
	case 60: disto_b=(float)(0.39); break;
	case 61: disto_b=(float)(0.38); break;
	case 62: disto_b=(float)(0.37); break;
	case 63: disto_b=(float)(0.36); break;
	case 64: disto_b=(float)(0.35); break;
	case 65: disto_b=(float)(0.34); break;
	case 66: disto_b=(float)(0.33); break;
	case 67: disto_b=(float)(0.32); break;
	case 68: disto_b=(float)(0.31); break;
	case 69: disto_b=(float)(0.30); break;
	case 70: disto_b=(float)(0.29); break;
	case 71: disto_b=(float)(0.28); break;
	case 72: disto_b=(float)(0.27); break;
	case 73: disto_b=(float)(0.26); break;
	case 74: disto_b=(float)(0.25); break;
	case 75: disto_b=(float)(0.24); break;
	case 76: disto_b=(float)(0.23); break;
	case 77: disto_b=(float)(0.22); break;
	case 78: disto_b=(float)(0.21); break;
	case 79: disto_b=(float)(0.20); break;
	case 80: disto_b=(float)(0.19); break;
	case 81: disto_b=(float)(0.18); break;
	case 82: disto_b=(float)(0.17); break;
	case 83: disto_b=(float)(0.16); break;
	case 84: disto_b=(float)(0.15); break;
	case 85: disto_b=(float)(0.14); break;
	case 86: disto_b=(float)(0.13); break;
	case 87: disto_b=(float)(0.12); break;
	case 88: disto_b=(float)(0.11); break;
	case 89: disto_b=(float)(0.10); break;
	case 90: disto_b=(float)(0.09); break;
	case 91: disto_b=(float)(0.08); break;
	case 92: disto_b=(float)(0.07); break;
	case 93: disto_b=(float)(0.06); break;
	case 94: disto_b=(float)(0.05); break;
	case 95: disto_b=(float)(0.04); break;
	case 96: disto_b=(float)(0.03); break;
	case 97: disto_b=(float)(0.02); break;
	case 98: disto_b=(float)(0.01); break;
	case 99: disto_b=(float)(0.00); break;
	case 100: disto_b=(float)(-0.01); break;
	case 101: disto_b=(float)(-0.02); break;
	case 102: disto_b=(float)(-0.03); break;
	case 103: disto_b=(float)(-0.04); break;
	case 104: disto_b=(float)(-0.05); break;
	case 105: disto_b=(float)(-0.06); break;
	case 106: disto_b=(float)(-0.07); break;
	case 107: disto_b=(float)(-0.08); break;
	case 108: disto_b=(float)(-0.09); break;
	case 109: disto_b=(float)(-0.10); break;
	case 110: disto_b=(float)(-0.11); break;
	case 111: disto_b=(float)(-0.12); break;
	case 112: disto_b=(float)(-0.13); break;
	case 113: disto_b=(float)(-0.14); break;
	case 114: disto_b=(float)(-0.15); break;
	case 115: disto_b=(float)(-0.16); break;
	case 116: disto_b=(float)(-0.17); break;
	case 117: disto_b=(float)(-0.18); break;
	case 118: disto_b=(float)(-0.19); break;
	case 119: disto_b=(float)(-0.20); break;
	case 120: disto_b=(float)(-0.21); break;
	case 121: disto_b=(float)(-0.22); break;
	case 122: disto_b=(float)(-0.23); break;
	case 123: disto_b=(float)(-0.24); break;
	case 124: disto_b=(float)(-0.25); break;
	case 125: disto_b=(float)(-0.26); break;
	case 126: disto_b=(float)(-0.27); break;
	case 127: disto_b=(float)(-0.28); break;
	case 128: disto_b=(float)(-0.29); break;
	case 129: disto_b=(float)(-0.30); break;
	case 130: disto_b=(float)(-0.31); break;
	case 131: disto_b=(float)(-0.32); break;
	case 132: disto_b=(float)(-0.33); break;
	case 133: disto_b=(float)(-0.34); break;
	case 134: disto_b=(float)(-0.35); break;
	case 135: disto_b=(float)(-0.36); break;
	case 136: disto_b=(float)(-0.37); break;
	case 137: disto_b=(float)(-0.38); break;
	case 138: disto_b=(float)(-0.39); break;
	case 139: disto_b=(float)(-0.40); break;
	case 140: disto_b=(float)(-0.41); break;
	case 141: disto_b=(float)(-0.42); break;
	case 142: disto_b=(float)(-0.43); break;
	case 143: disto_b=(float)(-0.44); break;
	case 144: disto_b=(float)(-0.45); break;
	case 145: disto_b=(float)(-0.46); break;
	case 146: disto_b=(float)(-0.47); break;
	case 147: disto_b=(float)(-0.48); break;
	case 148: disto_b=(float)(-0.49); break;
	case 149: disto_b=(float)(-0.50); break;
	case 150: disto_b=(float)(-0.51); break;
	case 151: disto_b=(float)(-0.52); break;
	case 152: disto_b=(float)(-0.53); break;
	case 153: disto_b=(float)(-0.54); break;
	case 154: disto_b=(float)(-0.55); break;
	case 155: disto_b=(float)(-0.56); break;
	case 156: disto_b=(float)(-0.57); break;
	case 157: disto_b=(float)(-0.58); break;
	case 158: disto_b=(float)(-0.59); break;
	case 159: disto_b=(float)(-0.60); break;
	case 160: disto_b=(float)(-0.61); break;
	case 161: disto_b=(float)(-0.62); break;
	case 162: disto_b=(float)(-0.63); break;
	case 163: disto_b=(float)(-0.64); break;
	case 164: disto_b=(float)(-0.65); break;
	case 165: disto_b=(float)(-0.66); break;
	case 166: disto_b=(float)(-0.67); break;
	case 167: disto_b=(float)(-0.68); break;
	case 168: disto_b=(float)(-0.69); break;
	case 169: disto_b=(float)(-0.70); break;
	case 170: disto_b=(float)(-0.71); break;
	case 171: disto_b=(float)(-0.72); break;
	case 172: disto_b=(float)(-0.73); break;
	case 173: disto_b=(float)(-0.74); break;
	case 174: disto_b=(float)(-0.75); break;
	case 175: disto_b=(float)(-0.76); break;
	case 176: disto_b=(float)(-0.77); break;
	case 177: disto_b=(float)(-0.78); break;
	case 178: disto_b=(float)(-0.79); break;
	case 179: disto_b=(float)(-0.80); break;
	case 180: disto_b=(float)(-0.81); break;
	case 181: disto_b=(float)(-0.82); break;
	case 182: disto_b=(float)(-0.83); break;
	case 183: disto_b=(float)(-0.84); break;
	case 184: disto_b=(float)(-0.85); break;
	case 185: disto_b=(float)(-0.86); break;
	case 186: disto_b=(float)(-0.87); break;
	case 187: disto_b=(float)(-0.88); break;
	case 188: disto_b=(float)(-0.89); break;
	case 189: disto_b=(float)(-0.90); break;
	case 190: disto_b=(float)(-0.91); break;
	case 191: disto_b=(float)(-0.92); break;
	case 192: disto_b=(float)(-0.93); break;
	case 193: disto_b=(float)(-0.94); break;
	case 194: disto_b=(float)(-0.95); break;
	case 195: disto_b=(float)(-0.96); break;
	case 196: disto_b=(float)(-0.97); break;
	case 197: disto_b=(float)(-0.98); break;
	case 198: disto_b=(float)(-0.99); break;
	default: disto_b=(float)(-0.25); 
	}
}

void CStillCapDlg::OnSelchangeListDistc() 
{
	switch(m_lisdistc.GetCurSel())
	{
	case 0: disto_c=(float)(0.99); break;
	case 1: disto_c=(float)(0.98); break;
	case 2: disto_c=(float)(0.97); break;
	case 3: disto_c=(float)(0.96); break;
	case 4: disto_c=(float)(0.95); break;
	case 5: disto_c=(float)(0.94); break;
	case 6: disto_c=(float)(0.93); break;
	case 7: disto_c=(float)(0.92); break;
	case 8: disto_c=(float)(0.91); break;
	case 9: disto_c=(float)(0.90); break;
	case 10: disto_c=(float)(0.89); break;
	case 11: disto_c=(float)(0.88); break;
	case 12: disto_c=(float)(0.87); break;
	case 13: disto_c=(float)(0.86); break;
	case 14: disto_c=(float)(0.85); break;
	case 15: disto_c=(float)(0.84); break;
	case 16: disto_c=(float)(0.83); break;
	case 17: disto_c=(float)(0.82); break;
	case 18: disto_c=(float)(0.81); break;
	case 19: disto_c=(float)(0.80); break;
	case 20: disto_c=(float)(0.79); break;
	case 21: disto_c=(float)(0.78); break;
	case 22: disto_c=(float)(0.77); break;
	case 23: disto_c=(float)(0.76); break;
	case 24: disto_c=(float)(0.75); break;
	case 25: disto_c=(float)(0.74); break;
	case 26: disto_c=(float)(0.73); break;
	case 27: disto_c=(float)(0.72); break;
	case 28: disto_c=(float)(0.71); break;
	case 29: disto_c=(float)(0.70); break;
	case 30: disto_c=(float)(0.69); break;
	case 31: disto_c=(float)(0.68); break;
	case 32: disto_c=(float)(0.67); break;
	case 33: disto_c=(float)(0.66); break;
	case 34: disto_c=(float)(0.65); break;
	case 35: disto_c=(float)(0.64); break;
	case 36: disto_c=(float)(0.63); break;
	case 37: disto_c=(float)(0.62); break;
	case 38: disto_c=(float)(0.61); break;
	case 39: disto_c=(float)(0.60); break;
	case 40: disto_c=(float)(0.59); break;
	case 41: disto_c=(float)(0.58); break;
	case 42: disto_c=(float)(0.57); break;
	case 43: disto_c=(float)(0.56); break;
	case 44: disto_c=(float)(0.55); break;
	case 45: disto_c=(float)(0.54); break;
	case 46: disto_c=(float)(0.53); break;
	case 47: disto_c=(float)(0.52); break;
	case 48: disto_c=(float)(0.51); break;
	case 49: disto_c=(float)(0.50); break;
	case 50: disto_c=(float)(0.49); break;
	case 51: disto_c=(float)(0.48); break;
	case 52: disto_c=(float)(0.47); break;
	case 53: disto_c=(float)(0.46); break;
	case 54: disto_c=(float)(0.45); break;
	case 55: disto_c=(float)(0.44); break;
	case 56: disto_c=(float)(0.43); break;
	case 57: disto_c=(float)(0.42); break;
	case 58: disto_c=(float)(0.41); break;
	case 59: disto_c=(float)(0.40); break;
	case 60: disto_c=(float)(0.39); break;
	case 61: disto_c=(float)(0.38); break;
	case 62: disto_c=(float)(0.37); break;
	case 63: disto_c=(float)(0.36); break;
	case 64: disto_c=(float)(0.35); break;
	case 65: disto_c=(float)(0.34); break;
	case 66: disto_c=(float)(0.33); break;
	case 67: disto_c=(float)(0.32); break;
	case 68: disto_c=(float)(0.31); break;
	case 69: disto_c=(float)(0.30); break;
	case 70: disto_c=(float)(0.29); break;
	case 71: disto_c=(float)(0.28); break;
	case 72: disto_c=(float)(0.27); break;
	case 73: disto_c=(float)(0.26); break;
	case 74: disto_c=(float)(0.25); break;
	case 75: disto_c=(float)(0.24); break;
	case 76: disto_c=(float)(0.23); break;
	case 77: disto_c=(float)(0.22); break;
	case 78: disto_c=(float)(0.21); break;
	case 79: disto_c=(float)(0.20); break;
	case 80: disto_c=(float)(0.19); break;
	case 81: disto_c=(float)(0.18); break;
	case 82: disto_c=(float)(0.17); break;
	case 83: disto_c=(float)(0.16); break;
	case 84: disto_c=(float)(0.15); break;
	case 85: disto_c=(float)(0.14); break;
	case 86: disto_c=(float)(0.13); break;
	case 87: disto_c=(float)(0.12); break;
	case 88: disto_c=(float)(0.11); break;
	case 89: disto_c=(float)(0.10); break;
	case 90: disto_c=(float)(0.09); break;
	case 91: disto_c=(float)(0.08); break;
	case 92: disto_c=(float)(0.07); break;
	case 93: disto_c=(float)(0.06); break;
	case 94: disto_c=(float)(0.05); break;
	case 95: disto_c=(float)(0.04); break;
	case 96: disto_c=(float)(0.03); break;
	case 97: disto_c=(float)(0.02); break;
	case 98: disto_c=(float)(0.01); break;
	case 99: disto_c=(float)(0.00); break;
	case 100: disto_c=(float)(-0.01); break;
	case 101: disto_c=(float)(-0.02); break;
	case 102: disto_c=(float)(-0.03); break;
	case 103: disto_c=(float)(-0.04); break;
	case 104: disto_c=(float)(-0.05); break;
	case 105: disto_c=(float)(-0.06); break;
	case 106: disto_c=(float)(-0.07); break;
	case 107: disto_c=(float)(-0.08); break;
	case 108: disto_c=(float)(-0.09); break;
	case 109: disto_c=(float)(-0.10); break;
	case 110: disto_c=(float)(-0.11); break;
	case 111: disto_c=(float)(-0.12); break;
	case 112: disto_c=(float)(-0.13); break;
	case 113: disto_c=(float)(-0.14); break;
	case 114: disto_c=(float)(-0.15); break;
	case 115: disto_c=(float)(-0.16); break;
	case 116: disto_c=(float)(-0.17); break;
	case 117: disto_c=(float)(-0.18); break;
	case 118: disto_c=(float)(-0.19); break;
	case 119: disto_c=(float)(-0.20); break;
	case 120: disto_c=(float)(-0.21); break;
	case 121: disto_c=(float)(-0.22); break;
	case 122: disto_c=(float)(-0.23); break;
	case 123: disto_c=(float)(-0.24); break;
	case 124: disto_c=(float)(-0.25); break;
	case 125: disto_c=(float)(-0.26); break;
	case 126: disto_c=(float)(-0.27); break;
	case 127: disto_c=(float)(-0.28); break;
	case 128: disto_c=(float)(-0.29); break;
	case 129: disto_c=(float)(-0.30); break;
	case 130: disto_c=(float)(-0.31); break;
	case 131: disto_c=(float)(-0.32); break;
	case 132: disto_c=(float)(-0.33); break;
	case 133: disto_c=(float)(-0.34); break;
	case 134: disto_c=(float)(-0.35); break;
	case 135: disto_c=(float)(-0.36); break;
	case 136: disto_c=(float)(-0.37); break;
	case 137: disto_c=(float)(-0.38); break;
	case 138: disto_c=(float)(-0.39); break;
	case 139: disto_c=(float)(-0.40); break;
	case 140: disto_c=(float)(-0.41); break;
	case 141: disto_c=(float)(-0.42); break;
	case 142: disto_c=(float)(-0.43); break;
	case 143: disto_c=(float)(-0.44); break;
	case 144: disto_c=(float)(-0.45); break;
	case 145: disto_c=(float)(-0.46); break;
	case 146: disto_c=(float)(-0.47); break;
	case 147: disto_c=(float)(-0.48); break;
	case 148: disto_c=(float)(-0.49); break;
	case 149: disto_c=(float)(-0.50); break;
	case 150: disto_c=(float)(-0.51); break;
	case 151: disto_c=(float)(-0.52); break;
	case 152: disto_c=(float)(-0.53); break;
	case 153: disto_c=(float)(-0.54); break;
	case 154: disto_c=(float)(-0.55); break;
	case 155: disto_c=(float)(-0.56); break;
	case 156: disto_c=(float)(-0.57); break;
	case 157: disto_c=(float)(-0.58); break;
	case 158: disto_c=(float)(-0.59); break;
	case 159: disto_c=(float)(-0.60); break;
	case 160: disto_c=(float)(-0.61); break;
	case 161: disto_c=(float)(-0.62); break;
	case 162: disto_c=(float)(-0.63); break;
	case 163: disto_c=(float)(-0.64); break;
	case 164: disto_c=(float)(-0.65); break;
	case 165: disto_c=(float)(-0.66); break;
	case 166: disto_c=(float)(-0.67); break;
	case 167: disto_c=(float)(-0.68); break;
	case 168: disto_c=(float)(-0.69); break;
	case 169: disto_c=(float)(-0.70); break;
	case 170: disto_c=(float)(-0.71); break;
	case 171: disto_c=(float)(-0.72); break;
	case 172: disto_c=(float)(-0.73); break;
	case 173: disto_c=(float)(-0.74); break;
	case 174: disto_c=(float)(-0.75); break;
	case 175: disto_c=(float)(-0.76); break;
	case 176: disto_c=(float)(-0.77); break;
	case 177: disto_c=(float)(-0.78); break;
	case 178: disto_c=(float)(-0.79); break;
	case 179: disto_c=(float)(-0.80); break;
	case 180: disto_c=(float)(-0.81); break;
	case 181: disto_c=(float)(-0.82); break;
	case 182: disto_c=(float)(-0.83); break;
	case 183: disto_c=(float)(-0.84); break;
	case 184: disto_c=(float)(-0.85); break;
	case 185: disto_c=(float)(-0.86); break;
	case 186: disto_c=(float)(-0.87); break;
	case 187: disto_c=(float)(-0.88); break;
	case 188: disto_c=(float)(-0.89); break;
	case 189: disto_c=(float)(-0.90); break;
	case 190: disto_c=(float)(-0.91); break;
	case 191: disto_c=(float)(-0.92); break;
	case 192: disto_c=(float)(-0.93); break;
	case 193: disto_c=(float)(-0.94); break;
	case 194: disto_c=(float)(-0.95); break;
	case 195: disto_c=(float)(-0.96); break;
	case 196: disto_c=(float)(-0.97); break;
	case 197: disto_c=(float)(-0.98); break;
	case 198: disto_c=(float)(-0.99); break;
	default: disto_c=(float)(0.00); 
	}

	
}

void CStillCapDlg::OnSelchangeListx() 
{
	int cx=0;
	switch(m_liscentrox.GetCurSel())
	{
	case 0: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*100)/100)); centrox=centrox+cx; break;
	case 1: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*99)/100)); centrox=centrox+cx; break;
	case 2: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*98)/100)); centrox=centrox+cx; break;
	case 3: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*97)/100)); centrox=centrox+cx; break;
	case 4: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*96)/100)); centrox=centrox+cx; break;
	case 5: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*95)/100)); centrox=centrox+cx; break;
	case 6: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*94)/100)); centrox=centrox+cx; break;
	case 7: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*93)/100)); centrox=centrox+cx; break;
	case 8: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*92)/100)); centrox=centrox+cx; break;
	case 9: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*91)/100)); centrox=centrox+cx; break;
	case 10: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*90)/100)); centrox=centrox+cx; break;
	case 11: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*89)/100)); centrox=centrox+cx; break;
	case 12: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*88)/100)); centrox=centrox+cx; break;
	case 13: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*87)/100)); centrox=centrox+cx; break;
	case 14: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*86)/100)); centrox=centrox+cx; break;
	case 15: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*85)/100)); centrox=centrox+cx; break;
	case 16: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*84)/100)); centrox=centrox+cx; break;
	case 17: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*83)/100)); centrox=centrox+cx; break;
	case 18: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*82)/100)); centrox=centrox+cx; break;
	case 19: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*81)/100)); centrox=centrox+cx; break;
	case 20: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*80)/100)); centrox=centrox+cx; break;
	case 21: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*79)/100)); centrox=centrox+cx; break;
	case 22: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*78)/100)); centrox=centrox+cx; break;
	case 23: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*77)/100)); centrox=centrox+cx; break;
	case 24: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*76)/100)); centrox=centrox+cx; break;
	case 25: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*75)/100)); centrox=centrox+cx; break;
	case 26: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*74)/100)); centrox=centrox+cx; break;
	case 27: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*73)/100)); centrox=centrox+cx; break;
	case 28: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*72)/100)); centrox=centrox+cx; break;
	case 29: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*71)/100)); centrox=centrox+cx; break;
	case 30: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*70)/100)); centrox=centrox+cx; break;
	case 31: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*69)/100)); centrox=centrox+cx; break;
	case 32: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*68)/100)); centrox=centrox+cx; break;
	case 33: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*67)/100)); centrox=centrox+cx; break;
	case 34: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*66)/100)); centrox=centrox+cx; break;
	case 35: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*65)/100)); centrox=centrox+cx; break;
	case 36: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*64)/100)); centrox=centrox+cx; break;
	case 37: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*63)/100)); centrox=centrox+cx; break;
	case 38: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*62)/100)); centrox=centrox+cx; break;
	case 39: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*61)/100)); centrox=centrox+cx; break;
	case 40: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*60)/100)); centrox=centrox+cx; break;
	case 41: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*59)/100)); centrox=centrox+cx; break;
	case 42: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*58)/100)); centrox=centrox+cx; break;
	case 43: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*57)/100)); centrox=centrox+cx; break;
	case 44: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*56)/100)); centrox=centrox+cx; break;
	case 45: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*55)/100)); centrox=centrox+cx; break;
	case 46: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*54)/100)); centrox=centrox+cx; break;
	case 47: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*53)/100)); centrox=centrox+cx; break;
	case 48: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*52)/100)); centrox=centrox+cx; break;
	case 49: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*51)/100)); centrox=centrox+cx; break;
	case 50: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*50)/100)); centrox=centrox+cx; break;
	case 51: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*49)/100)); centrox=centrox+cx; break;
	case 52: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*48)/100)); centrox=centrox+cx; break;
	case 53: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*47)/100)); centrox=centrox+cx; break;
	case 54: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*46)/100)); centrox=centrox+cx; break;
	case 55: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*45)/100)); centrox=centrox+cx; break;
	case 56: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*44)/100)); centrox=centrox+cx; break;
	case 57: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*43)/100)); centrox=centrox+cx; break;
	case 58: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*42)/100)); centrox=centrox+cx; break;
	case 59: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*41)/100)); centrox=centrox+cx; break;
	case 60: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*40)/100)); centrox=centrox+cx; break;
	case 61: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*39)/100)); centrox=centrox+cx; break;
	case 62: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*38)/100)); centrox=centrox+cx; break;
	case 63: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*37)/100)); centrox=centrox+cx; break;
	case 64: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*36)/100)); centrox=centrox+cx; break;
	case 65: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*35)/100)); centrox=centrox+cx; break;
	case 66: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*34)/100)); centrox=centrox+cx; break;
	case 67: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*33)/100)); centrox=centrox+cx; break;
	case 68: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*32)/100)); centrox=centrox+cx; break;
	case 69: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*31)/100)); centrox=centrox+cx; break;
	case 70: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*30)/100)); centrox=centrox+cx; break;
	case 71: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*29)/100)); centrox=centrox+cx; break;
	case 72: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*28)/100)); centrox=centrox+cx; break;
	case 73: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*27)/100)); centrox=centrox+cx; break;
	case 74: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*26)/100)); centrox=centrox+cx; break;
	case 75: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*25)/100)); centrox=centrox+cx; break;
	case 76: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*24)/100)); centrox=centrox+cx; break;
	case 77: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*23)/100)); centrox=centrox+cx; break;
	case 78: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*22)/100)); centrox=centrox+cx; break;
	case 79: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*21)/100)); centrox=centrox+cx; break;
	case 80: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*20)/100)); centrox=centrox+cx; break;
	case 81: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*19)/100)); centrox=centrox+cx; break;
	case 82: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*18)/100)); centrox=centrox+cx; break;
	case 83: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*17)/100)); centrox=centrox+cx; break;
	case 84: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*16)/100)); centrox=centrox+cx; break;
	case 85: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*15)/100)); centrox=centrox+cx; break;
	case 86: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*14)/100)); centrox=centrox+cx; break;
	case 87: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*13)/100)); centrox=centrox+cx; break;
	case 88: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*12)/100)); centrox=centrox+cx; break;
	case 89: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*11)/100)); centrox=centrox+cx; break;
	case 90: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*10)/100)); centrox=centrox+cx; break;
	case 91: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*9)/100)); centrox=centrox+cx; break;
	case 92: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*8)/100)); centrox=centrox+cx; break;
	case 93: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*7)/100)); centrox=centrox+cx; break;
	case 94: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*6)/100)); centrox=centrox+cx; break;
	case 95: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*5)/100)); centrox=centrox+cx; break;
	case 96: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*4)/100)); centrox=centrox+cx; break;
	case 97: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*3)/100)); centrox=centrox+cx; break;
	case 98: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*2)/100)); centrox=centrox+cx; break;
	case 99: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*1)/100)); centrox=centrox+cx; break;
	case 100: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*0)/100)); centrox=centrox+cx; break;
	case 101: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*1)/100)); centrox=centrox-cx; break;
	case 102: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*2)/100)); centrox=centrox-cx; break;
	case 103: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*3)/100)); centrox=centrox-cx; break;		
	case 104: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*4)/100)); centrox=centrox-cx; break;
	case 105: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*5)/100)); centrox=centrox-cx; break;
	case 106: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*6)/100)); centrox=centrox-cx; break;
	case 107: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*7)/100)); centrox=centrox-cx; break;
	case 108: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*8)/100)); centrox=centrox-cx; break;
	case 109: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*9)/100)); centrox=centrox-cx; break;
	case 110: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*10)/100)); centrox=centrox-cx; break;		
	case 111: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*11)/100)); centrox=centrox-cx; break;
	case 112: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*12)/100)); centrox=centrox-cx; break;
	case 113: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*13)/100)); centrox=centrox-cx; break;		
	case 114: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*14)/100)); centrox=centrox-cx; break;
	case 115: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*15)/100)); centrox=centrox-cx; break;
	case 116: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*16)/100)); centrox=centrox-cx; break;
	case 117: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*17)/100)); centrox=centrox-cx; break;
	case 118: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*18)/100)); centrox=centrox-cx; break;
	case 119: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*19)/100)); centrox=centrox-cx; break;
	case 120: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*20)/100)); centrox=centrox-cx; break;		
	case 121: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*21)/100)); centrox=centrox-cx; break;
	case 122: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*22)/100)); centrox=centrox-cx; break;
	case 123: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*23)/100)); centrox=centrox-cx; break;		
	case 124: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*24)/100)); centrox=centrox-cx; break;
	case 125: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*25)/100)); centrox=centrox-cx; break;
	case 126: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*26)/100)); centrox=centrox-cx; break;
	case 127: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*27)/100)); centrox=centrox-cx; break;
	case 128: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*28)/100)); centrox=centrox-cx; break;
	case 129: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*29)/100)); centrox=centrox-cx; break;
	case 130: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*30)/100)); centrox=centrox-cx; break;		
	case 131: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*31)/100)); centrox=centrox-cx; break;
	case 132: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*32)/100)); centrox=centrox-cx; break;
	case 133: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*33)/100)); centrox=centrox-cx; break;		
	case 134: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*34)/100)); centrox=centrox-cx; break;
	case 135: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*35)/100)); centrox=centrox-cx; break;
	case 136: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*36)/100)); centrox=centrox-cx; break;
	case 137: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*37)/100)); centrox=centrox-cx; break;
	case 138: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*38)/100)); centrox=centrox-cx; break;
	case 139: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*39)/100)); centrox=centrox-cx; break;
	case 140: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*40)/100)); centrox=centrox-cx; break;		
	case 141: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*41)/100)); centrox=centrox-cx; break;
	case 142: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*42)/100)); centrox=centrox-cx; break;
	case 143: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*43)/100)); centrox=centrox-cx; break;		
	case 144: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*44)/100)); centrox=centrox-cx; break;
	case 145: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*45)/100)); centrox=centrox-cx; break;
	case 146: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*46)/100)); centrox=centrox-cx; break;
	case 147: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*47)/100)); centrox=centrox-cx; break;
	case 148: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*48)/100)); centrox=centrox-cx; break;
	case 149: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*49)/100)); centrox=centrox-cx; break;
	case 150: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*50)/100)); centrox=centrox-cx; break;		
	case 151: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*51)/100)); centrox=centrox-cx; break;
	case 152: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*52)/100)); centrox=centrox-cx; break;
	case 153: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*53)/100)); centrox=centrox-cx; break;		
	case 154: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*54)/100)); centrox=centrox-cx; break;
	case 155: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*55)/100)); centrox=centrox-cx; break;
	case 156: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*56)/100)); centrox=centrox-cx; break;
	case 157: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*57)/100)); centrox=centrox-cx; break;
	case 158: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*58)/100)); centrox=centrox-cx; break;
	case 159: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*59)/100)); centrox=centrox-cx; break;
	case 160: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*60)/100)); centrox=centrox-cx; break;		
	case 161: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*61)/100)); centrox=centrox-cx; break;
	case 162: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*62)/100)); centrox=centrox-cx; break;
	case 163: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*63)/100)); centrox=centrox-cx; break;		
	case 164: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*64)/100)); centrox=centrox-cx; break;
	case 165: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*65)/100)); centrox=centrox-cx; break;
	case 166: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*66)/100)); centrox=centrox-cx; break;
	case 167: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*67)/100)); centrox=centrox-cx; break;
	case 168: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*68)/100)); centrox=centrox-cx; break;
	case 169: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*69)/100)); centrox=centrox-cx; break;
	case 170: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*70)/100)); centrox=centrox-cx; break;		
	case 171: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*71)/100)); centrox=centrox-cx; break;
	case 172: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*72)/100)); centrox=centrox-cx; break;
	case 173: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*73)/100)); centrox=centrox-cx; break;		
	case 174: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*74)/100)); centrox=centrox-cx; break;
	case 175: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*75)/100)); centrox=centrox-cx; break;
	case 176: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*76)/100)); centrox=centrox-cx; break;
	case 177: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*77)/100)); centrox=centrox-cx; break;
	case 178: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*78)/100)); centrox=centrox-cx; break;
	case 179: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*79)/100)); centrox=centrox-cx; break;
	case 180: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*80)/100)); centrox=centrox-cx; break;		
	case 181: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*81)/100)); centrox=centrox-cx; break;
	case 182: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*82)/100)); centrox=centrox-cx; break;
	case 183: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*83)/100)); centrox=centrox-cx; break;		
	case 184: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*84)/100)); centrox=centrox-cx; break;
	case 185: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*85)/100)); centrox=centrox-cx; break;
	case 186: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*86)/100)); centrox=centrox-cx; break;
	case 187: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*87)/100)); centrox=centrox-cx; break;
	case 188: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*88)/100)); centrox=centrox-cx; break;
	case 189: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*89)/100)); centrox=centrox-cx; break;
	case 190: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*90)/100)); centrox=centrox-cx; break;		
	case 191: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*91)/100)); centrox=centrox-cx; break;
	case 192: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*92)/100)); centrox=centrox-cx; break;
	case 193: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*93)/100)); centrox=centrox-cx; break;		
	case 194: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*94)/100)); centrox=centrox-cx; break;
	case 195: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*95)/100)); centrox=centrox-cx; break;
	case 196: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*96)/100)); centrox=centrox-cx; break;
	case 197: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*97)/100)); centrox=centrox-cx; break;
	case 198: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*98)/100)); centrox=centrox-cx; break;
	case 199: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*99)/100)); centrox=centrox-cx; break;
	case 200: centrox=puntomedioentero(MAX_RES_X); cx=redondeofi((float)((centrox*100)/100)); centrox=centrox-cx; break;		
	default: centrox=puntomedioentero(MAX_RES_X);
	}
}

