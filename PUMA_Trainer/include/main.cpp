/*
 *  main.cpp
 *  Jander
 *
 *  Created by Adalberto Llarena on 5/14/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#define TRAINING
#define VIRTUAL
//#define DEGREES

#include "main.h"
#define CONTROLWCAMERA
#define APPROX_DIST  //distance to send the robot fat from the ball
#define CORRECTION 1.0
#define POWER 2.0

bool training = false;
bool goal = false;
bool othergoal = false;
bool reset = false;
bool autoshoot = true;
bool autoctrl = true;
int  target = -1;

//advance
#define TRAIN "Invental.txt"
#define TEST "Invental.txt"

Communicate Comm;

//it must be defined globally to incorporate in threads and clips

#define THREADCOUNT 4
  
/*********** MAIN   ********/

float pel	[3] = {0, 10, 0};
float rob	[10][3] = {0, 0, 89.99};
float deltapel	[3] = {0, 0, 0};
float deltarob	[10][3] = {0, 0, 0};
float deltavel  [2] = {0,0};
float por	[3] = {0, 0, 0};
float vel	[2] = {0, 0};  //izq der
float distang [2] = {0,0};
float repulsion [2] = {0,0}; //repulsive velocities
bool shoot = false;
bool stopped = false;
float porteria[4]= {80,200,130,200}; //xi, yi, xf, yf

/******* NNETS ******/

int rcp_net = 0;
int behavior = 1; //default attack

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

//velocity goes from 0 to 512
void give_cmd(double vel, bool motorizq, char *str)
{
	if (fabs(vel)<=512)
	{
		if (motorizq==true)
			strcpy (str, "raw 5da0");
		else
			strcpy (str, "raw 5d0a");
		
		if (vel>0) strcat (str, "01");
		else
		if (vel<0) strcat (str, "02");
		else
		strcat (str, "00");
		
		unsigned int velo=0;
		char num[20];
		
		velo = (unsigned int)fabs(floor(vel));
		sprintf (num, "%04x", velo);
		strcat (str, num);
		
		//printf ("%s ",str);
	}
	else
		str[0]='\0'; //error
}

void Set_Vel (bool motorizq, double vel)
{
	char string[80];
	//we get the string for left motor
	give_cmd (vel, motorizq, string);
	sprintf (string,"%s\n",string);
	Comm.Channel_Send("Robot", string);
}

void shoot_on (void)
{
	if (shoot==true) return;
	Comm.Channel_Send("Robot", "shooton\n");
	shoot = true;
}

void shoot_off (void)
{
	if (shoot==false) return;
	Comm.Channel_Send("Robot", "shootoff\n");
	shoot = false;
}

int Robocup_train(void)
{
	int Layers [4] = {2, 2, 2}; //net 2x2x1
								//the first layer is the input layer,
								//does not have any neurons

	Kind_Functions Kinds [2] = { tansig, tansig }; //hidden & output layers

	//you have to take on count the input layer
	//returns the index of the new net
	rcp_net = Brain.Add_Net(ffn, 3, Layers, Kinds); 

	//we indicate there is no preprocessing
	Brain.Net_Ptr(rcp_net)->Prep = normalize;
	Brain.Net_Ptr(rcp_net)->Max = 1;
	Brain.Net_Ptr(rcp_net)->Min = -1;

	switch (Brain.Net_Ptr(rcp_net)->Net->Layer
		   [Brain.Net_Ptr(rcp_net)->Net->Number_of_Layers-1]->Kind)
	{
	case logsig:
	case hardlim:
	case satlin: { Brain.Net_Ptr(rcp_net)->Net->Learn_Factor = 2.0; } break;
	case purelin:{ Brain.Net_Ptr(rcp_net)->Net->Learn_Factor = 1.0; } break;
	case tansig:
	case ising: { Brain.Net_Ptr(rcp_net)->Net->Learn_Factor =  0.5; } break;
	case radbas: { Brain.Net_Ptr(rcp_net)->Net->Learn_Factor = 0.5; } break;
	}

	//we define parameters
	Brain.Net_Ptr(rcp_net)->Net->Num_Epochs = 100;
	Brain.Net_Ptr(rcp_net)->Net->Momentum = 0.0; //if it has many inputs
	Brain.Net_Ptr(rcp_net)->Net->Trace_Train = true;
	Brain.Net_Ptr(rcp_net)->Net->Trace_Input = false;
	Brain.Net_Ptr(rcp_net)->Net->Trace_Output = false;
	Brain.Net_Ptr(rcp_net)->Net->Trace_Actualize = false;
	Brain.Net_Ptr(rcp_net)->Net->Batch_Train = true;
	Brain.Net_Ptr(rcp_net)->Net->Min_Gradient = -0.000001;
	Brain.Net_Ptr(rcp_net)->Net->Min_Error = 0.005;

	//we train the network
	double error = Brain.Train(Brain.Net_Ptr(rcp_net),TRAIN,TEST);
	//we show the training error
	printf ("Train error = %f\n", error);

	//we write the weights to files
	Brain.Write_Weights(Brain.Net_Ptr(rcp_net), "RCP_weigths_");

	//now we read the weights again, only for demonstration
	Brain.Read_Weights(Brain.Net_Ptr(rcp_net), "RCP_weigths_");

	error = Brain.Test(Brain.Net_Ptr(rcp_net),TEST);

	//we show the training error
	printf ("Test error = %f\n", error);
	
	//Try_Net();
	
	return 0;
}

