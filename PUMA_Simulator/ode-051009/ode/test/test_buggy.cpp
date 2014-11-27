/*************************************************************************
 *                                                                       *
 * Open Dynamics Engine, Copyright (C) 2001,2002 Russell L. Smith.       *
 * All rights reserved.  Email: russ@q12.org   Web: www.q12.org          *
 *                                                                       *
 * This library is free software; you can redistribute it and/or         *
 * modify it under the terms of EITHER:                                  *
 *   (1) The GNU Lesser General Public License as published by the Free  *
 *       Software Foundation; either version 2.1 of the License, or (at  *
 *       your option) any later version. The text of the GNU Lesser      *
 *       General Public License is included with this library in the     *
 *       file LICENSE.TXT.                                               *
 *   (2) The BSD-style license that is included with this library in     *
 *       the file LICENSE-BSD.TXT.                                       *
 *                                                                       *
 * This library is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files    *
 * LICENSE.TXT and LICENSE-BSD.TXT for more details.                     *
 *                                                                       *
 *************************************************************************/

/*

buggy with suspension.
this also shows you how to use geom groups.
Modificated by Adalberto H. Llarena to be a small-size simulator

*/
#define _CRT_SECURE_NO_DEPRECATE

#ifdef WIN32
#include "StdAfx.h"
#endif

#define DEMOBALL
//#define CLOSED
//#define SEND_TO_EACH

#define FRAMES_PER_SECOND 30

#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include <math.h>
#include "pthread.h"
#include "Threadtime.h"
#include "sched.h"
#include <GL/gl.h> 
#include <time.h>

pthread_mutex_t lock; //to lock vision when modifying simulation
pthread_cond_t cond; //to fire threads

void delay(float s)
{
	clock_t ti,tf,t;
	ti = clock();
	do
	{
		tf = clock();
		t = tf - ti;
	}while((float)((float)t/(float)CLOCKS_PER_SEC) < s);
}

//#define DEGREES

#ifdef _MSC_VER
#pragma warning(disable:4244 4305)  // for VC++, no precision loss complaints
#endif

// select correct drawing functions

#ifdef dDOUBLE
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCappedCylinder dsDrawCappedCylinderD
#endif

// some constants

#define WORLDSTEP 0.05
#define THREADCOUNT 2

#define LENGTH 0.7	// chassis length
#define WIDTH 0.9	// chassis width
#define HEIGHT 0.7	// chassis height
#define RADIUS 0.2	// wheel radius
#define STARTZ 0.5	// starting height of chassis
#define CMASS 0.5		// chassis mass
#define WMASS 0.5	// wheel mass
#define SMASS 1.0   //shooter mass
#define ITERS 20		// number of iterations
#define RRADIUS 0.9 //robot radius
#define BALLMASS 0.3		// ball mass
#ifndef DEMOBALL
#define BALL_RADIUS 0.15 //ball radius
#else
#define BALL_RADIUS 0.4 //ball radius
#endif

#define LEVELGRND 0.01 //ground, lines, dots, and circles refenence

#define HEIGHTWALL 2
#define HEIGHTGOALS 4.0

#define HEIGHTDOTS 0.04
#define HEIGHTGRCIRC 0.03
#define HEIGHTWHCIRC 0.02
#define HEIGHTGROUND 0.01

#define GOALTIME  200 //celebrating
#define PATCH_RADIUS 0.2

#ifdef DEMOBALL
int TOT_PUMAS   = 1; //number of pumas
int TOT_AMERICA = 0; //number of america
#else
int TOT_PUMAS   = 4; //number of pumas
int TOT_AMERICA = 4; //number of america
#endif

// dynamics and collision objects (chassis, 3 wheels, environment)

static dWorldID world;
static dSpaceID space;
static dJointGroupID contactgroup;
static int bodies;

//ground
static dGeomID ground;

//for field
static dGeomID ground_box, b1, b2, b3, b4;
static dGeomID lines[30];
static dGeomID circles[30];
static dGeomID boxes[30];

//for ball
static dBodyID b;
static dGeomID ball[1];

//for goals
static dGeomID goal[2];

//colors for patches
#define BLACK 0
#define PINK 1
#define RED 2
#define ORANGE 3
#define YELLOW 4
#define GREEN 5
#define BLUE 6
#define CYAN 7
#define GRAY 8
#define WHITE 9

float colors [10] [3]= {
{ 0.0, 0.0, 0.0 }, //black
{ 0.9, 0.0, 0.9 }, //pink
{ 1.0, 0.0, 0.0 }, //red
{ 1.0, 0.5, 0.0 }, //orange
{ 1.0, 1.0, 0.0 }, //yellow
{ 0.0, 1.0, 0.2 }, //green
{ 0.0, 0.0, 1.0 }, //blue
{ 0.0, 0.8, 1.0 }, //cyan
{ 0.5, 0.5, 0.5 }, //grat
{ 1.0, 1.0, 1.0 }  //white
};

int color_combination [10][4] = {
{ YELLOW, GREEN, PINK, GREEN },
{ YELLOW, PINK, PINK, GREEN },
{ YELLOW, GREEN, GREEN, PINK },
{ YELLOW, PINK, GREEN, GREEN },
{ YELLOW, GREEN, GREEN, PINK },
{ BLUE, GREEN, PINK, GREEN },
{ BLUE, PINK, PINK, GREEN },
{ BLUE, GREEN, GREEN, PINK },
{ BLUE, PINK, GREEN, GREEN },
{ BLUE, GREEN, GREEN, PINK }
};

float positions [10] [2]= {
{ -10.0, 5.0 }, 
{  10.0, 5.0 },  
{ -10.0, 15.0 }, 
{  10.0, 15.0 }, 
{   0.0, 25.0 }, 
{ -10.0, -5.0 }, 
{  10.0, -5.0 },  
{ -10.0, -15.0 }, 
{  10.0, -15.0 }, 
{   0.0, -25.0 },   
};

/* For the communications with sockets */
#include "msocket.h"
#define BUFFERSIZE 1024

char Chost[BUFFERSIZE],Ahost[BUFFERSIZE], Phost[BUFFERSIZE];

/********** ANGLES **********/

/* Convert from radians to degrees */
float rad2deg( float rad ) {
	return( rad*180.0/M_PI );
}

/* Convert from degrees to radians */
float deg2rad( float deg ) {
	return( deg*M_PI/180.0 );
}

