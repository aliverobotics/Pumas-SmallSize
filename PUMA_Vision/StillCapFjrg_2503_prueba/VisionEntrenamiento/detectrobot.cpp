#include "stdafx.h"

#include "math.h"
#include "detectRobots.h"

/*==================================================================================
*						hsiEuclidianDistance()
==================================================================================*/
/** Calculates the euclidean distance 
*
* \param a0	First point
* \param a1	Second point
*
* \return	The euclidean distance
*/

int hsiEuclidianDistance( Point a0, Point a1 )
{
	int dist;

	dist = (int)sqrt( (a1.x-a0.x)*(a1.x-a0.x) + (a1.y-a0.y)*(a1.y-a0.y) );

	return( dist );
}

/*==================================================================================
*						GroupsObjects()
==================================================================================*/

/** Searches for the auxiliar patches inside a ratio that surrounds the central one
*
*  
*
* \param pPatch		Points to the linked list main patches.
* \param pAux		Points to the linked list of the auxiliar patches.
* \param pWin		Points to the linked list that will store the nearest auxiliar patches.
* \param dist		Max distance the auxiliar patch must be. 
*
* \return			-1 on error; otherwise, the number of main patches founded
*
* \todo				Perhaps, once used, erase the element from the auxiliar patches list in order to
*					increment speed in subsecuent searches. 
*/
int GroupsObjects( LinkListBLOB *pPatch, LinkListBLOB *pAux, GROUPOBJECTS pWin[], int dist )
{
	int diff, contelem = 0, contmain = -1;
	LinkListBLOB patch, aux;

	//static int contm = 0;


	patch = *pPatch;
	

	while( patch != NULL )
	{
		contelem = 0;
		aux = *pAux;

/*		contm++;
		if ((int)aux < 0x100)
			aux=NULL;
*/
		pWin[++contmain].elem[contelem++] = *patch;

		while( aux != NULL )
		{
			diff = abs( hsiEuclidianDistance( patch->info.ctr, aux->info.ctr ) );
			
			if( diff < dist )
			{
				pWin[contmain].elem[contelem++] = *aux;

				if( contelem > 4 )
					break;
			}
			aux = aux->next;
		}
		pWin[contmain].cont = contelem-1;

		patch = patch->next;
	}

	return (contmain+1);
}


#define MIN(x,y) ( (x<y) ? x : y ) 


/*==================================================================================
*						BubbleSort()
==================================================================================*/

/** Sort algorithm 
*
* After the sorting, the array is sorted from the largest to the shortest distance
* from patch to patch
* 
* \param win	Array that contains the info from the 4 aux patches
* \param n		The number of item in win. Ideally it will be 4
*/
void BubbleSort( VECTOR win[], int n )
{
	int i, j;
	VECTOR swap;

	for( i = 0; i < n-1; ++i )
		for( j = 1; j < n-i; ++j )
			if( win[ j-1 ].dist < win[ j ].dist )
			{
				swap = win[ j-1 ];
				win[ j-1 ] = win[ j ];
				win[ j ] = swap;
			}
}



/*==================================================================================
*						DetectsRobotOrientation()
==================================================================================*/

