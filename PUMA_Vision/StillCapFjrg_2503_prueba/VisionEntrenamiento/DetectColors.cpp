#include "stdafx.h"
#include "point.h"
#include "linklistblob.h"
#include "linklistRLE.h"
#include "colors.h"
#include "detectcolors.h"

#define SUB_SAMP	1

extern int VideoP;
 
//#define _SHOWDOTS

/*==================================================================================
*						detectColors()
==================================================================================*/

/** Creates list of blobs for each color.
*
*  This function calls addToRLE for each pixel. Once all RLE lists are created, 
*  formBlobs is called to create the blobs.
*
* \param pBuffer		Buffer containing image.
* \param listBlobOrange List which will store orange blobs
* \param listBlobOrange List which will store blue blobs
* \param listBlobOrange List which will store yellow blobs
* \param listBlobOrange List which will store green blobs
* \param listBlobOrange List which will store pink blobs
* \param listBlobOrange List which will store cyan blobs
* \param width			Image width
* \param height			Image height
*
*/
void detectColors(BYTE *pBuffer , LinkListBLOB *listBlobOrange, LinkListBLOB *listBlobBlue, LinkListBLOB *listBlobYellow, LinkListBLOB *listBlobGreen, LinkListBLOB *listBlobPink, LinkListBLOB *listBlobCyan, int width, int height, Point fieldLim1, Point fieldLim2){
	LinkListRLE lRLEOrange, lRLEBlue, lRLEYellow, lRLEGreen, lRLEPink, lRLECyan, lAux;
	int h, s, i, m, cont, rtag, tempcont;
	//int r, g, b;
	int orangeflag = 0, blueflag = 0, yellowflag = 0, greenflag = 0, pinkflag=0, cyanflag=0;
	float temp;		
	RGB myrgb;
	HSI myhsi;
	Point scann, p0, p1;
	BYTE rgb[3];

	rtag = 0;

	initRLE(&lRLEOrange);
	initRLE(&lRLEBlue);
	initRLE(&lRLEYellow);
	initRLE(&lRLEPink);
	initRLE(&lRLECyan);
	initRLE(&lRLEGreen);

	/*for( cont = 0, scann.y = fieldLim1.y; scann.y < fieldLim2.y; scann.y += SUB_SAMP )
		{
			for( scann.x = fieldLim1.x; scann.x < fieldLim2.x; scann.x += SUB_SAMP, cont += (3*SUB_SAMP) )
			{*/

	for( cont = 0, scann.y = 0; scann.y < height; scann.y += SUB_SAMP )
	{
		if ( scann.y < fieldLim1.y ||  scann.y > fieldLim2.y ) continue;
		for( scann.x = 0; scann.x < width; scann.x += SUB_SAMP, cont += (3*SUB_SAMP) )
		{
			if ( scann.x < fieldLim1.x ||  scann.x > fieldLim2.x ) continue;

            /*
			*(rgb) = *(pBuffer+cont);
			*(rgb+1) = *(pBuffer+cont+1);
			*(rgb+2) = *(pBuffer+cont+2);
			

			myrgb.r = *(rgb+2);
			myrgb.g = *(rgb+1);
			myrgb.b = *(rgb);
			*/

			myrgb.r = *(pBuffer+cont+2);
			myrgb.g = *(pBuffer+cont+1);
			myrgb.b = *(pBuffer+cont);

		
			myhsi = RGB2HSI(myrgb);

			addToRLE(scann, myhsi, orange, &orangeflag, &lRLEOrange);
			addToRLE(scann, myhsi, yellow, &yellowflag, &lRLEYellow);
			addToRLE(scann, myhsi, blue, &blueflag, &lRLEBlue);
			addToRLE(scann, myhsi, green, &greenflag, &lRLEGreen);
			addToRLE(scann, myhsi, pink, &pinkflag, &lRLEPink);
			addToRLE(scann, myhsi, cyan, &cyanflag, &lRLECyan);

//			showDots(pBuffer, myhsi, cont);

		}
		tempcont = (scann.y+1) * width * 3;
		cont = tempcont + ((width)*3*(SUB_SAMP-1));
	}
	
	formBlobs(listBlobOrange, &lRLEOrange);
	destroyRLE(&lRLEOrange);
	orderBlobs(listBlobOrange);

	formBlobs(listBlobYellow, &lRLEYellow);
	destroyRLE(&lRLEYellow);
	orderBlobs(listBlobYellow);

	formBlobs(listBlobBlue, &lRLEBlue);
	destroyRLE(&lRLEBlue);
	orderBlobs(listBlobBlue);

	formBlobs(listBlobGreen, &lRLEGreen);
	destroyRLE(&lRLEGreen);
	orderBlobs(listBlobGreen);	

	formBlobs(listBlobPink, &lRLEPink);
	destroyRLE(&lRLEPink);
	orderBlobs(listBlobPink);	

	formBlobs(listBlobCyan, &lRLECyan);
	destroyRLE(&lRLECyan);
	orderBlobs(listBlobCyan);		

	
	if( VideoP>0 ){
		for( cont = 0, scann.y = 0; scann.y < height; scann.y ++ )
		{
			for( scann.x = 0; scann.x < width; scann.x ++, cont += 3 )
			{
				*(pBuffer+cont) = *(pBuffer+cont+2);
				*(pBuffer+cont+1) = *(pBuffer+cont+2);
				//*(pBuffer+cont+2) = *(pBuffer+cont+2);
			}
		}
	}
	


	
}