/* ¬¨¬¨  its obvious */
float normalizeAngle( float angle ) {
   
   while( angle < -M_PI ) angle += (2.0*M_PI);
   while( angle >  M_PI ) angle -= (2.0*M_PI);
			
   return( angle );
}

// funcion que devuelve el angulo de un vector en radianes
float Angulo(float VX, float VY)
{
	if (fabs(VX)<0.1 && fabs(VY)<0.1) return 0;
	if (VX==0)
		if (VY >0)
		   return M_PI/2.0; // pi/2 radianes = 90o
		else
		   return 1.5 * M_PI; //-pi / 2
	else
		if (VY==0)
		   if (VX >0)
		      return 0; // 0 radianes = 0o
		   else
			  return M_PI; //pi
		else
		{
		   double Theta = atan(VY/VX);

		   if (VY<0)
			   if (VX<0)
				   return M_PI + Theta; //3er cuadrante
			   else
				   return 2*M_PI + Theta; //4o cuadrante
		   else
			   if (VX<0)
				   return M_PI + Theta; //2o cuadrante
			   else
				   return Theta; //1er cuadrante
		}
}

void resetSimulation();

/****************************/
// this is called by dSpaceCollide when two objects in space are
// potentially colliding.

static void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
  int i,n;

  //discard field
  //lines
  for (i=0;i<17;i++)
	  if (o1==lines[i] || o2==lines[i]) return;

  //circles
  for (i=0;i<6;i++)
	  if (o1==circles[i] || o2==circles[i]) return;
		  
  //areas
  for (i=0;i<2;i++)
	  if (o1==boxes[i] || o2==boxes[i]) return;

  //goals
  for (i=0;i<2;i++)
	  if (o1==goal[i] || o2==goal[i]) return;
				
  const int N = 10;
  dContact contact[N];
  n = dCollide (o1,o2,N,&contact[0].geom,sizeof(dContact));
  if (n > 0) {
    for (i=0; i<n; i++) {
      contact[i].surface.mode = dContactSlip1 | dContactSlip2 |
	dContactSoftERP | dContactSoftCFM | dContactApprox1;
      contact[i].surface.mu = dInfinity;
      contact[i].surface.slip1 = 0.1;
      contact[i].surface.slip2 = 0.1;
      contact[i].surface.soft_erp = 0.5;
      contact[i].surface.soft_cfm = 0.3;
      dJointID c = dJointCreateContact (world,contactgroup,&contact[i]);
      dJointAttach (c,
		    dGeomGetBody(contact[i].geom.g1),
		    dGeomGetBody(contact[i].geom.g2));
    }
  }
}