int Robocup_Load(char *files)
{
	int Layers [4] = {2, 2, 2}; //net 2x2x1
								//the first layer is the input layer,
								//does not have any neurons

	Kind_Functions Kinds [2] = { tansig, tansig }; //hidden & output layers

	//you have to take on count the input layer
	//returns the index of the new net
	rcp_net = Brain.Add_Net(ffn, 3, Layers, Kinds); 

	//we indicate there is no preprocessing
	Brain.Net_Ptr(rcp_net)->Prep = normalize;
	Brain.Net_Ptr(rcp_net)->Max = 1.0;
	Brain.Net_Ptr(rcp_net)->Min = -1.0;

	//we pepare the network
	Brain.Analize(Brain.Net_Ptr(rcp_net),TRAIN,TEST);

	//now we read the weights 
	Brain.Read_Weights(Brain.Net_Ptr(rcp_net), files);

	double error = Brain.Test(Brain.Net_Ptr(rcp_net),TEST);

	//we show the training error
	printf ("Test error = %f\n", error);

	//Try_Net();

	return 0;
}

/****** END NNETS *****/

void imprime_pos_mini(void)
{
	int i;
	for (i=0;i<2;i++) printf ("%5.3f\t", pel[i]);
	for (i=0;i<2;i++) printf ("%5.3f\t", deltapel[i]);
	for (i=0;i<3;i++) printf ("%5.3f\t", rob[i]);
	for (i=0;i<3;i++) printf ("%5.3f\t", deltarob[i]);
}

void imprime_pos(void)
{
	int i,j;
	for (i=0;i<2;i++) printf ("pel [%d] = %5.3f ", i, pel[i]);
	printf("\n");
	for (j=0;j<10;j++)
	{
		for (i=0;i<3;i++) printf ("rob [%d][%d] = %5.3f ", j, i, rob[j][i]);
		printf("\n");
	}
}

char* xtracts(char *messg, char* cmd)
{
	int i=0;

	//we copy string before comma
	while (messg[0]!= ',' && messg[0]!= '\0' && messg[0]!= '\n' && messg[0]!= '\r') {
		cmd[i]=messg[0];
		messg++;  i++;  
	}
	cmd[i]='\0'; //terminator

	//jump the space
	if (messg[0]==',') {		
		messg++;
		if (messg[0]==' ') messg++; 
	}		
	return messg;
}