/*=================================================================================
							detectColors2()
=================================================================================*/

void detectColors2(BYTE *pBuffer , LinkListBLOB *listBlobOrange, LinkListBLOB *listBlobBlue, LinkListBLOB *listBlobYellow, LinkListBLOB *listBlobGreen, LinkListBLOB *listBlobPink, LinkListBLOB *listBlobCyan, int width, int height, Point fieldLim1, Point fieldLim2){
	LinkListRLE lRLEOrange, lRLEBlue, lRLEYellow, lRLEGreen, lRLEPink, lRLECyan, lAux;
	int h, s, i, m, cont, rtag, tempcont;
	//int r, g, b;
	int orangeflag = 0, blueflag = 0, yellowflag = 0, greenflag = 0, pinkflag=0, cyanflag=0;
	float temp;		
	RGB myrgb;
	HSI myhsi;
	Point scann, p0, p1;
	BYTE rgb[3];
	int a[3],b[3];

	rtag = 0;

	initRLE(&lRLEOrange);
	initRLE(&lRLEBlue);
	initRLE(&lRLEYellow);
	initRLE(&lRLEPink);
	initRLE(&lRLECyan);
	initRLE(&lRLEGreen);

	for( cont = 0, scann.y = 0; scann.y < height; scann.y += SUB_SAMP )
	{
		if ( scann.y < fieldLim1.y ||  scann.y > fieldLim2.y ) continue;
		for( scann.x = 0; scann.x < width; scann.x += SUB_SAMP, cont += (3*SUB_SAMP) )
		{
			if ( scann.x < fieldLim1.x ||  scann.x > fieldLim2.x ) continue;

			myrgb.r = *(pBuffer+cont+2);
			myrgb.g = *(pBuffer+cont+1);
			myrgb.b = *(pBuffer+cont);

		/*
			myhsi = RGB2HSI(myrgb);
			b[0] = myhsi.h;
			b[1] = myhsi.s;
			b[2] = myhsi.i;
			
			a[2] = (hsi[0].i.max+hsi[0].i.min)>>1;
			a[1] = (hsi[0].s.max+hsi[0].s.min)>>1;
			a[0] = (hsi[0].h.min+(hsi[0].h.win>>1)%360;
			hsiDistColor( a, b );
			*/

			/*
			addToRLE(scann, myhsi, orange, &orangeflag, &lRLEOrange);
			addToRLE(scann, myhsi, yellow, &yellowflag, &lRLEYellow);
			addToRLE(scann, myhsi, blue, &blueflag, &lRLEBlue);
			addToRLE(scann, myhsi, green, &greenflag, &lRLEGreen);
			addToRLE(scann, myhsi, pink, &pinkflag, &lRLEPink);
			addToRLE(scann, myhsi, cyan, &cyanflag, &lRLECyan);
			*/
		}
		tempcont = (scann.y+1) * width * 3;
		cont = tempcont + ((width)*3*(SUB_SAMP-1));
	}
	
	formBlobs(listBlobOrange, &lRLEOrange);
	destroyRLE(&lRLEOrange);
	orderBlobs(listBlobOrange);

	formBlobs(listBlobYellow, &lRLEYellow);
	destroyRLE(&lRLEYellow);
	orderBlobs(listBlobYellow);

	formBlobs(listBlobBlue, &lRLEBlue);
	destroyRLE(&lRLEBlue);
	orderBlobs(listBlobBlue);

	formBlobs(listBlobGreen, &lRLEGreen);
	destroyRLE(&lRLEGreen);
	orderBlobs(listBlobGreen);	

	formBlobs(listBlobPink, &lRLEPink);
	destroyRLE(&lRLEPink);
	orderBlobs(listBlobPink);	

	formBlobs(listBlobCyan, &lRLECyan);
	destroyRLE(&lRLECyan);
	orderBlobs(listBlobCyan);		
}




