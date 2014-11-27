#include "stdafx.h"
#include "math.h"
#include "detectRobots.h"
#include "colors.h"


	
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

	dist = (int)sqrt( (double)((a1.x-a0.x)*(a1.x-a0.x) + (a1.y-a0.y)*(a1.y-a0.y) ));

	return( dist );
}



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

float hsiEuclidianDistance( FPOINT a0, FPOINT a1 )
{
	int dist;

	dist = (int)sqrt( (double)((a1.x-a0.x)*(a1.x-a0.x) + (a1.y-a0.y)*(a1.y-a0.y) ));

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
int GroupsObjects( LinkListBLOB *pPatch, LinkListBLOB *pAux, GROUPOBJECTS pWin[], int maxDist )
{
#ifndef comment

	int i, mindist, minDistTo = -1, d, contelem[5]={0,0,0,0,0}, contmain = 0, tempDist;
	LinkListBLOB patch, aux;
	nodeBlob temp;

	//static int contm = 0;

	patch = *pPatch;
    
	for (i=0;i<5; i++)
		pWin[i].cont = 0;
	
	while( patch != NULL )
	{
		contelem[contmain] = 1;
		pWin[contmain++].elem[0] = *patch;
		patch = patch->next;
		if ( contmain >= 5 ) break;
	}
	
	aux = *pAux;
	
	if ( contmain > 0 ){
		while( aux != NULL )
		{
			mindist = 999999;
			minDistTo = -1;
			for ( i=0; i < contmain; i++){
				d = hsiEuclidianDistance( pWin[i].elem[0].info.ctr, aux->info.ctr );
				if ( d < mindist) {
 					mindist = d;
					minDistTo = i;
				}
			}
			if ( mindist > maxDist ) {
				aux = aux->next;
				continue;
			}
			if ( contelem[minDistTo] < 4 ){
				pWin[minDistTo].elem[contelem[minDistTo]] = *aux;
				pWin[minDistTo].dist[contelem[minDistTo]] = mindist;
				contelem[minDistTo]++;
			}
			for ( i = contelem[minDistTo]-1; i>0 ; i-- )
			{
				if (mindist <= pWin[minDistTo].dist[i])
				//	hsiEuclidianDistance( pWin[minDistTo].elem[i].info.ctr, pWin[minDistTo].elem[0].info.ctr ) )
				{
					temp = pWin[minDistTo].elem[i];
					tempDist = pWin[minDistTo].dist[i];
					pWin[minDistTo].elem[i] = *aux;
					pWin[minDistTo].dist[i] = mindist;
					if ( i < 3 ){
						pWin[minDistTo].elem[i+1] = temp;
						pWin[minDistTo].dist[i+1] = tempDist;
					}
				}
				else break;
			}
			
			pWin[minDistTo].cont = contelem[minDistTo]-1;
			aux = aux->next;
		}
	}
	return (contmain);

#endif


#ifdef comment

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

				if( contelem > 3 )
					break;
			}
			aux = aux->next;
		}
		pWin[contmain].cont = contelem-1;

		patch = patch->next;
	}
	return (contmain+1);