int process_data (char *messg)
{
	float unused1;
	char parse[80];
	char cmd[80];
	float pela[3], roba[3];
	int process = 0;	
	int i;
	
	//printf ("\nReceived : %s\n",messg);
	do {
		
		messg = xtracts(messg, cmd); //we get next info

	if (cmd[0]=='R' && cmd[1]=='E' && cmd[2]=='S' && cmd[3]=='E' && cmd[4]=='T' && autoctrl){
		reset = true;
	}
	else
		if (cmd[0]=='I' && cmd[1]=='N' && cmd[2]=='F' && cmd[3]=='O')
		{		
			if (cmd[5]=='B') //ball
			{
				strcpy (parse, &cmd[7]);
				sscanf (parse, "%f %f %f", &pela[0], &pela[1], &pela[2]);
				if (fabs(pela[0])<1000 && fabs(pela[1])<1000 && fabs(pela[2])<1000)
					for (i=0;i<2;i++) 
					{
						deltapel[i]=pela[i]-pel[i];
						pel[i]=pela[i];
					}
					//printf ("%s\n",cmd);
			}
			else
				if ( cmd[5]=='P' || cmd[5]=='A')
					{
						int index = atoi(&cmd[8]); //robot number
						if (cmd[5]=='A') index+=5;

						strcpy (parse, &cmd[10]);
						sscanf (parse, "%f %f %f %f", &roba[0], &roba[1], &unused1, &roba[2]);
						if (fabs(roba[0])<1000 && fabs(roba[1])<1000 && fabs(roba[2])<1000)
							for (i=0;i<3;i++)
							{
								deltarob[index][i]=roba[i]-rob[index][i];
								rob[index][i]=roba[i];
							}
							//printf ("%s\n",cmd);
					}
					//imprime_pos();
					//printf ("\n%s\n",cmd);	
		}
		else
			if (cmd[0]=='G' && cmd[1]=='O' && cmd[2]=='A' && cmd[3]=='L' && autoctrl) {
				if ( (cmd[5]=='1' && going_left) ||
					(cmd[5]=='2' && !going_left) )
					goal = true;
				else
					othergoal = true;
			}
			else
				if (cmd[0]=='F' && cmd[1]=='R' && cmd[2]=='A' && cmd[3]=='M'
				 && cmd[4]=='E' && cmd[5]=='E' && cmd[6]=='N' && cmd[7]=='D') 
					return true;
		
	} while (messg[0]!= '\0' && messg[0]!= '\n' && messg[0]!= '\r');
	
	return false; //success command
}

void imprime_pos_rel (void)
{
	printf ("%5.3f\t%5.3f", distang[0], distang[1]);
}

void get_repulsion (int destiny) //-1 for ball, else robot number
{
	int i;
	int index = ROBOT_NUM;
	if (!pumas_team) index += 5;

	float n; //factor para Frepul 
    float d0; //distancia minima al obstaculo
    float e1; //Constante Fatrac
    float e2; //Constante FatracLejos
    double DistMin; //Distancia minima para tomar Fatraclejos

	n = 100; //factor para Frepul 
	d0 = 1; //distancia minima al obstaculo
	if (behavior) {e1 = 10; e2 = 0; DistMin = 2;}
	else {e1 = 5; e2 = 2; DistMin = 20;} //only to get final approach to the ball
     //Distancia minima para tomar Fatraclejos

	//imprime_pos();
	double Fx=0;
	double Fy=0;
	double Xobst,Yobst;

	//we get the sum of forces
	for (i=-1;i<10;i++)
		if (i!=index)
		{
			if (i<0)
			{
				Xobst = pel[0];
				Yobst = pel[1];
			}
			else
			{
				Xobst = rob[i][0];
				Yobst = rob[i][1];
			}

			float VX = rob[index][0]-Xobst;
			float VY = rob[index][1]-Yobst;
			float dist = sqrt (VX*VX+VY*VY);

			if (i==destiny){ //attractive force must go out
					if (dist>DistMin){ //fatraclejos
						Fx+= -1.0 * e2 * VX/dist;
						Fy+= -1.0 * e2 * VY/dist;
					}
					else { //fatrac cerca
						Fx+= -1.0 * e1 * VX;
						Fy+= -1.0 * e1 * VY;
					}
		    }
			else { //repulsive forces
				Fx+= -1.0 * n * (1/dist - 1/d0) * 1/(dist*dist) * VX/dist;
				Fy+= -1.0 * n * (1/dist - 1/d0) * 1/(dist*dist) * VY/dist;
			}
		}

		float magnitude = sqrt (Fx*Fx+Fy*Fy);
		float angulo_to_force = Angulo (Fx,Fy);

#ifdef DEGREES
		float angulo_robot = deg2rad (rob[index][2]);
#else
		float angulo_robot = rob[index][2];
#endif

		float delta_ang = normalizeAngle(angulo_to_force-angulo_robot);

	//only for debugging
	/*
	printf ("Angulo to pel = %f ", angulo_to_pel*180/M_PI);
	printf ("Angulo robot = %f ", angulo_robot*180/M_PI);
	printf ("deltaang  = %f\n ", delta_ang*180/M_PI);
	*/
	
	repulsion[0]=magnitude*10*(cos(delta_ang)-sin(delta_ang)); //left velocity
	repulsion[1]=magnitude*10*(cos(delta_ang)+sin(delta_ang)); //right velocity
	
	//printf ("%5.3f\t%5.3f\t%5.3f ", dist, angulo_to_pel, angulo_robot/*delta_ang*/);
}

