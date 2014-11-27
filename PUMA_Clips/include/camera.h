#include <math.h>

//#include Communicate.h
//Communicate Comm;

//field dimensions
#define MinX -20.0
#define MaxX  20.0
#define MinY -30.0
#define MaxY  30.0

float pel	[3] = {0, 10, 0};
float rob	[10][3];

//opens a server
int open_vision(char *host, int inport)
{
	int i;
	//opens the server
	i = Comm.Add_Channel("vision",host,inport,inport+1);
	Comm.Open_Channel(i);

	return 0;
}

//closes the vision
void close_vision()
{
	//close the server
	Comm.Close_Channel("vision");
}

//closes socket
float xb()
{
	// Check for exactly 0 arguments. 
	if (ArgCountCheck("xb",EXACTLY,0) == -1)
	{ return(1L); }

	return pel[0];
}

//closes socket
float yb()
{
	// Check for exactly 0 arguments. 
	if (ArgCountCheck("yb",EXACTLY,0) == -1)
	{ return(1L); }

	return pel[1];
}

//robot x 
float xr()
{
	DATA_OBJECT temp;
	int robotnum;

	// Check for exactly 1 arguments. 
	if (ArgCountCheck("xr",EXACTLY,1) == -1)
	{ return(1L); }
	if (ArgTypeCheck("xr",1,INTEGER,&temp) == 0)
	{ return(1L); }
	{ robotnum = (int)DOToLong(temp); }

	return rob[robotnum][0];
}

//robot y
float yr()
{
	DATA_OBJECT temp;
	int robotnum;

	// Check for exactly 1 arguments. 
	if (ArgCountCheck("yr",EXACTLY,1) == -1)
	{ return(1L); }
	if (ArgTypeCheck("yr",1,INTEGER,&temp) == 0)
	{ return(1L); }
	{ robotnum = (int)DOToLong(temp);  }

	return rob[robotnum][1];
}

//robot angle
float ar()
{
	DATA_OBJECT temp;
	int robotnum;

	// Check for exactly 1 arguments. 
	if (ArgCountCheck("ar",EXACTLY,1) == -1)
	{ return(1L); }
	if (ArgTypeCheck("ar",1,INTEGER,&temp) == 0)
	{ return(1L); }
	{ robotnum = (int)DOToLong(temp);  }

	return rob[robotnum][2];
}

