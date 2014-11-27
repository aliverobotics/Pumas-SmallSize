   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.21  04/15/03          */
   /*                                                     */
   /*                     MAIN MODULE                     */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

/***************************************************************************/
/*                                                                         */
/* Permission is hereby granted, free of charge, to any person obtaining   */
/* a copy of this software and associated documentation files (the         */
/* "Software"), to deal in the Software without restriction, including     */
/* without limitation the rights to use, copy, modify, merge, publish,     */
/* distribute, and/or sell copies of the Software, and to permit persons   */
/* to whom the Software is furnished to do so.                             */
/*                                                                         */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS */
/* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF              */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT   */
/* OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY  */
/* CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES */
/* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN   */
/* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF */
/* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.          */
/*                                                                         */
/***************************************************************************/
#define THREADCOUNT 3 //clips, visual and referee
#define ENVFILE "clips.clp"

#include <stdio.h>
#include <stdlib.h>
#ifndef WIN32
#include <sys/time.h>
#else
#include <time.h>
#endif
#include "pthread.h"

#include "Communicate.h"
Communicate Comm;

#include "setup.h"
#include "sysdep.h"
#include "envrnmnt.h"
#include "extnfunc.h"
#include "commline.h"
#include "clips.h"

int main(int,char *[]);
void UserFunctions(void);
void EnvUserFunctions(void *);

void* TEnv;
bool stopped = true;

#include "Angles.h"
#include "SocketInput.h"
#include "camera.h"
#include "smallctrl.h"
#include "user.h"

int process_order(char *cmd)
{
	//we remove enters
	int i;
	printf ("received %s",cmd);
	for (i=0;i<100;i++)
		if (cmd[i]=='\r' || cmd[i]=='\n')
			cmd[i]='\0';

	if (strcmp(cmd,"Halt")==0)
		EnvAssertString(TEnv,"(halt)");
	else if	(strcmp(cmd,"TimeoutHomeTeam")==0)
		EnvAssertString(TEnv,"(timeouthome)");
	else if	(strcmp(cmd,"TimeoutAwayTeam")==0)
		EnvAssertString(TEnv,"(timeoutaway)");
	else if	(strcmp(cmd,"FoulHomeTeam")==0)
		EnvAssertString(TEnv,"(foulhome)");
	else if	(strcmp(cmd,"FoulAwayTeam")==0)
		EnvAssertString(TEnv,"(foulaway)");
	else if	(strcmp(cmd,"GoalHomeTeam")==0)
		EnvAssertString(TEnv,"(goalhome)");
	else if	(strcmp(cmd,"GoalAwayTeam")==0)
		EnvAssertString(TEnv,"(goalaway)");
	else if	(strcmp(cmd,"KickoffHomeTeam")==0)
		EnvAssertString(TEnv,"(kickoffhome)");
	else if	(strcmp(cmd,"KickoffAwayTeam")==0)
		EnvAssertString(TEnv,"(kickoffaway)");
	else if	(strcmp(cmd,"Play")==0)
		EnvAssertString(TEnv,"(play)");
	else
		return 0;

	return 1;
}

void* camera_loop(void *args)
 {
	int *i = (int *)args;
	int count = 0;
	char *buff, *cmd;
	
	time_t it,ft;
		
	printf ("%s running in thread %d\n","Camera", *i);

	//we take time
	it = time(NULL);
	
	do {
		if ((buff=Comm.Channel_Receive("vision"))!=NULL && stopped==false)
			//if its valid  data
			if (process_data(buff)==1) //some sserting have occur
			{
				Run(-1); //runs the environment

				//we take time
				ft = time(NULL);
		
				if ((double) (ft-it)>=1.0) //seconds waiting
				{
					count=0;
					it=ft;
				}

				count++;
				//printf ("%5d => \n",count);

				//we calculate relative position
				//calcula_pos_rel(target);
			}
	} while (true);
	
	printf ("Exiting Camera Loop\n");
	pthread_exit(NULL);
 }

 void* referee_loop(void *args)
 {
	int *i = (int *)args;
	int count = 0;
	char *buff;

	printf ("%s running in thread %d\n","Referee", *i);
	
	do {
		//referee marking
		if ((buff=Comm.Channel_Receive("referee"))!=NULL) 
		{
			if (process_order(buff))
				Run(-1); //runs the clips environment
		}

	} while (true);
	
	printf ("Exiting Referee Loop\n");
	pthread_exit(NULL);
 }

 //opens a referee
int open_referee(char *host, int inport)
{
	int i;
	//opens the server
	i = Comm.Add_Channel("referee",host,inport,inport+1);
	Comm.Open_Channel(i);

	return 0;
}

//closes the referee
void close_referee()
{
	//close the server
	Comm.Close_Channel("referee");
}

 //initializes the game initial facts
 //depending on user