/** Detects the orientation of the robot
*
* \param pWin	Array that contains the info of the auxiliar patches (from index 1 to 4)
* \param dir	It will contain the direction of the robot
*
* \return		0 if error; 1 otherwise
*/
int DetectsRobotOrientation( GROUPOBJECTS pWin, ROBOT *robot )
{
	int cont, x, y, cuad;;
	float dir;

	VECTOR vector[4], *pV; 
	

	pV = vector;
	

	// Fills the p0/p1 info
	//for( cont = 0; cont < 3; cont++ )
	for( cont = 0; cont < pWin.cont-1; cont++ )
	{
		(pV+cont)->p0 = pWin.elem[cont+1].info.ctr;
		(pV+cont)->p1 = pWin.elem[cont+2].info.ctr;
		(pV+cont)->dist = hsiEuclidianDistance( (pV+cont)->p0, (pV+cont)->p1 );
	}
	(pV+cont)->p0 = pWin.elem[cont+1].info.ctr;
	(pV+cont)->p1 = pWin.elem[1].info.ctr;
	(pV+cont)->dist = hsiEuclidianDistance( (pV+cont)->p0, (pV+cont)->p1 );

	// Calculates the middle point
	//for( cont = 0; cont < 4; cont++ )
	for( cont = 0; cont < pWin.cont; cont++ )
	{
		(pV+cont)->mid.x = ( (pV+cont)->p1.x + (pV+cont)->p0.x ) / 2.0;
		(pV+cont)->mid.y = ( (pV+cont)->p1.y + (pV+cont)->p0.y ) / 2.0;
	}

	
	/* Sorts the array. vector[0] is the largest value */
	BubbleSort( vector, pWin.cont );
	
	/* Gets the distance between the middle point from the shortest and largest axis */
	x = vector[0].mid.x - vector[pWin.cont-1].mid.x;
	y = vector[0].mid.y - vector[pWin.cont-1].mid.y;

	
	if (x==0) dir = (float)(3.14159 / 2);
	else
		dir = atan( (float)(y / x) );

	dir *= 57.29;

	/* In which cuadrant the vector is?? */
	if( x >= 0 && y >= 0 )
		cuad = 0;
	else if( x < 0 && y >= 0 )
		dir = 180 - dir;
	
	else if( x < 0 && y < 0 )
		dir = 180 + dir;
	else
		dir = 360 - dir;

	
	
	robot->angle = dir;
	
	return 1;

}

/*==================================================================================
*						DetectsRobotVelocity
==================================================================================*/

void DetectsRobotVelocity( GROUPOBJECTS pWin, ROBOT *robot )
{
	robot->vel = 0.0;
}




/*==================================================================================
*						DecodeColorCode
==================================================================================*/

/** Decodes  the code color for a robot
*
* \param pWin	Array that contains the info of the patches ([0] for main; [1..4] for aux)
*
* \return 		The value decoded
*/
void DecodesColorCode( GROUPOBJECTS pWin, int *val )
{
	int cont, value, color;
	
	for( cont = 1, value = 0; cont <= pWin.cont; cont++ )
	{
		color = pWin.elem[cont].info.color;
		switch( color )
		{
			case green:		value += 1; break;
			case pink:		value += 2; break;
			case cyan:	value += 4; break;
		}
	}

	*val = value;
}

/*==================================================================================
*						IdentifiesRobot
==================================================================================*/

/** Gets the robot's id and its position
*
* \param pWin	Array that contains the info of the patches ([0] for main; [1..4] for aux)
* \param value	The code color that was detected
* \param robot	Struct tha will contain the robot's id, position and eventually its direction
* \return		-1 if 'value' doesn't match which any robot; 1 otherwise
*/
int IdentifiesRobot( GROUPOBJECTS pWin, int value, ROBOT *robot )
{
	int index;

	switch( value )
	{
		case r0: index = ROBOT0; break;
		case r1: index = ROBOT1; break;
		case r2: index = ROBOT2; break;
		case r3: index = ROBOT3; break;
		case r4: index = ROBOT4; break;
		case r5: index = ROBOT5; break;
		case r6: index = ROBOT6; break;
		case r7: index = ROBOT7; break;
		case r8: index = ROBOT8; break;
		case r9: index = ROBOT9; break;

		default: index = -1; break;
	}

	if( index == -1 )
		return( -1 );
	else
	{
		robot->robot = index;
		robot->pos = pWin.elem[0].info.ctr;
		return( 1 );
	}
}


/*==================================================================================
*						FillsRobotInfoString
==================================================================================*/

void FillsRobotInfoString( char *SckBuf, ROBOT robot, int TeamColor )
{
	CString str;

	str = TeamColor == 0 ? "INFO P " : "INFO A ";

	sprintf( SckBuf, "%02d %04d %04d %04f %04f END ", 
			 robot.robot, robot.pos.x, robot.pos.y, robot.vel, robot.angle );
	str += SckBuf;
	
	sprintf( SckBuf, "%s", str );	
}