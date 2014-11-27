#include "stdafx.h"
#include "LinkListRLE.h"


/*==================================================================================
*						initRLE()
==================================================================================*/

/** Initialize the link list
*
* Points the list to NULL.
*
* \param L Link list
*
*/
void initRLE(LinkListRLE *L){
    *L = NULL;
}


/*==================================================================================
*						insertRLE()
==================================================================================*/

/** Inserts a RLE at the begining of the list.
*
* \param L	Link list
* \param e	RLE to insert 
*
*/
void insertRLE(LinkListRLE *L, RLE e){
	nodeRLE *n;
	n = createnodeRLE(e);
	n->next = *L;
	*L = n;
}


/*==================================================================================
*						listDeleteRLE()
==================================================================================*/

/** Deletes RLE at the begining of the list.
*
* \param L	Link list
*
* \return	Deleted RLE
*
*/
RLE listDeleteRLE(LinkListRLE *L){
	nodeRLE *aux;
	RLE e;
	aux = *L;
	*L = (*L)->next;
	e = aux->info;
	free(aux);
    return e;
}


/*==================================================================================
*						createnodeRLE()
==================================================================================*/

/** Allocates memory for a node
*
* \param e	Information of the RLE
*
* \return	Pointer to new node
*
*/
nodeRLE * createnodeRLE(RLE e){
	nodeRLE *n;
	if ( (n = (nodeRLE *)malloc(sizeof(nodeRLE))) == NULL)
		exit(1);
	n->info = e;
	n->next = NULL;
	n->prev = NULL;
	return n;
}


/*==================================================================================
*						queryTopRLE()
==================================================================================*/

/** Returns the information of the first element of the list
*
* \param L	List
*
* \return	Information of the first node
*
*/
RLE * queryTopRLE(LinkListRLE *L){
	return &((*L)->info);
}


/*==================================================================================
*						nextRLE()
==================================================================================*/

/** Points the list to the next node.
*
* \param L	List
*
*/
void nextRLE(LinkListRLE *L){
	(*L) = (*L)->next;
}


/*==================================================================================
*						isEmptyRLE()
==================================================================================*/

/** Query if the list is empty. 
*
* \param L	List
*
* \return	1 if it's empty; 0 otherwise.
*
*/
int isEmptyRLE(LinkListRLE *L){
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
void destroyRLE(LinkListRLE *L){
	while( !isEmptyRLE(L) )
		listDeleteRLE(L);
}


/*==================================================================================
*						duplicateRLE()
==================================================================================*/

/** Douplicates the pointer to a list.
*
* \param lDest	Destination list
* \param lSrc	Source list
*
*/
void duplicateRLE(LinkListRLE *lDest, LinkListRLE *lSrc){
	*lDest = *lSrc;
}