#ifdef comment
/*==================================================================================
*						hsiIsColor()
==================================================================================*/

/** Determines if a pixel's color (in HSI) is the one recieved in the parameter color.
*
* \param h		Pixel's color in HSI
* \param color	Color to compare 
*
* \return			1 when true; otherwise, 0.
*
*/
int hsiIsColor( HSI h , int color )
{
	int temp;

	if( (hsi[color].s.min <= h.s && h.s <= hsi[color].s.max) )
		if (hsi[color].i.min <= h.i && h.i <= hsi[color].i.max){
            if( hsi[color].h.min <= hsi[color].h.max ){
				if( (hsi[color].h.min <= h.h && h.h <= hsi[color].h.max) )
					return TRUE;
			}
			else if( (hsi[color].h.min <= h.h && h.h <= 360) || ( 0 <= h.h  && h.h <= hsi[color].h.max ) )
				return TRUE;
		}
	return FALSE;
}

#else
//int hsiIsColor( BYTE h, int color ) 
int hsiIsColor( HSI h, int color ) 
{
	if( hsi[color].i.min <= h.i && h.i <= hsi[color].i.max )
	{	
		if( hsi[color].s.min <= h.s && h.s <= hsi[color].s.max )
		{
			if( hsi[color].h.min > hsi[color].h.max )
			{
				if( hsi[color].h.min <= h.h || h.h <= hsi[color].h.max )
					return 1;
			}
			else
			{
				if( hsi[color].h.min <= h.h && h.h <= hsi[color].h.max )
					return 1;
			}
		}
	}
	


	
	return 0;
}
#endif

