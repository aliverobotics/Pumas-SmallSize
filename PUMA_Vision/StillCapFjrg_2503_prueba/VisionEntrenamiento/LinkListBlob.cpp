#include <io.h>
#include "stdafx.h"
#include "LinkListBlob.h"

/*==================================================================================
*						initBlob()
==================================================================================*/

/** Initialize the link list
*
* Points the list to NULL.
*
* \param L Link list
*
*/
void initBlob(LinkListBLOB *L){
    *L = NULL;
}


/*==================================================================================
*						insertBlob()
==================================================================================*/

/** Inserts blob at the begining of the list.
*
* \param L	Link list
* \param e	Blob to insert 
*
*/
void insertBlob(LinkListBLOB *L, BLB e){
	nodeBlob *n;
	n = createnodeBlob(e);
	n->next = *L;
	*L = n;
}


/*==================================================================================
*						insertOrderBlob()
==================================================================================*/

/** Inserts blob in order by area.
*
* \param L	Link list
* \param e	Blob to insert 
*
*/
void insertOrderBlob(LinkListBLOB *L, BLB e){
	nodeBlob *n;
	LinkListBLOB auxList, tempList;
	n = createnodeBlob(e);
	if ( isEmptyBlob(L) || e.area > (*L)->info.area ){
		n->next = *L;
		*L = n;
	}
	else {
		duplicateBlob(&auxList, L);
		duplicateBlob(&tempList, L);
		while(auxList->next!=NULL && e.area < auxList->next->info.area)
			auxList = auxList->next;
		n->next = auxList->next;
		auxList->next = n;
	}
}


/*==================================================================================
*						insertBlob()
==================================================================================*/

/** Inserts blob at the begining of the list.
*
* \param L	Link list
* \param e	Blob to insert 
*
*/

/*void listDeleteNextBlob(LinkListBLOB *L){
	nodeBlob *aux;
	if (*L == NULL) 
		return;
	
    aux = (*L)->next;
	(*L)->next = aux->next;
	free(aux);
}*/



/*==================================================================================
*						listDeleteBlob()
==================================================================================*/

/** Deletes blob at the begining of the list.
*
* \param L	Link list
*
* \return	Deleted blob
*
*/
BLB listDeleteBlob(LinkListBLOB *L){
	nodeBlob *aux;
	BLB e;
	if ((*L) == NULL){
		e.area = -1;
		return e;
	}
	aux = *L;
	*L = (*L)->next;
	e = aux->info;
	free(aux);
	return e;
}


/*==================================================================================
*						createnodeBlob()
==================================================================================*/

/** Allocates memory for a node
*
* \param e	Information of the blob
*
* \return	Pointer to new node
*
*/
nodeBlob * createnodeBlob(BLB e){
	nodeBlob *n;
	n = (nodeBlob *)malloc(sizeof(nodeBlob));
	n->info = e;
	n->next = NULL;
	return n;
} 


/*==================================================================================
*						queryTopBlob()
==================================================================================*/

/** Returns the information of the first element of the list
*
* \param L	List
*
* \return	Information of the first node
*
*/
BLB *queryTopBlob(LinkListBLOB *L){
	return &((*L)->info);
}


/*==================================================================================
*						nextBlob()
==================================================================================*/

/** Points the list to the next node.
*
* \param L	List
*
*/
void nextBlob(LinkListBLOB *L){
	(*L) = (*L)->next;
}


/*==================================================================================
*						isEmptyBlob()
==================================================================================*/

/** Query if the list is empty. 
*
* \param L	List
*
* \return	1 if it's empty; 0 otherwise.
*
*/
int isEmptyBlob(LinkListBLOB *L){
	return (*L == NULL);
}


/*==================================================================================
*						destroyBlob()
==================================================================================*/

/** Destroys the list completely, freeing all memory.
*
* \param L	List
*
*/
void destroyBlob(LinkListBLOB *L){
	while( !isEmptyBlob(L) )
		listDeleteBlob(L);
}


/*==================================================================================
*						locateTagBlob()
==================================================================================*/

/** Points the list to the node whos tag is "id".
*
* \param L	List
* \param id	Tag to locate
*
*/
void locateTagBlob(LinkListBLOB *L, int id){
	while(*L!=NULL){
		if((*L)->info.tag==id) break;
		else *L = (*L)->next;
	}
}


/*void locateTagNextBlob(LinkListBLOB *L, int id){
	while((*L)->next!=NULL){
		if((*L)->next->info.tag==id) break;
		else *L = (*L)->next;
	}
}
*/


/*==================================================================================
*						duplicateBlob()
==================================================================================*/

/** Douplicates the pointer to a list.
*
* \param lDest	Destination list
* \param lSrc	Source list
*
*/
void duplicateBlob(LinkListBLOB *lDest, LinkListBLOB *lSrc){
	*lDest = *lSrc;
}


/*==================================================================================
*						listCatBlob()
==================================================================================*/

/** Concatenates two lists.
*
* \param l1	First list.
* \param l2	Second list.
*
*/
void listCatBlob(LinkListBLOB *l1, LinkListBLOB *l2){
	LinkListBLOB lAux;
	lAux = *l1;
	if ( *l1 == NULL )
		*l1 = *l2;
	else {
		while ( lAux->next != NULL )
			lAux = lAux->next;
		lAux->next = *l2;
	}
}


/*==================================================================================
*						list2file()
==================================================================================*/

/** Creates a file with all the list information
*
* \param fName	File name
* \param L		List
*
*/
void list2file(char *fName, LinkListBLOB L){
	FILE *fp;
	fp = fopen(fName, "w");
	while (!isEmptyBlob(&L)){
		fwrite( &(L->info), sizeof(BLB), 1, fp);
		L = L->next;
	}
	fclose(fp);
}


/*==================================================================================
*						file2list()
==================================================================================*/

/** Creates a list using the information in the file
*
* \param L		List
* \param fName	File name
*
*/
void file2list(LinkListBLOB *L, char *fName){
	FILE *fp;
	BLB aux;
	initBlob(L);
	int n=1;
	fp = fopen(fName, "r");	
	while (1){
		if ( fread( &aux, sizeof(BLB), 1, fp ) == 0 ) 
			break;
		insertOrderBlob(L, aux);
	}
    fclose(fp);
}


/*==================================================================================
*						list2Afile()
==================================================================================*/

/** Creates a text file containing all the list information
*
* \param fName	File name
* \param L		List
*
*/
void list2Afile(char *fName, LinkListBLOB L){
	FILE *fp;
	fp = fopen(fName, "wt");
	while (!isEmptyBlob(&L)){
		fprintf(fp,"tag: %d, area: %d, color: %d, x: %d, y: %d\n", L->info.tag, L->info.area, L->info.color, L->info.ctr.x, L->info.ctr.y);
		L = L->next;
	}
	fclose(fp);
}