int init()
{
	char Thost[BUFFERSIZE], Vhost[BUFFERSIZE], Rhost[BUFFERSIZE];
	int tport = 6000;
	int vport = 4000;
	int rport = 2000;
	int i;
	
	printf ("Vision host: ");
	scanf  ("%s", &Vhost);

	printf ("Trainer host: ");
	scanf  ("%s", &Thost);

	printf ("Referee host: ");
	scanf  ("%s", &Rhost);

	printf ("Conencting to: Trainer 0, AutoCtrl 1 [0 or 1] : ");
	scanf ("%d",&i);
	
	if(i) tport -= 2001;
	
	printf ("Are we: Pumas 0 or America 1 [0 or 1] ? : ");
	scanf ("%d",&i);

	if (i==0) {
		printf ("We are Pumas !!\n");
		EnvAssertString (TEnv,"(we_are pumas)");
	}
	else 
	{
		printf ("Sorry, we are America.\n");
		EnvAssertString (TEnv,"(we_are america)");
		vport += 1000;
		tport += 1000;
		rport += 1000;
	}

	printf ("Are we: Home 0 or Visitor 1 [0 or 1] ? : ");
	scanf ("%d",&i);

	if (i==0) {
		EnvAssertString (TEnv,"(we_are home)");
	}
	else 
	{
		EnvAssertString (TEnv,"(we_are visitor)");
	}

	printf ("Field side: Left 0, Right 1 [0 or 1] : ");
	scanf ("%d",&i);

	if (i) {
		printf ("We are attacking to the left.\n");
		EnvAssertString (TEnv,"(goingleft)");
	}
	else {
		printf ("We are attacking to the right.\n");
	}

	open_vision(Vhost, vport);
	open_referee(Rhost, rport);
	init_sockets(Thost, tport);

	return 0;
}

int play()
{
	//lets play !!
	stopped = false;
	return 1;
}

int stop()
{
	//stops the game
	stopped = true;
	return 1;
}

void* clips_loop(void *args)
 {
	int *i = (int *)args;
	printf ("%s running in thread %d\n","Clips", *i);

    TEnv = CreateEnvironment();
    printf ("Env created \n");
	
   	int error;
    
	//error = EnvLoad(TEnv,ENVFILE);
	error = EnvBatchStar(TEnv,ENVFILE);
	if(!error)
	{
		printf("load error \n");	
		exit(1);
	}
	printf ("Env loaded\n");

	// reset the Clips environment 
	//EnvReset(TEnv); //in batch mode we have to wait

	//run the Clips rules 
	//printf ("running\n");
	//EnvRun(TEnv,-1L); //in batch mode we have to wait

	RerouteStdin(TEnv,0,NULL); 
	
    CommandLoop(TEnv);
    pthread_exit(NULL);
  }

/****************************************/
/* main: Starts execution of the expert */
/*   system development environment.    */
/****************************************/
int main( int argc, char *argv[])
  {
    int i;
    pthread_t threads[THREADCOUNT];
	int thread_ids[THREADCOUNT];
    pthread_attr_t attr;

	/* Se llenan los identificadores de los threads */
	for(i = 0; i < THREADCOUNT; i++)
		thread_ids[i] = i;

    /* Initialize mutex*/
//    pthread_mutex_init(&count_mutex, NULL);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	pthread_create(&threads[0], &attr, camera_loop, (void *)&thread_ids[0]);
	pthread_create(&threads[1], &attr, referee_loop, (void *)&thread_ids[1]);
	pthread_create(&threads[2], &attr, clips_loop, (void *)&thread_ids[2]);

	/* Wait for all threads to complete */
    for (i = 0; i < THREADCOUNT; i++)
		pthread_join(threads[i], NULL);
  
    printf ("Exiting threads\n");
	
    /* Clean up and exit */
    pthread_attr_destroy(&attr);

	close_vision();
	close_sockets();
	close_referee();

 //   pthread_mutex_destroy(&count_mutex);
    pthread_exit (NULL);

    return(0);
  }
  /*********************************************************/
/* UserFunctions: Informs the expert system environment  */
/*   of any user defined functions. In the default case, */
/*   there are no user defined functions. To define      */
/*   functions, either this function must be replaced by */
/*   a function with the same name within this file, or  */
/*   this function can be deleted from this file and     */
/*   included in another file.                           */
/*********************************************************/

void UserFunctions()
  {
	DefineFunction2("mysockOn",'b',MysockOn, "MysockOn","33u");
	DefineFunction("mysockOff",'b',MysockOff, "MysockOff");
	DefineFunction2("openchn",'l',PTIF openchn,"openchn","33u");
	DefineFunction2("closechn",'l',PTIF closechn,"closechn","11n");
	DefineFunction2("chnsend",'l',PTIF chnsend,"chnsend","22u");
	DefineFunction2("chnrecv",'s',PTIF chnrecv,"chnrecv","11n");
	DefineFunction2("xb",'f',PTIF xb,"xb","00n");
	DefineFunction2("yb",'f',PTIF yb,"yb","00n");
	DefineFunction2("xr",'f',PTIF xr,"xr","11n");
	DefineFunction2("yr",'f',PTIF yr,"yr","11n");
	DefineFunction2("ar",'f',PTIF ar,"ar","11n");
	DefineFunction2("sendrobot",'l',PTIF sendrobot,"sendrobot","22u");
	DefineFunction2("sendall",'l',PTIF sendall,"sendall","11u");
	DefineFunction2("play",'i',PTIF play,"play","00u");
	DefineFunction2("stop",'i',PTIF stop,"stop","00u");
	DefineFunction2("init",'i',PTIF init,"init","00u");
	DefineFunction2("dist",'d',PTIF distance,"distance","22u");
	DefineFunction2("angle",'d',PTIF angleto,"angleto","22u");
	DefineFunction2("infield",'b',PTIF infield, "infield","22n");
  }
  
/***********************************************************/
/* EnvUserFunctions: Informs the expert system environment */
/*   of any user defined functions. In the default case,   */
/*   there are no user defined functions. To define        */
/*   functions, either this function must be replaced by   */
/*   a function with the same name within this file, or    */
/*   this function can be deleted from this file and       */
/*   included in another file.                             */
/***********************************************************/
void EnvUserFunctions(
  void *theEnv)
  {
#if MAC_MCW || IBM_MCW || MAC_XCD
#pragma unused(theEnv)
#endif
  }