/*==============================================================================
								hsiDistColor()
==============================================================================*/
//unsigned int hsiDistColor( HSI h, int color ) 
unsigned int hsiDistColor( int a[3], int b[3] ) 
{
	unsigned t0, t1;

	
	if( a[0] < 270 && b[0] >= 270 )
		t0 = 360-b[0]+a[0];
	else if( a[0] > 270 && b[0] <= 270 )
		t0 = 360-a[0]+b[0];
	else 
		t0 = (a[0]-b[0]);
	

#ifdef comment
	// Lo mismo de arriba pero optimizado
	if( b[0] >= 270 )
		if( a[0] < 270 )
			t0 = 360-b[0]+a[0];
	else if( a[0] > 270 
		if( b[0] <= 270 )
			t0 = 360-a[0]+b[0];
	else 
		t0 = (a[0]-b[0]);
#endif

	// Distancia euclidiana
//	t1 = (cal.i-pnt.i)*(cal.i-pnt.i) + (cal.s-pnt.i)*(cal.s-pnt.i) + t0*t0;
		
	/*
	if( hsi[color].i.min <= h.i && h.i <= hsi[color].i.max )
	{	
		if( hsi[color].s.min <= h.s && h.s <= hsi[color].s.max )
		{
			if( hsi[color].h.min > hsi[color].h.max )
			{
				if( hsi[color].h.min <= h.h || h.h <= hsi[color].h.max )
					return 1;
			}
			else
			{
				if( hsi[color].h.min <= h.h && h.h <= hsi[color].h.max )
					return 1;
			}
		}
	}
	


	
	return 0;
	*/
	return t1;
}

/*==================================================================================
*						addToRLE()
==================================================================================*/

/** If a pixel is of a certain color, this function adds it to a RLE list.
*
* This function is a state machine which adds a pixel to a RLE list depending on 
* its color. If there is more than one pixel of the same color next to each other,
* they are stored in the same RLE. If there are pixels of other colors before the
* one that's beeing analized, a new RLE is created.
*
*
* \param scann	Coordinates of the pixel being analized
* \param color	Color to compare
* \param flag	Flag holds the state of the state machine
* \param L		List in which RLEs are being stored
*
*/
void addToRLE(Point scann, HSI myhsi, int color, int *flag, LinkListRLE *L){
	LinkListRLE *aux,*last;
	RLE myrle, *nrle;
	
	switch( *flag )
	{
	case 0:
		if (hsiIsColor( myhsi , color) )
		{
			*flag = 1;
			if ( isEmptyRLE(L) )
				myrle.tag = 1;
			else{
				nrle = queryTopRLE(L);
				myrle.tag = nrle->tag + 1;
			}
			myrle.x0 = scann.x;
			myrle.y = scann.y;
			myrle.x1 = scann.x;
			myrle.size = 1;
			myrle.color = color;
			myrle.blobTag = 0;
			insertRLE( L , myrle );
		}
		break;
	case 1:
		if (hsiIsColor( myhsi , color) ){
			nrle = queryTopRLE(L);
			nrle->size+=SUB_SAMP;
			nrle->x1+=SUB_SAMP;
		}			
		else 
			*flag = 0;
		break;
	}
}

/*==================================================================================
								addToRLE2()
==================================================================================*/
void addToRLE2(Point scann, HSI myhsi, int color, int *flag, LinkListRLE *L, int close){
	LinkListRLE *aux,*last;
	RLE myrle, *nrle;
	
	switch( *flag )
	{
	case 0:
		if (close){
			*flag = 1;
			if ( isEmptyRLE(L) )
				myrle.tag = 1;
			else{
				nrle = queryTopRLE(L);
				myrle.tag = nrle->tag + 1;
			}
			myrle.x0 = scann.x;
			myrle.y = scann.y;
			myrle.x1 = scann.x;
			myrle.size = 1;
			myrle.color = color;
			myrle.blobTag = 0;
			insertRLE( L , myrle );
		}
		break;
	case 1:
		if ( close ){
			nrle = queryTopRLE(L);
			nrle->size+=SUB_SAMP;
			nrle->x1+=SUB_SAMP;
		}			
		else 
			*flag = 0;
		break;
	}
}

/*==================================================================================
*						formBlobs()
==================================================================================*/

/** Creates a list of blobs of a certain color, using the RLE list.
*
* This function recieves a list of RLEs of a certain color and creates the list of 
* blobs. If the x-coordinate interval of two RLEs overlap, and they are in adjacent
* rows (y-coordinate), they correspond to the same blob.
*
* \param listBlob	List which will store the blobs 
* \param listRLE	List containing the RLEs.
*
*/
void formBlobs(LinkListBLOB *listBlob, LinkListRLE *listRLE){
	LinkListRLE aux1, aux2;
	LinkListBLOB auxLBlob;
	int currentTag;
	RLE *auxRLE1, *auxRLE2;
	BLB *auxBlob,newBlob;
	
	
	if ( isEmptyRLE(listRLE) ) return;
	
	auxRLE1 = queryTopRLE(listRLE);
	currentTag = 0;
			
 	duplicateRLE(&aux1, listRLE);

	while(!isEmptyRLE(&aux1))
	{
		auxRLE1 = queryTopRLE(&aux1);
		if (auxRLE1->blobTag==0)
		{		
			newBlob.tag = auxRLE1->blobTag = ++currentTag;		
			newBlob.p0.x = auxRLE1->x0;
			newBlob.p0.y = auxRLE1->y;
			newBlob.p1.x = auxRLE1->x1;
			newBlob.p1.y = auxRLE1->y;
			newBlob.color = auxRLE1->color;
	//		newBlob.area = (abs(newBlob.p0.x-newBlob.p1.x) + 1)*(abs(newBlob.p0.y-newBlob.p1.y)+1);
			insertBlob(listBlob, newBlob);
		}
		
		duplicateRLE(&aux2, &aux1);
	    nextRLE(&aux2);
				
		while(!isEmptyRLE(&aux2))
		{
			auxRLE2 = queryTopRLE(&aux2);
			if ( auxRLE1->y >= auxRLE2->y && auxRLE1->y <= (auxRLE2->y)+ SUB_SAMP ) // || auxRLE1->y == auxRLE2->y+2 )
			{
				if( ((auxRLE2->x0 >= auxRLE1->x0-(2*SUB_SAMP)) && (auxRLE2->x0 <= auxRLE1->x1+SUB_SAMP)) ||
					((auxRLE2->x1 >= auxRLE1->x0-SUB_SAMP) && (auxRLE2->x1 <= auxRLE1->x1+SUB_SAMP)) ||
					((auxRLE1->x0 >= auxRLE2->x0-SUB_SAMP) && (auxRLE1->x0 <= auxRLE2->x1+SUB_SAMP)) ||
					((auxRLE1->x1 >= auxRLE2->x0-SUB_SAMP) && (auxRLE1->x1 <= auxRLE2->x1+SUB_SAMP)) )
				{
					if (auxRLE2->blobTag==0){
						duplicateBlob(&auxLBlob,listBlob);
						locateTagBlob(&auxLBlob, auxRLE1->blobTag);
						auxBlob = queryTopBlob(&auxLBlob);
						auxRLE2->blobTag = auxBlob->tag;
						auxBlob->p0.x = auxBlob->p0.x <= auxRLE2->x0 ? auxBlob->p0.x : auxRLE2->x0;
						auxBlob->p1.x = auxBlob->p1.x >= auxRLE2->x1 ? auxBlob->p1.x : auxRLE2->x1;
						auxBlob->p0.y = auxRLE2->y;
//						auxBlob->area = (abs(auxBlob->p0.x-auxBlob->p1.x) + 1)*(abs(auxBlob->p0.y-auxBlob->p1.y)+1);
					}
					else if ( auxRLE2->blobTag != auxRLE1->blobTag )
						uniteBlob(listBlob,auxRLE1->blobTag,auxRLE2->blobTag, listRLE);						
                }						
			}
			else if (auxRLE1->y > auxRLE2->y+SUB_SAMP) break;
			
			nextRLE(&aux2);
		}
		nextRLE(&aux1);
	}
}
		
/*==================================================================================
*						uniteBlobs()
==================================================================================*/

/** Unites two blobs that are adjacent to the same RLE.
*
* When two different blobs are adjacent to one RLE, it means there should only be 
* one blob. This function merges the two blobs, by adding all the RLEs of one blob
* to the other blob. Then, one of the blobs is destroyed.
*
* \param listBlob	Blob list
* \param tag1		Tag of the blob that will remain
* \parag tag2		Tag of the blob that's going to disapear
* \listRLE			RLE list
*
*/
void uniteBlob(LinkListBLOB *listBlob, int tag1, int tag2, LinkListRLE *listRLE)
{
	int deadTag;
	LinkListRLE auxListRLE;
	RLE *auxRLE;
	LinkListBLOB aux1, aux2;
	BLB *auxB1, *auxB2;

    duplicateBlob(&aux1, listBlob);
	locateTagBlob(&aux1, tag1);
	auxB1 = queryTopBlob(&aux1);

	duplicateBlob(&aux2, listBlob);
	locateTagBlob(&aux2, tag2);
	auxB2 = queryTopBlob(&aux2);
	deadTag = auxB2->tag;
	
	auxB1->p0.x = auxB1->p0.x <= auxB2->p0.x ? auxB1->p0.x : auxB2->p0.x;
	auxB1->p0.y = auxB1->p0.y <= auxB2->p0.y ? auxB1->p0.y : auxB2->p0.y;
	auxB1->p1.x = auxB1->p1.x >= auxB2->p1.x ? auxB1->p1.x : auxB2->p1.x;
	auxB1->p1.y = auxB1->p1.y >= auxB2->p1.y ? auxB1->p1.y : auxB2->p1.y;
	auxB1->area = ( abs( auxB1->p0.x-auxB1->p1.x ) + 1 ) * ( abs( auxB1->p0.y-auxB1->p1.y) + 1 );

	auxB2->tag=0;

	duplicateRLE(&auxListRLE, listRLE);
	while(!isEmptyRLE(&auxListRLE)){
        auxRLE = queryTopRLE(&auxListRLE);
		if( auxRLE->blobTag == deadTag )
			auxRLE->blobTag = auxB1->tag;
		nextRLE(&auxListRLE);
	}
}

/*==================================================================================
*						orderBlobs()
==================================================================================*/

/** This function orders the list of blobs according to their area.
*
* \param L	Blob list 
*
*/
void orderBlobs(LinkListBLOB *L){
	LinkListBLOB auxList;
	BLB auxBLB;
	initBlob(&auxList);
	while (!isEmptyBlob(L)){
		auxBLB = listDeleteBlob(L);
		if ( auxBLB.tag != 0 ){
			auxBLB.area = ( abs( auxBLB.p0.x-auxBLB.p1.x ) + 1 ) * ( abs( auxBLB.p0.y-auxBLB.p1.y ) +1 );
			auxBLB.ctr.x = (auxBLB.p0.x + auxBLB.p1.x)/2;
			auxBLB.ctr.y = (auxBLB.p0.y + auxBLB.p1.y)/2;
			insertOrderBlob(&auxList,auxBLB);				
		}
	}
	duplicateBlob(L,&auxList);
}

/*==================================================================================
*						firstNNodes()
==================================================================================*/

/** Keeps only the first "n" elements of the blob list. It also destroys all elements 
* with an area lees than "area" 
*
* \param L		Blob list
* \param n		Number of blobs to keep
* \param area	Minumum area
*/
void firstNNodes(LinkListBLOB *L, int n, int area){
	LinkListBLOB auxList;
	BLB auxBLB;
	int i=0;
	initBlob(&auxList);
	while ( !isEmptyBlob(L) && i<n ){
		auxBLB = listDeleteBlob(L);
		if ( auxBLB.area < area ) break;
        insertOrderBlob(&auxList,auxBLB);				
		i++;
	}
	while (!isEmptyBlob(L))
		listDeleteBlob(L);
	duplicateBlob(L,&auxList);
}

/*==================================================================================
*						firstNNodes()
==================================================================================*/

/** Destroys all elements of the blob list with an area lees than "area" 
*
* \param L		Blob list
* \param area	Minumum area
*/
void firstNNodes(LinkListBLOB *L, int area){
	LinkListBLOB auxList;
	BLB auxBLB;
	initBlob(&auxList);
	while ( !isEmptyBlob(L) ){
		auxBLB = listDeleteBlob(L);
		if ( auxBLB.area < area ) break;
        insertOrderBlob(&auxList,auxBLB);				
	}
	duplicateBlob(L,&auxList);
}

void showDots(BYTE *b, HSI myhsi, int cont){

	if (hsiIsColor( myhsi , orange) )
	{
		*(b+cont) = 0;
		*(b+cont+1) = 0;
		*(b+cont+2) = 255;
	}
	if (hsiIsColor( myhsi , yellow) )
	{
		*(b+cont) = 0;
		*(b+cont+1) = 230;
		*(b+cont+2) = 255;
	}
	if (hsiIsColor( myhsi , blue) )
	{
		*(b+cont) = 255;
		*(b+cont+1) = 0;
		*(b+cont+2) = 0;
	}
	if (hsiIsColor( myhsi , green) )
	{
		*(b+cont) = 100;
		*(b+cont+1) = 255;
		*(b+cont+2) = 60;
	}
	if (hsiIsColor( myhsi , pink) )
	{
		*(b+cont) = 244;
		*(b+cont+1) = 4;
		*(b+cont+2) = 202;
	}
	if (hsiIsColor( myhsi , cyan) )
	{
		*(b+cont) = 255;
		*(b+cont+1) = 255;
		*(b+cont+2) = 70;
	}

}