void calcula_pos_rel (int destiny) //-1 for ball, else robot number
{
	int index = ROBOT_NUM;
	if (!pumas_team) index += 5;

	double Xdest,Ydest;
	if (destiny<0)
	{
		Xdest = pel[0];
		Ydest = pel[1];
	}
	else
	{
		Xdest = rob[destiny][0];
		Ydest = rob[destiny][1];
	}

	float VX = Xdest-rob[index][0];
	float VY = Ydest-rob[index][1];
	float dist = sqrt (VX*VX+VY*VY);
					
	float angulo_to_pel = Angulo (VX,VY);

#ifdef DEGREES
	float angulo_robot = deg2rad (rob[index][2]);
#else
	float angulo_robot = rob[index][2];
#endif

	float delta_ang = normalizeAngle(angulo_to_pel-angulo_robot);
	
	//only for debugging
	//printf ("Angulo to pel = %f ", angulo_to_pel*180/M_PI);
	//printf ("Angulo robot = %f ", angulo_robot*180/M_PI);
	//printf ("deltaang  = %f\n ", delta_ang*180/M_PI);
	
	distang[0]=dist;
	distang[1]=delta_ang;
	
	//printf ("%5.3f\t%5.3f\t%5.3f ", dist, angulo_to_pel, angulo_robot/*delta_ang*/);
}

void process_cmd(char *cad)
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
		    velocidad = -1*velo; //direction negative
			
	else return; //bad string
	
	if (abs(velocidad)<=512)
	{
	    //to fit between 0 and 255
		if (motor[0]=='0' && motor[1]=='a')
			vel[0] = velocidad/2.0f;
		
		if (motor[0]=='a' && motor[1]=='0')
			vel[1] = velocidad/2.0f;
	}	
}


int Add_Channels (char *Vhost, int Vinport, int Voutport,
				  char *Jhost, int Jinport, int Joutport,
				  char *Rhost, int Rinport, int Routport)
{
	//we add a new channel for camera positions
	int Camera_Chn = Comm.Add_Channel("Vision",Vhost,
									   Vinport,Voutport);
	if (Camera_Chn)
		Comm.Open_Channel(Camera_Chn);
	else
		printf ("Error opening vision port\n");
	
	//we add a new channel for joystic
	int Joystick_Chn = Comm.Add_Channel("Joystick",Jhost,
									   Jinport, Joutport);
	if (Joystick_Chn)
		Comm.Open_Channel(Joystick_Chn);
	else
		printf ("Error opening joystick port\n");
		
	//we add a new channel for sending commands to robot
	int Robot_Chn = Comm.Add_Channel("Robot",Rhost,
									   Rinport, Routport);
	if (Robot_Chn)
		Comm.Open_Channel(Robot_Chn);
	else
		printf ("Error opening robot port\n");
		
	return 0;
}

