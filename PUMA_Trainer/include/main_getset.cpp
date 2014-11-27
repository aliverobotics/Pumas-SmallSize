/*
 *  main.cpp
 *  Jander
 *
 *  Created by Adalberto Llarena on 5/14/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

//#define TRAINING
#define CONTROLWCAMERA

#include "main.h"
//#define TRAINI "Robocup4inpretraso.txt"
//#define TRAINO "Robocup_2outs.txt"
//#define TRAINI "Robocup12inputs.txt"
//#define TRAINI "Robocup7inputsretraso.txt"
//#define TRAINO "Robocup2outputs.txt"
//#define TRAIN "R2RTrain.txt"
//#define TEST "R2RTest.txt"
#define TRAIN "Invental.txt"
#define TEST "Invental2.txt"


Communicate Comm;

//it must be defined globally to incorporate in threads and clips

#define THREADCOUNT 3
  
/*********** MAIN   ********/

float pel	[3] = {0, 10, 0};
float rob	[3] = {0, 0, 89.99};
float deltapel	[3] = {0, 0, 0};
float deltarob	[3] = {0, 0, 0};
float por	[3] = {0, 0, 0};
float vel	[2] = {0, 0};  //izq der
float distang [2] = {0,0};
bool shoot = false;
bool training = false;

/******* NNETS ******/

int rcp_net = 0;

//velocity goes from 0 to 512
void give_cmd(double vel, bool motorizq, char* str)
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

void shooton (void)
{
	char string[80];
	strcpy(string,"shooton");
	sprintf (string,"%s\n",string);
	Comm.Channel_Send("Robot", string);
	shoot = true;
}

void shootoff (void)
{
	char string[80];
	strcpy(string,"shootoff");
	sprintf (string,"%s\n",string);
	Comm.Channel_Send("Robot", string);
	shoot = false;
}

int Try_Net(void)
{
	double data[6*10] = {

5.399,	-0.045,	0,	4,	 0,	44,
5.399,	-0.045,	0,	44,	 48,44,
5.399,	-1.647,	48,	44,	 48,128,
5.399,	-1.647,	48,	128, 132,128,
5.394,	0.049,	132, 128,132,173,
5.394,	0.049,	132, 173,177,173,
5.399,	-1.647,	177, 173,177,62,
5.399,	-1.647,	177, 62,66,	62,
5.237,	0.068,	66,	62,	66,	4,
5.237,	0.068,	66,	4,	0,	4
						} ; //2 net's inputs + 2 outputs olds + 2 new
	
	Brain.Net_Ptr(rcp_net)->Net->Trace_Input = true;
	Brain.Net_Ptr(rcp_net)->Net->Trace_Output = true;
	
	int y,j;
	
	for (y=0;y<10;y++)
	{
		//we print input data
		printf ("[ ");
		for (j=0;j<4;j++)
			printf ("%3.2f ,", data[y*6+j]);
		printf("] =>");
		
		//we try the net with inputs
		Brain.Try(Brain.Net_Ptr(rcp_net), &data[y*6]);
			
		//we get the string for left motor
		Set_Vel(true, data[y*6+4]*2.0);
			
		//we get the string for right motor
		Set_Vel(false,data[y*6+5]*2.0);
		
		for (int h=0;h<10000000;h++);
		
		//we print output results
		printf (" [ ");
		for (j=0;j<2;j++)
			printf ("%3.2f ,", data[y*6+4+j] );
		printf("] \n");
	}
	
	//we stop motors
	Set_Vel(true, 0);
	Set_Vel(false,0);
	
	return 0;
}

int Robocup_train(void)
{
	int Layers [3] = {2, 2, 2}; //net 2x2x1
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
	case ising: { Brain.Net_Ptr(rcp_net)->Net->Learn_Factor =  1.5; } break;
	case radbas: { Brain.Net_Ptr(rcp_net)->Net->Learn_Factor = 0.5; } break;
	}

	//we define parameters
	Brain.Net_Ptr(rcp_net)->Net->Num_Epochs = 1000;
	Brain.Net_Ptr(rcp_net)->Net->Momentum = 0.0; //if it has many inputs
	Brain.Net_Ptr(rcp_net)->Net->Trace_Train = true;
	Brain.Net_Ptr(rcp_net)->Net->Trace_Input = false;
	Brain.Net_Ptr(rcp_net)->Net->Trace_Output = false;
	Brain.Net_Ptr(rcp_net)->Net->Trace_Actualize = false;
	Brain.Net_Ptr(rcp_net)->Net->Batch_Train = true;
	Brain.Net_Ptr(rcp_net)->Net->Min_Gradient = -0.000001;
	Brain.Net_Ptr(rcp_net)->Net->Min_Error = 0.005;

	//we train the network
	double error = Brain.Train(Brain.Net_Ptr(rcp_net),TEST,TEST);
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

