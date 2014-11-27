/*
 *  Setup.h
 *  ViRbot
 *
 *  Created by Adalberto H. Llarena on 9/29/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */
 
#define VIR_LINUX 0   /* UNIX System Linux, Darwin				*/
#define VIR_MACXC 1   /* Apple Macintosh, with Xcode 1.2		*/
#define VIR_PCVST 0   /* IBM PC, with Microsoft Visual C++ 6.0  */

//#define ROBOCUP

#define VISION_HOST					"127.0.0.1" //self
#define VISION_IN_PORT				8011
#define VISION_OUT_PORT				8010 

#define JOYSTICK_HOST				"192.168.213.189"
#define JOYSTICK_IN_PORT			8001
#define JOYSTICK_OUT_PORT			8002

#define ROBOT_HOST					"127.0.0.1" //self
#define ROBOT_IN_PORT				6002
#define ROBOT_OUT_PORT				6000
int		ROBOT_NUM     =				0;
bool    pumas_team	  =			    true;
bool    going_left	  =			    false; //false for Pumas
