#include "linklistrle.h"
#include "point.h"

#ifndef __listblob__
	#define __listblob__

	/** It contains the most important info of a blob */
	typedef struct
	{
		int area;
		int color;
		Point p0;
		Point p1;
		Point ctr;
		int tag;
	}BLB;

	/**	Contains the structure of a node */
	typedef struct nBLOB{
		struct nBLOB * next;
		BLB info;
	} nodeBlob;
	
	/** Definition of a linklist */
	typedef nodeBlob * LinkListBLOB;

	void initBlob(LinkListBLOB *L);
	void insertBlob(LinkListBLOB *L, BLB e);
	void insertOrderBlob(LinkListBLOB *L, BLB e);
	nodeBlob * createnodeBlob(BLB e);
	BLB listDeleteBlob(LinkListBLOB *L);
	//void listDeleteNextBlob(LinkListBLOB *L);
	BLB *queryTopBlob(LinkListBLOB *L);
	void nextBlob(LinkListBLOB *L);
	void destroyBlob(LinkListBLOB *L);
	int isEmptyBlob(LinkListBLOB *L);
	void locateTagBlob(LinkListBLOB *L, int id);
	//void locateTagNextBlob(LinkListBLOB *L, int id);
	void duplicateBlob(LinkListBLOB *lDest, LinkListBLOB *lSrc);
	void listCatBlob(LinkListBLOB *l1, LinkListBLOB *l2);
	void file2list(LinkListBLOB *L, char *fName);
	void list2file(char *fName, LinkListBLOB L);
	void list2Afile(char *fName, LinkListBLOB L);
#endif