double data[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

void stop(void)
{
	stopped = true;
	Set_Vel (true,0);
	Set_Vel (false,0);
	vel[0] = 0.0;
	vel[1] = 0.0;
}

void reverse(void)
{
	stop();
	Set_Vel (true,-180);
	Set_Vel (false,-180);
	vel[0] = -180;
	vel[1] = -180;
}

void rotatel(void)
{
	stop();
	Set_Vel (true,-180);
	Set_Vel (false,+180);
	vel[0] = -180;
	vel[1] = +180;
}

void rotater(void)
{
	stop();
	Set_Vel (true,+180);
	Set_Vel (false,-180);
	vel[0] = +180;
	vel[1] = -180;
}

int Test_Net(int net)
{
	int j;
	int index = ROBOT_NUM;
	if (!pumas_team) index += 5;

	if (net>rcp_net)
		return -1;

	if (reset) {
		data[2]=0.0;
		data[3]=0.0;
		goal = false;
		othergoal = false;
		reset = false;
		shoot_off();
	}
	else
		if (othergoal==true)
		{
			data[2]=0.0;
			data[3]=0.0;
			printf ("OOPS !");
		}
		else
			if (goal == false)
			{
				if (net) 
				{
					//we fill inputs with data
					data[0]=distang[0];
					data[1]=distang[1];

					Brain.Net_Ptr(net)->Net->Trace_Input = false;
					Brain.Net_Ptr(net)->Net->Trace_Output = false;

					if (distang[0]>10 && distang[1]>0.3)
					//approach
					Brain.Try(Brain.Net_Ptr(2), data);
					else
					//we try the net with inputs
					Brain.Try(Brain.Net_Ptr(net), data);

					//we correct between left and rigth motors
					data[3]*=CORRECTION;

					//augmented power
					data[2]*=(double)POWER;
					data[3]*=(double)POWER;

					//we print input data
					printf ("[ ");
					for (j=0;j<2;j++)
						printf ("%2.1f ,", data[j] );
					printf("] =>");

					//we print output data
					printf (" [ ");
					for (j=0;j<2;j++)
						printf ("%2.1f ,", data[2+j] );
					printf("] ");

/////////////////////////////////// VPOT
					//get_repulsion (target);
					//data[2]=data[2]+repulsion[0];
					//data[3]=data[3]+repulsion[1];

				}
				else //potential fields
				{
					get_repulsion (target);
					data[2]=repulsion[0];
					data[3]=repulsion[1];
				}
			}
			else { //lets celebrate
				data[2]=500.0;
				data[3]=-500.0;
				printf ("GOOOOOOOOOOOOOOOOOOOOAAAALLLLLLLLLLLLL !!!!!");
			}

	//we calculate delta velocities
	deltavel[0] = data[2]-vel[0];
	deltavel[1] = data[3]-vel[1];

	if (fabs(deltavel[0])>10 || fabs(deltavel[1])>10)
	{
		double factor=0.35;

		//we adjust new velocities
		vel[0] = data[2];
		vel[1] = data[3];	
		
		if (vel[0]*vel[1]<0.0)
		{
			//we adjust new velocities
			vel[0]*=factor;
			vel[1]*=factor;
		}
			
		//we get the string for left motor
		Set_Vel(true,vel[0]);
		
		//we get the string for right motor
		Set_Vel(false,vel[1]);

		printf (" ... sent\n");
		
		return 1;
	}
	
	//we check for shooter
	if (autoshoot)
	if ( (going_left==true  && rob[index][2]<3.0*M_PI/4.0 && rob[index][2]>M_PI/4.0) ||
		 (going_left==false && rob[index][2]<7.0*M_PI/4.0 && rob[index][2]>5.0*M_PI/4.0) 
		)
			shoot_on();
		else
			shoot_off();
				
	printf ("\n");
	
	return 0; //not changed
}

void* Camera_Loop(void *args)
 {
	int *i = (int *)args;
	char* string;
	int count = 0;
	
	time_t it,ft;
		
	printf ("%s Running in thread %d\n","Camera", *i);

	//we take time
	it = time(NULL);
	
	do {

		if ((string= Comm.Channel_Receive("Vision"))!=NULL)
			//if its valid  data
			if (process_data(string)==1)
			{
#ifdef CONTROLWCAMERA
				//we take time
				ft = time(NULL);
		
				if ((double) (ft-it)>=1.0) //seconds waiting
				{
					count=0;
					it=ft;
				}
				
				if (!stopped)
				{
					count++;
					printf ("%5d => ",count);

					//we calculate relative position
					calcula_pos_rel(target);
					//we test the net
					if (behavior==1 && (distang[0]>5.0 || fabs(distang[1])>0.5))
						Test_Net(2); //approach
					else
						Test_Net(behavior);
				}
#else
				; //nothing
#endif
			}
		
	} while (true);
	
	printf ("Exiting Loop\n");
	pthread_exit(NULL);
 }
 
 void* Joystick_Loop(void *args)
 {
	int *i = (int *)args;
	char *string;
	time_t it,ft;
	int count = 0;
	
	printf ("%s Running in thread %d\n", "Joystick", *i);
	
	//we take time
	it = time(NULL);
	
	do {
		if ((string= Comm.Channel_Receive("Joystick"))!=NULL)
		{
			//we take time
			ft = time(NULL);

			//printf ("joystick %s\n",string);
		
			if ((double) (ft-it)>=1.0) //seconds waiting
			{
				count=0;
				it=ft;
			}
			count++;

			if (strcmp(string,"stop\n")==0)
				stop();
			else
			if (string[0]=='t' && string[1]=='a' && string[2]=='r' &&
				string[3]=='g' && string[4]=='e' && string[5]=='t') {
				if (string[7]=='b')
					target = -1;
				else
				{
				   target = atoi(&string[8]);
				   if (string[7]=='a') target+= 5;
				}
			}
			else
			if (strcmp(string,"go\n")==0)
				stopped = false;
			else
			if (strcmp(string,"reverse\n")==0)
				reverse();
			else
			if (strcmp(string,"rotl\n")==0)
				rotatel();
			else
			if (strcmp(string,"rotr\n")==0)
				rotater();
			else
			if (strcmp(string,"evade\n")==0)
				behavior = 0;
			else
			if (strcmp(string,"attack\n")==0)
				behavior = 1;
			else
			if (strcmp(string,"approach\n")==0)
				behavior = 2;
			else
			if (strcmp(string,"reset\n")==0)
				reset = true;
			else
			if (strcmp(string,"goal\n")==0)
				goal = true;
			else
			if (strcmp(string,"othergoal\n")==0)
				othergoal = true;
			else
			if (strcmp(string,"tron\n")==0)
				training = true;
			else
			if (strcmp(string,"troff\n")==0)
				training = false;
			else
			if (strcmp(string, "shooton\n")==0)
				shoot_on();
			else
			if (strcmp(string, "shootoff\n")==0)
				shoot_off();
			else 
			if (strcmp(string, "youshoot\n")==0)
				autoshoot = true;
			else
			if (strcmp(string, "dontshoot\n")==0)
				autoshoot = false;
			else 
			if (strcmp(string, "takectrl\n")==0)
			{
				autoctrl = true;
				printf ("Control taken !.\n");
			}
			else
			if (strcmp(string, "givectrl\n")==0)
			{
				autoctrl = false;
				printf ("Control given to Clips !\n");
			}
			else 
			{
				char cad[100];
				sprintf (cad,"%s\n",string);
				Comm.Channel_Send("Robot", cad);
				
				//we process command
				process_cmd (string);
				
				//we calculate relative position
				calcula_pos_rel(target);
				
				if (training)
				{
					printf ("%5d => ", count);
					
					//we print current position
					imprime_pos_mini();

					//we print relative position
					imprime_pos_rel();
					
					printf ("%5.0f\t%5.0f\t%d\n", vel[0], vel[1], shoot);
				}
			}
			//printf ("%s\n",string);
		}

	} while (true);
	
	printf ("Exiting Loop\n");
	pthread_exit(NULL);
 }

 void* Train_Loop(void *args)
 {
	time_t it,ft;
	int *i = (int *)args;
	
	printf ("%s Running in thread %d\n", "Train", *i);

	//we take time
	it = time(NULL);
			
	do {
		
		//we take time
		ft = time(NULL);
	
		//if ((double) (ft-it)>=1.0) //seconds waiting
		{
			//we calculate relative position
			calcula_pos_rel(target);
			//we test the net
			Test_Net(behavior);
		
			it = ft;
		}
		
	} while (true);

	printf ("Exiting Loop\n");
	pthread_exit(NULL);

 }
 
 //faces to robot and runs to it
 void* Calculate_Loop(void *args)
 {
//	time_t it,ft;
	int *i = (int *)args;

	printf ("%s Running in thread %d\n", "Calculate", *i);

	//we take time
	//it = time(NULL);
			
	int index = ROBOT_NUM;
	if (!pumas_team) index += 5;

	do {
		//we calculate relative position
		calcula_pos_rel(target);
	
		//we check for shooter
		if (rob[index][2]<100.0 && rob[index][2]>80.00) //degrees
			shoot_on();
		else
			shoot_off();
		
	} while (true);
	
	printf ("Exiting Loop\n");
	pthread_exit(NULL);

 }
 
 void test_robot(void)
 {
	int i;
	
	for (i=0;i<512;i++)
	{
		Set_Vel (true, i);	
		Set_Vel (false, i);
		printf ("%d\n",i);
		//we wait a bit
		for (int j=0;j<1000000;j++);
	}

	for (i=512;i>=0;i--)
	{
		Set_Vel (true, i);	
		Set_Vel (false, i);
		printf ("%d\n",i);
		//we wait a bit
		for (int j=0;j<1000000;j++);
	}

		//we wait a bit
		for (int j=0;j<1000000;j++);
		
		Set_Vel (true, 0);	
		Set_Vel (false, 0);
 }
 
	
int main(int argc, char *argv[])
{		
    int i;
	
    pthread_t threads[THREADCOUNT];
	int thread_ids[THREADCOUNT];
	char Chost[BUFFERSIZE],Jhost[BUFFERSIZE], Rhost[BUFFERSIZE];
	int  Cinport, Coutport, Jinport, Joutport, Rinport, Routport, port; 
	
    pthread_attr_t attr;
	
	printf ("Camera host: ");
	scanf  ("%s", &Chost);

	strcpy(Jhost,"127.0.0.1");

	printf ("Robot host: ");
	scanf  ("%s", &Rhost);

	printf ("Robot number [0 to 4] : ");
	scanf ("%d",&ROBOT_NUM);

	printf ("Pumas 0, America 1 [0 or 1] : ");
	scanf ("%d",&i);
	pumas_team = i==0;
	
	port = 6000;
	if (pumas_team) {
		printf ("We are Pumas !!\n");
		going_left = false;
	}
	else 
	{
		printf ("Sorry, we are America.\n");
		port += 1000;
		going_left = true;
	}

	Cinport = port+2011 + 100*(ROBOT_NUM);
	Coutport = port+2012 + 100*(ROBOT_NUM);
	Rinport = port+ 1 + 100*(ROBOT_NUM);
	Routport = port + 100*(ROBOT_NUM);
	Jinport = port+2001 + 100*(ROBOT_NUM);
	Joutport = port+2002 + 100*(ROBOT_NUM);

	/* Se llenan los identificadores de los threads */
	for(i = 0; i < THREADCOUNT; i++)
		thread_ids[i] = i;
	
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	//we open the channels
	Add_Channels (Chost,Cinport,Coutport,
				  Jhost,Jinport,Joutport,
				  Rhost,Rinport,Routport);

	//Robocup_train();
	Robocup_Load("RCP_weigths_");
	Robocup_Load("RCP_weigths_A");

	clean_pos();
	//return 0;
	
	//test_robot();
	//shoot_off();
	//return 0;

	pthread_create(&threads[0], &attr, Camera_Loop, (void *)&thread_ids[0]);
	pthread_create(&threads[1], &attr, Joystick_Loop, (void *)&thread_ids[1]);
	#ifndef TRAINING
	pthread_create(&threads[2], &attr, Train_Loop, (void *)&thread_ids[2]);
	#endif
	//pthread_create(&threads[3], &attr, Calculate_Loop, (void *)&thread_ids[3]);
	
	/* Wait for all threads to complete */
    for (i = 0; i < THREADCOUNT; i++)
		pthread_join(threads[i], NULL);
  
    printf ("Exiting threads\n");
	
    /* Clean up and exit */
    pthread_attr_destroy(&attr);
    pthread_exit (NULL);
	
	return 0;	
}