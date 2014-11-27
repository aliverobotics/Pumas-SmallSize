/* standard c libraries */

   #include <stdio.h>

/* user libraries */

   #include "socklib.h"
   #define BUFFERSIZE   1024

int main( int argc,char *argv[] )
{
	int sd;
    char buffer[BUFFERSIZE];
	buffer[0]=0;


	sd = OpenSocketClient( 8000 );
	printf ("Opening client 8000 => %d\n", sd );

   while( buffer[0]!='@' ) {
      if( ReadSocket( sd, buffer, BUFFERSIZE ) >=0 )
      printf( "<%s>: ",buffer );
   }
/*
	sd = OpenSocketServer( "132.248.161.5", 8000 );
	printf ("Opening server 132.248.161.5, port 8000 => %d\n", sd );


   while( buffer[0]!='@' ) {
      printf( "$ " ); fgets( buffer,BUFFERSIZE,stdin );
      WriteSocket( sd, buffer, BUFFERSIZE );
	  buffer[0]=0;
   }
*/
/* close socket */

   CloseSocket ( sd );

/* end of program */

   return( 0 ); /* return no error */
}

/* end of file: server.cpp ******************************** */