#endif

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
	int cont, x, y;
	float dir;

	VECTOR vector[3], *pV;
	Point pat;
	pat.x = 0;
	pat.y = 0;


	pV = vector;


	// Fills the p0/p1 info
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
	for( cont = 0; cont < pWin.cont; cont++ )
	{
		(pV+cont)->mid.x = ( (pV+cont)->p1.x + (pV+cont)->p0.x ) / 2.0;
		(pV+cont)->mid.y = ( (pV+cont)->p1.y + (pV+cont)->p0.y ) / 2.0;
	}

	/* Sorts the array. vector[0] is the largest value whereas vector[2] is the
shortest value*/
	BubbleSort( vector, pWin.cont );

	/* Finds the fartest patch */

	if( (vector[0].p1.x == vector[1].p0.x) && (vector[0].p1.y == vector[1].p0.y))
	{
		pat = vector[0].p1;
	}
	else if( (vector[0].p0.x == vector[1].p1.x) && (vector[0].p0.x ==
vector[1].p1.x) )
	{
		pat = vector[0].p0;
	}

	x = (float)(pat.x + pWin.elem->info.ctr.x)/2.0;
	y = (float)(pat.y + pWin.elem->info.ctr.y)/2.0;

	x -= vector[2].mid.x;
	y -= vector[2].mid.y;

	/* Gets the distance between the middle point from the shortest and largest
axis */
	/*
	x = vector[0].mid.x - vector[pWin.cont-1].mid.x;
	y = vector[0].mid.y - vector[pWin.cont-1].mid.y;
	*/




	if (x==0) dir = (float)(3.14159 / 2);
	else
		dir = (float)atan( (float)(fabs((float)y) / fabs((float)x)) ) ;

	dir *= 57.29;

	/* In which cuadrant the vector is?? */
	if( x < 0 && y >= 0 )
		dir = 180 - dir;

	else if( x < 0 && y < 0 )
		dir = 180 + dir;
	else if ( x > 0 && y < 0)
		dir = 360 - dir;



	robot->angle = dir;

	return 1;

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
			case green:		value += 10; break;
			case pink:		value += 100; break;
			case cyan:	value += 1000; break;
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
//		case r7: index = ROBOT7; break;
//		case r8: index = ROBOT8; break;
//		case r9: index = ROBOT9; break;

		default: index = -1; break;
	}

	if( index == -1 )
		return( -1 );
	else
	{
		robot->robot = index;
		robot->pos = pWin.elem[0].info.ctr;
		robot->team = pWin.elem[0].info.color;
		return( 1 );
	}
}


/*==================================================================================
*						FillsRobotInfoString
==================================================================================*/
void FillsRobotInfoString( char *SckBuf, ROBOT robot, int color )
{
	if( robot.team == color )
	{

sprintf( SckBuf, "INFO P %02d %04f %04f %04f %04f END", robot.robot, (float)(robot.realPos.x), (float)(robot.realPos.y), robot.vel, robot.angle);
	}
	else
	{
sprintf( SckBuf, "INFO A %04f %04f %04f END", (float)(robot.realPos.x), (float)(robot.realPos.y), robot.vel);

	}

	/*
	str = TeamColor == 0 ? "INFO P " : "INFO A ";

	sprintf( SckBuf, "%02d %04d %04d %04f %04f END ",
			 robot.robot, robot.pos.x, robot.pos.y, robot.vel, robot.angle );
	str += SckBuf;


	sprintf( SckBuf, "%s", str );
	*/
}


/*==================================================================================
*						FillsBallInfoString
==================================================================================*/

void FillsBallInfoString( char *SckBuf, ROBOT robot)
{
		sprintf( SckBuf, "INFO B %04f %04f %04f END", (float)(robot.realPos.x), (float)(robot.realPos.y), robot.vel );
}

 





/*==================================================================================
*						potentialFields
==================================================================================*/

