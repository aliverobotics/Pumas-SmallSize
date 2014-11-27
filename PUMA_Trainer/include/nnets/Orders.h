/*
 *  Orders.h
 *  McBrain
 *
 *  Created by Adalberto H. Llarena on 11/13/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */
 
#ifndef _ORDERS_
#define _ORDERS_

int* Order_PTR = NULL; //uset to establish a reading order

//gets a random order of test
void Generate_Order (int MAX)
{
	//we get memory
	if (Order_PTR == NULL)
		Order_PTR =(int*)malloc(MAX*sizeof(int));
		
	int i;

	//we clear data
	for (i=0;i<MAX;i++)
		*(Order_PTR+i) = int (-1);

	//we get the numbers
	for (i=0;i<MAX;i++)
	{
		//we get randomly a position
		int random_int = Random_Int (0,MAX-1);

		int offset = 0;
		//we search next free position near to random int
		while (*(Order_PTR+random_int+offset) != -1)
		{
			offset++;
			if ((offset+random_int)==MAX) //if we reach the end
				offset = -1*random_int; //we go to start
		}

		//we set the value
		*(Order_PTR+random_int+offset) = i;
	}
}

void Free_Order (void)
{
	if (Order_PTR!=NULL)
		free (Order_PTR);
}

int Give_Order (int i)
{
	return *(Order_PTR+i); //starting in 0
}

void Print_Order(int MAX)
{
	fprintf (stdout, "[");
	for (int i=0;i<MAX;i++)
		fprintf (stdout, "%4d ",*(Order_PTR+i));
	fprintf (stdout, "]\n");
}

#endif
