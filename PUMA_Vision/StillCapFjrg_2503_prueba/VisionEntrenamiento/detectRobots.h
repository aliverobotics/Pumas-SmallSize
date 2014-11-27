#include "linklistblob.h"

#ifndef __DETECTROBOTS_H__
	
	#define __DETECTROBOTS_H__
	
	typedef struct
	{
		nodeBlob elem[5];	/**< Auxiliar patches. elem[0] is the main patch */
		int dist[5];
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
		int		team;		/**< The robot's team */
		int		radius;		/**< The robot's radius */
		FPOINT	realPos;	/**< Robot's position in dm. */
	} ROBOT;


#ifdef comment
	/** Robot's codes
	*
	* v=1, r=2, a=4; v=green, r=pink, a=cyan
	*/
	enum ROBOT_CODES
	{
		r0 = 3,		/**< vvv */
		r1,			/**< vvr */
		r2 = 6,		/**< vva */ 
		r3,			/**< vra */
		r4,			/**< rva */
		r5,			/**< rra */
		r6,			/**< arv */
		r7 = 12,	/**< aav */
//		r8,			/**< aar */
//		r9 = 16		/**< aaa */
	};
#endif
/*******Jair codigos de color 21agosto06***********/
	/* v = 10, r = 100, a = 1000 */
	#define r0 120		/* vvr */
	#define r1 30		/* vvv */
	#define r2 300		/* rrr */
	#define r3 210		/* rrv */
	#define r4 1200		/* rra */
	#define r5 2010		/* aav */
	#define r6 2100		/* aar */


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
	int GroupsObjects( LinkListBLOB *pPatch, LinkListBLOB *pAux, GROUPOBJECTS pWin[], int dist );
	void BubbleSort( VECTOR win[], int n );
	int DetectsRobotOrientation( GROUPOBJECTS pWin, ROBOT *robot );
	void DetectsRobotVelocity( GROUPOBJECTS pWin, ROBOT *robot );
	void DecodesColorCode( GROUPOBJECTS pWin, int *val );
	int IdentifiesRobot( GROUPOBJECTS pWin, int value, ROBOT *robot );
	void FillsRobotInfoString( char *SckBuf, ROBOT robot, int TeamColor );
	void FillsBallInfoString( char *SckBuf, ROBOT robot);

	void potentialFields(ROBOT myRobots[10], int potRobot, FPOINT pelota, float *mvDist, float *mvAngle);

#endif
