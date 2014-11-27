#include "stdafx.h"

#ifndef __listrle__
	#define __listrle__

	/** It contains the most important info about a RLE */
	typedef struct{
		int x0;
		int x1;
		int y;
		int color;
		int size;
		int tag;
		int blobTag;
	} RLE;

	/**	Contains the structure of a node */
	typedef struct nRLE{
		struct nRLE * next;
		struct nRLE *prev;		
		RLE info;
	}nodeRLE;

	/** Definition of a linklist */
	typedef nodeRLE * LinkListRLE;

	void initRLE(LinkListRLE *L);
	void insertRLE(LinkListRLE *L, RLE e);
	nodeRLE * createnodeRLE(RLE e);
	RLE listDeleteRLE(LinkListRLE *L);
	RLE * queryTopRLE(LinkListRLE *L);
	void nextRLE(LinkListRLE *L);
	void destroyRLE(LinkListRLE *L);
	int isEmptyRLE(LinkListRLE *L);
	void duplicateRLE(LinkListRLE *lDest, LinkListRLE *lSrc);
#endif