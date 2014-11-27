/* server.cpp: ************************************************

   Program to open a server socket to transmit data.

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

   msocketData outport;

   int outportnum,error;
   char buffer[BUFFERSIZE],realhost[BUFFERSIZE];
   buffer[0]=0;

   printf ("Server host (to send data): ");
   scanf  ("%s", &realhost);

   printf ("outport: ");
   scanf  ("%d", &outportnum);

   if( outportnum<MIN_PORT_NUM ) { printf( "port must be > %d\n",MIN_PORT_NUM ); return( 1 ); }

   printf( "connecting @ %s:%d\n\n",realhost,outportnum );

/* init socket for transmission */

   if( error=outport.msocketInitSocket( realhost,outportnum,TX_SOCKET ) )
      { printf( "error opening server: %s",msocketGetError( error ) ); return( 1 ); }

/* main loop */

   while( buffer[0]!='@' ) {
      printf( "$ " ); fgets( buffer,BUFFERSIZE,stdin );
      outport.msocketTxData( buffer,BUFFERSIZE );
   }

/* close socket */

   outport.msocketKillSocket( );

/* end of program */

   return( 0 ); /* return no error */
}

/* end of file: server.cpp ******************************** */

