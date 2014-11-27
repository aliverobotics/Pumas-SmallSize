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

#define VISION_HOST					"192.168.213.189" //laptop
#define VISION_IN_PORT				8011
#define VISION_OUT_PORT				8010 

#define JOYSTICK_HOST				"192.168.213.189"
#define JOYSTICK_IN_PORT			8001
#define JOYSTICK_OUT_PORT			8002

#define ROBOT_HOST					"192.168.213.35"
#define ROBOT_IN_PORT				6002
#define ROBOT_OUT_PORT				6000
int		ROBOT_NUM     =				0;

#define WEIGTHS_FOLLOW_WALL			"../Resources/Weights_Brain"
#define TRAIN_SET_IN_FOLLOW_WALL	"../Resources/Train_Inputs.txt"
#define TRAIN_SET_OUT_FOLLOW_WALL	"../Resources/Train_8_Outputs.txt"

/*** VOICE ***/
//put zero if you do not want to wait server to receive & finish
#define SAY_ACK						0

/*** GUI ****/
#define GUI_INPUT_FILE				"../Resources/sonar_lbr.raw"
#define GUI_MAP_FILE				"../Resources/LBR.FAC"

/*** LOCALIZER ***/
#define TEST_OBS_SNAP				"/MyDocs/XDevelop/Jander/nodes/origin/origin_see.txt"
#define LOCALIZER_MAP				"/MyDocs/Xdevelop/Jander/build/Map.TGA"
#define CONVOLUTION_FILES			"/MyDocs/Xdevelop/Jander/build/Convolution" 
#define FUZZY_MAP					"/MyDocs/Xdevelop/Jander/build/Concentrate.TGA"

/*** SOCKETINPUT ***/
#define REMOTE_COMM_HOST			"localhost"
#define COMM_IN_PORT				9000
#define COMM_OUT_PORT				9001

/*** THINK ***/
#define STOPMAP_GEN_MAP				"../../../Robot.MAP"
#define SNAP_IMAGES					"../../../Ambience.TGA"
#define CLIPS_ENV_FILES				"ViRbot.app/Contents/Resources/Jander.clp", "example.clp"

/*** MAIN ***/
#define TX8_RADIO_ROBOT				1.5
#define TX8_SAFE_DIST				0.5
#define TX8_SENSORS_FILE_OUT		"/MyDocs/Xdevelop/Jander/sonar_lbr_map_X.raw"
#define TX8_HOST					"192.168.169.55" //.3.2
#define TX8_OUT_PORT				8001 //2000
#define TX8_IN_MR					8000 //3002
#define TX8_IN_SS					3004
#define TX8_IN_SC					3006
#define PERC_SENDOBS_HOST			"127.0.0.1"
#define PERC_OUT_PORT				6000
#define VOICE_SERVER				"localhost"
#define VOICE_OUT_PORT				4000
#define VOICE_IN_PORT				5000