int Robocup_Load(void)
{
	int Layers [3] = {2, 2, 2}; //net 2x2x2
								//the first layer is the input layer,
								//does not have any neurons

	Kind_Functions Kinds [2] = { tansig, tansig }; //hidden & output layers

	if (rcp_net==0)
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
	Brain.Read_Weights(Brain.Net_Ptr(rcp_net), "RCP_weigths_");

	double error = Brain.Test(Brain.Net_Ptr(rcp_net),TEST);

	//we show the training error
	printf ("Test error = %f\n", error);

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
	int i;
	for (i=0;i<2;i++) printf ("pel [%d] = %5.3f ", i, pel[i]);
	printf("\n");
	for (i=0;i<3;i++) printf ("rob [%d] = %5.3f ", i, rob[i]);
	printf("\n");
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
	int process = 1; //by default we must run clips	
	int i;
	
	printf ("\nReceived : %s\n",cmd);
	do {

		messg = xtracts(messg, cmd); //we get next info

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

					//printf ("%s\n",messg);
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
							deltarob[i]=roba[i]-rob[i];
							rob[i]=roba[i];
						}
						//printf ("%s\n",messg);
				}
				else
					if (cmd[0]=='F' && cmd[1]=='R' && cmd[2]=='A' && cmd[3]=='M'
						&& cmd[4]=='E' && cmd[5]=='E' && cmd[6]=='N' && cmd[7]=='D') 
						return true;
		}
		imprime_pos();
		printf ("\n%s\n",cmd);	

	} while (messg[0]!= '\0' && messg[0]!= '\n' && messg[0]!= '\r');
	return false; //success command
}

void imprime_pos_rel (void)
{
	printf ("%5.3f\t%5.3f", distang[0], distang[1]);
}

void calcula_pos_rel (void)
{
	float VX = pel[0]-rob[0];
	float VY = pel[1]-rob[1];
	float dist = sqrt (VX*VX+VY*VY);
					
	float angulo_to_pel = Angulo (VX,VY);
	float angulo_robot = rob[2];
	float delta_ang = normalizeAngle(angulo_to_pel-angulo_robot);
	
	//only for debugging
	/*
	printf ("Angulo to pel = %f ", angulo_to_pel*180/M_PI);
	printf ("Angulo robot = %f ", angulo_robot*180/M_PI);
	printf ("deltaang  = %f\n ", delta_ang*180/M_PI);
	*/
	
	
	distang[0]=dist;
	distang[1]=delta_ang;
	
	//printf ("%5.3f\t%5.3f\t%5.3f ", dist, angulo_to_pel, angulo_robot/*delta_ang*/);
}

void process_cmd(char *cad)
{
	if (strcmp (cad, "shooton")==0)
		shoot=true;
	else
		if (strcmp (cad, "shootoff")==0)
			shoot = false;
		else
			if (strcmp (cad, "tron")==0)
				training=true;
			else
				if (strcmp (cad, "troff")==0)
					training = false;
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
					printf("cad = %s = %d\n", &cad[10], velocidad);
					//printf ("direction = %s, ", dir);
					if (dir[0]=='0')
						if (dir[1]=='1')
							velocidad = velo; //direction positive
						else
							if (dir[1]=='2')
							velocidad = -velo; //direction negative
							
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
}


int Add_Channels (void)
{
	//we add a new channel for camera positions
	int Camera_Chn = Comm.Add_Channel("Vision",VISION_HOST,
									   VISION_IN_PORT,VISION_OUT_PORT);
	if (Camera_Chn)
		Comm.Open_Channel(Camera_Chn);
	else
		printf ("Error opening vision port\n");
	
	//we add a new channel for joystic
	int Joystick_Chn = Comm.Add_Channel("Joystick",JOYSTICK_HOST,
									   JOYSTICK_IN_PORT, JOYSTICK_OUT_PORT);
	if (Joystick_Chn)
		Comm.Open_Channel(Joystick_Chn);
	else
		printf ("Error opening joystick port\n");
		
	//we add a new channel for sending commands to robot
	int Robot_Chn = Comm.Add_Channel("Robot",ROBOT_HOST,
									   ROBOT_IN_PORT, ROBOT_OUT_PORT);
	if (Robot_Chn)
		Comm.Open_Channel(Robot_Chn);
	else
		printf ("Error opening robot port\n");
		
	return 0;
}