//returns distance
double distance()
{
	char r1[80],r2[80];
	double x1,y1,x2,y2;
	char *ptr;
	int index = 0;

	/* Check for exactly 2 argument. */
	if (ArgCountCheck("openchn",EXACTLY,2) == -1)
	{ return(1L); }
	ptr=RtnLexeme(1);
	sprintf (r1,"%s",ptr);

	ptr=RtnLexeme(2);
	sprintf (r2,"%s",ptr);

	switch (r1[0])
	{
	case 'b': { x1=pel[0]; y1=pel[1]; } break;
	case 'p': { x1=rob[atoi(&r1[1])][0]; 
		        y1=rob[atoi(&r1[1])][1]; } break;
	case 'a': { x1=rob[atoi(&r1[1])+5][0]; 
		        y1=rob[atoi(&r1[1])+5][1]; } break;
	}

	switch (r2[0])
	{
	case 'b': { x2=pel[0]; y2=pel[1]; } break;
	case 'p': { x2=rob[atoi(&r2[1])][0]; 
		        y2=rob[atoi(&r2[1])][1]; } break;
	case 'a': { x2=rob[atoi(&r2[1])+5][0]; 
		        y2=rob[atoi(&r2[1])+5][1]; } break;
	}

	//opens the server
	return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

//returns distance
double angleto()
{
	char r1[80],r2[80];
	double x1,y1,x2,y2;
	char *ptr;
	int index = 0;

	/* Check for exactly 2 argument. */
	if (ArgCountCheck("openchn",EXACTLY,2) == -1)
	{ return(1L); }
	ptr=RtnLexeme(1);
	sprintf (r1,"%s",ptr);

	ptr=RtnLexeme(2);
	sprintf (r2,"%s",ptr);

	switch (r1[0])
	{
	case 'b': { x1=pel[0]; y1=pel[1]; } break;
	case 'p': { x1=rob[atoi(&r1[1])][0]; 
		        y1=rob[atoi(&r1[1])][1]; } break;
	case 'a': { x1=rob[atoi(&r1[1])+5][0]; 
		        y1=rob[atoi(&r1[1])+5][1]; } break;
	}

	switch (r2[0])
	{
	case 'b': { x2=pel[0]; y2=pel[1]; } break;
	case 'p': { x2=rob[atoi(&r2[1])][0]; 
		        y2=rob[atoi(&r2[1])][1]; } break;
	case 'a': { x2=rob[atoi(&r2[1])+5][0]; 
		        y2=rob[atoi(&r2[1])+5][1]; } break;
	}

	//opens the server
	return rad2deg(normalizeAngle(Angulo((x2-x1),(y2-y1))));
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

bool is_out (float x, float y)
{
	return (x<MinX || x>MaxX || y<MinY || y>MaxY);
}

//if its in the field
bool infield()
{
	DATA_OBJECT temp;
	float x,y;

	// Check for exactly 1 arguments. 
	if (ArgCountCheck("infield",EXACTLY,2) == -1)
	{ return(1L); }
	
	if (ArgTypeCheck("infield",1,INTEGER_OR_FLOAT,&temp) == 0)
	{ return(1L); }
	
	if (GetType(temp) == INTEGER)
	{ x = (float)DOToLong(temp);  }
	else /* the type must be FLOAT */
	{ x = (float)DOToDouble(temp);  }

	if (ArgTypeCheck("infield",2,INTEGER_OR_FLOAT,&temp) == 0)
	{ return(1L); }
	
	if (GetType(temp) == INTEGER)
	{ y = (float)DOToLong(temp);  }
	else
	{ y = (float)DOToDouble(temp);  }

	return !is_out(x,y);
}

int process_data (char *messg)
{
	float unused1;
	char parse[80];
	char cmd[80];
	float pela[3], roba[3];
	int process = 1; //by default we must run clips	
	int i;
	
	//printf ("\nReceived : %s\n",cmd);
	do {
		
		messg = xtracts(messg, cmd); //we get next info

		if (cmd[0]=='R' && cmd[1]=='E' && cmd[2]=='S' && cmd[3]=='E' && cmd[4]=='T'){
			EnvAssertString(TEnv,"(resetall)");
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
							pel[i]=pela[i];
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
								rob[index][i]=roba[i];
						//printf ("%s\n",messg);
					}
					//imprime_pos();
					//printf ("\n%s\n",cmd);	
			}
			else
				if (cmd[0]=='G' && cmd[1]=='O' && cmd[2]=='A' && cmd[3]=='L') 
				{
					if (cmd[5]=='1') 
						EnvAssertString(TEnv,"(goal 1)");
					else
						EnvAssertString(TEnv,"(goal 2)");
				}
				else
					if (cmd[0]=='O' && cmd[1]=='U' && cmd[2]=='T')
						EnvAssertString(TEnv,"(ballout)");
					else
						if (cmd[0]=='F' && cmd[1]=='R' && cmd[2]=='A' && cmd[3]=='M'
							&& cmd[4]=='E' && cmd[5]=='E' && cmd[6]=='N' && cmd[7]=='D') 
							process=0; //dont run clips

	} while (messg[0]!= '\0' && messg[0]!= '\n' && messg[0]!= '\r');
	
	//game facts
	if (is_out(pel[0],pel[1]))
	{
		EnvAssertString(TEnv,"(ballout)");
		process=1;
	}
						
	return process; //success command
}