void potentialFields(ROBOT myRobots[10], int potRobot, FPOINT pelota, float *mvDist, float *mvAngle)
{
	//Constantes
		
	float orientation=0.0, dmin=2, d0=0.07, n=100000, E2=0.00001, dminBorder=2,dist=1.0;

	char msg[255];
	
	float fattr,frep,d,thetai,thetaii;
	FPOINT repell,attract,force,totrepell;
	repell.x = attract.y = 0.0;
	attract.x = attract.y = 0.0;
	force.x = force.y = 0;
	totrepell.x = totrepell.y = 0;
	int i;
	//ATRACCION
	fattr=(float)-E2/hsiEuclidianDistance( pelota,myRobots[potRobot].realPos);
	attract.x=fattr*(myRobots[potRobot].realPos.x-pelota.x);
	attract.y=fattr*(myRobots[potRobot].realPos.y-pelota.y);
	//REPULSION
	totrepell.x = totrepell.y = 0.0f;
	for( i = 0 ; i < 10 ;i++ )
	{
		if ( i == potRobot ) continue;
		if ( myRobots[i].team == -10 ) break;

		d=hsiEuclidianDistance( myRobots[potRobot].realPos, myRobots[i].realPos);
		
		if (d>dmin)
		{
			repell.x = 0;
			repell.y = 0;
		}
		else 
		{
			frep = n*((float)(1/d)-(float)(1/d0));
			frep = frep*(float)(1/(d*d));
			frep = (float)(frep/d);
			repell.x=myRobots[potRobot].realPos.x-myRobots[i].realPos.x;
			repell.y=myRobots[potRobot].realPos.y-myRobots[i].realPos.y;
			repell.x=repell.x*frep;
			repell.y=repell.y*frep;
		}
		totrepell.x=totrepell.x+repell.x;
		totrepell.y=totrepell.y+repell.y;
	}	
/*
	#region Bordes	
	Point P[4];
		
	P[0]=new PointF(this.Position.X,this.myField.Field.extraBound);
	P[1]=new PointF(this.myField.Field.width-myField.Field.extraBound,
				   this.Position.Y);
	P[2]=new PointF(this.Position.X,
				   this.myField.Field.height-myField.Field.extraBound);
	P[3]=new PointF(myField.Field.extraBound,this.Position.Y);

	for(i=0;i<4;i++)
	{
		d=getDistancePoints(this.Position,P[i]);	
		if (d>dminBorder)
		{
			repell.X=0;
			repell.Y=0;
		}
		else 
		{
			frep=n*((float)(1/d)-(float)(1/d0));
			frep=frep*(float)(1/(d*d));
			frep=(float)(frep/d);
			repell.X=this.Position.X-P[i].X;
			repell.Y=this.Position.Y-P[i].Y;
			repell.X=repell.X*frep;
			repell.Y=repell.Y*frep;
		}
		totrepell.X=totrepell.X+repell.X;
		totrepell.Y=totrepell.Y+repell.Y;
	}
	#endregion
*/
	force.x=(attract.x)-totrepell.x;
	force.y=(attract.y)-totrepell.y;
		//			force.X=attract.X;
		//			force.Y=attract.Y;
	if ( fabs(force.x) == 0.0 ){
		if ( force.y > 0 ) thetai = 3.14159 / 2;
		else if ( force.y < 0 ) thetai = -1.0 * 3.14159 / 2;
		else thetai = 0.0;
	}
	else 
		//thetai=(float)atan((double)(fabs(force.y)/fabs(force.x)));
		thetai=(float)atan2((double)(force.y),(double)(force.x));
		/*if(force.x<0 && force.y>0) thetai=(float)(thetai+3.14159/2.0);
		else if (force.x<0 && force.y<0) thetai=(float)(thetai+3.14159);
		else if (force.x>0 && force.y<0) thetai=(float)(thetai+3.14159*3.0/2.0);*/

	thetaii = thetai-myRobots[potRobot].angle;
	//286°
	//80°
	*mvDist = dist;
 	
	if ( thetaii > 3.14159 ) 
		*mvAngle = -1 * ( 2*3.14159 - thetaii );
	else if ( thetaii < -3.14159 ) 
		*mvAngle = 2*3.14159 + thetaii ;
	else *mvAngle = thetaii;
	
	//sprintf(msg,"move %f %f 0",dist,thetaii);
	//sprintf( msg, "Atrac: %.2f,%.2f\tDist: %.2f\tAng: %.2f", attract.x, attract.y, *mvDist, *mvAngle );
	//AfxMessageBox( msg );
	
	
	
	
	//this.Position.X=this.Position.X+dist*cos(thetai);
	//this.Position.Y=this.Position.Y+dist*sin(thetai);
	//this.Position.t=thetai;
} 