float pel	[3] = {0, 10, 0};
float rob	[10][3] = {0, 0, 89.99};
float deltapel	[3] = {0, 0, 0};
float deltarob	[10][3] = {0, 0, 0};
float deltavel  [2] = {0,0};
float por	[3] = {0, 0, 0};
int rcp_net = 0;
float porteria[4]= {80,200,130,200}; //xi, yi, xf, yf
double data[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; //to test nets

void clean_pos(void)
{
	int i,j;
	for (i=0;i<10;i++)
		for (j=0;j<3;j++)
		{
			rob	[i][j] = -100.0;
			deltarob [i][j] = 0.0;
		}
}

class small_size {
public:
	dBodyID body[10];
	dJointID joint[9];	// joint[0] is the front wheel
	dGeomID sphere[3];   //wheels plus crazy wheel
	dSpaceID car_space;
	dGeomID box[3]; //for chassis body and shooter
	dGeomID patches[4]; //cilinders for patches
	//dGeomID cylinder[1]; //for chassis body, not working
	
	bool shooting;

	// things that the user controls
	dReal speed,speedleft,speedright,steer,shooterspeed;	// user commands

	/* Socket to receive the commands */
	msocketData inport;
	/* Socket to send data */
	msocketData outport;

	bool training;
	bool goal;
	bool othergoal;
	bool reset;
	bool autoshoot;
	bool autoctrl;
	int  target;
	int behavior; //default attack
	bool shoot;
	bool stopped;
	bool pumas_team;
	bool going_left; //false for Pumas
	
	float vel [2]; //izq der
	float distang [2];
	float repulsion [2]; //repulsive velocities

	// robot's ID
    static int	robot_count;
	int			robot_num;
	 
	//constructor
	small_size(int robot_number, bool arewepumas)
	{
		robot_num = robot_number;
		robot_count++;
		shooting = false;
		
		// things that the user controls
		speed=0;speedleft=0;speedright=0;steer=0;shooterspeed=0; // user commands
		
		training = false;
		goal = false;
		othergoal = false;
		reset = false;
		autoshoot = true;
		autoctrl = true;
		target = -1; //ball
		behavior = 0; //default potentials
		shoot = false;
		stopped = false;
		pumas_team = arewepumas;
		going_left = !pumas_team; //false for Pumas
		
		vel[0]=0; vel[1]=0;  //izq der
		distang [0] = 0; distang [1] = 0;
		repulsion [0] =0; repulsion [1] =0;//repulsive velocities
	}
	
	//destructor
	~small_size()
	{
	  dGeomDestroy (box[0]);
	  //dGeomDestroy (cylinder[0]);
	  dGeomDestroy (sphere[0]);
	  dGeomDestroy (sphere[1]);
	  dGeomDestroy (sphere[2]);
	  dGeomDestroy (box[1]);
	}
	
	int init_sockets(int inport_num, char* outport_addr, int outport_num)
	{
		int error;
		
		/* The server, where it will receive a command from  */
			if ((error = inport.msocketInitSocket( "127.0.0.1", inport_num, RX_SOCKET ))) {
			   printf("error opening server: %s", msocketGetError( error ) );
			   return 1;
			}
			
			/* The client, where it will send the results  */
			if ((error = outport.msocketInitSocket( outport_addr, outport_num, TX_SOCKET ))) {
			   printf("error opening client: %s", msocketGetError( error ) );
			   return 2;
			}
		
		printf ("Ports Opened Succesfully\n");
		return 0;
	}

	void close_sockets(void)
	{
		outport.msocketKillSocket( );
		inport.msocketKillSocket( );
	}

	void stopcar(void)
	{
		this->speedleft=0;
		this->speedright=0;
		this->speed=0;
		this->shooterspeed=0;
	}

	//parses command string
	void process_cmd(char *cad)
	{
		if (strcmp (cad,"FF00000001\n")==0 || 
			strcmp (cad,"FF00000001")==0)
			  shooterspeed = 20.0;
		  else
		  if (strcmp (cad,"FF00000000\n")==0 ||
			  strcmp (cad,"FF00000000")==0)
		      shooterspeed = 0.0;
			else 
			{
				char motor[2]; //a0 izq, 0a derecha
				motor[0]=cad[6];
				motor[1]=cad[7];
				
				char dir[2]; //01 adelante, 02 atras
				dir[0]=cad[8];
				dir[1]=cad[9];

				unsigned int velo=0;
				int velocidad = 0;
				sscanf(&cad[10], "%x", &velo);
				//printf("cad = %s = %d\n", &cad[10], velocidad);
				//printf ("direction = %s, ", dir);
				if (dir[0]=='0')
					if (dir[1]=='1')
						velocidad = velo; //direction positive
					else
						if (dir[1]=='2')
						velocidad = -velo; //direction negative
						else
						   if (dir[1]=='0')
							velocidad = 0; //direction negative
						
				else return; //bad string
				
				if (abs(velocidad)<=512)
				{
					//to fit between 0 and 255
					if (motor[0]=='0' && motor[1]=='a')
						speedright = velocidad/32.0f;
					
					if (motor[0]=='a' && motor[1]=='0')
						speedleft = velocidad/32.0f;
				}	
			}
		
		return;
	}

	void receive_cmd(void) {
	   char buffer_in[BUFFERSIZE];
	   /* Wait until it receive something from the tx8 */
	   if(inport.msocketRxData(buffer_in, BUFFERSIZE) < 0)
		  return;
		else
		{
	      process_cmd(buffer_in); //it is the velocity
		  buffer_in[0]=0;
		}
	}
	
	void sim_joints(void)
	{
		// motor
		dJointSetHinge2Param (joint[1],dParamVel2,-speedleft);
		dJointSetHinge2Param (joint[1],dParamFMax2,0.1);
		dJointSetHinge2Param (joint[2],dParamVel2,-speedright);
		dJointSetHinge2Param (joint[2],dParamFMax2,0.1);
		
		//shooter
		dJointSetHinge2Param (joint[3],dParamVel2,shooterspeed);
		dJointSetHinge2Param (joint[3],dParamFMax2,0.1);
		
		// steering
		dReal v = steer - dJointGetHinge2Angle1 (joint[0]);
		if (v > 0.1) v = 0.1;
		if (v < -0.1) v = -0.1;
		v *= 10.0;
		dJointSetHinge2Param (joint[0],dParamVel,v);
		dJointSetHinge2Param (joint[0],dParamFMax,0.2);
		dJointSetHinge2Param (joint[0],dParamLoStop,-0.75);
		dJointSetHinge2Param (joint[0],dParamHiStop,0.75);
		dJointSetHinge2Param (joint[0],dParamFudgeFactor,0.1);

		dSpaceCollide (space,0,&nearCallback);
		dWorldStep (world,WORLDSTEP);

		// remove all contact joints
		dJointGroupEmpty (contactgroup);
	}
	
	void draw_car(void)
	{
	  int i;
	  
	  dsSetColor (0,0,0);
	  dsSetTexture (DS_WOOD);
	  
	  //drawing the car's body
	  dsDrawCylinder (dBodyGetPosition(body[0]),
					  dBodyGetRotation(body[0]),HEIGHT,RRADIUS);
	  
	  dsSetColor (1,1,1);
	  dsDrawSphere (dBodyGetPosition(body[1]),
					dBodyGetRotation(body[1]),RADIUS*0.8);
						   
	  for (i=2; i<=3; i++) dsDrawCylinder (dBodyGetPosition(body[i]),
						   dBodyGetRotation(body[i]),0.1f,RADIUS);
	  
	  //shooter
	  dReal sides2[3] = {0.05,WIDTH*1.2,0.6};
	  dsDrawBox (dBodyGetPosition(body[4]),dBodyGetRotation(body[4]),sides2);
	  
	  dsSetTexture (NULL);
	  //patches
	  for (i=6; i<=9; i++) 
	  {
			int color_code; 
			if (pumas_team) 
				color_code = color_combination[robot_num][i-6];
			else
			    color_code = color_combination[robot_num+5][i-6];
				
			dsSetColor (colors[color_code][0],
						colors[color_code][1],
						colors[color_code][2]);
						
			dsDrawCylinder (dBodyGetPosition(body[i]),
						   dBodyGetRotation(body[0]),0.02f,PATCH_RADIUS);
	  }
	}
	
	void create_car(float init_x, float init_y, float init_angle)
	{
		  dMass m;
		  int i;

		  //stop car
		  this->stopcar();
	
		  // chassis body
		  body[0] = dBodyCreate (world);
		  dBodySetPosition (body[0],init_x,init_y,STARTZ);
		  
		  dMatrix3 R2;
		  dRFromAxisAndAngle (R2,0,0,0,M_PI/2.0);
		  dBodySetRotation (body[0],R2);
		  
		  //dMassSetCylinder (&m, 1, 3, RRADIUS, HEIGHT);
		  //cylinder[0] = dCreateCCylinder (0, RRADIUS,HEIGHT);
			
		  dMassSetBox (&m,1,LENGTH*1.8,WIDTH*1.8,HEIGHT);
		  box[0] = dCreateBox (0,LENGTH*1.8,WIDTH*1.8,HEIGHT);
		  
		  dMassAdjust (&m,CMASS);
		  dBodySetMass (body[0],&m);
		  
		  //dGeomSetBody (cylinder[0],body[0]);
		  dGeomSetBody (box[0],body[0]);

		  // wheel bodies
		  for (i=1; i<=3; i++) {
			body[i] = dBodyCreate (world);
			dQuaternion q;
			dQFromAxisAndAngle (q,1,0,0,M_PI*0.5);
			dBodySetQuaternion (body[i],q);
			dMassSetSphere (&m,1,RADIUS);
			dMassAdjust (&m,WMASS);
			dBodySetMass (body[i],&m);
			if (i==1)
				sphere[i-1] = dCreateSphere (0,RADIUS*0.8);
			else
				sphere[i-1] = dCreateSphere (0,RADIUS);
				
			dGeomSetBody (sphere[i-1],body[i]);
		  }		  
		  dBodySetPosition (body[1],init_x-0.8*LENGTH,init_y,STARTZ-HEIGHT*0.4+0.05);
		  dBodySetPosition (body[2],init_x+0.2*LENGTH,init_y+WIDTH*0.95,STARTZ-HEIGHT*0.4+0.05);
		  dBodySetPosition (body[3],init_x+0.2*LENGTH,init_y-WIDTH*0.95,STARTZ-HEIGHT*0.4+0.05);

		  //shooter
		  body[4] = dBodyCreate (world);
		  dMassSetBox (&m,1,0.1,WIDTH*1.2,0.6);
		  dMassAdjust (&m,SMASS);
		  dBodySetMass (body[4],&m);
		  box[1] = dCreateBox (0,0.1,WIDTH*1.2,0.6);
		  dGeomSetBody (box[1],body[4]);
		  dBodySetPosition (body[4],init_x+1.1*LENGTH,init_y,STARTZ-HEIGHT*0.5+0.35);	  
		  
		  //compensatory mass for shooter
		  body[5] = dBodyCreate (world);
		  dMassSetBox (&m,1,LENGTH*0.5,WIDTH*0.5,HEIGHT*0.5);
		  dMassAdjust (&m,SMASS*2.0);
		  dBodySetMass (body[5],&m);
		  box[2] = dCreateBox (0,LENGTH*0.5,WIDTH*0.5,HEIGHT*0.5);
		  dGeomSetBody (box[2],body[5]);
		  dBodySetPosition (body[5],init_x-LENGTH*0.7,init_y,STARTZ);
		  
		  // wheel patches
		  for (i=6; i<=9; i++) {
			body[i] = dBodyCreate (world);
			dMassSetSphere (&m,0.01,PATCH_RADIUS);
			dMassAdjust (&m,0.01);
			dBodySetMass (body[i],&m);
				patches[i-6] = dCreateSphere (0,PATCH_RADIUS);
				
			dGeomSetBody (patches[i-6],body[i]);
		  }	
		  dBodySetPosition (body[6],init_x,init_y,STARTZ+HEIGHT*0.5+0.05); //central
		  dBodySetPosition (body[7],init_x+0.7*LENGTH,init_y,STARTZ+HEIGHT*0.5+0.05);
		  dBodySetPosition (body[8],init_x-0.6*LENGTH,init_y+WIDTH*0.3,STARTZ+HEIGHT*0.5+0.05);
		  dBodySetPosition (body[9],init_x-0.6*LENGTH,init_y-WIDTH*0.3,STARTZ+HEIGHT*0.5+0.05);
		  
		  // front wheel hinge
		  /*
		  joint[0] = dJointCreateHinge2 (world,0);
		  dJointAttach (joint[0],body[0],body[1]);
		  const dReal *a = dBodyGetPosition (body[1]);
		  dJointSetHinge2Anchor (joint[0],a[0],a[1],a[2]);
		  dJointSetHinge2Axis1 (joint[0],0,0,1);
		  dJointSetHinge2Axis2 (joint[0],0,1,0);
		  */

		  // front and back wheel hinges
		  for (i=0; i<9; i++) {
			joint[i] = dJointCreateHinge2 (world,0);
			dJointAttach (joint[i],body[0],body[i+1]);
			const dReal *a = dBodyGetPosition (body[i+1]);
			dJointSetHinge2Anchor (joint[i],a[0],a[1],a[2]);
			dJointSetHinge2Axis1 (joint[i],0,0,1);
			dJointSetHinge2Axis2 (joint[i],0,1,0);
		  }

		  // set joint suspension
		  for (i=0; i<4; i++) {
			dJointSetHinge2Param (joint[i],dParamSuspensionERP,0.0); //before 0.4
			dJointSetHinge2Param (joint[i],dParamSuspensionCFM,0.0); //before 0.8
		  }

		  // lock back wheels along the steering axis
		  for (i=1; i<4; i++) {
			// set stops to make sure wheels always stay in alignment
			dJointSetHinge2Param (joint[i],dParamLoStop,0);
			dJointSetHinge2Param (joint[i],dParamHiStop,0);
			// the following alternative method is no good as the wheels may get out
			// of alignment:
			//   dJointSetHinge2Param (joint[i],dParamVel,0);
			//   dJointSetHinge2Param (joint[i],dParamFMax,dInfinity);
		  }

		  // create car space and add it to the top level space
		  car_space = dSimpleSpaceCreate (space);
		  dSpaceSetCleanup (car_space,0);
		  
		  dSpaceAdd (car_space,box[0]);
		  //dSpaceAdd (car_space,cylinder[0]);
		  dSpaceAdd (car_space,sphere[0]);
		  dSpaceAdd (car_space,sphere[1]);
		  dSpaceAdd (car_space,sphere[2]);
		  dSpaceAdd (car_space,box[1]);

		  //rotate finally
		  //dRFromAxisAndAngle (R2,0,0,1,init_angle);
		  //dBodySetRotation (body[0],R2);
	}
};

//initialize static members
int small_size::robot_count = 0;
   
//robots
small_size * pumas[5] = { NULL, NULL, NULL, NULL, NULL };
small_size * america[5] = { NULL, NULL, NULL, NULL, NULL };

/* Socket to send the info of positions*/
msocketData outport_camera[12];  //10 robots plus 2 intelligences

void init_robots(void)
{
   int i;

   for (i=0;i<TOT_PUMAS;i++) {
		pumas[i] = new small_size (i, true);
   }

   for (i=0;i<TOT_AMERICA;i++) {
		america[i] = new small_size (i, false);
   }
}

void send_info(char *info) {
   char temp[BUFFERSIZE];
   temp[0]='\0';
   int i;
   
   sprintf (temp,"%s\n",info);

   for (i=0;i<TOT_PUMAS;i++) {
	  outport_camera[i].msocketTxData(temp, BUFFERSIZE);
	  outport_camera[i+5].msocketTxData(temp, BUFFERSIZE);     
   }
   
   //intelligence
  outport_camera[10].msocketTxData(temp, BUFFERSIZE);
  outport_camera[11].msocketTxData(temp, BUFFERSIZE);     
}

void process_and_send_data(int numrobot) {
   char data[BUFFERSIZE], temp[BUFFERSIZE];
   int i;
  
   //lock mutex
   pthread_mutex_lock (&lock);

   //ball
   const dReal* pos = dGeomGetPosition(ball[0]);
   sprintf(temp, "INFO B %5.3f %5.3f 0.0 END, ", pos[0],pos[1]);	
   pel[0]=pos[0]; pel[1]=pos[1];
   sprintf(data, temp);   

   for (i=0;i<TOT_PUMAS;i++) {
	  const dReal* qrot = dBodyGetQuaternion (pumas[i]->body[0]);

#ifdef DEGREES
	  float ang  = normalizeAngle (Angulo (qrot[0],qrot[3])*2.0) / (2*M_PI) *  360; //in degrees
#else
	  float ang  = normalizeAngle (Angulo (qrot[0],qrot[3])*2.0); //in raidans
#endif

	  //printf ("Angulo = %5.3f\n", ang);
	  const dReal* pos = dBodyGetPosition(pumas[i]->body[0]);
      sprintf(temp, "INFO P %02d %5.3f %5.3f 0.0 %5.3f END, ", i, pos[0],pos[1],ang);
	  rob[i][0]=pos[0]; rob[i][1]=pos[1]; rob[i][2]=ang;	
      strcat(data, temp);  
	      
   }   
   
   for (i=0;i<TOT_AMERICA;i++) {
	  const dReal* qrot = dBodyGetQuaternion (america[i]->body[0]);	 

#ifdef DEGREES
	  float ang  = normalizeAngle (Angulo (qrot[0],qrot[3])*2.0) / (2*M_PI) *  360; //in degrees
#else
	  float ang  = normalizeAngle (Angulo (qrot[0],qrot[3])*2.0); //in radians
#endif

	  //printf ("Angulo = %5.3f\n", ang);
	  const dReal* pos = dBodyGetPosition(america[i]->body[0]);
      sprintf(temp, "INFO A %02d %5.3f %5.3f 0.0 %5.3f END, ", i, pos[0],pos[1],ang);
	  rob[i+5][0]=pos[0]; rob[i+5][1]=pos[1]; rob[i+5][2]=ang;		  
      strcat(data, temp);      
   }

   sprintf(temp, "FRAMEEND\n");
   strcat(data, temp);   
   		
#ifdef SEND_TO_EACH

   if (numrobot<TOT_PUMAS)
   {
		outport_camera[numrobot].msocketTxData(data, BUFFERSIZE);
		outport_camera[numrobot+5].msocketTxData(data, BUFFERSIZE);
   }
#endif

	//intelligences
	outport_camera[10].msocketTxData(data, BUFFERSIZE);
	outport_camera[11].msocketTxData(data, BUFFERSIZE);

	//unlock mutex
   pthread_mutex_unlock (&lock);
}

void init_sockets(void)
{
	int error;
	int i;
 
#ifdef SEND_TO_EACH

	for (i=0;i<TOT_PUMAS;i++)
        /* The client, where it will send the results to the tx8 */
        /*         192.168.2.189 is the IP from the tx8          */
        if((error = outport_camera[i].msocketInitSocket( Phost, 8011+100*i, TX_SOCKET ))) {
           printf("error opening server: %s", msocketGetError( error ) );
           exit(1);
        }
		
	for (i=0;i<TOT_AMERICA;i++)
        /* The client, where it will send the results to the tx8 */
        /*         192.168.2.189 is the IP from the tx8          */
        if((error = outport_camera[i+5].msocketInitSocket( Ahost, 9011+100*i, TX_SOCKET ))) {
           printf("error opening server: %s", msocketGetError( error ) );
           exit(1);
        }
	
	if (TOT_PUMAS>0)
	//intelligences
	if((error = outport_camera[10].msocketInitSocket( Phost, 4000, TX_SOCKET ))) {
	   printf("error opening Pumas server: %s", msocketGetError( error ) );
	   exit(1);
	}
	if (TOT_AMERICA>0)
	if((error = outport_camera[11].msocketInitSocket( Ahost, 5000, TX_SOCKET ))) {
	   printf("error opening America server: %s", msocketGetError( error ) );
	   exit(1);

#else

	if (TOT_PUMAS>0)
	//intelligences
	if((error = outport_camera[10].msocketInitSocket( Phost, 8011, TX_SOCKET ))) {
	   printf("error opening Pumas server: %s", msocketGetError( error ) );
	   exit(1);
	}
	if (TOT_AMERICA>0)
	if((error = outport_camera[11].msocketInitSocket( Ahost, 9011, TX_SOCKET ))) {
	   printf("error opening America server: %s", msocketGetError( error ) );
	   exit(1);
	}

#endif

	for (i=0;i<TOT_PUMAS;i++)
		pumas[i]->init_sockets (6000+100*i, Phost, 6100+100*i);
	
	for (i=0;i<TOT_AMERICA;i++)
		america[i]->init_sockets (7000+100*i, Ahost, 7100+100*i);
		
	printf ("Ports Opened Succesfully\n");
}

void close_sockets(void)
{
   int i;

   for (i=0;i<TOT_PUMAS;i++)
	   /* Send the data */
	   outport_camera[i].msocketKillSocket( );

   for (i=0;i<TOT_AMERICA;i++)
	   /* Send the data */
	   outport_camera[i+5].msocketKillSocket( );
	   
   //intelligences
   outport_camera[10].msocketKillSocket( );
   outport_camera[11].msocketKillSocket( );
}

// start simulation - set viewpoint
static void start()
{
  //static float xyz[3] = {0.8317f,-0.9817f,0.8000f};
  //static float hpr[3] = {121.0000f,-27.5000f,0.0000f};
  
  static float xyz[3] = {0,0,42.7600};
  static float hpr[3] = {0,-90.0000,0.0000};

  dsSetViewpoint (xyz,hpr);
  
  printf ("Press:\t'a' to increase speed.\n"
	  "\t'z' to decrease speed.\n"
	  "\t',' to steer left.\n"
	  "\t'.' to steer right.\n"
	  "\t' ' to reset speed and steering.\n"
	  "\t'1' to save the current state to 'state.dif'.\n");
}

int puma_act = 0;

// called when a key pressed
static void command (int cmd)
{
  switch (cmd) {
  case 'g': case 'G': //goal
	resetSimulation();
	send_info ("RESET");
	break;
  case 'q': case 'Q':
    pumas[puma_act]->speedleft += 0.1;
    break;
  case 'w': case 'W':
    pumas[puma_act]->speedleft -= 0.1;
    break;
  case 'e': case 'E':
    pumas[puma_act]->speedright += 0.1;
    break;
  case 'r': case 'R':
    pumas[puma_act]->speedright -= 0.1;
    break;
  case 'a': case 'A':
    pumas[puma_act]->speed += 0.3;
    break;
  case 'z': case 'Z':
    pumas[puma_act]->speed -= 0.3;
    break;
  case ',':
    pumas[puma_act]->steer -= 0.5;
    break;
  case '.':
    pumas[puma_act]->steer += 0.5;
    break;
  case ' ':
    pumas[puma_act]->speed = 0;
	pumas[puma_act]->speedleft = 0;
	pumas[puma_act]->speedright = 0;
    pumas[puma_act]->steer = 0;
	pumas[puma_act]->shooterspeed = 0;
    break;
  case 'x': case 'X' :
	if (pumas[puma_act]->shooterspeed == 0)
		pumas[puma_act]->shooterspeed = 20.0;
	else
		pumas[puma_act]->shooterspeed = 0;
    break;
  case '1':
    if (1<=TOT_PUMAS) puma_act = 0;
    break;
  case '2':
    if (2<=TOT_PUMAS) puma_act = 1;
    break;
  case '3':
    if (3<=TOT_PUMAS) puma_act = 2;
    break;
  case '4':
    if (4<=TOT_PUMAS) puma_act = 3;
    break;
  case '5':
    if (5<=TOT_PUMAS) puma_act = 4;
    break;
  case '=': {
      FILE *f = fopen ("state.dif","wt");
      if (f) {
        dWorldExportDIF (world,f,"");
        fclose (f);
      }
    }
  }
}

// simulation loop
int count = 0;

void draw_field(void)
{
  int i;
  dVector3 ss;

  //field
  dsSetColor (0,0.5,0);
  dsSetTexture (DS_WOOD);
  dGeomBoxGetLengths (ground_box,ss);
  dsDrawBox (dGeomGetPosition(ground_box),dGeomGetRotation(ground_box),ss);
  
  //walls
  dsSetTexture (NULL);
  dsSetColor (1,1,1);
  dGeomBoxGetLengths (b1,ss);
  dsDrawBox (dGeomGetPosition(b1),dGeomGetRotation(b1),ss);
  dGeomBoxGetLengths (b2,ss);
  dsDrawBox (dGeomGetPosition(b2),dGeomGetRotation(b2),ss);
  dGeomBoxGetLengths (b3,ss);
  dsDrawBox (dGeomGetPosition(b3),dGeomGetRotation(b3),ss);
  dGeomBoxGetLengths (b4,ss);
  dsDrawBox (dGeomGetPosition(b4),dGeomGetRotation(b4),ss);
  
  //lines
  for (i=0;i<23;i++)
  {
    dGeomBoxGetLengths (lines[i],ss);
    dsDrawBox (dGeomGetPosition(lines[i]),dGeomGetRotation(lines[i]),ss);
  }
  
  //dots
  for (i=0;i<3;i++)
  dsDrawCylinder (dGeomGetPosition(circles[i]),
				  dGeomGetRotation(circles[i]),HEIGHTDOTS,0.3);
				  
  for (i=3;i<6;i++)
  //big white circles
  dsDrawCylinder (dGeomGetPosition(circles[i]),
				  dGeomGetRotation(circles[i]),HEIGHTWHCIRC,5);
		
  dsSetColor (0,0.5,0);
  dsSetTexture (DS_WOOD);
  
  for (i=3;i<6;i++)
  //big green circles
  dsDrawCylinder (dGeomGetPosition(circles[i]),
				  dGeomGetRotation(circles[i]),HEIGHTGRCIRC,4.8);
				  
  //areas
  for (i=0;i<2;i++)
  {
    dGeomBoxGetLengths (boxes[i],ss);
    dsDrawBox (dGeomGetPosition(boxes[i]),dGeomGetRotation(boxes[i]),ss);
  }	
				
  //goals
  dsSetColor (1.0,1.0,0); //yellow
  dsSetTexture (NULL);
  dGeomBoxGetLengths (goal[0],ss);
  dsDrawBox (dGeomGetPosition(goal[0]),dGeomGetRotation(goal[0]),ss);
  
  dsSetColor (0.0,0,1.0); //blue
  dsSetTexture (NULL);
  dGeomBoxGetLengths (goal[1],ss);
  dsDrawBox (dGeomGetPosition(goal[1]),dGeomGetRotation(goal[1]),ss);
}

float angle = 0.0;
float radius = 20.0;
float delta = 0.1;

bool up = true;

float ballset[2] = {0,0};

void MoveBallEight (float *angle, float *coords)
{
	if (up==true) {
		*angle += delta;

		if (*angle>=360.0)
			*angle = 0.0;
		else
			if (*angle>90.0 && *angle <270.0){
				up = false;
				*angle = 270;
			}
	}
	else
	{
		*angle -= delta;

		if (*angle<=90.0){
			up = true;
			*angle = 270;
		}
	}

	float ang = *angle *(2.0*M_PI)/360.0;
	coords[0] = radius * sin (ang) * fabs(cos (ang));
	coords[1] = radius * cos (ang) * fabs(cos (ang));
}


static void simLoop (int pause)
{
  int i;

  //lock mutex
  //pthread_mutex_lock (&lock);

  if (!pause)
  {
    //for (i=0;i<TOT_PUMAS;i++)
	//	pumas[i]->sim_joints();

	pumas[count]->sim_joints();
		
	//for (i=0;i<TOT_AMERICA;i++)
	//	america[i]->sim_joints();

	america[count]->sim_joints();
  }

  //unlock mutex
  //pthread_mutex_unlock (&lock);

  //drawing vehicles
  for (i=0;i<TOT_PUMAS;i++)
	pumas[i]->draw_car();
	
  //drawing vehicles
  for (i=0;i<TOT_AMERICA;i++)
	america[i]->draw_car();
	
  //ball
  	dsSetColor (1,0.1,0);
	dsDrawSphere (dGeomGetPosition(ball[0]),
				 dGeomGetRotation(ball[0]),BALL_RADIUS);
				 
  //field
  draw_field();
  
  /*
  printf ("%.10f %.10f %.10f %.10f\n",
	  dJointGetHingeAngle (joint[1]),
	  dJointGetHingeAngle (joint[2]),
	  dJointGetHingeAngleRate (joint[1]),
	  dJointGetHingeAngleRate (joint[2]));
  */

  glReadBuffer( GL_FRONT );

  #ifdef DEMOBALL

	  /* we calculate new ball�s position */
		MoveBallEight (&angle, ballset);

	  /* we set ball�s position */
		dBodySetPosition (b,ballset[0],ballset[1],0.11);

  #endif
}

void resetSimulation()
{
	int i;
	dMass m;

	// destroy world if it exists
	if (bodies)
	{
		dJointGroupDestroy (contactgroup);
		dSpaceDestroy (space);
		dWorldDestroy (world);
	}
	bodies = 1;

	// recreate world
	world = dWorldCreate();
	space = dHashSpaceCreate (0);
	contactgroup = dJointGroupCreate (0);
	dWorldSetGravity (world,0,0,-0.5);

	//dWorldSetGravity (world,0,0,-1.5);
	//dWorldSetCFM (world, 1e-5);
	//dWorldSetERP (world, 0.8);
	//dWorldSetQuickStepNumIterations (world,ITERS);

	ground = dCreatePlane (space,0,0,1,0);

	//create cars
	for (i=0;i<TOT_PUMAS;i++)
		pumas[i]->create_car(positions[i][0], positions[i][1], -0.88);

	//create cars
	for (i=0;i<TOT_AMERICA;i++)
		america[i]->create_car(positions[i+5][0], positions[i+5][1], 0.92);

	// field
	ground_box = dCreateBox (space,50,70,HEIGHTGROUND);
	dGeomSetPosition (ground_box,0,0,LEVELGRND);

	//lines
	lines[0] = dCreateBox (space,40,0.2,HEIGHTDOTS); //midfield
	dGeomSetPosition (lines[0],0,0,LEVELGRND);
	lines[1] = dCreateBox (space,20,0.2,HEIGHTDOTS); //hor. sup area
	dGeomSetPosition (lines[1],0,18,LEVELGRND);
	lines[4] = dCreateBox (space,20,0.2,HEIGHTDOTS); //hor. inf area
	dGeomSetPosition (lines[4],0,-18,LEVELGRND);

	lines[2] = dCreateBox (space,0.2,12,HEIGHTDOTS); //ver. sup area left
	dGeomSetPosition (lines[2],-10,24,LEVELGRND);
	lines[3] = dCreateBox (space,0.2,12,HEIGHTDOTS); //ver. sup area right
	dGeomSetPosition (lines[3],10,24,LEVELGRND);

	lines[5] = dCreateBox (space,0.2,12,HEIGHTDOTS); //ver. inf area left
	dGeomSetPosition (lines[5],-10,-24,LEVELGRND);
	lines[6] = dCreateBox (space,0.2,12,HEIGHTDOTS); //ver. inf area right
	dGeomSetPosition (lines[6],10,-24,LEVELGRND);

	lines[7] = dCreateBox (space,10,0.2,HEIGHTDOTS); //hor. sup goal area
	dGeomSetPosition (lines[7],0,26,LEVELGRND);
	lines[8] = dCreateBox (space,10,0.2,HEIGHTDOTS); //hor. inf goal  area
	dGeomSetPosition (lines[8],0,-26,LEVELGRND);

	lines[9] = dCreateBox (space,0.2,4,HEIGHTDOTS); //ver. goal area left
	dGeomSetPosition (lines[9],-5,28,LEVELGRND);
	lines[10] = dCreateBox (space,0.2,4,HEIGHTDOTS); //ver. goal area right
	dGeomSetPosition (lines[10],5,28,LEVELGRND);

	lines[11] = dCreateBox (space,0.2,4,HEIGHTDOTS); //ver. goal area left
	dGeomSetPosition (lines[11],-5,-28,LEVELGRND);
	lines[12] = dCreateBox (space,0.2,4,HEIGHTDOTS); //ver. goal area right
	dGeomSetPosition (lines[12],5,-28,LEVELGRND);

#ifndef CLOSED
	//boundaries
	lines[13] = dCreateBox (space,40,0.5,0.1);
	dGeomSetPosition (lines[13],0,-30,LEVELGRND);
	lines[14] = dCreateBox (space,40,0.5,0.1);
	dGeomSetPosition (lines[14],0,30,LEVELGRND);
	lines[15] = dCreateBox (space,0.5,60,0.1);
	dGeomSetPosition (lines[15],20,0,LEVELGRND);
	lines[16] = dCreateBox (space,0.5,60,0.1);
	dGeomSetPosition (lines[16],-20,0,LEVELGRND);
#else
	//boundaries
	lines[13] = dCreateBox (space,40,0.5,HEIGHTWALL);
	dGeomSetPosition (lines[13],0,-30,HEIGHTWALL/2);
	lines[14] = dCreateBox (space,40,0.5,HEIGHTWALL);
	dGeomSetPosition (lines[14],0,30,HEIGHTWALL/2);
	lines[15] = dCreateBox (space,0.5,60,HEIGHTWALL);
	dGeomSetPosition (lines[15],20,0,HEIGHTWALL/2);
	lines[16] = dCreateBox (space,0.5,60,HEIGHTWALL);
	dGeomSetPosition (lines[16],-20,0,HEIGHTWALL/2);
#endif

	//goals
	lines[17] = dCreateBox (space,10,0.2,HEIGHTGOALS); //hor. sup goal area
	dGeomSetPosition (lines[17],0,34,LEVELGRND);
	lines[18] = dCreateBox (space,10,0.2,HEIGHTGOALS); //hor. inf goal  area
	dGeomSetPosition (lines[18],0,-34,LEVELGRND);

	lines[19] = dCreateBox (space,0.2,4,HEIGHTGOALS); //ver. goal area left
	dGeomSetPosition (lines[19],-5,32,LEVELGRND);
	lines[20] = dCreateBox (space,0.2,4,HEIGHTGOALS); //ver. goal area right
	dGeomSetPosition (lines[20],5,32,LEVELGRND);

	lines[21] = dCreateBox (space,0.2,4,HEIGHTGOALS); //ver. goal area left
	dGeomSetPosition (lines[21],-5,-32,LEVELGRND);
	lines[22] = dCreateBox (space,0.2,4,HEIGHTGOALS); //ver. goal area right
	dGeomSetPosition (lines[22],5,-32,LEVELGRND);

	circles[0] = dCreateBox (space,0.6,0.6,HEIGHTDOTS); //central dot
	dGeomSetPosition (circles[0],0,0,LEVELGRND);
	circles[1] = dCreateBox (space,0.6,0.6,HEIGHTDOTS); //upper penalty
	dGeomSetPosition (circles[1],0,22,LEVELGRND);
	circles[2] = dCreateBox (space,0.6,0.6,HEIGHTDOTS); //lower penalty
	dGeomSetPosition (circles[2],0,-22,LEVELGRND);

	circles[3] = dCreateBox (space,10,10,HEIGHTGRCIRC); //central circle outline
	dGeomSetPosition (circles[3],0,0,LEVELGRND);
	circles[4] = dCreateBox (space,10,10,HEIGHTGRCIRC); //upper circle outline
	dGeomSetPosition (circles[4],0,18,LEVELGRND);
	circles[5] = dCreateBox (space,10,10,HEIGHTGRCIRC); //lower circle outline
	dGeomSetPosition (circles[5],0,-18,LEVELGRND);

	boxes[0] = dCreateBox (space,20,12,HEIGHTGRCIRC); //upper area
	dGeomSetPosition (boxes[0],0,24,LEVELGRND);
	boxes[1] = dCreateBox (space,20,12,HEIGHTGRCIRC); //lower area
	dGeomSetPosition (boxes[1],0,-24,LEVELGRND);

	//walls
	b1 = dCreateBox (space,50,0.5,HEIGHTWALL);
	dGeomSetPosition (b1,0,-35,HEIGHTWALL/2);
	b2 = dCreateBox (space,50,0.5,HEIGHTWALL);
	dGeomSetPosition (b2,0,35,HEIGHTWALL/2);
	b3 = dCreateBox (space,0.5,70,HEIGHTWALL);
	dGeomSetPosition (b3,25,0,HEIGHTWALL/2);
	b4 = dCreateBox (space,0.5,70,HEIGHTWALL);
	dGeomSetPosition (b4,-25,0,HEIGHTWALL/2);

	//ball
	b = dBodyCreate (world);
	dBodySetPosition (b,0,0,1);
	dMassSetSphere (&m,1,BALL_RADIUS);
	dMassAdjust (&m, 1);
	dBodySetMass (b,&m);
	ball[0] = dCreateSphere (space,BALL_RADIUS);
	dGeomSetBody (ball[0],b);

	//goals
	goal[0] = dCreateBox (space,10,4.0-BALL_RADIUS,0.1); //upper area
	dGeomSetPosition (goal[0],0,32+BALL_RADIUS,LEVELGRND);
	goal[1] = dCreateBox (space,10,4.0-BALL_RADIUS,0.1); //upper area
	dGeomSetPosition (goal[1],0,-32-BALL_RADIUS,LEVELGRND);
}

//sends vision data to hosts
void* vision_loop(void *args)
 {
	int *i = (int *)args;
	int count = 0;
		
	printf ("%s waiting in thread %d\n","Vision", *i);

	//stop execution
	pthread_mutex_lock (&lock);

	//receives starting signal
	pthread_cond_wait(&cond, &lock);

	//allow execution
	pthread_mutex_unlock (&lock);

	printf ("%s running in thread %d\n","Vision", *i);
	
	do {
		process_and_send_data(count); //send to vision
		count++;

		if (count==TOT_PUMAS)
			count = 0;

		//FRAMES_PER_SECOND times per second
		thread_sleep (1.0/FRAMES_PER_SECOND);

	} while (true);
	
	printf ("Exiting Vision Loop\n");
	pthread_exit(NULL);
 }

void* control_loop(void *args)
 {
	int *i = (int *)args;
	int count = 0;
	char *buff, *cmd;
	
	printf ("%s waiting in thread %d\n","Control", *i);

	//stop execution
	pthread_mutex_lock (&lock);

	//receives starting signal
	pthread_cond_wait(&cond, &lock);

	//allow execution
	pthread_mutex_unlock (&lock);

	time_t it,ft;
		
	printf ("%s running in thread %d\n","Control", *i);

	//we take time
	it = time(NULL);
	
	do {

		//we receive commands
		pumas[count]->receive_cmd();
		//we receive commands
		america[count]->receive_cmd();
		count++;
		if (count==TOT_PUMAS)
			count = 0;

		//fixed times per second depending on FRAMES_PER_SECOND and TOT ROBOTS
		thread_sleep (1.0/(FRAMES_PER_SECOND*(TOT_PUMAS+TOT_AMERICA)));

	} while (true);
	
	printf ("Exiting Control Loop\n");
	pthread_exit(NULL);
 }

int main (int argc, char **argv)
{
  int i;
  int players= 0;

	printf ("Pumas host: (to send data): ");
	scanf  ("%s", &Phost);
	printf ("get %s\n", Phost);

#ifndef DEMOBALL
	printf ("America host: (to send data): ");
	scanf  ("%s", &Ahost);
	printf ("get %s\n", Ahost);

	printf ("Number of players by team [1 to 5] : ");
	scanf ("%d",&players);
	printf ("get %d\n", players);
	if (players>5) players = 5;

	if (players)
	{
		TOT_PUMAS = players;
		TOT_AMERICA = players;
	}
#endif

  init_robots();
  
  //initializes sockets
  init_sockets();
  
  // setup pointers to drawstuff callback functions
  dsFunctions fn;
  fn.version = DS_VERSION;
  fn.start = &start;
  fn.step = &simLoop;
  fn.command = &command;
  fn.stop = 0;
  fn.path_to_textures = "./textures";

  resetSimulation();

  //create threads
  pthread_t threads[THREADCOUNT];
  int thread_ids[THREADCOUNT];
  pthread_attr_t attr;
  pthread_mutex_init (&lock, NULL);
  pthread_cond_init (&cond, NULL);
  
  /* Se llenan los identificadores de los threads */
  for(i = 0; i < THREADCOUNT; i++)
	  thread_ids[i] = i;
  
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  
  //stop execution
  //pthread_mutex_lock (&lock);

  printf ("filling threads\n");

  pthread_create(&threads[0], &attr, vision_loop, (void *)&thread_ids[0]);
  pthread_create(&threads[1], &attr, control_loop, (void *)&thread_ids[1]);
  
  //sends starting signal
  //pthread_mutex_unlock (&lock);

  thread_sleep (1.0);
  printf ("sending start signal\n");
  pthread_cond_broadcast (&cond);

  // run simulation ///////////////////////
  dsSimulationLoop (argc,argv,352,288,&fn);
  ///////////////////////////////////////

  /* Wait for all threads to complete */
  for (i = 0; i < THREADCOUNT; i++)
	  pthread_join(threads[i], NULL);

  printf ("Exiting threads\n");
  
  /* Clean up and exit */
  pthread_attr_destroy(&attr);

  dJointGroupDestroy (contactgroup);
  dSpaceDestroy (space);
  dWorldDestroy (world);
  
  dGeomDestroy (ball[0]);

  //closes sockets
  close_sockets();
  for (i=0;i<TOT_PUMAS;i++) 
		pumas[i]->close_sockets();
  for (i=0;i<TOT_AMERICA;i++) 
		america[i]->close_sockets();
  
  pthread_mutex_destroy(&lock);
  pthread_cond_destroy (&cond);

  return 0;
}