void Feed_Net(void)
{
	double data[4] = {0.0, 0.0, 0.0, 0.0};
	int j;
	
	//we calculate relative position
	calcula_pos_rel();

	//we fill inputs with data
	data[0]=distang[0];
	data[1]=distang[1];
				
	Brain.Net_Ptr(rcp_net)->Net->Trace_Input = false;
	Brain.Net_Ptr(rcp_net)->Net->Trace_Output = false;

	//we print input data
	printf ("[ ");
	for (j=0;j<2;j++)
		printf ("%2.1f ,", data[j] );
	printf("] =>");

	//we try the net with inputs
	Brain.Try(Brain.Net_Ptr(rcp_net), data);

	//we print output data
	printf (" [ ");
	for (j=0;j<2;j++)
		printf ("%2.1f ,", data[2+j] );
	printf("] ");
	
	//we check for shooter
	if (distang[0]<4.0 && !shoot)	shooton();
	if (distang[0]>4.0 && shoot)	shootoff();
	
	if (fabs(data[2]-vel[0])>5 || fabs(data[3]-vel[1])>5)
	{
		float factor = 2.0;
		
		//we adjust new velocitys
		vel[0] = data[2];
		vel[1] = data[3];
		
		if (vel[0]*vel[1]<0)
		   factor *=0.25;
			
		//we get the string for left motor
		Set_Vel(true,vel[0]*factor);
		
		//we get the string for right motor
		Set_Vel(false,vel[1]*factor);
		
		printf (" ... sent. \n");
	}
	else
		printf (" ... not sent. \n");
}


