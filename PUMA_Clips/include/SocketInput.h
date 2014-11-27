/*
 *  Socketinput.h
 *
 *  Created by Adalberto H. Llarena on 2/24/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */
/*
 First of all, we need a file pointer to the batch file which will contain
 the batch command information.
 */
 
#ifndef _SOCKETINPUT_
#define _SOCKETINPUT_
 
#include <stdio.h>
#include "clips.h"

#ifndef WIN32
#include <sys/time.h>
#else
#include <time.h>
#endif

/****************** BEGIN SOCKETS ***************************************/

#include "msocket.h"

#ifndef BUFFERSIZE
	#define BUFFERSIZE   1024
#endif

/* General outport_clips socket */
msocketData  outport_clips;

/* General inport_clips socket */
msocketData  inport_clips;

char sock_buffer_in[BUFFERSIZE];
	
int Init_Sockets_Clips (char *realhost, int inportnum_clips, int outportnum_clips)
{
	int error = 0;

	if( error = inport_clips.msocketInitSocket( realhost, inportnum_clips, RX_SOCKET ) ) {
		printf( "error opening client: %s", msocketGetError( error ) );
		return -1; 
	}

	printf( "connecting @ %s:%d\n\n",realhost,outportnum_clips );
	
	/* init socket for transmission */

	if( error = outport_clips.msocketInitSocket( realhost, outportnum_clips, TX_SOCKET ) ) {
		printf( "error opening server: %s", msocketGetError( error ) ); 
		return -1; 
	}
	
	return 0;
}

void Close_Sockets_Clips(void)
{
	outport_clips.msocketKillSocket( );
	inport_clips.msocketKillSocket( );
}

void sock_clean_buffer(void)
{
	int i;
	for (i=0;i<BUFFERSIZE;i++)
		//we clean buffer
		sock_buffer_in[i] = 0;
}

/* Function that receives a string */
void sock_receive(void) {
	char buffer_out[BUFFERSIZE];
		
	//we clean buffer
	sock_clean_buffer();
	
	//we wait for socket
	while( inport_clips.msocketRxData( sock_buffer_in, BUFFERSIZE ) < 0 );
	
	//we check kind of input, if it is not acommand
	if (sock_buffer_in[0]!='(' && sock_buffer_in[strlen(sock_buffer_in)]!=')')
	{
		char temp[200];
		//it is a spoken string it has to be added as a phrase
		sprintf (temp,"(assert (parse \"%s\"))\r(run)",sock_buffer_in);
		sprintf (sock_buffer_in,"%s",temp);
	}
	
	//we send an ack
	sprintf(buffer_out, "%d", 1);
	outport_clips.msocketTxData( buffer_out, BUFFERSIZE );
}

/* Function that indicates the command has beeen completed */
void sock_send_done(void) {
	char buffer_out[BUFFERSIZE];
		
	printf ("done\n");
	
	//we send an done flag
	sprintf(buffer_out, "%d", 2);
	outport_clips.msocketTxData( buffer_out, BUFFERSIZE );
}

/* sends a string */
void sock_send(char  *message)
{
	char buffer_in[BUFFERSIZE];
	char buffer_out[BUFFERSIZE];
	time_t it, ft;
	double t=0;
		
	do {
		
		it = time(NULL);
		
		do {
			sock_clean_buffer();
			
			//we clean outport
			buffer_out[0] = 0;
			
			sprintf(buffer_out, "%s", message);
			outport_clips.msocketTxData( buffer_out, BUFFERSIZE );
			// wait confirmation 
			
			if( inport_clips.msocketRxData( buffer_in, BUFFERSIZE ) < 0) 
			{
				ft = time(NULL);
				t = (double) (ft-it);
			}
			
		}
		while (buffer_in[0]!='1' && t<5.0); //seconds
		
		if (t>=5.0) printf ("Waiting for the server...\n");
		
	} while (buffer_in[0]!='1');
	
	sock_clean_buffer();
}

/************************* END SOCKETS ********************************/

/*
 We want to recognize any input requested from the logical name "stdin"
 because all user input is received from this logical name. The recognizer
 function for our router is defined below.
 */
int FindMysock(
				char *logicalName)
{
	if (strcmp(logicalName,"stdin") == 0) return(TRUE);
	return(FALSE);
}
/*
 We now need to define a function which will get and unget characters from our
 batch file. The get and ungetc character functions for our router are
 defined below.
 */

static char CommandBuffer[80];
static int CommandLocation = 0;

int GetcMysock(
				char *logicalName)

{		
	char rv;
	rv = sock_buffer_in[CommandLocation];
	
	//if EOS
	if (rv == 0)
	{
		if (CommandLocation) //if we have a string without \n or \r
		   rv = '\n';
		else
		{
			//we wait for new command
			sock_receive();
			//we read new character
			rv = sock_buffer_in[CommandLocation];
			
			printf  (":%s\n", sock_buffer_in);
			
			//DeleteRouter("mysock");
			//return(GetcRouter(logicalName));
		}
	}
	
	//we add character
	CommandBuffer[CommandLocation] = rv;
	CommandLocation++;
	CommandBuffer[CommandLocation] = EOS;
	
	if ((rv == '\n') /*|| (rv == '\r')*/)
	{
		//PrintRouter("wprompt",CommandBuffer);
		CommandLocation = 0;
		
		//command finished
		sock_clean_buffer();
	}
	
	return(rv);
}

int UngetcMysock(
				  int ch,
				  char *logicalName) /* unused */
				  {
	if (CommandLocation > 0) CommandLocation--;
	int temp = CommandBuffer[CommandLocation];
	CommandBuffer[CommandLocation] = EOS;
	return(temp);
				  }
				  
  /*
   When we exit CLIPS the batch file needs to be closed. The exit function for
   our router is defined below.
   */
  int ExitMysock(
				  int exitCode) /* unused */
				  {
	  Close_Sockets_Clips();

	  return 0;
				  }
/*
There is no need to define a print function since this router does not handle
output except for echoing the command line.
Now we define a function that turns the batch mode on.
*/
int MysockOn()
{
	int inport,outport;
	char *host;
	DATA_OBJECT temp;

	/* Check for exactly 2 argument. */
	if (ArgCountCheck("MysockOn",EXACTLY,3) == -1)
	{ return(1L); }
	
	host=RtnLexeme(1);

	if (ArgTypeCheck("MysockOn",2,INTEGER,&temp) == 0)
	{ return(1L); }
	{ inport = (int)DOToLong(temp); }

	if (ArgTypeCheck("MysockOn",3,INTEGER,&temp) == 0)
	{ return(1L); }
	{ outport = (int)DOToLong(temp); }

  /* we initialize sockets */
  if (Init_Sockets_Clips(host, inport, outport) != 0) return(FALSE);
  
  AddRouter("mysock", /* Router name */
			20, /* Priority */
			FindMysock, /* Query function */
			NULL, /* Print function */
			GetcMysock, /* Getc function */
			UngetcMysock, /* Ungetc function */
			ExitMysock); /* Exit function */
			
	printf ("Reditecting stdin to socket.\n");
	
	return(TRUE);
}

int MysockOff()
{
  //we close sockets
  Close_Sockets_Clips();
  
  /* Remove I/O rerouting and deactivate CURSES. */
  DeleteRouter("mysock");

  return (TRUE);
}

/*
 Compile and link the appropriate files. The batch function should now be
 accessible within CLIPS as external function. For Example
 CLIPS> (mybatch)
 */
 
 #endif
 