/*
 *  Communicate.h
 *  Jander
 *
 *  Created by Adalberto H. Llarena on 7/23/05.
 *  Copyright 2005 __Adalberto H. Llarena__. All rights reserved.
 *  This library must contain also a communications protocole to ensure 
 *  correct transmision and reception even when loosing communications 
 */

#ifndef _COMMUNICATE_
#define _COMMUNICATE_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
	
#include "msocket.h"
#define BUFFERSIZE 1024
#define MAXCHANNELS 100
//#define ECHOED  //if there is some response from receiver when got a string

//class to send speech strings
class Communicate
{
public:	

	//node for communicate list
	struct Channel {
		int num;
		char name[80];
		/* General outport socket */
		msocketData  outport;
		/* General inport socket */
		msocketData  inport;
		//address of host
		char realhost[80];
		//number of outport
		int  outportnum;
		/* Port to receive */
		int  inportnum;
		//received strings
		char string [BUFFERSIZE];
		char buffer_in [BUFFERSIZE];
		
		struct Channel *next;
	};
	
	struct Channel * Head;
	int channelCount;
	
	Communicate()
	{
		this->Head = NULL;
		this->channelCount = 0;
	}
	
	//deletes structure
	void Clean_Channels(void)
	{
		struct Channel * node;
		node = this->Head;
		
		while (node)
		{
			node = node->next;
			free (this->Head);
			this->Head = node;
		}
		
		this->Head = NULL;
		this->channelCount = 0;
	}
		
	~Communicate()
	{
		this->Clean_Channels();
	}
	
	//returns the number for a given channel name
	int Channel_No (char *nameChannel)
	{
		struct Channel * node;
		node = this->Head;
		
		while (node && strcmp(node->name,nameChannel)!=0)
				node = node->next;
				
		if (node)
			return node->num;
		else
			return -1;
	}

	//returns the pointer to given channel number
	struct Channel * Channel_Ptr (int numChannel)
	{
		struct Channel * node;
		node = this->Head;
		
		while (node && node->num!=numChannel)
				node = node->next;
				
		return node;
	}

	//adds a channel
	int Add_Channel   (char *aname,
					   char *arealhost,
					   int  aninportnum,
					   int  anoutportnum) 
	{
	  struct Channel* node;
	  int i;

	  //we get the memory
	  node = (Channel*)malloc(sizeof(*node));

	  if (!node) {
		return 0;
	  }
	  
	  printf ("reciving %d %d\n", anoutportnum, aninportnum);
	  this->channelCount++;
	  node->num  = this->channelCount;
	  
	  sprintf (node->name,"%s",aname);
	  sprintf (node->realhost,"%s",arealhost);
	  
	  node->outportnum = anoutportnum;
	  node->inportnum = aninportnum;
	  		   
	  node->next = this->Head;
	  this->Head = node;
	  
	  //we clean buffer
	  for (i=0;i<BUFFERSIZE;i++)
		node->string[i] = 0;
	  
	  return node->num;
	}
	
	//opens a channel
	int Open_Channel (int channelNo)
	{
		struct Channel* C;
		C=this->Channel_Ptr(channelNo);
		
		if (C)
		{
			int error = 0;

			if( error = C->inport.msocketInitSocket( "localhost", C->inportnum, RX_SOCKET ) ) {
				printf( "error opening client: %s", msocketGetError( error ) );
				return -2; 
			}

			printf( "connecting @ %s:%d\n\n",C->realhost,C->outportnum );
			
			/* init socket for transmission */

			if( error = C->outport.msocketInitSocket( C->realhost, C->outportnum, TX_SOCKET ) ) {
				printf( "error opening server: %s", msocketGetError( error ) ); 
				return -3; 
			}
		
			return 0;
		}
		else return -1;
	}
	
	//opens a channel
	int Open_Channel (char *channelName)
	{
		return (Open_Channel(Channel_No (channelName)));
	}
	
	//closes the channel
	int Close_Channel(int channelNo)
	{
		struct Channel* C;
		C=this->Channel_Ptr(channelNo);
		
		if (C)
		{
			C->outport.msocketKillSocket( );
			C->inport.msocketKillSocket( );
			
			return 0;
		}
		else return -1;
	}
	
	//closes a channel
	int Close_Channel (char *channelName)
	{
		return (Close_Channel(Channel_No (channelName)));
	}
	
	/* Function that receives a string */
	char * Channel_Receive(int channelNo)
	{
		struct Channel* C;
		#ifdef ECHOED
		char buffer_out[BUFFERSIZE];
		#endif
		
		if ((C=this->Channel_Ptr(channelNo))!=NULL)
		{
			//we wait for socket
			while( C->inport.msocketRxData( C->buffer_in, BUFFERSIZE ) <= 0 );
			
			strcpy (C->string, C->buffer_in);
			//we clean buffer
			
			for (int h=0;h<BUFFERSIZE;h++)
				C->buffer_in[h]=0;
			
			#ifdef ECHOED
			//we send an ack, comment if not desired
			sprintf(buffer_out, "%d", 1);
			C->outport.msocketTxData( buffer_out, BUFFERSIZE );
			#endif
			
			return C->string;
		}
		else return NULL;
	}

	/* Function that receives a string */
	char * Channel_Receive(char *channelName)
	{
		int i;
		i = Channel_No (channelName);
		
		if (i > 0) {
			return Channel_Receive (i);
		}
		else 
			return NULL;
	}
	
	/* sends a string */
	int Channel_Send(int channelNo, char  *message)
	{
		char buffer_out[BUFFERSIZE];
		struct Channel* C;
		
#ifdef ECHOED
		char buffer_in[BUFFERSIZE];
		time_t, it,ft;
		double t;
#endif
		//if the channel exist
		C=this->Channel_Ptr(channelNo);
		
		if (C)
		{
		
#ifdef ECHOED
			do {
				//we take time
				it = time(NULL);
#endif
				//we clean buffer
				buffer_out[0] = 0;
				
				sprintf(buffer_out, "%s", message);
				C->outport.msocketTxData( buffer_out, BUFFERSIZE );
#ifdef ECHOED
				do {	
					if( C->inport.msocketRxData( buffer_in, BUFFERSIZE ) < 0); 
					{
						ft = time(NULL);
						t = (double) (ft-it);
					}
				}
				while (buffer_in[0]!='1' && t<5.0); //seconds
				
				if (t>=5.0) printf ("Waiting for the server...\n");
				
			} while (buffer_in[0]!='1');
#endif
			return 0;
		}
		return -1;
	}
	
		/* sends a string */
	int Channel_Send(char *channelName, char  *message)
	{
		int i;
		i = Channel_No (channelName);
		
		if (i > 0) {
			return Channel_Send(i, message);
		}
		else
		   return -1;
	}

};

#endif