void* Camera_Loop(void *args)
 {
	int *i = (int *)args;
	char* string;
	time_t it,ft;
	int count = 0;
	
	printf ("%s Running in thread %d\n","Camera", *i);

	//we take time
	it = time(NULL);
	
	do {
	
		//we take time
		ft = time(NULL);

		if ((string= Comm.Channel_Receive("Vision"))!=NULL)
		{
			if (process_data (string))
			
			{
			
		#ifdef CONTROLWCAMERA
			
			count++;
			printf ("%2d => ",count);
			Feed_Net();
		
		#endif
			;
			}
		
		}
		
		if ((double) (ft-it)>=1.0) // 1 seconds waiting
		{
			it = ft;
			count = 0;
		}
		
	} while (true);
	
	printf ("Exiting Loop\n");
	
	pthread_exit(NULL);
 }
 
 void* Joystick_Loop(void *args)
 {
	int *i = (int *)args;
	char *string;
	
	printf ("%s Running in thread %d\n", "Joystick", *i);
	
	do {
	
		if ((string= Comm.Channel_Receive("Joystick"))!=NULL)
		{
			char cad[100];
			sprintf (cad,"%s\n",string);
			Comm.Channel_Send("Robot", cad);
			
			//we process command
			process_cmd (string);
			
			if (training) 
			{
				//we print current position
				imprime_pos_mini();
				
				//we calculate relative position
				calcula_pos_rel();
				
				//we print relative position
				imprime_pos_rel();
				
				printf ("%5.0f\t%5.0f\t%d\n", vel[0], vel[1], shoot);
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

	double data[4] = {0.0, 0.0, 0.0, 0.0};
	int j;
	
	printf ("%s Running in thread %d\n", "Train", *i);

	//we take time
	it = time(NULL);
			
	do {
		
		//we take time
		ft = time(NULL);
		
		//if ((double) (ft-it)>=1.0) //seconds waiting
		{
			//we calculate relative position
			calcula_pos_rel();
			
			//we print current position
			//imprime_pos();
			//imprime_pos_rel();
			
			
			//we fill inputs with data
			data[0]=distang[0];
			data[1]=distang[1];
			//data[2]=vel[0];
			//data[3]=vel[1]; 
			
	//		inputs[0]=pel[0];
	//		inputs[1]=pel[1];
	//		inputs[2]=deltapel[0];
	//		inputs[3]=deltapel[1];
	//		inputs[4]=rob[0];
	//		inputs[5]=rob[1];
	//		inputs[6]=rob[2];
	//		data[0]=deltarob[0];
	//		data[1]=deltarob[1];
	//		data[2]=deltarob[2];
	//		data[3]=distang[0];
	//		data[4]=distang[1];
	//		inputs[7]=vel[0];
	//		inputs[8]=vel[1]; 
						
			Brain.Net_Ptr(rcp_net)->Net->Trace_Input = false;
			Brain.Net_Ptr(rcp_net)->Net->Trace_Output = false;
		
			//we print input data
			printf ("[ ");
			for (j=0;j<2;j++)
				printf ("%2.1f ,", data[j] );
			printf("] =>");
		
			//we try the net with inputs
			Brain.Try(Brain.Net_Ptr(rcp_net), data);

			//we print output data
			printf (" [ ");
			for (j=0;j<2;j++)
				printf ("%2.1f ,", data[2+j] );
			printf("] ");
			
			if (fabs(data[2]-vel[0])>5 && fabs(data[3]-vel[1])>5)
			{
				float factor = 1.5;
				
				//we adjust new velocitys
				vel[0] = data[2];
				vel[1] = data[3];
				
				if (vel[0]*vel[1]<0)
				   factor *=0.25;
				    
				//we get the string for left motor
				Set_Vel(true,vel[0]*factor);
				
				//we get the string for right motor
				Set_Vel(false,vel[1]*factor);
				
				printf (" ... sent. \n");
			}
			else
				printf (" ... not sent. \n");
			
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

	printf ("%s Running in thread %d\n", "Train", *i);

	//we take time
	//it = time(NULL);
			
	do {
	
		//we put robot facing ball
		calcula_pos_rel();
		imprime_pos_rel();
		printf ("\n");

		while (fabs(distang[1])>0.5)
		{
			//we make it turn
			Set_Vel(true,80);
			Set_Vel(false,-80);
			
			calcula_pos_rel();
			imprime_pos_rel();
			printf ("\n");
			for (int y=0;y<100000;y++);
		}
		
		//we stop it
		Set_Vel(true,0);
		Set_Vel(false,0);
			
	} while (true);
	
	printf ("Exiting Loop\n");
	
	pthread_exit(NULL);

 }
 
 void test_robot(void)
 {
	int i;
	char string [80];
	
	for (i=0;i<512;i++)
	{
		//we get the string for left motor
		give_cmd (i, true, string);
		sprintf (string,"%s\n",string);
		Comm.Channel_Send("Robot", string);
		printf ("%d\n",i);
		
		//we get the string for right motor
		give_cmd ((int)((double)i*1.0), false, string);
		sprintf (string,"%s\n",string);
		Comm.Channel_Send("Robot", string);
		printf ("%d\n",(int)((double)i*1.0));
		
		//we wait a bit
		for (int j=0;j<3000000;j++);
	}
	
	for (i=512;i>=0;i--)
	{
		//we get the string for left motor
		give_cmd (i+150, true, string);
		sprintf (string,"%s\n",string);
		Comm.Channel_Send("Robot", string);
		printf ("%d\n",i);
		
		//we get the string for right motor
		give_cmd ((int)((double)i*1.0), false, string);
		sprintf (string,"%s\n",string);
		Comm.Channel_Send("Robot", string);
		printf ("%d\n",(int)((double)i*1.0));
		
		//we wait a bit
		for (int j=0;j<3000000;j++);
	}

		//we wait a bit
		for (int j=0;j<3000000;j++);
			
		//we get the string for left motor
		give_cmd (0, true, string);
		sprintf (string,"%s\n",string);
		Comm.Channel_Send("Robot", string);
		
		//we get the string for right motor
		give_cmd (0, false, string);
		sprintf (string,"%s\n",string);
		Comm.Channel_Send("Robot", string);
 }
 
	
int main(int argc, char *argv[])
{		
    int i;
    pthread_t threads[THREADCOUNT];
	int thread_ids[THREADCOUNT];
	
    pthread_attr_t attr;
	
	/* Se llenan los identificadores de los threads */
	for(i = 0; i < THREADCOUNT; i++)
		thread_ids[i] = i;
	
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	//we open the channels
	Add_Channels ();
	
	//Robocup_train();
	Robocup_Load();
	//return 0;
	
	//test_robot();
	//return 0;
		
	pthread_create(&threads[0], &attr, Camera_Loop, (void *)&thread_ids[0]);
	//pthread_create(&threads[1], &attr, Joystick_Loop, (void *)&thread_ids[1]);
	#ifndef TRAINING
	//pthread_create(&threads[2], &attr, Train_Loop, (void *)&thread_ids[2]);
	#endif
	//pthread_create(&threads[2], &attr, Calculate_Loop, (void *)&thread_ids[2]);
	
	/* Wait for all threads to complete */
    for (i = 0; i < THREADCOUNT; i++)
		pthread_join(threads[i], NULL);
  
    printf ("Exiting threads\n");
	
    /* Clean up and exit */
    pthread_attr_destroy(&attr);
    pthread_exit (NULL);
	
	return 0;	
}