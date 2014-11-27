#include "linklistblob.h"

#ifndef __DETECTROBOTS_H__
#define __DETECTROBOTS_H__
	
	typedef struct
	{
		nodeBlob elem[5];	/**< Auxiliar patches. elem[0] is the main patch */
		int cont;			/**< Stores how many aux patches were recognized. elem[0] is not counted */
	} GROUPOBJECTS; 

	typedef struct
	{
		Point p0;
		Point p1;
		int dist;

		struct
		{
			float x;
			float y;
		} mid;
	} VECTOR;


	/** It contains the most important info about a robot */
	typedef struct
	{
		int		robot;		/**< The robot's ID (0 to up to 10) */
		Point	pos;		/**< The coords of the robot */
		float	angle;		/**< The direction at which the robot is looking at (0-360)*/
		float	vel;		/**< The velocity at which robot is moving it */
	} ROBOT;


	/** Robot's codes
	*
	* v=1, r=2, a=4; v=green, r=pink, a=cyan
	*/
	enum ROBOT_CODES
	{
		r0 = 4,		/**< vvvv */
		r1,			/**< vvvr */
		r2 = 7,		/**< vvva */ 
		r3,			/**< vvra */
		r4,			/**< rrva */
		r5,			/**< rrra */
		r6,			/**< aarv */
		r7 = 13,	/**< aaav */
		r8,			/**< aaar */
		r9 = 16		/**< aaaa */
	};

	/** Robot's id's */
	enum ROBOT_ID
	{	
		ROBOT0,
		ROBOT1,
		ROBOT2,
		ROBOT3,
		ROBOT4,
		ROBOT5,
		ROBOT6,
		ROBOT7,
		ROBOT8,
		ROBOT9
	};



	int hsiEuclidianDistance( Point a0, Point a1 );
	float hsiEuclidianDistance( FPOINT a0, FPOINT a1 );
	int GroupsObjects( LinkListBLOB *pPatch, LinkListBLOB *pAux, GROUPOBJECTS pWin[], int dist );
	void BubbleSort( VECTOR win[], int n );
	int DetectsRobotOrientation( GROUPOBJECTS pWin, ROBOT *robot );
	void DetectsRobotVelocity( GROUPOBJECTS pWin, ROBOT *robot );
	void DecodesColorCode( GROUPOBJECTS pWin, int *val );
	int IdentifiesRobot( GROUPOBJECTS pWin, int value, ROBOT *robot );
	void FillsRobotInfoString( char *SckBuf, ROBOT robot, int TeamColor );
#endif