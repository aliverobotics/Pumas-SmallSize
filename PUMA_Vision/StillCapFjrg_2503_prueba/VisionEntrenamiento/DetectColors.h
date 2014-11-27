#include "linklistrle.h"
#include "linklistblob.h"
#include "colors.h"

#ifndef __detectcolors__
	#define __detectcolors__

	void detectColors(BYTE *pBuffer , LinkListBLOB *listBlobOrange, LinkListBLOB *listBlobBlue, LinkListBLOB *listBlobYellow, LinkListBLOB *listBlobGreen, LinkListBLOB *listBlobPink, LinkListBLOB *listBlobCyan, int width, int height, Point fieldLim1, Point fieldLim2);
	BOOL hsiIsColor( HSI h , int color );
	void addToRLE(Point scann, HSI myhsi, int color, int *flag, LinkListRLE *L);
	void formBlobs(LinkListBLOB *listBlob, LinkListRLE *listRLE);
	void uniteBlob(LinkListBLOB *listBlob, int tag1, int tag2, LinkListRLE *listRLE);
	void orderBlobs(LinkListBLOB *L);
	void firstNNodes(LinkListBLOB *L, int n, int area);
	void firstNNodes(LinkListBLOB *L, int area);
	void showDots(BYTE *b, HSI myhsi, int cont);
	
#endif