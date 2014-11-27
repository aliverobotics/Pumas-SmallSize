/* client.cpp: ************************************************

   Program to open a client socket to receive data.

   21/April/2003: Hiram Galicia. Version 2.0

   NOTES:

 - Object Oriented Programming

************************************************************ */

/* standard c libraries */

   #include <stdio.h>

/* user libraries */

   #include "msocket.h"
   
/* program macros */

   #define BUFFERSIZE   1024
   #define MIN_PORT_NUM 2000

/* main program ******************************************* */

int main( int argc,char *argv[] )
{
/* program variables declaration */

   msocketData inport;
   
   int inportnum,error;
   char buffer[BUFFERSIZE],realhost[BUFFERSIZE];
   buffer[0]=0;

   printf ("inport: ");
   scanf  ("%d", &inportnum);

   if( inportnum<MIN_PORT_NUM ) { printf( "port must be > %d\n",MIN_PORT_NUM ); return( 1 ); }
   
   printf( "connecting @ localhost:%d\n\n",inportnum );

/* init socket for reception */

   if( error=inport.msocketInitSocket( "",inportnum,RX_SOCKET ) )
      { printf( "error opening client: %s",msocketGetError( error ) ); return( 1 ); }

/* main loop */

   while( buffer[0]!='@' ) {
      //clean the buffer
	  buffer[0]=0;
      //we wait for socket
      inport.msocketRxData( buffer, BUFFERSIZE );
      if (strlen(buffer)>0)
		  printf( "%s",buffer );
   }

/* close socket */

   inport.msocketKillSocket( );

/* end of program */

   return( 0 ); /* return no error */
}

/* end of file: client.cpp ******************************** */

